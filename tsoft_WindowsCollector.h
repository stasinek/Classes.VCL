#ifndef __tsoft_WindowsCollector_HDR__
#define __tsoft_WindowsCollector_HDR__
//---------------------------------------------------------------------------
#include <windows.h>
#include <winuser.h>
#include "tsoft_WindowsTile.h"
#include "tsoft_Context.h"
namespace ts
{
//---------------------------------------------------------------------------
#define KILL_ALL               (int)0
#define KILL_PROCESS           (int)1
//---------------------------------------------------------------------------
#define SET_PRIORITY           (int)3
#define GET_PRIORITY           (int)4
//---------------------------------------------------------------------------
#define SET_TRANSPARENCY       (int)5
#define GET_TRANSPARENCY       (int)6
//---------------------------------------------------------------------------
#define MAX_NUM_OF_WINDOWS     (int)0x00001000
#define NUM_OF_DESKS           (int)5
//---------------------------------------------------------------------------
// klasa bazowa kolektora okien
//---------------------------------------------------------------------------

#define SCALE 3

class WindowsWINDOW {
public:
HWND Hwnd;
char *Text;
bool Visible, Minimized;
RECT Rect;
     __stdcall  WindowsWINDOW   (void) { Text = new char[128]; /*Context = new ts::Context;*/ Hwnd = NULL; Visible = false; Minimized = false;}
     __stdcall ~WindowsWINDOW   (void) {  delete Text; /*delete Context*/ }
void __stdcall  Set     (HWND AHandle);
//ts::Context *Context;
void __stdcall  Capture();
void __stdcall  R	    () { Hwnd = NULL; Visible = false; Minimized = false; Text[0]='\0'; };                //usun okno z listy pulpitu
};
//---------------------------------------------------------------------------

class WindowsVIRTUALDESKTOP {
__int32 Active; __int32 Count;
public:
// funkcje na pojedynczych oknach
bool Printed;
 ts::Context *Context;
RECT Rect;
WindowsWINDOW *Window[MAX_NUM_OF_WINDOWS];
__int32  Active_Window_Index() { return Active; }
WindowsWINDOW *Active_Window() { return Window[Active_Window_Index()]; }
__int32  Windows_Count() { return Count; }

__stdcall  WindowsVIRTUALDESKTOP(void);
__stdcall ~WindowsVIRTUALDESKTOP(void);

void __stdcall  Capture     ();                //migawka pulpitu

void __stdcall  Add         (HWND hwnd);       //zarejestruj/przypisz okno na pulpicie
void __stdcall  Delete	    (HWND hwnd);                //usun okno z listy pulpitu
HWND __stdcall  Handle_at_XY  (__int32 X, __int32 Y);    //uchwyt okna na podstawie wskazania kursora
void __stdcall  R	    () { Count = 0; Printed = false; };                //usun okno z listy pulpitu

};
//---------------------------------------------------------------------------

class WindowsSCREEN {
public:
  HWND  Hwnd;
  HDC   Hdc;
  RECT  Rect;
  // zawiera bitmapy
  ts::Context *Context;
  // w³aœciciel - wirtualny pulpit
  __stdcall  WindowsSCREEN(void);
  __stdcall ~WindowsSCREEN(void);
  void __stdcall  Capture      ();
void __stdcall  R	    () {};                //usun okno z listy pulpitu
  };
//---------------------------------------------------------------------------

class WindowsCOLLECTOR
{
private:
//kolekcjonuj
static BOOL CALLBACK DoEnumWindowsProc  (HWND hwnd, LPARAM lparam);
__int32 Active, Previous;
public:
// Konstruktor i destruktor klasy
     __stdcall  WindowsCOLLECTOR   (void);
     __stdcall ~WindowsCOLLECTOR   (void);
//kolekcjonuj okna na aktywnym pulpicie
void __stdcall  Collect      ();
//przechwyæ screen pulpitu
void __stdcall  Capture      ();
//wykonaj akcje na aktywnym oknie pulpitu
__int32  __stdcall  Action              (__int32 action, HWND hwnd, __int32 lparam, __int32 hparam);
// przenieœ okna z jednego pulpitu na drugi
bool __stdcall  Move                (__int32 isrc, __int32 idsk);
// klasa przechowuje wirtualne pulpity
WindowsSCREEN *Screen;
WindowsVIRTUALDESKTOP *Virtual[5];
__int32   Active_Desktop_Index() { return Active; }
__int32 Previous_Desktop_Index() { return Previous; }
WindowsVIRTUALDESKTOP *Active_Desktop() { return Virtual[(unsigned char)Active_Desktop_Index()]; }
// aktywny pulpit, poprzednio aktywny pulpit
void __stdcall  Activate	    (__int32 inew);       //przesun okno na inny pulpit
bool __stdcall  Move	    (HWND hwnd, __int32 isrc, __int32 idsk);       //przesun okno na inny pulpit
__int32 __stdcall Error(void);
void __stdcall  R	    () {};                //usun okno z listy pulpitu
};
//---------------------------------------------------------------------------
#endif
/*
void  __stdcall set_text_size(long  asize);
long  __stdcall get_text_size(void) {return maciez.ftext_size;}
void  __stdcall set_text	  (char *atext);
char *__stdcall get_text	  (void) {return maciez.ftext;}
//------------------------------------
__declspec(property(get=get_ipos, put=set_ipos)) long ipos;
__declspec(property(get=get_node, put=set_node)) tsoftDatabase *node;
__declspec(property(get=get_text_size, put=set_text_size)) long text_size;
__declspec(property(get=get_text, put=set_text)) char *text;*/
}
