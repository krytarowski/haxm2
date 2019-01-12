#ifndef HAX_CORE_CPU_H_
#define HAX_CORE_CPU_H_

/*
 * Virtual Machine Control Structure (VMCS)
 *
 * Operations:
 *  - VMPTRLD,
 *  - VMPTRST,
 *  - VMCLEAR,
 *  - VMREAD,
 *  - VMWRITE,
 *  - VMCALL,
 *  - VMLAUNCH,
 *  - VMRESUME,
 *  - VMXOFF, and
 *  - VMXON
 *
 * VMCS is manipulated with VMCLEAR, VMPTRLD, VMREAD, VMWRITE
 *
 * VMCS struct must be page-size aligned
 * VMCS is (can be?) per logical CPU
 *
 * logical processor may maintain a number of VMCSs that are active
 *
 * VMCS state can be stored in memory or on the processor or both
 *
 * at any given time only a single VMCS is current.
 *
 * VMLAUNCH, VMREAD, VMREAD, VMWRITE operate only on the current VMCS
 *
 * VMCS shadowing - nesting virtualization
 *
 * state "clear" -> VMLAUNCH state "launched" (only from clear)
 * VMRESUME requires state "launched"
 * VMCLEAR sets state "clear"
 * state "launched" can be changed only by VMCLEAR to "clear"
 *
 * States of VMCS:
 * - active vs inactive
 * - current vs not current
 * - clear vs launched
 *
 * VMPTRST stores the address of the logical processor's current VMCS
 * into a specified memory location
 * (0xff_ff_ff_ff_ff_ff_ff_ff if there is no current VMCS)
 *
 * Size of VMCS is implementation specific and is at most PAGE_SIZE
 * sizeof(VMCS) is detectable with MSR IA32_VMX_BASIC.
 *
 * Format of the VMCS region:
 *  +-------------+------------------------------------
 *  | byte offset | contents
 *  +-------------+------------------------------------
 *  | 0           | bits 30:0 VMCS revision identifier
 *  | 0           | bit 31    shadow-VMCS identifier
 *  +-------------+------------------------------------
 *  | 4           | VMX-abort indicator
 *  +-------------+------------------------------------
 *  | 8           | VMCS data (implementation specific format)
 *  +-------------+------------------------------------
 *
 * Software should write VMCS revision identifier to the VMCS region
 * before using that region for VMCS.
 *
 * VMCS revision is never written by CPU.
 *
 * VMCS identifier used by a processor can be read with MSR IA32_VMX_BASIC.
 *
 * VMPTRLD fails if its operand references a VMCS region whose VMCS revision
 * identifier differs from that used by the processor.
 *
 * Software should clear or set the shadow-VMCS indicator depending on
 * whether the VMCS is to be an ordinary VMCS or a shadow VMCS.
 *
 * VMPTRLD fails if the shdaow-VMCS indicator is set and the processor
 * does not support the 1-setting of the VMCS shadowing.
 *
 * VMCS shadowing can be detected with reading VMX capability with
 * MSR IA32_VMX_PROCBASED_CTLS2.
 *
 * Next 4 bytes are used for the VMX-abort indicator, CPU writes a
 * non-zero value into these bits if a VMX abort occurs. Software might
 * write there too.
 *
 * The remainer of VMCS region is used for VMCS data.
 * Software shall maintain the VMCS region and related structures
 * in writeback cacheable memory.
 *
 * Organization of VMCS DATA
 * -------------------------
 * VMCS data are organized into six logical groups:
 * - guest-state area : CPU state is saved here on VM exits and loaded
 *                      from here on VM entries
 * - host-state area : CPU state is loaded from here on VM exits
 * - vm-execution control fields : control processor behavior in VMX
 *                                 non-root operation, they determine in
 *                                 part the causes of VM exits
 * - vm-exit control fields : controls VM exits
 * - vm-entry control fields : controls VM entries
 * - vm-exit information fields : receive information on VM exits
 *
 * VMX controls = vm-execution control, vm-exit control, vm-entry control
 *
 * Guest-state area
 * ----------------
 * control registers:
 *   CR0, CR3, CR4 (64-bit on 64-bit CPU, on 32-bit on 32-bit CPU)
 * debug registers:
 *   DR7 (64-bit on 64-bit CPU, on 32-bit on 32-bit CPU)
 * RSP, RIP, RFLAGS (64-bit on 64-bit CPU, on 32-bit on 32-bit CPU)
 * CS, SS, DS, ES, FS, GS, LDTR, TR
 *  - selector (16-bits)
 *  - base address (64-bit on 64-bit CPU, on 32-bit on 32-bit CPU)
 *  - segment limit (32-bits)
 *  - access rights (32-bits)
 * GDTR, IDTR
 *  - base address (64-bit on 64-bit CPU, on 32-bit on 32-bit CPU)
 *  - limit (32-bits)
 * MSRs:
 *  - IA32_DEBUGCTL (64-bits)
 *  - IA32_SYSENTER_CS (32-bits)
 *  - IA32_SYSENTER_ESP and IA32_SYSENTER_EIP (64-bit on 64-bit CPU, on 32-bit on 32-bit CPU)
 *  - IA32_PERF_GLOBAL_CTRL (64-bits)
 *  - IA32_PAT (64-bits)
 *  - IA32_EFER (64-bits)
 *  - IA32_BNDCFGS (64-bits)
 * SMBASE (32-bits)
 *
 * Guest Non-Register State
 * ------------------------
 * - activity state (32-bits)
 *   # 0 = active : the logical processor is executing instructions normally
 *   # 1 = HLT : the logical cpu is inactive because it executed the HLT
 *               instruction
 *   # 2 = shutdown : cpu is inactive because it incurred triple fault or other
 *                    serious error
 *   # 3 = wait-for-SIPI : cpu waiting for SIPI (SIPI = startup IPI)
 *
 *   MSR IA32_VMX_MISC - software should read it to detect activity states
 *
 * - interruptibility state (32-bits) : features that permit certain events to
 *                                      be blocked for a period of time
 *   # bit 0 - blocking by STI (set interrupt flag)
 *   # bit 1 - blocking by mov ss/pop ss
 *   # bit 2 - blocking by SMI (system-management interrupts)
 *   # bit 3 - blocking by NMI (non-maskable interrupts)
 *   # bit 4 - enclave interruption : vm exit caught
 *   # bit 5-31 - reserved - vm entry will fail if these bits are not 0
 *
 * - pending debug exceptions (64 bits on 64-bit cpu, 32bit on 32bit)
 *   # bit 3-0 
 *
 * - VMCS link pointer (64-bits)
 *
 * - VMX-preemption timer value (32-bits)
 *
 * - page-directory-pointer-table entries (PDPEs, 64-bits each) : EPT needed
 *   # PDPTE0
 *   # PDPTE1
 *   # PDPTE2
 *   # PDPTE3
 * This corresponds to PDPTE referenced by CR3 when PAE paging is in use.
 *
 * - guest interrupt status (16-bits)
 *   # requesting virtual interrupt (RVI)
 *   # servicing virtual interrupt (SVI)
 *
 * - PML index (16-vit)
 *
 * HOST-STATE AREA
 * ---------------
 * CR0, CR3, CR4
 *
 * RSP, RIP
 *
 * selector fields (16-bit) for the segment registers CS, SS, DS, ES, FS, GS, TR
 * (no field for LDTR)
 *
 * MSRs: IA32_SYSENTER_CS (32-bits)
 *       IA32_SYSENTER_ESP and IA32_SYSENTER_EIP (64/32bit)
 *       IA32_PERF_GLOBAL_CTRL (64-bits)
 *       IA32_PAT (64-bit)
 *       IA32_EFER (64-bit)
 *
 * VM-EXECUTION CONTROL FIELDS
 * ---------------------------
 * - pin-based vm-execution controls
 * - processor-based vm-execution controls
 * - exception bitmap
 * - i/o-bitmap addresses
 * - time-stamp counter offset and multiplier
 * - guest/host masks and read shadows for CR0 and CR4
 * - CR3-Target Controls
 * - Controls for APIC Virtualization
 */

struct per_cpu_data {
	struct hax_page		*vmxon_page;
	struct hax_page		*vmcx_page;
};

#endif // HAX_CORE_CPU_H_