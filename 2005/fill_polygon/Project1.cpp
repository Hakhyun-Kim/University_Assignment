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

//å�� �ִ� DDA �˰���!!
void DrawLine(int ys,int xs,int ye,int xe)	//y �� x�� �ٲپ �׸���.->y������ ��ĵ�Ҽ� �ֵ���..!!
{
	if(ye < ys)
	{
		swap(xe,xs);
		swap(ye,ys);
	}
	int xi = xs;
	int xf = -(ye-ys);
	int mi;
	if( ye-ys == 0 )	//�簢������ �׸��� 0 �̵Ǵ� ��� ����.
		mi = 0;
	else
		mi = (xe-xs)/(ye-ys);
	if(xe-xs < 0)	//�������� �׻� ����� �Ǿ�� �ϹǷ�><�ФФ�
		mi -= 1;
	int mf = 2*( (xe-xs) - (ye-ys)*mi );

	for( int y = ys; y < ye; y++)
	{
		lineBuffer[y][xi]++;	//y �� x�� �ٲپ �׸���.->y������ ��ĵ�Ҽ� �ֵ���..!!

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
	memset(lineBuffer,0,width*height);//��ĵ ���� ����Ʈ�� ������ �迭 �ʱ�ȭ.!!

	for (int i = 0; i < num-1; i++)
	{
		int xs = array_point[i].x;
		int ys = array_point[i].y;

		int xe = array_point[i+1].x;
		int ye = array_point[i+1].y;
		
		DrawLine(xs,ys,xe,ye);		
	}

	//������ ���� ó�� �� ����
	if( array_point[num-1].x != array_point[0].x &&
		array_point[num-1].x != array_point[0].y)
		DrawLine(array_point[num-1].x,array_point[num-1].y,array_point[0].x,
			array_point[0].y);

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{	
			if( *(int*)(&lineBuffer[x][y]) == 0)	//4���� ��� 0 �ΰ��.
				y+=3;
			else if(*(int*)(&lineBuffer[x][y]) == 0x1111)	//4���� ��� 1 �ΰ��
				y+=3;
			else if(lineBuffer[x][y] % 2 == 1)
			{
				int sy = y;
				
				while(lineBuffer[x][++y] % 2 == 0)	//ä�� ������ ã�´�.
				{
					if(*(int*)(&lineBuffer[x][y+1]) == 0)
						y+=4;
				}

				memset(&buffer[x][sy],0,(y-sy)*sizeof(Color));
			}
		}
	}
}

