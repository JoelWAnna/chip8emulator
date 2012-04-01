#ifndef __CHIP_8_DEFINITIONS
#define __CHIP_8_DEFINITIONS

const int NUMBER_OF_REGS = 16;
const int STACK_SIZE = 16;
const int FONT_SIZE = 80;
const int MEMORY_SIZE = 4096;
const int PROGRAM_START = 0x200;
const int GFX_SIZE = 64*32;


const int OPCODE_ZERO = 0x0;
	const int OPCODE_ZERO_RETURN = 0xEE;
	const int OPCODE_ZERO_CLEAR_SCREEN = 0xE0;
	const int SCHIP11_OPCODE_ZERO_SCROLL_DOWN = 0xC0;
	const int SCHIP11_OPCODE_ZERO_SCROLL_RIGHT = 0xFB;
	const int SCHIP11_OPCODE_ZERO_SCROLL_LEFT = 0xFC;
	const int SCHIP10_OPCODE_ZERO_EXIT = 0xFD;
	const int SCHIP10_OPCODE_ZERO_DISABLE_EXT_SCN = 0xFE;
	const int SCHIP10_OPCODE_ZERO_ENABLE_EXT_SCN = 0xFF;
const int OPCODE_JUMP = 0x1;
const int OPCODE_CALL = 0x2;
const int OPCODE_JUMPEQ = 0x3;
const int OPCODE_JNE = 0x4;
const int OPCODE_JEQ_REG = 0x5;
const int OPCODE_SETREG = 0x6;
const int OPCODE_ADD = 0x7;
const int OPCODE_8 = 0x8;
	const int OPCODE_8_MOV = 0;
	const int OPCODE_8_OR = 1;
	const int OPCODE_8_AND = 2;
	const int OPCODE_8_XOR = 3;
	const int OPCODE_8_ADD = 4;
	const int OPCODE_8_SUB_X_Y = 5;
	const int OPCODE_8_RSHIFT = 6;
	const int OPCODE_8_SUB_Y_X = 7;
	const int OPCODE_8_LSHIFT = 0xE;
const int OPCODE_JNE_REG = 0x9;
const int OPCODE_SET_I = 0xA;
const int OPCODE_JUMP_OFFSET = 0xB;
const int OPCODE_RAND = 0xC;
const int OPCODE_SPRITE = 0xD;
const int OPCODE_JUMP_KEY = 0xE;
	const int OPCODE_JUMP_PRESSED = 0x9E;
	const int OPCODE_JUMP_NOT_PRESSED = 0xA1;
const int OPCODE_TIMER = 0xF;
	const int OPCODE_TIMER_MOV = 0x07;
	const int OPCODE_TIMER_WAIT_KEY = 0x0A;
	const int OPCODE_TIMER_SET_DELAY = 0x15;
	const int OPCODE_TIMER_SET_SOUND = 0x18;
	const int OPCODE_TIMER_ADD_VX_I = 0x1E;
	const int OPCODE_TIMER_SET_I = 0x29;
	const int OPCODE_TIMER_STORE_DECIMAL = 0x33;
	const int OPCODE_TIMER_STORE_REG = 0x55;
	const int OPCODE_TIMER_RETRIEVE_REG = 0x65;

typedef signed __int8 u8;
typedef unsigned __int16 u16;

#pragma pack(push,1)
struct u12
{

	u16 _u12 : 0xC;
	u12() {_u12=0;}
	u12(u16 opcode) { _u12 = opcode & 0xFFF;}
	friend u12 operator+ (u12 l, u8 r) { return l._u12 + r;}
	friend u12 operator+ (u8 l, u12 r) { return r._u12 + l;}
	u12& operator+= (u8 r) {_u12 += r; return *this;}
	friend u12 operator* (u12 l, u8 r) { return l._u12 * r;}
	friend u12 operator* (u8 l, u12 r) { return r._u12 * l;}
	u12& operator= (u12 l) {_u12 = l._u12; return *this;}
	u16 to_u16() {return _u12;}
};
#pragma pack(pop)
#define HI_BYTE(x) (x >> 8 & 0xFF)
#define HI_UPPER_NIBBLE(x) ((HI_BYTE(x)>>4) & 0xF)
#define HI_LOWER_NIBBLE(x) (HI_BYTE(x) & 0xF)
#define LO_BYTE(x) (x & 0xFF)
#define LO_UPPER_NIBBLE(x) (LO_BYTE(x)>>4 & 0xF)
#define LO_LOWER_NIBBLE(x) (LO_BYTE(x) & 0xF)

//inline u8 HI_BYTE(u16 opcode) {return (opcode >> 8 & 0xFF);}
//inline u8 HI_UPPER_NIBBLE(u16 opcode) { return (HI_BYTE(opcode)>>4) & 0xF; }
//inline u8 HI_LOWER_NIBBLE(u16 opcode) { return HI_BYTE(opcode) & 0xF; }
//inline u8 LO_BYTE(u16 opcode) {return (opcode & 0xFF);}
//inline u8 LO_UPPER_NIBBLE(u16 opcode) { return LO_BYTE(opcode)>>4 & 0xF; }
//inline u8 LO_LOWER_NIBBLE(u16 opcode) { return LO_BYTE(opcode) & 0xF; }

inline u12 MEMORY_ADDRESS(u16 opcode) {return u12(opcode);}
static const u8 chip8_fontset[FONT_SIZE] = { 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
#endif