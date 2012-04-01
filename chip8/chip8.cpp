#include "chip8.h"
#include <string>
#include <cmath>

bool chip8::isdirty()
{
	if (dirty)
	{
		dirty = false;
		return true;
	}
	return false;
}

bool chip8::needsBeep()
{
	if (beep)
	{
		beep = false;
		return true;
	}
	return false;
}

void chip8::keyevent(int key, bool released)
{
	int value = released ? 0 : 1;
	if (value)
	{
		if (key==27)
			exit(0);
	}

	int number = key - '0';
	int hex = key - 'a';
	if (0 <= number && number <= 9)
	{
		pressedkey[number] = value;
	}
	if (0 <= hex && hex <= 5)
	{
		pressedkey[hex+0xa] = value;
	}
	
}

void chip8::initialize()
{
	dirty = true;
	beep = false;
	srand(0xDEADBEEF);
	PC     = PROGRAM_START;  // Program counter starts at 0x200
	opcode = 0;      // Reset current opcode
	I      = 0;      // Reset index register
	sp     = 0;      // Reset stack pointer
 
	// Clear display
	clear_screen();
	memset(pressedkey, 0, 16);
	// Clear stack
	memset(stack, 0, STACK_SIZE);
	// Clear registers V0-VF
	memset(reg, 0, NUMBER_OF_REGS);
	// Clear memory
	memset(memory, 0, MEMORY_SIZE);
 
	// Load fontset
	for(int i = 0; i < FONT_SIZE; ++i)
		memory[i] = chip8_fontset[i];
 
	// Reset timers
	delay_timer = 0;
	sound_timer = 0;
}

int chip8::load_rom(u8 *buffer, u16 bufferSize)
{
	for(int i = 0; i < bufferSize; ++i)
		memory[i + PROGRAM_START] = buffer[i];
	return 1;
}
void chip8::runCycle()
{
	u16 opcode = memory[PC.to_u16()] << 8 | memory[PC.to_u16()+1] & 0xFF;
	PC = runOP(opcode);
}

void chip8::clear_screen() 
{
	dirty = true;
	memset(gfx, 0, GFX_SIZE);
}

void chip8::draw_sprite(u8 x, u8 y, u8 N)
{
	dirty = true;
	reg[0xF] = 0;
	u8 pixel;

	for (int yline = 0; yline < N; yline++)
	{
		pixel = memory[I.to_u16() + yline];
		for(int xline = 0; xline < 8; xline++)
		{
			if((pixel & (0x80 >> xline)) != 0)
			{
				if(gfx[(x + xline + ((y + yline) * 64))] == 1)
					reg[0xF] = 1;
				gfx[x + xline + ((y + yline) * 64)] ^= 1;
			}
		}
	}
 debugRender();
	//Draws a sprite at coordinate (x, y) that has a width of 8 pixels and a height of N pixels.
	// Each row of 8 pixels is read as bit-coded (with the most significant bit of each byte displayed on the left) starting from memory location I; I value doesn't change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn't happen.

	/*	for (int i = 0; i < N; ++i)
	{
		u8 toWrite = memory[I.to_u16() + i];
		for (int j = 0; j < 8; ++j)
		{
			if ((toWrite >> (7-j)) & 1)
			{
				if (gfx[x+j+y*64] == 1)
					reg[0xF] = 1;
			}
			gfx[x+j+y*64] ^= 1;
		}
	}
	*/
}

