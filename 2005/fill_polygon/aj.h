#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

typedef struct color {
	UCHAR		B;
	UCHAR		G;
	UCHAR		R;
}Color;

void ImageDIB(int width,int height,Color **buffer);
void DrawDIB(HDC hDC);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Render(int width,int height,Color **buffer,int num,POINT * array_point);


void clearbuffer(int width,int height,Color **buffer);

#define WINDOW_CAPTION	"View Window"
#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480

