package com.lpfaint99;

public class Chip8core {
////////////////////////////
 int HI_BYTE(int opcode) {return (opcode >> 8 & 0xFF);}
 int HI_UPPER_NIBBLE(int opcode) { return (HI_BYTE(opcode)>>4) & 0xF; }
 int HI_LOWER_NIBBLE(int opcode) { return HI_BYTE(opcode) & 0xF; }
 int LO_BYTE(int opcode) {return (opcode & 0xFF);}
 int LO_UPPER_NIBBLE(int opcode) { return LO_BYTE(opcode)>>4 & 0xF; }
 int LO_LOWER_NIBBLE(int opcode) { return LO_BYTE(opcode) & 0xF; }
 int MEMORY_ADDRESS(int opcode) {return opcode & 0xFFF;}
public static final int NUMBER_OF_REGS = 16;
public static final int STACK_SIZE = 16;
public static final int FONT_SIZE = 80;
public static final int MEMORY_SIZE = 4096;
public static final int PROGRAM_START = 0x200;
public static final int GFX_SIZE = 64*32;


public static final int OPCODE_ZERO = 0x0;
public static final int OPCODE_ZERO_RETURN = 0xEE;
public static final int OPCODE_ZERO_CLEAR_SCREEN = 0xE0;
public static final int SCHIP11_OPCODE_ZERO_SCROLL_DOWN = 0xC0;
public static final int SCHIP11_OPCODE_ZERO_SCROLL_RIGHT = 0xFB;
public static final int SCHIP11_OPCODE_ZERO_SCROLL_LEFT = 0xFC;
public static final int SCHIP10_OPCODE_ZERO_EXIT = 0xFD;
public static final int SCHIP10_OPCODE_ZERO_DISABLE_EXT_SCN = 0xFE;
public static final int SCHIP10_OPCODE_ZERO_ENABLE_EXT_SCN = 0xFF;
public static final int OPCODE_JUMP = 0x1;
public static final int OPCODE_CALL = 0x2;
public static final int OPCODE_JUMPEQ = 0x3;
public static final int OPCODE_JNE = 0x4;
public static final int OPCODE_JEQ_REG = 0x5;
public static final int OPCODE_SETREG = 0x6;
public static final int OPCODE_ADD = 0x7;
public static final int OPCODE_8 = 0x8;
public static final int OPCODE_8_MOV = 0;
public static final int OPCODE_8_OR = 1;
public static final int OPCODE_8_AND = 2;
public static final int OPCODE_8_XOR = 3;
public static final int OPCODE_8_ADD = 4;
public static final int OPCODE_8_SUB_X_Y = 5;
public static final int OPCODE_8_RSHIFT = 6;
public static final int OPCODE_8_SUB_Y_X = 7;
public static final int OPCODE_8_LSHIFT = 0xE;
public static final int OPCODE_JNE_REG = 0x9;
public static final int OPCODE_SET_I = 0xA;
public static final int OPCODE_JUMP_OFFSET = 0xB;
public static final int OPCODE_RAND = 0xC;
public static final int OPCODE_SPRITE = 0xD;
public static final int OPCODE_JUMP_KEY = 0xE;
public static final int OPCODE_JUMP_PRESSED = 0x9E;
public static final int OPCODE_JUMP_NOT_PRESSED = 0xA1;
public static final int OPCODE_LOAD = 0xF;
public static final int OPCODE_LOAD_MOV = 0x07;
public static final int OPCODE_LOAD_WAIT_KEY = 0x0A;
public static final int OPCODE_LOAD_SET_DELAY = 0x15;
public static final int OPCODE_LOAD_SET_SOUND = 0x18;
public static final int OPCODE_LOAD_ADD_VX_I = 0x1E;
public static final int OPCODE_LOAD_SET_I = 0x29;
public static final int OPCODE_LOAD_SET_I_SCHIP10 = 0x30;
public static final int OPCODE_LOAD_STORE_DECIMAL = 0x33;
public static final int OPCODE_LOAD_STORE_REG = 0x55;
public static final int OPCODE_LOAD_RETRIEVE_REG = 0x65;
public static final int OPCODE_LOAD_STORE_REG_FLAGS_SCHIP10 = 0x75;
public static final int OPCODE_LOAD_RETRIEVE_REG_FLAGS_SCHIP10 = 0x85;
public static final int chip8_fontset[] = { 
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

	public Chip8core()
	{
		initialize();
	}
	private void initialize()
	{isloaded = false;
		dirty = true;
		beep = false;
		PC     = PROGRAM_START;  // Program counter starts at 0x200
		opcode = 0;      // Reset current opcode
		I      = 0;      // Reset index register
		sp     = 0;      // Reset stack pointer
	 
		// Clear display
		gfx = new byte[GFX_SIZE];
		clear_screen();
		pressedkey = new byte[16];
		for (int i = 0; i < 16; ++i)
			pressedkey[i] = 0;
		// Clear stack
		stack = new int[STACK_SIZE];
		for (int i = 0; i < STACK_SIZE; ++i)
			stack[i] = 0;
		// Clear registers V0-VF
		reg = new byte[NUMBER_OF_REGS];

		for (int i = 0; i < NUMBER_OF_REGS; ++i)
			reg[i] = 0;
		// Clear memory
		memory = new byte[MEMORY_SIZE];
		for (int i = 0; i < MEMORY_SIZE; ++i)
			memory[i] = 0;
	 
		// Load fontset
		for(int i = 0; i < FONT_SIZE; ++i)
			memory[i] = (byte) chip8_fontset[i];
	 
		// Reset timers
		delay_timer = 0;
		sound_timer = 0;
	}

	public int load_rom(byte []buffer, int bufferSize)
	{
		for(int i = 0; i < bufferSize; ++i)
			memory[i + PROGRAM_START] = buffer[i];
		isloaded = true;
		return 1;
	}
	
	public boolean isdirty()
	{
		if (dirty)
		{
			dirty = false;
			return true;
		}
		return false;
	}

	public boolean needsBeep()
	{
		if (beep)
		{
			beep = false;
			return true;
		}
		return false;
	}
	private void keyevent(int key, boolean released)
	{
		byte value = (byte) (released ? 0 : 1);
		if (!released)
		{
			if (key==27)
				System.exit(0);
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

	public void keypressed(int key)
	{
		keyevent(key, false);
	}

	public void keyreleased(int key)
	{
		keyevent(key, true);
	}

	public void runCycle()
	{
		int opcode = memory[PC] << 8 | memory[PC+1] & 0xFF;
		PC = runOP(opcode);
	}
	byte[] getGraphics() { return gfx;}

	byte gfx[];//GFX_SIZE];
	void debugRender(){}
	boolean dirty;
	byte pressedkey[];//16];
	boolean beep;
	int runOP(int opcode)
	{
		//printf("running opcode %x\n", opcode);
		int NPC = PC + 2;
		byte nibble = (byte) HI_UPPER_NIBBLE(opcode);
		switch (nibble)
		{
			
		case OPCODE_ZERO:
			runOP0(opcode);
			break;
		case OPCODE_JUMP:
		{
			// 1NNN	Jumps to address NNN.
			int address = MEMORY_ADDRESS(opcode);
			NPC = address;
		}
			break;
		case OPCODE_CALL:
		{
			// 2NNN	Calls subroutine at NNN.
			int address = MEMORY_ADDRESS(opcode);
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
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			byte constant = (byte) LO_BYTE(opcode);
			if (reg[X] == constant)
				NPC = NPC + 2;//TODO	
		}
			break;
		case OPCODE_JNE:
		{
		//4XNN	Skips the next instruction if VX doesn't equal NN.
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			byte constant = (byte) LO_BYTE(opcode);
			if (reg[X] != constant)
				NPC = NPC + 2;//TODO	
		}
			break;
		case OPCODE_JEQ_REG:
		{
		//5XY0	Skips the next instruction if VX equals VY.
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			byte Y = (byte) LO_UPPER_NIBBLE(opcode);
			if (reg[X] == reg[Y])
				NPC = NPC + 2;//TODO
		}
			break;
		case OPCODE_JNE_REG:
		{
		// 9XY0	Skips the next instruction if VX doesn't equal VY.
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			byte Y = (byte) LO_UPPER_NIBBLE(opcode);
			if (reg[X] != reg[Y])
				NPC = NPC + 2;//TODO	
			break;
		}
		case OPCODE_SETREG:
		{
		// 6XNN	Sets VX to NN.
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			reg[X] = (byte) LO_BYTE(opcode);
		}
			break;
		case OPCODE_ADD:
		{
		//7XNN	Adds NN to VX.
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			reg[X] += LO_BYTE(opcode);
		}
			break;
		case OPCODE_8:
		{
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			byte Y = (byte) LO_UPPER_NIBBLE(opcode);

			switch (opcode & 0xF)
			{
			case OPCODE_8_MOV:
			// 8XY0	Sets VX to the value of VY.
				reg[X] = reg[Y];
				break;
			case OPCODE_8_OR:		
			// 8XY1	Sets VX to VX or VY.
				reg[X] = (byte) (reg[X] | reg[Y]);
				break;
			case OPCODE_8_AND:
			// 8XY2	Sets VX to VX and VY.
				reg[X] = (byte) (reg[X] & reg[Y]);
				break;
			case OPCODE_8_XOR:
			// 8XY3	Sets VX to VX xor VY.
				reg[X] = (byte) (reg[X] ^ reg[Y]);
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
				reg[X] = (byte) (reg[X] + reg[Y]);
			}
				break;
			case OPCODE_8_SUB_X_Y:
			// 8XY5	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			{
				byte sum = (byte) (reg[X] - reg[Y]);
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
				reg[0xF] = (byte) (reg[X] & 1);
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
				reg[X] = (byte) (reg[Y] - reg[X]);
			}
				break;
			case OPCODE_8_LSHIFT:
			// 8XYE	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.[2]
				reg[0xF] = (byte) (reg[X]>>7 & 1);
				reg[X] <<= 1;
				break;
			default:
				//printf("invalid op %x", opcode);
			}
		}
			break;
		case OPCODE_SET_I:
		{
			// ANNN	Sets I to the address NNN.
			int address = MEMORY_ADDRESS(opcode);
			I = address; //TODO
		}
			break;
		case OPCODE_JUMP_OFFSET:
		{
			//BNNN	Jumps to the address NNN plus V0.
			int address = MEMORY_ADDRESS(opcode);
			
			NPC = reg[0] + address; //TODO
		}
			break;
		case OPCODE_RAND:
		{
		//CXNN	Sets VX to a random number and NN.
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			int constant = LO_BYTE(opcode);
			reg[X] = (byte) (constant & (int)Math.random()%0xFF); // TODO
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
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			byte Y = (byte) LO_UPPER_NIBBLE(opcode);
			byte N = (byte) LO_LOWER_NIBBLE(opcode);
			if (N == 0)
				;//printf("implementme");
			draw_sprite(reg[X], reg[Y], N);
		}
			break;
		case OPCODE_JUMP_KEY:
		{
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			boolean keyPressed = pressedkey[reg[X]] != 0; // check this key
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
				//printf("invalid op %x", opcode);
			}
		}
			break;
		case OPCODE_LOAD:
		{
			byte X = (byte) HI_LOWER_NIBBLE(opcode);
			switch (LO_BYTE(opcode))
			{
			case OPCODE_LOAD_MOV:
			//FX07	Sets VX to the value of the delay timer.
				reg[X] = delay_timer;
				break;
			case OPCODE_LOAD_WAIT_KEY:
			{
				// FX0A	A key press is awaited, and then stored in VX.
				boolean keypressed = false;
				for (int i = 0; i < 16; ++i)
				{
					if (pressedkey[i] == 1)
					{
						reg[X] = (byte) i;
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
				if ((0xFFF - I) < reg[X])
					reg[0xF] = 1;
				else
					reg[0xF] = 0;
				I = I + reg[X];
				break;
			case OPCODE_LOAD_SET_I:
			// FX29	Sets I to the location of the sprite for the character in VX.
			// Characters 0-F (in hexadecimal) are represented by a 4x5 font.
				I = reg[X] * 5;
				break;
			case OPCODE_LOAD_SET_I_SCHIP10:
			// FX29	Sets I to the location of the sprite for the character in VX.
			// Characters 0-F (in hexadecimal) are represented by a 4x5 font.
				///I = int(reg[X]) * 5;
				//printf("implementme");
				break;

			case OPCODE_LOAD_STORE_DECIMAL:
			// FX33	Stores the Binary-coded decimal representation of VX,
			// with the most significant of three digits at the address in I,
			// the middle digit at I plus 1, and the least significant digit at I plus 2.
			{
				int value = reg[X];
				memory[I] = (byte) (value / 100);
				memory[I + 1] = (byte) ((value / 10) % 10);
				memory[I + 2] = (byte) (value % 10);
			}
				break;
			case OPCODE_LOAD_STORE_REG:
			// FX55	Stores V0 to VX in memory starting at address I.[4]
				for (int i = 0; i <= X; ++i)
				{
					memory[I + i] = reg[i];
				}
				break;
			case OPCODE_LOAD_RETRIEVE_REG:
			// FX65	Fills V0 to VX with values from memory starting at address I.[4]
				for (int i = 0; i <= X; ++i)
				{
					reg[i] = memory[I + i];
				}
				I = I + X + 1;
				break;
			
			case OPCODE_LOAD_STORE_REG_FLAGS_SCHIP10:
			//FX75
			case OPCODE_LOAD_RETRIEVE_REG_FLAGS_SCHIP10:
			//FX85
				//printf("implementme");
				break;
			default:
				//printf("invalid op %x", opcode);
			}
		}
		default:
				//printf("invalid op %x", opcode);
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

	
	private void clear_screen()
	{
		dirty = true;
		for (int i = 0; i < GFX_SIZE; ++i)
			gfx[i] = 0;
	}

	void draw_sprite(byte x, byte y, byte N)
	{
		dirty = true;
		reg[0xF] = 0;
		byte pixel;
	
		for (int yline = 0; yline < N; yline++)
		{
			pixel = memory[I + yline];
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
	}

	int opcode;
	// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	// 0x200-0xFFF - Program ROM and work RAM
	byte memory[];//MEMORY_SIZE];
	byte reg[];//NUMBER_OF_REGS];

	int I;
	int PC;
	int NPC;
	byte delay_timer;
	byte sound_timer;

	int stack[];//STACK_SIZE];
	byte sp;
	public boolean isloaded;
	void runOP0(int opcode)
	{

		switch (LO_BYTE(opcode))
		{
			case OPCODE_ZERO_CLEAR_SCREEN:
			//00E0	Clears the screen.
				clear_screen();
				break;
			case OPCODE_ZERO_RETURN:
			{
			//00EE	Returns from a subroutine.
				int ret = stack[sp];
				stack[sp] = 0;
				if (sp > 0)
					sp--;
				NPC = ret+2;
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
	

}
