#ifndef __tsoft_Context_HDR__
#define __tsoft_Context_HDR__
//---------------------------------------------------------------------
#include <windows.h>
#include <winuser.h>
//---------------------------------------------------------------------------
namespace ts
{
class Bitmap {
HDC Hdc;
      public:
      	HBITMAP Handle;
	BITMAPINFO *Info;
	LPVOID lpBits;

     __stdcall  Bitmap(HDC ahdc);
void __stdcall  Resize		 (int width, int height);
void Save(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
                  HBITMAP hBMP, HDC hDC);
     __stdcall ~Bitmap();
  };

class Context
{
//---------------------------------------------------------------------------
private:
//---------------------------------------------------------------------------
public:
//---------------------------------------------------------------------------
     __stdcall  Context();
     __stdcall ~Context();
//---------------------------------------------------------------------------
void __stdcall  Resize		 (int width, int height);
void __stdcall  Uncolorize	 ();
//---------------------------------------------------------------------------
ts::Bitmap *Bitmap;
HDC Hdc;
//---------------------------------------------------------------------------
};
//---------------------------------------------------------------------------
}

#endif
