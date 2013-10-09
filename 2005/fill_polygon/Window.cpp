#include "aj.h"

int init_point(int num,POINT * array_point);

FILE *fp = NULL;

// the main Wodows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX	windowClass;		// window class
	HWND		hWnd;				// window handle
	MSG			msg;				// message

	// fill out the window class structure
	windowClass.cbSize				= sizeof(WNDCLASSEX);
	windowClass.style				= CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc			= WndProc;
	windowClass.cbClsExtra			= 0;
	windowClass.cbWndExtra			= 0;
	windowClass.hInstance			= hInstance;
	windowClass.hIcon				= LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hIconSm				= LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor				= LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground		= (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName		= NULL;		// no menu
	windowClass.lpszClassName		= "view";

	// register the windows class
	if (!RegisterClassEx(&windowClass))
		return 0;

	// class registered, so now create our indow
	hWnd = CreateWindowEx(NULL,				// extended style
		"view",								// class name
		WINDOW_CAPTION,						// app name
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,	// style
		CW_USEDEFAULT, CW_USEDEFAULT,		// x, y coordinate
		WINDOW_WIDTH, WINDOW_HEIGHT,		// width, height
		NULL,								// handle to parent
		NULL,								// handle to menu
		hInstance,							// application instance
		NULL);								// no extra params

	// check if window creation failed (hwnd would equal MULL)
	if (!hWnd)	return 0;
	ShowWindow(hWnd, SW_SHOW);				// display the window
	UpdateWindow(hWnd);						// update the window

	// main message loop
	while (GetMessage(&msg, hWnd, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// the Windows Procedure event Handler
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC			hDC = 0;	// device context
	PAINTSTRUCT	ps;
	Color **screen_buffer;
	static double STime,ETime,Time;
	static char Time_cha[30];
	int i;
	POINT *array_point = NULL;
	int LOOP = 1;
	int point_num;
	    
	switch (msg) {
	case WM_CREATE:
		fp = fopen("point.txt","r");
		fscanf(fp,"%d\n",&LOOP);
		fscanf(fp,"%d\n",&point_num);
		array_point = (POINT *)malloc(sizeof(POINT)*(point_num+1));
		init_point(point_num,array_point);		
		screen_buffer = (Color **)malloc(sizeof(Color *) * WINDOW_WIDTH);
		for(i = 0;i < WINDOW_WIDTH;i++)
		{
			screen_buffer[i] = (Color *)malloc(sizeof(Color) * WINDOW_HEIGHT);
		}
		clearbuffer(WINDOW_WIDTH,WINDOW_HEIGHT,screen_buffer);
		STime = GetTickCount();
		for(i = 0;i < LOOP;i++)
		{
			Render(WINDOW_WIDTH,WINDOW_HEIGHT,screen_buffer,point_num,array_point);
		}
		ETime = GetTickCount();
		Time = (ETime - STime)/1000.0;
		sprintf(Time_cha,"Time : %15.3lf",Time);
		ImageDIB(WINDOW_WIDTH,WINDOW_HEIGHT,screen_buffer);
		DrawDIB(hDC);	
		TextOut(hDC,0,0,Time_cha,(int)strlen(Time_cha));
		return 0;

	//case WM_DESTROY:
		//	PostQuitMessage(0);
		//	return 0;

	case WM_CLOSE:
		//PostQuitMessage(0);
		exit(0);
		return 0;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		DrawDIB(hDC);	
		TextOut(hDC,0,0,Time_cha,(int)strlen(Time_cha));		
		EndPaint(hWnd, &ps);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void clearbuffer(int width,int height,Color **buffer)
{
	int i,j;
	for(j = 0;j<height;j++)
	{
		for(i = 0;i<width;i++)
		{
			buffer[i][j].R = 255;
			buffer[i][j].G = 255;
			buffer[i][j].B = 255;
		}
	}
}

int init_point(int num,POINT * array_point)
{
	int i;
	
	for(i = 0;i < num;i++)
	{
		int x,y;
		fscanf(fp,"%d\t%d\n",&x,&y);
		array_point[i].x = x;
		array_point[i].y = y;
	}
	array_point[num].x = array_point[0].x;
	array_point[num].y = array_point[0].y;
	fclose(fp);
	return num;
}