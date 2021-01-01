#ifndef SYS_CPU_EXCHANDLERS_H_INCLUDED
#define SYS_CPU_EXCHANDLERS_H_INCLUDED

void DivbyzeroHandler(void);
void DebugHandler(void);
void NMIHandler(void);
void BreakpointHandler(void);
void OverflowHandler(void);
void BoundRangeHandler(void);
void InvalidOpcodeHandler(void);
void DoubleFaultHandler(void);
void InvalidTSSHandler(void);
void SegNotPresentHandler(void);
void StackSegFaultHandler(void);
void GPFaultHandler(void);
void PageFaultHandler(void);
void x87FPExceptHandler(void);
void AlignCheckHandler(void);
void MachineCheckHandler(void);
void SIMDFPExceptHandler(void);

#endif
