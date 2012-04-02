#include "GL/glew.h"
#include "GL/wglew.h"
#include <windows.h>
#include <windowsx.h>
#include "chip8.h"
#include "chip8definitions.h"
#include <iostream>
#include "glut.h"
#include <MMSystem.h>
#include <CommDlg.h>
chip8 CHIP8;


void paint(int x, int y)
{
	static int modifier = 10;
	glBegin(GL_QUADS);
			glVertex3f((x * modifier) + 0.0f,     (y * modifier) + 0.0f,	 0.0f);
			glVertex3f((x * modifier) + 0.0f,     (y * modifier) + modifier, 0.0f);
			glVertex3f((x * modifier) + modifier, (y * modifier) + modifier, 0.0f);
			glVertex3f((x * modifier) + modifier, (y * modifier) + 0.0f,	 0.0f);
	glEnd();
}
void renderSprites()
{
	for (int x = 0; x < 64; ++x)
	{
		for (int y = 0; y < 32; ++y)
		{
			if (CHIP8.gfx[x+(y*64)] != 0)
				glColor3f(1.0f,1.0f,1.0f);
			else
				glColor3f(0.0f,0.0f,0.0f);
			paint(x,y);

		}
	}
}

void render(void)
{
	if (CHIP8.isdirty())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		renderSprites();
		glutSwapBuffers();    
	}
	if (CHIP8.needsBeep())
		PlaySound(L"beep.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void key_pressed(unsigned char key, int x, int y)
{
	CHIP8.keyevent(key);
}

void key_released(unsigned char key, int x, int y)
{
	CHIP8.keyevent(key, true);
}

void RunCPU (int value)
{
	value++;
	glutTimerFunc(3, RunCPU, value);
	CHIP8.runCycle();
	render();
}

void resizewindow(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);        
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

int main(int argc, char ** argv)
{
	char name[512] = {0};
	OPENFILENAMEA opf;
	opf.hwndOwner = 0;
opf.lpstrFilter = "All files\0\*.*\0\0";
opf.lpstrCustomFilter = 0;
opf.nMaxCustFilter = 0L;
opf.nFilterIndex = 1L;
opf.lpstrFile = name;
opf.nMaxFile = 255;
opf.lpstrFileTitle = 0;
opf.nMaxFileTitle=50;
opf.lpstrInitialDir = 0;
opf.lpstrTitle = "save";
opf.nFileOffset = 0;
opf.nFileExtension = 2;
opf.lpstrDefExt = "*.*";
opf.lpfnHook = NULL;
opf.lCustData = 0;
opf.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
opf.lStructSize = sizeof(OPENFILENAMEA);

	if (!GetOpenFileNameA(&opf))
		exit(0);
	
//	OpenFileDialog a = new OpenFileDialog();
	FILE* fp = fopen(opf.lpstrFile, "rb");
	fseek(fp, 0, SEEK_END);
	int buffsz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	u8 *buffer = new u8[buffsz];
	u16 size = fread(buffer, 1, buffsz, fp);
	fclose(fp);
	CHIP8.load_rom(buffer, size);

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(640,480);
	glutCreateWindow("Chip8");
	glutDisplayFunc(render);
	//glutIdleFunc(render);
    glutReshapeFunc(resizewindow); 
	glutKeyboardFunc(key_pressed);
	glutKeyboardUpFunc(key_released); 
	glutTimerFunc(0, RunCPU, 0);
	//wglSwapIntervalEXT(0);
	
	glutMainLoop();

    return 0;
}


   
	