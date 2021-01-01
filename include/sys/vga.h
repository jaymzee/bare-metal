// only color vga is supported

// CRT Controller Registers
#define VGA_CRTC_ADDR       0x3D4   // 0x3B4 for monochome display
#define VGA_CRTC_DATA       0x3D5   // 0x3B5 for monochrome display

// Attribute Controller Registers
#define VGA_ATTR_ADDR       0x3C0   // attribute address/data register
#define VGA_ATTR_DATA_WR    0x3C0
#define VGA_ATTR_DATA_RD    0x3C1

// Sequencer Registers
#define VGA_SEQ_ADDR        0x3C4
#define VGA_SEQ_DATA        0x3C5

// External Registers (General Registers)
//   Miscellaneous Output Register
#define VGA_MISC_OUT_WR     0x3C2   // write only
#define VGA_MISC_OUT_RD     0x3CC

//   Input Status Registers
#define VGA_INPUT_STAT0     0x3C2   // read only
#define VGA_INPUT_STAT1     0x3DA   // read only

//   Feature Control Register
#define VGA_FEAT_CTRL_RD    0x3CA
#define VGA_FEAT_CTRL_WR    0x3DA   // write only

// Graphics Registers
#define VGA_GFX_ADDR        0x3CE
#define VGA_GFX_DATA        0x3CF

// Color Registers
#define VGA_DAC_MASK        0x3C6
#define VGA_DAC_ADDR_RD     0x3C7   // writing initiates DAC Read
#define VGA_DAC_ADDR_WR     0x3C8   // writing initiates DAC Write
#define VGA_DAC_DATA        0x3C9   // R then G then B
#define VGA_DAC_STATE       0x3C7   // read only

// CRT Controller Indices
#define CRTC_HORZ_TOTAL         0x00    // Horizontal Total register
#define CRTC_HORZ_DISP_END      0x01    // Horizontal Display End register
#define CRTC_HORZ_BLANK_STRT    0x02    // Horizontal Blanking Start register
#define CRTC_HORZ_BLANK_END     0x03    // Horizontal Blanking End register
#define CRTC_HORZ_RETRACE_STRT  0x04    // Horizontal Retrace Start register
#define CRTC_HORZ_RETRACE_END   0x05    // Horizontal Retrace End register
#define CRTC_VERT_TOTAL         0x06    // Vertical Total register
#define CRTC_OVERFLOW           0x07    // Overflow register
#define CRTC_PRESET_ROW_SCAN    0x08    // Preset Row Scan register
#define CRTC_MAX_SCAN_LINE      0x09    // Maximum Scan Line register
#define CRTC_CUR_STRT           0x0A    // Cursor Start register
#define CRTC_CUR_END            0x0B    // Cursor End register
#define CRTC_STRT_ADDR_HI       0x0C    // Start Address High register
#define CRTC_STRT_ADDR_LO       0x0D    // Start Address Low register
#define CRTC_CUR_LOC_HI         0x0E    // Cursor Location High register
#define CRTC_CUR_LOC_LO         0x0F    // Cursor Location Low register
#define CRTC_VERT_RETRACE_STRT  0x10    // Vertical Retrace Start register
#define CRTC_VERT_RETRACE_END   0x11    // Vertical Retrace End register
#define CRTC_VERT_DISP_END      0x12    // Vertical Display End register
#define CRTC_OFFSET             0x13    // Offset register
#define CRTC_UNDERLINE_LOC      0x14    // Underline Location register
#define CRTC_VERT_BLANK_STRT    0x15    // Vertical Blanking Start register
#define CRTC_VERT_BLANK_END     0x16    // Vertical Blanking End register
#define CRTC_MODE_CTRL          0x17    // CRTC Mode Control register
#define CRTC_LINE_COMP          0x18    // Line Compare register

// Graphics Controller register indices
#define GFX_SET_RST         0
#define GFX_EN_SET_RST      1
#define GFX_COLOR_CMP       2
#define GFX_DATA_ROT        3
#define GFX_RD_MAP_SEL      4
#define GFX_MODE            5
#define GFX_MISC            6
#define GFX_COLOR_DC        7
#define GFX_BIT_MASK        8

// Sequencer register indices
#define VGA_SEQ_RST         0       // Reset register
#define VGA_SEQ_CLK_MODE    1       // Clocking Mode register
#define VGA_SEQ_MAP_MASK    2       // Map Mask register
#define VGA_SEQ_CHAR_MAP    3       // Character Map Select register
#define VGA_SEQ_MEM_MODE    4       // Sequencer Memory Mode register

// Attribute Controller register indices
//   pallete registers (Index 00-0Fh)
//   PAS bit (bit 5) is already set in all the values below
#define VGA_ATTR_MODE_CTRL  0x30    // Attribute Mode Control register
#define VGA_ATTR_OS_COLOR   0x31    // Overscan Color register
#define VGA_ATTR_COLORP_EN  0x32    // Color Plane Enable register
#define VGA_ATTR_HORIZ_PAN  0x33    // Horizontal Pixel Panning register
#define VGA_ATTR_COLOR_SEL  0x34    // Color Select register

#define VGA_VIDEO_MEM       0xA0000 // beginning of video memory window
