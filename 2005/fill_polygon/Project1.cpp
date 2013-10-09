#include "aj.h"
//#include <assert.h>

Color color = {0,0,0};
BYTE lineBuffer[WINDOW_WIDTH][WINDOW_HEIGHT];

//fast swap..
inline void swap(int &a,int &b)
{
	a ^= b;
	b ^= a;
	a ^= b;
}

//책에 있는 DDA 알고리즘!!
void DrawLine(int ys,int xs,int ye,int xe)	//y 와 x를 바꾸어서 그린다.->y축으로 스캔할수 있도록..!!
{
	if(ye < ys)
	{
		swap(xe,xs);
		swap(ye,ys);
	}
	int xi = xs;
	int xf = -(ye-ys);
	int mi;
	if( ye-ys == 0 )	//사각형등을 그릴때 0 이되는 경우 생김.
		mi = 0;
	else
		mi = (xe-xs)/(ye-ys);
	if(xe-xs < 0)	//나머지는 항상 양수가 되어야 하므로><ㅠㅠㅠ
		mi -= 1;
	int mf = 2*( (xe-xs) - (ye-ys)*mi );

	for( int y = ys; y < ye; y++)
	{
		lineBuffer[y][xi]++;	//y 와 x를 바꾸어서 그린다.->y축으로 스캔할수 있도록..!!

		xi = xi+mi;
		xf = xf+mf;
		if(xf>0)
		{
			xi = xi+1;
			xf = xf-2*(ye-ys);
		}
	}
}


void Render(int width,int height,Color **buffer,int num, POINT * array_point)
{		
	memset(lineBuffer,0,width*height);//스캔 라인 리스트를 관리할 배열 초기화.!!

	for (int i = 0; i < num-1; i++)
	{
		int xs = array_point[i].x;
		int ys = array_point[i].y;

		int xe = array_point[i+1].x;
		int ye = array_point[i+1].y;
		
		DrawLine(xs,ys,xe,ye);		
	}

	//마지막 점과 처음 점 연결
	if( array_point[num-1].x != array_point[0].x &&
		array_point[num-1].x != array_point[0].y)
		DrawLine(array_point[num-1].x,array_point[num-1].y,array_point[0].x,
			array_point[0].y);

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{	
			if( *(int*)(&lineBuffer[x][y]) == 0)	//4점이 모두 0 인경우.
				y+=3;
			else if(*(int*)(&lineBuffer[x][y]) == 0x1111)	//4점이 모두 1 인경우
				y+=3;
			else if(lineBuffer[x][y] % 2 == 1)
			{
				int sy = y;
				
				while(lineBuffer[x][++y] % 2 == 0)	//채울 공간을 찾는다.
				{
					if(*(int*)(&lineBuffer[x][y+1]) == 0)
						y+=4;
				}

				memset(&buffer[x][sy],0,(y-sy)*sizeof(Color));
			}
		}
	}
}

