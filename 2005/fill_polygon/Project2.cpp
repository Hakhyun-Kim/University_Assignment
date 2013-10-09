#include "aj.h"

inline void swap(int &a,int &b)
{
	a ^= b;
	b ^= a;
	a ^= b;
}

void DrawLine(int xs,int ys,int xe,int ye,int width,Color **buffer,Color color)
{
	if(ye < ys)
	{
		swap(xe,xs);
		swap(ye,ys);
	}
	int xi = xs;
	int xf = -(ye-ys);
	int mi = (xe-xs)/(ye-ys);
	if(xe-xs < 0)
		mi -= 1;
	int mf = 2*( (xe-xs) - (ye-ys)*mi );

	for( int y = ys; y < ye; y++)
	{
		//buffer[xi][y] = color;
		//DrawPixel(xi,y,buffer,color);

		if(buffer[xi][y].G == color.G)	//이미 그려졌으면
		{
			buffer[xi][y].R++;	//R 값의 변경분만큼 에쥐가 겹친다.
		}
		else
		{
			buffer[xi][y] = color;
		}

		xi = xi+mi;
		xf=xf+mf;
		if(xf>0)
		{
			xi = xi+1;
			xf = xf-2*(ye-ys);
		}
	}
}

Color color = {0,0,0};

void Render(int width,int height,Color **buffer,int num, POINT * array_point)
{	
	for(int xi = 0; xi < width; xi++)
		memset(buffer[xi],0xff,height*sizeof(Color));
	Color colorLine = color;
	for (int i = 0; i < num-1; i++)
	{
		int x0 = array_point[i].x;
		int y0 = array_point[i].y;

		int x1 = array_point[i+1].x;
		int y1 = array_point[i+1].y;
		
		DrawLine(x0,y0,x1,y1,width,buffer,colorLine);

		colorLine.B+=29;
	}

	//마지막 점과 처음 점 연결
	DrawLine(array_point[num-1].x,array_point[num-1].y,array_point[0].x,array_point[0].y,width,buffer,color);

	for(int y = 0; y < height; y++)
	{	
		for(int x = 0; x < width; x++)
		{
			if(buffer[x][y].G == color.G && buffer[x][y].R % 2 == 0)
			{
				int sx = x;
				
				while(buffer[sx][y].G != buffer[++x][y].G || 
					buffer[sx][y].B == buffer[x][y].B ||
					buffer[x][y].R % 2 == 1);

				if(buffer[x][y].G != 255 && buffer[x][y].R % 2 == 1)
				{
					x--;
				}

				for(int xx = sx; xx < x; xx++)
					memset(&buffer[xx][y],color.G,sizeof(Color));				
			}
		}
	}
}