u12 chip8::runOP (u16 opcode)
{
	//printf("running opcode %x\n", opcode);
	u12 NPC = PC + 2;
	u8 nibble = HI_UPPER_NIBBLE(opcode);
	switch (nibble)
	{
		
	case OPCODE_ZERO:
		runOP0(opcode);
		break;
	case OPCODE_JUMP:
	{
		// 1NNN	Jumps to address NNN.
		u12 address = MEMORY_ADDRESS(opcode);
		NPC = address;
	}
		break;
	case OPCODE_CALL:
	{
		// 2NNN	Calls subroutine at NNN.
		u12 address = MEMORY_ADDRESS(opcode);
		if (sp == 0xF)
			;//error
		sp++;
		stack[sp] = PC;
		NPC = address;
	}
		break;
	case OPCODE_JUMPEQ:
	{
	// 3XNN	Skips the next instruction if VX equals NN.
		u8 X = HI_LOWER_NIBBLE(opcode);
		u8 constant = LO_BYTE(opcode);
		if (reg[X] == constant)
			NPC = NPC + 2;//TODO	
	}
		break;
	case OPCODE_JNE:
	{
	//4XNN	Skips the next instruction if VX doesn't equal NN.
		u8 X = HI_LOWER_NIBBLE(opcode);
		u8 constant = LO_BYTE(opcode);
		if (reg[X] != constant)
			NPC = NPC + 2;//TODO	
	}
		break;
	case OPCODE_JEQ_REG:
	{
	//5XY0	Skips the next instruction if VX equals VY.
		u8 X = HI_LOWER_NIBBLE(opcode);
		u8 Y = LO_UPPER_NIBBLE(opcode);
		if (reg[X] == reg[Y])
			NPC = NPC + 2;//TODO
	}
		break;
	case OPCODE_JNE_REG:
	{
	// 9XY0	Skips the next instruction if VX doesn't equal VY.
		u8 X = HI_LOWER_NIBBLE(opcode);
		u8 Y = LO_UPPER_NIBBLE(opcode);
		if (reg[X] != reg[Y])
			NPC = NPC + 2;//TODO	
		break;
	}
	case OPCODE_SETREG:
	{
	// 6XNN	Sets VX to NN.
		u8 X = HI_LOWER_NIBBLE(opcode);
		reg[X] = LO_BYTE(opcode);
	}
		break;
	case OPCODE_ADD:
	{
	//7XNN	Adds NN to VX.
		u8 X = HI_LOWER_NIBBLE(opcode);
		reg[X] += LO_BYTE(opcode);
	}
		break;
	case OPCODE_8:
	{
		u8 X = HI_LOWER_NIBBLE(opcode);
		u8 Y = LO_UPPER_NIBBLE(opcode);

		switch (opcode & 0xF)
		{
		case OPCODE_8_MOV:
		// 8XY0	Sets VX to the value of VY.
			reg[X] = reg[Y];
			break;
		case OPCODE_8_OR:		
		// 8XY1	Sets VX to VX or VY.
			reg[X] = reg[X] | reg[Y];
			break;
		case OPCODE_8_AND:
		// 8XY2	Sets VX to VX and VY.
			reg[X] = reg[X] & reg[Y];
			break;
		case OPCODE_8_XOR:
		// 8XY3	Sets VX to VX xor VY.
			reg[X] = reg[X] ^ reg[Y];
			break;
		case OPCODE_8_ADD:
		{
		// 8XY4	Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
			if (reg[X] > (0xFF - reg[Y]))
			{
				reg[0xF] = 1;
			}
			else
			{
				reg[0xF] = 0;
			}
			reg[X] = reg[X] + reg[Y];
		}
			break;
		case OPCODE_8_SUB_X_Y:
		// 8XY5	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
		{
			u8 sum = reg[X] - reg[Y];
			if (reg[X] < reg[Y])
			{
				reg[0xF] = 0;
			}
			else
			{
				reg[0xF] = 1;
			}
			reg[X] = sum;
		}
			break;
		case OPCODE_8_RSHIFT:
		// 8XY6	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.[2]
			reg[0xF] = reg[X] & 1;
			reg[X] >>= 1;
			break;
		case OPCODE_8_SUB_Y_X:
		// 8XY7	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
		{
			if (reg[Y] < reg[X])
			{
				reg[0xF] = 0;
			}
			else
			{
				reg[0xF] = 1;
			}
			reg[X] = reg[Y] - reg[X];
		}
			break;
		case OPCODE_8_LSHIFT:
		// 8XYE	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.[2]
			reg[0xF] = (reg[X]>>7 & 1);
			reg[X] <<= 1;
			break;
		default:
			printf("invalid op %x", opcode);
		}
	}
		break;
	case OPCODE_SET_I:
	{
		// ANNN	Sets I to the address NNN.
		u12 address = MEMORY_ADDRESS(opcode);
		I = address; //TODO
	}
		break;
	case OPCODE_JUMP_OFFSET:
	{
		//BNNN	Jumps to the address NNN plus V0.
		u12 address = MEMORY_ADDRESS(opcode);
		
		NPC = reg[0] + address; //TODO
	}
		break;
	case OPCODE_RAND:
	{
	//CXNN	Sets VX to a random number and NN.
		u8 X = HI_LOWER_NIBBLE(opcode);
		u16 constant = LO_BYTE(opcode);
		reg[X] = constant & rand()%0xFF; // TODO
	}
		break;
	case OPCODE_SPRITE:
	{
	//DXYN	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
	// Each row of 8 pixels is read as bit-coded (with the most significant bit of each byte displayed
	// on the left) starting from memory location I;
	// I value doesn't change after the execution of this instruction. As described above, VF is set to 1
	// if any screen pixels are flipped from set to unset when the sprite is drawn,
	// and to 0 if that doesn't happen.
		u8 X = HI_LOWER_NIBBLE(opcode);
		u8 Y = LO_UPPER_NIBBLE(opcode);
		u8 N = LO_LOWER_NIBBLE(opcode);
		if (N == 0)
			printf("implementme");
		draw_sprite(reg[X], reg[Y], N);
	}
		break;
	case OPCODE_JUMP_KEY:
	{
		u8 X = HI_LOWER_NIBBLE(opcode);
		bool keyPressed = pressedkey[reg[X]] != 0; // check this key
		switch (LO_BYTE(opcode))
		{
		case OPCODE_JUMP_PRESSED:
		//EX9E	Skips the next instruction if the key stored in VX is pressed.
			if (keyPressed)
				NPC = NPC + 2;//TODO	
			break;
		case OPCODE_JUMP_NOT_PRESSED:
		//EXA1	Skips the next instruction if the key stored in VX isn't pressed.
			if (!keyPressed)
				NPC = NPC + 2;//TODO	
			break;
		default:
			printf("invalid op %x", opcode);
		}
	}
		break;
	case OPCODE_LOAD:
	{
		u8 X = HI_LOWER_NIBBLE(opcode);
		switch (LO_BYTE(opcode))
		{
		case OPCODE_LOAD_MOV:
		//FX07	Sets VX to the value of the delay timer.
			reg[X] = delay_timer;
			break;
		case OPCODE_LOAD_WAIT_KEY:
		{
			// FX0A	A key press is awaited, and then stored in VX.
			bool keypressed = false;
			for (int i = 0; i < 16; ++i)
			{
				if (pressedkey[i] == 1)
				{
					reg[X] = i;
					keypressed = true;
				}
			}
			//if (!keypressed) NPC = PC;
		}
			break;
		case OPCODE_LOAD_SET_DELAY:
		// FX15	Sets the delay timer to VX.
			delay_timer = reg[X];
			break;
		case OPCODE_LOAD_SET_SOUND:
		// FX18	Sets the sound timer to VX.
			sound_timer = reg[X];
			break;
		case OPCODE_LOAD_ADD_VX_I:
		// FX1E	Adds VX to I.[3]
			if ((0xFFF - I.to_u16()) < reg[X])
				reg[0xF] = 1;
			else
				reg[0xF] = 0;
			I = I + reg[X];
			break;
		case OPCODE_LOAD_SET_I:
		// FX29	Sets I to the location of the sprite for the character in VX.
		// Characters 0-F (in hexadecimal) are represented by a 4x5 font.
			I = u12(reg[X]) * 5;
			break;
		case OPCODE_LOAD_SET_I_SCHIP10:
		// FX29	Sets I to the location of the sprite for the character in VX.
		// Characters 0-F (in hexadecimal) are represented by a 4x5 font.
			///I = u12(reg[X]) * 5;
			printf("implementme");
			break;

		case OPCODE_LOAD_STORE_DECIMAL:
		// FX33	Stores the Binary-coded decimal representation of VX,
		// with the most significant of three digits at the address in I,
		// the middle digit at I plus 1, and the least significant digit at I plus 2.
		{	u8 value = reg[X];
			memory[I.to_u16()] = value / 100;
			memory[I.to_u16() + 1] = (value / 10) % 10;
			memory[I.to_u16() + 2] = (value % 10);
		}
			break;
		case OPCODE_LOAD_STORE_REG:
		// FX55	Stores V0 to VX in memory starting at address I.[4]
			for (int i = 0; i <= X; ++i)
			{
				memory[I.to_u16() + i] = reg[i];
			}
			break;
		case OPCODE_LOAD_RETRIEVE_REG:
		// FX65	Fills V0 to VX with values from memory starting at address I.[4]
			for (int i = 0; i <= X; ++i)
			{
				reg[i] = memory[I.to_u16() + i];
			}
			I = I + X + 1;
			break;
		
		case OPCODE_LOAD_STORE_REG_FLAGS_SCHIP10:
		//FX75
		case OPCODE_LOAD_RETRIEVE_REG_FLAGS_SCHIP10:
		//FX85
			printf("implementme");
			break;
		default:
			printf("invalid op %x", opcode);
		}
	}
	default:
			printf("invalid op %x", opcode);
		break;
	}

		if(delay_timer > 0)
		--delay_timer;

	if(sound_timer > 0)
	{
		if(sound_timer == 1)
			beep = true;
		--sound_timer;
	}
	return NPC;
}

