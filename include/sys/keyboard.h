#ifndef SYS_KEYBOARD_H_INCLUDED
#define SYS_KEYBOARD_H_INCLUDED

// state
//  0 - wait for byte
//  1 - E0 recvd wait for one more byte
//  2 - E1 recvd wait for 1 of 2 more bytes
//  3 - E1 recvd wait for 2 of 2 more bytes
struct virt_kb {
    unsigned int lshft:1;
    unsigned int rshft:1;
    unsigned int lctrl:1;
    unsigned int rctrl:1;
    unsigned int lalt:1;
    unsigned int ralt:1;
    unsigned int numlock:1;
    unsigned int capslock:1;
    unsigned int state:2;
};

extern struct virt_kb vkbd;

int ScanKeyboard(void);
void DumpKeyboardScan(int scancode);
extern char kbd_decode[256];
extern char kbd_decode_numlock[128];

#endif
