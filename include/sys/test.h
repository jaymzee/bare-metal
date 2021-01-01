#ifndef SYS_TEST_H_INCLUDED
#define SYS_TEST_H_INCLUDED

// for testing
void CauseGPFault(void);
void CausePageFault(void);
void CauseDivbyzero(void);
void CauseInvalidOpcode(void);

#endif
