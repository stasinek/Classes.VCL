//---------------------------------------------------------------------------
#include "tsoft_Context.h"
//---------------------------------------------------------------------------

__stdcall ts::Bitmap::Bitmap(HDC ahdc)
{
Info = (BITMAPINFO*)LocalAlloc(LPTR,sizeof(BITMAPINFOHEADER)
                    + sizeof(RGBQUAD) * (2^32));
Info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
Info->bmiHeader.biWidth = 1;
Info->bmiHeader.biHeight = 1;
Info->bmiHeader.biPlanes = 1;
Info->bmiHeader.biBitCount = 32;
Info->bmiHeader.biCompression = BI_RGB;
Info->bmiHeader.biSizeImage = 0;
Info->bmiHeader.biXPelsPerMeter = 96;
Info->bmiHeader.biYPelsPerMeter = 96;
Info->bmiHeader.biClrUsed = 0;
Info->bmiHeader.biClrImportant = 0;
Handle = CreateDIBSection(ahdc,Info,DIB_RGB_COLORS,&lpBits,NULL,0);
Hdc = ahdc;
//Handle = CreateCompatibleBitmap(hdc,Bitmap.Info->bmiHeader.biWidth,Bitmap.Info->bmiHeader.biHeight);
}
//---------------------------------------------------------------------------

void __stdcall ts::Bitmap::Resize(int width, int height)
{
if (Info->bmiHeader.biWidth==width && Info->bmiHeader.biHeight==height)
	 return;
Info->bmiHeader.biSizeImage = height * width * 4;
Info->bmiHeader.biWidth = width;
Info->bmiHeader.biHeight = height;
DeleteObject(Handle);
Handle = CreateDIBSection(Hdc,Info,DIB_RGB_COLORS,&lpBits,NULL,0);
}
//---------------------------------------------------------------------------

__stdcall ts::Bitmap::~Bitmap()
{
DeleteObject(Handle);
LocalFree(Info);
}
//---------------------------------------------------------------------------

void ts::Bitmap::Save(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{
    HANDLE hf;                  /* file handle */
    BITMAPFILEHEADER hdr;       /* bitmap file-header */
    PBITMAPINFOHEADER pbih;     /* bitmap info-header */
    LPBYTE lpBits;              /* memory pointer */
    DWORD dwTotal;              /* total count of bytes */
    DWORD cb;                   /* incremental count of bytes */
    BYTE *hp;                   /* byte pointer */

    DWORD dwTmp;
    pbih = (PBITMAPINFOHEADER) pbi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
    if (!lpBits); // TODO: print error if memory could not be allocated

    /* Retrieve the color table (RGBQUAD array) and the bits
     * (array of palette indices) from the DIB. */
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS)); // TODO: print error if cant allocate

    /* Create the .BMP file. */
    hf = CreateFile(pszFile,
                   GENERIC_READ | GENERIC_WRITE,
                   (DWORD) 0,
                   (LPSECURITY_ATTRIBUTES) NULL,
                   CREATE_ALWAYS,
                   FILE_ATTRIBUTE_NORMAL,
                   (HANDLE) NULL);

    if (hf == INVALID_HANDLE_VALUE); // TODO: abort if file can't be created
    hdr.bfType = 0x4d42; /* 0x42 = "B" 0x4d = "M" */

    /* Compute the size of the entire file. */
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER)
                          + pbih->biSize
                          + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    /* Compute the offset to the array of color indices. */
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER)
                    + pbih->biSize
                    + pbih->biClrUsed * sizeof (RGBQUAD);

    /* Copy the BITMAPFILEHEADER into the .BMP file. */
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),(LPDWORD) &dwTmp, (LPOVERLAPPED) NULL));

    /* Copy the BITMAPINFOHEADER and RGBQUAD array into the file. */
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)
                  + pbih->biClrUsed * sizeof (RGBQUAD),
                  (LPDWORD) &dwTmp,
                  (LPOVERLAPPED) NULL));

    /* Copy the array of color indices into the .BMP file. */
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    while (cb > 10*1024*1024)  {
            if (!WriteFile(hf, (LPSTR) hp, (int) 10*1024*1024,
                          (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL));
            cb -= 10*1024*1024;
            hp += 10*1024*1024;
    }
    if (!WriteFile(hf, (LPSTR) hp, (int) cb,(LPDWORD) &dwTmp, (LPOVERLAPPED) NULL));

    /* Close the .BMP file. */
    if (!CloseHandle(hf)); // TODO: print if error ocurred?
    GlobalFree((HGLOBAL)lpBits); // free memory
}
//---------------------------------------------------------------------------

__stdcall ts::Context::Context()
{
// create our DIB section and select the bitmap into the dc
HDC thdc = GetDC(0); Hdc = CreateCompatibleDC(thdc);
ReleaseDC(0,thdc);
Bitmap = new ts::Bitmap(Hdc); SelectObject(Hdc,Bitmap->Handle);
}
//---------------------------------------------------------------------------

__stdcall ts::Context::~Context()
{
DeleteDC(Hdc); delete Bitmap;
}
//---------------------------------------------------------------------------

void __stdcall ts::Context::Resize(int width, int height)
{
Bitmap->Resize(width, height);
SelectObject(Hdc,Bitmap->Handle);
}
//---------------------------------------------------------------------------

void __stdcall ts::Context::Uncolorize(void)
{
for (__int32 data, suma, I = 0, E = Bitmap->Info->bmiHeader.biSizeImage>>2; I < E; I++)
    {
     data = ((__int32*)(Bitmap->lpBits))[I];

     suma = 0;
     suma += data & 0x000000FF;
     data  = data>>8;
     suma += data & 0x000000FF;
     data  = data>>8;
     suma += data & 0x000000FF;

     suma = suma / 3;

     data  = 0;
     data += suma & 0x000000FF;
     data  = data<<8;
     data += suma & 0x000000FF;
     data  = data<<8;
     data += suma & 0x000000FF;

     ((__int32*)(Bitmap->lpBits))[I] = data;
    }
}
//---------------------------------------------------------------------------
