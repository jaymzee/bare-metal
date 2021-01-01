#include <string.h>
#include <sys/conio.h>
#include <sys/io.h>
#include <sys/ps2.h>
#include <sys/vga.h>

#define TEXT_ATTR   0x07
#define TEXT_COLS   80
#define TEXT_ROWS   25
#define VIDEOMEM    0xB8000

static char *vga_text = (char *)VIDEOMEM;
struct virt_kb vkbd;
char keyboard_buffer[256];
unsigned char kbbuf_in;
unsigned char kbbuf_out;

void ClearText(unsigned char attr)
{
    unsigned short *vm = (unsigned short *)VIDEOMEM;
    for (int i = 0; i < 2000; i++) {
        vm[i] = attr << 8 | ' ';
    }
    SetTextCursor(0, 0);
}

void SetTextCursor(int x, int y)
{
    int offset = TEXT_COLS * y + x;
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_LO);
    outb(VGA_CRTC_DATA, offset & 0xFF);
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_HI);
    outb(VGA_CRTC_DATA, (offset >> 8) & 0xFF);
}

void GetTextCursor(int *x, int *y)
{
    int offset = 0;
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_LO);
    offset = inb(VGA_CRTC_DATA);
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_HI);
    offset |= inb(VGA_CRTC_DATA) << 8;
    *y = offset / TEXT_COLS;
    *x = offset % TEXT_COLS;
}

void LocateText(int x, int y, const char *str)
{
    int offset = 2 * (TEXT_COLS * y + x);
    while (*str) {
        vga_text[offset] = *str++;
        offset += 2;
    }
}

static void ScrollText()
{
    int char_count = TEXT_COLS * (TEXT_ROWS - 1);
    int byte_count = 2 * char_count;
    memcpy(vga_text, vga_text + 2 * TEXT_COLS, byte_count);
    for (int i = 0; i < TEXT_COLS; i++) {
        vga_text[byte_count + 2 * i] = ' ';
    }
}

void WriteText(const char *str)
{
    int x, y;
    char ch;

    GetTextCursor(&x, &y);
    while ((ch = *str++) != '\0') {
        if (ch > 31 && ch < 127) {
            vga_text[2 * (TEXT_COLS * y + x++)] = ch;
        }
        if (ch == 10 || (x > TEXT_COLS - 1 && *str != 10)) {
            x = 0;
            y++;
        }
        while (y > TEXT_ROWS - 1) {
            y--;
            ScrollText();
        }
    }
    SetTextCursor(x, y);
}

void EnableBlinkingText(void)
{
    inb(VGA_INPUT_STAT1); // this will reset index/data flip-flop
    outb(VGA_ATTR_ADDR, VGA_ATTR_MODE_CTRL); // select attr mode control reg
    outb(VGA_ATTR_DATA_WR, 0x08 | inb(VGA_ATTR_DATA_RD)); // turn on bit 3
}

void DisableBlinkingText(void)
{
    inb(VGA_INPUT_STAT1); // this will reset index/data flip-flop
    outb(VGA_ATTR_ADDR, VGA_ATTR_MODE_CTRL); // select attr mode control reg
    outb(VGA_ATTR_DATA_WR, 0xf7 & inb(VGA_ATTR_DATA_RD)); // turn off bit 3
}

void ShowTextCursor(void)
{
    outb(VGA_CRTC_ADDR, CRTC_CUR_STRT);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & 0xdf);
}

void HideTextCursor(void)
{
    outb(VGA_CRTC_ADDR, CRTC_CUR_STRT);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x20);
}

void TextCursorShape(int top, int bot)
{
    outb(VGA_CRTC_ADDR, CRTC_CUR_STRT);
    outb(VGA_CRTC_DATA, (inb(VGA_CRTC_DATA) & 0xe0) | (top & 0x1f));
    outb(VGA_CRTC_ADDR, CRTC_CUR_END);
    char b = inb(VGA_CRTC_DATA);
    outb(VGA_CRTC_DATA, (b & 0xe0) | (bot & 0x1f));

}

// read scancode from PS/2 port
int ScanKeyboard(void)
{
    int status = inb(KB_STATUS);
    if (status & 1) {
        return inb(KB_DATA);
    }
    return -1; // kb buffer empty
}

