#ifndef __CHIP_8_DEFINITIONS
#define __CHIP_8_DEFINITIONS

const int NUMBER_OF_REGS = 16;
const int NUMBER_OF_RPL_FLAGS = 8;
const int STACK_SIZE = 16;
const int FONT_SIZE = 80;
const int MEMORY_SIZE = 4096;
const int PROGRAM_START = 0x200;
const int GFX_SIZE = 64*32;
const int MAX_OFFSET = 0;
typedef u16 (*opfunc)(u16) opfunc;
struct optable
{
	u8 optype;
	opfunc function;
};
u16 op0(u16);
u16 op_jump_NNN(u16);
u16 op_call_NNN(u16);
u16 op_skip_VX_equal_NN(u16);
u16 op_skip_VX_not_equal_NN(u16);
u16 op_skip_VX_equal_VY(u16);
u16 op_load_VX_VY(u16);
u16 op_add_VX_NN(u16);
u16 op_arith_VX_VY(u16);
u16 op_skip_VX_not_equal_VY(u16);
u16 op_load_I_NNN(u16);
u16 op_jump_V0_NNN(u16);
u16 op_load_rand_NN(u16);
u16 op_draw_VX_VY_N(u16);
u16 op_skip_VX(u16);
u16 opF(u16);
optable operations[0X10] =
{
	{0x0, &op0},
	{0x1, &op_jump_NNN},
	{0x2, &op_call_NNN},
	{0x3, &op_skip_VX_equal_NN},
	{0x4, &op_skip_VX_not_equal_NN},
	{0x5, &op_skip_VX_equal_VY},
	{0x6, &op_load_VX_VY},
	{0x7, &op_add_VX_NN},
	{0x8, &op_arith_VX_VY},
	{0x9, &op_skip_VX_not_equal_VY},
	{0xA, &op_load_I_NNN},
	{0xB, &op_jump_V0_NNN},
	{0xC, &op_load_rand_NN},
	{0xD, &op_draw_VX_VY_N},
	{0xE, &op_skip_VX},
	{0xF, &opF},
};

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
const int OPCODE_LOAD = 0xF;
	const int OPCODE_LOAD_MOV = 0x07;
	const int OPCODE_LOAD_WAIT_KEY = 0x0A;
	const int OPCODE_LOAD_SET_DELAY = 0x15;
	const int OPCODE_LOAD_SET_SOUND = 0x18;
	const int OPCODE_LOAD_ADD_VX_I = 0x1E;
	const int OPCODE_LOAD_SET_I = 0x29;
	const int OPCODE_LOAD_SET_I_SCHIP10 = 0x30;
	const int OPCODE_LOAD_STORE_DECIMAL = 0x33;
	const int OPCODE_LOAD_STORE_REG = 0x55;
	const int OPCODE_LOAD_RETRIEVE_REG = 0x65;
	const int OPCODE_LOAD_STORE_REG_FLAGS_SCHIP10 = 0x75;
	const int OPCODE_LOAD_RETRIEVE_REG_FLAGS_SCHIP10 = 0x85;

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