void chip8::debugRender()
{
	return;
	// Draw
	for(int y = 0; y < 32; ++y)
	{
		for(int x = 0; x < 64; ++x)
		{
			if(gfx[(y*64) + x] == 0) 
				printf("O");
			else 
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}


void chip8::runOP0(u16 opcode)
{
	switch (HI_LOWER_NIBBLE(opcode))
	{
	case 0:
		switch (LO_BYTE(opcode))
		{
			case OPCODE_ZERO_CLEAR_SCREEN:
			//00E0	Clears the screen.
				clear_screen();
				break;
			case OPCODE_ZERO_RETURN:
			{
			//00EE	Returns from a subroutine.
				u12 ret = stack[sp];
				stack[sp] = 0;
				if (sp > 0)
					sp--;
				NPC = ret+2;
			}
				break;
			case SCHIP11_OPCODE_ZERO_SCROLL_RIGHT:
			{
			// 00FB
			}
				break;
			case SCHIP11_OPCODE_ZERO_SCROLL_LEFT:
			{
			//00FC
			}
				break;
			case SCHIP10_OPCODE_ZERO_EXIT:
			{
			//00FD
			}
				break;	
			case SCHIP10_OPCODE_ZERO_DISABLE_EXT_SCN:
			{
			//00FE
			}
				break;	
			case SCHIP10_OPCODE_ZERO_ENABLE_EXT_SCN:
			{
			//00FF
			}
				break;	
			default:
				if ((LO_BYTE(opcode) & 0xF0) == SCHIP11_OPCODE_ZERO_SCROLL_DOWN)
				{
				//00CN
				}
				else
					printf("invalid op, %x", opcode);
				break;
		}
		break;
	default:
		/*{
		//0NNN	Calls RCA 1802 program at address NNN.
			u12 address = MEMORY_ADDRESS(opcode);
			if (sp == 0xF)
				;//error
			sp++;
			stack[sp] = NPC;
			NPC = address;
		}*/
		break;
	}
}