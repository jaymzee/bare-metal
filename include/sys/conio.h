#include <sys/keyboard.h>

void SetTextCursor(int x, int y);
void GetTextCursor(int *x, int *y);
void ClearText(unsigned char attr);
void LocateText(int x, int y, const char *str);
void WriteText(const char *str);
void EnableBlinkingText(void);
void DisableBlinkingText(void);
void HideTextCursor(void);
void ShowTextCursor(void);
void TextCursorShape(int top, int bot);
