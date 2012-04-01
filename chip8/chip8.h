#ifndef __CHIP_8
#define __CHIP_8
#include "chip8definitions.h"


class chip8
{
public:
	chip8() { initialize();}
	~chip8() { initialize();}
	int load_rom(u8 *buffer, u16 bufferSize);
	bool isdirty();
	bool needsBeep();
	void keyevent(int key, bool released=false);
	void runCycle();
	u8* getGraphics() { return gfx;}

	u8 gfx[GFX_SIZE];
	void debugRender();
private:
	bool dirty;
	u8 pressedkey[16];
	bool beep;
	u12 runOP(u16 opcode);
	void initialize();
	void clear_screen();
	void draw_sprite(u8 X, u8 Y, u8 N);
	u16 opcode;
	// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	// 0x200-0xFFF - Program ROM and work RAM
	u8 memory[MEMORY_SIZE];
	u8 reg[NUMBER_OF_REGS];

	u12 I;
	u12 PC;

	u8 delay_timer;
	u8 sound_timer;

	u12 stack[STACK_SIZE];
	u8 sp;

};
#endif