void DumpKeyboardScan(int scancode)
{
    char sbuf[80], nbuf[20];
    char ch, chstr[] = " \n";

    strcpy(sbuf, "0x");
    strcat(sbuf, itoa(scancode, 16, 2, nbuf));
    strcat(sbuf, " ");
    // if key pressed
    if (scancode >= 0 && scancode < 128) {
        if (vkbd.lshft || vkbd.rshft) {
            ch = kbd_decode[scancode + 128];
        } else {
            ch = kbd_decode[scancode];
        }
        if (vkbd.lctrl || vkbd.rctrl) {
            strcat(sbuf, "CTRL+");
            if (ch >= 'a' && ch <= 'z') {
                ch &= 0xdf; // make uppercase
            }
        } else if (vkbd.lalt || vkbd.ralt) {
            strcat(sbuf, "ALT+");
            if (ch >= 'a' && ch <= 'z') {
                ch &= 0xdf; // make uppercase
            }
        } else if (vkbd.capslock && ch >= 'a' && ch <= 'z') {
            ch &= 0xdf; // make uppercase
        } else if (vkbd.numlock) {
            char chnum = kbd_decode_numlock[scancode];
            if ((chnum >= '0' && chnum <= '9') || chnum == '.') {
                ch = chnum;
            }
        }
        if (ch == '\n') {
            strcat(sbuf, "ENTER");
        } else if (ch == '\b') {
            strcat(sbuf, "BS");
        } else if (ch == '\t') {
            strcat(sbuf, "TAB");
        } else if (ch == '\x1b') {
            strcat(sbuf, "ESC");
        }
        if (ch > 31 && ch < 127) {
            chstr[0] = ch;
        }
    }
    strcat(sbuf, chstr);
    WriteText(sbuf);
}

char ScanCodeToASCII(int scancode)
{
    char ch;
    if (vkbd.lshft || vkbd.rshft) {
        ch = kbd_decode[scancode + 128];
    } else {
        ch = kbd_decode[scancode];
    }
    if ((vkbd.lctrl || vkbd.rctrl) && ch >= 'a' && ch <= 'z') {
        ch &= 0x1f; // control character
    } else if (vkbd.capslock && ch >= 'a' && ch <= 'z') {
        ch &= 0xdf; // make uppercase
    } else if (vkbd.numlock) {
        char chnum = kbd_decode_numlock[scancode];
        if ((chnum >= '0' && chnum <= '9') || chnum == '.') {
            ch = chnum;
        }
    }
    return ch;
}

void KeyboardHandlerM(void) {
    int scancode = ScanKeyboard();

    switch (scancode) {
    case 0x2a:
        vkbd.lshft = 1;
        break;
    case 0xaa:
        vkbd.lshft = 0;
        break;
    case 0x36:
        vkbd.rshft = 1;
        break;
    case 0xb6:
        vkbd.rshft = 0;
        break;
    case 0x1d:
        if (vkbd.state == 0) {
            vkbd.lctrl = 1;
        } else if (vkbd.state == 1) {
            vkbd.rctrl = 1;
        } else if (vkbd.state == 2) {
            vkbd.state = 3;
        }
        break;
    case 0x9d:
        if (vkbd.state == 0) {
            vkbd.lctrl = 0;
        } else if (vkbd.state == 2) {
            vkbd.state = 3;
        }
        break;
    case 0x38:
        vkbd.lalt = 1;
        break;
    case 0x3A:
        vkbd.capslock ^= 1;
        break;
    case 0xb8:
        vkbd.lalt = 0;
        break;
    case 0xe0:
        vkbd.state = 1;
        break;
    case 0xe1:
        vkbd.state = 2;
        break;
    case 0x45:
        if (vkbd.state == 0) {
            vkbd.numlock ^= 1;
        } else if (vkbd.state == 3) {
            // pause pressed (part 1)
            vkbd.state = 0;
        }
        break;
    case 0xc5:
        if (vkbd.state == 0) {
            //vkbd.numlock = 0;
        } else if (vkbd.state == 3) {
            // pause pressed (part 2)
            vkbd.state = 0;
        }
        break;
    default:
        if (scancode >= 0 && scancode < 128) {
            char ch = ScanCodeToASCII(scancode);
            keyboard_buffer[kbbuf_in++] = ch;
            // echo character
            char chstr[] = " ";
            chstr[0] = (ch == '\r') ? '\n' : ch;
            WriteText(chstr);
        }
        break;
    }
}

