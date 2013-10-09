#include"aj.h"

int					g_iDIBSize = 0;
UCHAR		*g_pDIB = NULL;
BITMAPINFO			BMPInfo;

void DrawDIB(HDC hDC)
{
	if (!g_pDIB) return;

	::SetStretchBltMode(hDC, COLORONCOLOR);
	::SetDIBitsToDevice(hDC, 0, 0, BMPInfo.bmiHeader.biWidth, BMPInfo.bmiHeader.biHeight, 0, 0,
						0, (WORD)BMPInfo.bmiHeader.biHeight, g_pDIB, &BMPInfo, DIB_RGB_COLORS);
}
 
void ImageDIB(int width,int height,Color **buffer)
{
    int x, y, /*ry, rx, i, j,*/ lineSize;

	lineSize = width * 3;

	if (g_pDIB != NULL) {
		if (g_iDIBSize < lineSize*height) {
			free(g_pDIB);
			g_pDIB = NULL;
		}
	}

	if (g_pDIB == NULL) {
		g_pDIB = (UCHAR *)malloc(sizeof(UCHAR) * lineSize * height);
		g_iDIBSize = lineSize*height;
		if (g_pDIB == NULL) {
			g_iDIBSize = 0;
			return;
		}
	}

	UCHAR *LPtr = g_pDIB;

    for (y = height-1; y >= 0; y--) { 
		for (x = 0; x < width; x++) {
			*LPtr++ = buffer[x][y].B;
			*LPtr++ = buffer[x][y].G;
			*LPtr++ = buffer[x][y].R;
		}
	}

	BMPInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BMPInfo.bmiHeader.biWidth = width;
	BMPInfo.bmiHeader.biHeight = height;
	BMPInfo.bmiHeader.biPlanes = 1;
	BMPInfo.bmiHeader.biBitCount = 24;
	BMPInfo.bmiHeader.biCompression = BI_RGB;
	BMPInfo.bmiHeader.biSizeImage = width * height * 3;
	BMPInfo.bmiHeader.biXPelsPerMeter = 0;
	BMPInfo.bmiHeader.biYPelsPerMeter = 0;
	BMPInfo.bmiHeader.biClrUsed = 0;
	BMPInfo.bmiHeader.biClrImportant = 0;
}


