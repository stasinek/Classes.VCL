#include "tsoft_WindowsCollector.h"
//---------------------------------------------------------------------------

__stdcall ts::WindowsCOLLECTOR::WindowsCOLLECTOR(void)
{
Screen = new WindowsSCREEN();

for (__int32 d = 0; d < NUM_OF_DESKS; d++)
    {
     Virtual[d] = new WindowsVIRTUALDESKTOP();
     Virtual[d]->Rect = Screen->Rect;
     Virtual[d]->Context->Resize(Virtual[d]->Rect.right,Virtual[d]->Rect.bottom);
    }
Active = Previous = 0;
}
//---------------------------------------------------------------------------

__stdcall ts::WindowsCOLLECTOR::~WindowsCOLLECTOR(void)
{
__int32 iact = this->Active;

for (__int32 d = 0; d < NUM_OF_DESKS; d++)
    {
     if (d!=iact)
     {
        for (__int32 i = Virtual[d]->Windows_Count()-1; i >= 0; i--)
        {
            if (Virtual[d]->Window[i]->Visible==true && IsWindow((HWND)Virtual[d]->Window[i]->Hwnd))
                {
                    ShowWindow(Virtual[d]->Window[i]->Hwnd,SW_SHOW);
                }
        }
     }
     delete Virtual[d];
    }
delete Screen;
}
//---------------------------------------------------------------------------

BOOL CALLBACK ts::WindowsCOLLECTOR::DoEnumWindowsProc(HWND hwnd, LPARAM lparam) {
ts::WindowsCOLLECTOR* lpdesk = (ts::WindowsCOLLECTOR*)lparam;
register __int32 iact = lpdesk->Active_Desktop_Index();

for (register __int32 d = 0; d < NUM_OF_DESKS; d++)
    {
     if (d!=iact)
    for (register __int32 i = lpdesk->Virtual[d]->Windows_Count()-1; i >= 0; i--)
     if ((__int64)lpdesk->Virtual[d]->Window[i]->Hwnd==(__int64)hwnd)
        {return true;
        }
    }
lpdesk->Virtual[iact]->Add(hwnd);
return true;
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsCOLLECTOR::Collect()
{
if (Active>=0 && Active<=4) Virtual[Active]->R();
else return;
//Virtual[Active]->Active_Window->Hwnd = GetForegroundWindow();
EnumWindows((WNDENUMPROC)&ts::WindowsCOLLECTOR::DoEnumWindowsProc,(LPARAM)this);
Virtual[Active]->Printed = false;
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsCOLLECTOR::Capture()
{
Sleep(100);
GetWindowRect(Screen->Hwnd,&Virtual[Active]->Rect);
Screen->Context->Resize(Virtual[Active]->Rect.right - Virtual[Active]->Rect.left,Virtual[Active]->Rect.bottom - Virtual[Active]->Rect.top);
Virtual[Active]->Context->Resize(Virtual[Active]->Rect.right - Virtual[Active]->Rect.left,Virtual[Active]->Rect.bottom - Virtual[Active]->Rect.top);
BitBlt(Virtual[Active]->Context->Hdc,
    	 0,0,Virtual[Active]->Rect.right,Virtual[Active]->Rect.bottom,
    	 Screen->Hdc,
    	 0,0,SRCCOPY);
Virtual[Active]->Printed = true;
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsCOLLECTOR::Activate(__int32 inew)
{
register __int32 iact = this->Active;
this->Collect(); this->Capture();

if (inew < 0 || inew > 4 || inew==iact) // jesli inew jest poza zakresem wyjdz
	 return;
Virtual[iact]->Context->Uncolorize(); // wyszarzenie zrzutu pulpitu

if (iact!=0)  //0 oznacza wspolny pulpit i okien ze wspolnego pulpitu nie ruszy
   {
   for (register __int32 i = 0, ei = Virtual[iact]->Windows_Count(); i < ei; i++) // ukryj stare
	if (Virtual[iact]->Window[i]->Visible==true && IsWindow(Virtual[iact]->Window[i]->Hwnd))
	   {ShowWindow(Virtual[iact]->Window[i]->Hwnd,SW_HIDE);
	   }
   }
Sleep(100);

if (inew!=0)
   {
    if (Virtual[inew]->Windows_Count() > 0)
	   {BringWindowToTop((HWND)Virtual[inew]->Active_Window()->Hwnd);
	   }
   for (register __int32 i = Virtual[inew]->Windows_Count(); --i>=0;) // przywróæ nowe
	if (Virtual[inew]->Window[i]->Visible==true && IsWindow(Virtual[inew]->Window[i]->Hwnd))
	   {ShowWindow(Virtual[inew]->Window[i]->Hwnd,SW_SHOWNA);
	   }
   }
Sleep(100);

Previous = iact; Active = inew;
this->Collect(); this->Capture();
}
//---------------------------------------------------------------------------

bool __stdcall ts::WindowsCOLLECTOR::Move(__int32 isrc, __int32 idst)
{
if (isrc==0 || isrc==idst)
	 return false;

register __int32 old_srccnt = Virtual[isrc]->Windows_Count(), old_dstcnt = Virtual[idst]->Windows_Count();
register __int32 iact = this->Active;

for (register __int32 i = 0; i < old_srccnt; i++)
    {
    Virtual[idst]->Add(Virtual[isrc]->Window[i]->Hwnd);
    }
Virtual[isrc]->R();

if (isrc==iact && idst!=0)
   {
   for (register __int32 i = old_dstcnt, cnt = old_dstcnt+old_srccnt; i < cnt; i++)
    if (Virtual[idst]->Window[i]->Visible==true && IsWindow(Virtual[idst]->Window[i]->Hwnd))
       {ShowWindow(Virtual[idst]->Window[i]->Hwnd,SW_HIDE);
       }
   }
if (idst==iact)
   {
   for (__int32 i = old_dstcnt+old_srccnt-1, cnt = old_dstcnt; i >= cnt; i--)
	if (Virtual[idst]->Window[i]->Visible==true && IsWindow(Virtual[idst]->Window[i]->Hwnd))
	   {ShowWindow(Virtual[idst]->Window[i]->Hwnd,SW_SHOWNA);
	   }
   }
return true;
}
//---------------------------------------------------------------------------

__int32 __stdcall ts::WindowsCOLLECTOR::Action(__int32 action,HWND hwnd,__int32 lparam,__int32 hparam)
{
BOOL ret; DWORD dwProcessId; HANDLE hProcess;

//--------------------------
if (action==GET_TRANSPARENCY) {
//--------------------------
if ((GetWindowLong(hwnd,GWL_EXSTYLE) & WS_EX_LAYERED)==0) return -1;
else
    return 255;
}
else
//--------------------------
if (action==SET_TRANSPARENCY) {
//--------------------------
unsigned long iexlong = GetWindowLong(hwnd,GWL_EXSTYLE) & ~(WS_EX_TRANSPARENT | WS_EX_LAYERED);
if (hparam != 0) iexlong |= WS_EX_TRANSPARENT;
if (lparam >= 0) iexlong |= WS_EX_LAYERED;

if (SetWindowLong(hwnd,GWL_EXSTYLE,iexlong)==false)
    return false;

if (lparam >= 0)
   {ret = SetLayeredWindowAttributes(hwnd,0,((100-lparam)*255)/100,LWA_ALPHA);
    return ret;
   }
}
else
//--------------------------
if (action==SET_PRIORITY) {
//--------------------------
    GetWindowThreadProcessId(hwnd,&dwProcessId);
if (GetCurrentProcessId()==dwProcessId)
   {return false;
   }
hProcess = OpenProcess(PROCESS_TERMINATE|PROCESS_SET_INFORMATION,false,dwProcessId);
if (hProcess==NULL)
   {return false;
   }
ret = SetPriorityClass(hProcess,lparam);
CloseHandle(hProcess);
return ret;
}
else
//--------------------------
if (action==KILL_PROCESS) {
//--------------------------
GetWindowThreadProcessId(hwnd, &dwProcessId);
if (GetCurrentProcessId()==dwProcessId)
   {return false;
   }
hProcess = OpenProcess(PROCESS_TERMINATE|PROCESS_SET_INFORMATION,false,dwProcessId);
if (hProcess==NULL)
   {return false;
   }
ret = TerminateProcess(hProcess,-1);
CloseHandle(hProcess);
return ret;
//--------------------------
}
//--------------------------
return 0;
}
//---------------------------------------------------------------------------

bool __stdcall ts::WindowsCOLLECTOR::Move(HWND hwnd,__int32 isrc,__int32 idsk) {
if (idsk==isrc)
    return false;
Virtual[isrc]->Delete(hwnd); Virtual[idsk]->Add(hwnd);
return true;
}
//---------------------------------------------------------------------------

__int32 __stdcall ts::WindowsCOLLECTOR::Error(void)
{
char lpMsgBuf[256];
FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
	      NULL,GetLastError(),
	      MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
	      &lpMsgBuf[0],sizeof(lpMsgBuf),NULL);
MessageBeep(MB_ICONEXCLAMATION);
return MessageBoxA(NULL,lpMsgBuf,"Last Error:",
		  MB_OKCANCEL + MB_DEFBUTTON1 + MB_ICONERROR);
}
//---------------------------------------------------------------------------

__stdcall  ts::WindowsVIRTUALDESKTOP::WindowsVIRTUALDESKTOP(void) {
 Count = 0; Printed = false;
 Context = new  ts::Context;
 Active = 0;
 for (register __int32 i = 0; i < MAX_NUM_OF_WINDOWS; i++) Window[i] = new WindowsWINDOW;
 }
//---------------------------------------------------------------------------

__stdcall ts::WindowsVIRTUALDESKTOP::~WindowsVIRTUALDESKTOP(void) {
 delete Context;
 for (register __int32 i = 0; i < MAX_NUM_OF_WINDOWS; i++) if (Window[i]) delete Window[i];
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsVIRTUALDESKTOP::Add(HWND hwnd) {
Window[this->Count]->Hwnd = hwnd;
Window[this->Count]->Visible = IsWindowVisible(hwnd);
this->Count++;
return;
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsVIRTUALDESKTOP::Delete(HWND hwnd) {
register __int32 i;

for (i = 0; i < this->Count;)
 if ((__int64)Window[i]->Hwnd==(__int64)hwnd)
    {
    for (__int32 i2 = i, ei2 = this->Count-1; i2 < ei2; i2++)
        {
        this->Window[i2]->Hwnd  = this->Window[i2 + 1]->Hwnd;
        this->Window[i2]->Visible = this->Window[i2 + 1]->Visible;
        }
    this->Count--;
    break;
    }
else
    {i++;
    }
return;
}
//---------------------------------------------------------------------------

HWND __stdcall ts::WindowsVIRTUALDESKTOP::Handle_at_XY(__int32 X, __int32 Y) {
RECT rect;

for (__int32 i = 0, ei = this->Windows_Count(); i < ei; i++)
    {
     if (this->Window[i]->Visible==false)
	 continue;
     if (IsWindowVisible(this->Window[i]->Hwnd)==false || IsIconic(this->Window[i]->Hwnd)==true)
	 continue;
     ::GetWindowRect(this->Window[i]->Hwnd,&rect);
     if (X >= rect.left && X < rect.right && Y >= rect.top && Y < rect.bottom)
	 return this->Window[i]->Hwnd;
}
return NULL;
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsVIRTUALDESKTOP::Capture()
{
Sleep(100);
HWND thandle = GetDesktopWindow();
HDC thdc = GetDC(thandle);
GetWindowRect(thandle,&Rect);
Context->Resize(Rect.right - Rect.left,Rect.bottom - Rect.top);
BitBlt(Context->Hdc,
    	 0,0,Rect.right,Rect.bottom,
    	 thdc,
    	 0,0,SRCCOPY);
ReleaseDC(thandle, thdc);
}
//---------------------------------------------------------------------------

__stdcall  ts::WindowsSCREEN::WindowsSCREEN(){
Hwnd = GetDesktopWindow();
GetWindowRect(Hwnd,&Rect);
Hdc = GetDC(Hwnd);
Context = new ts::Context;
Context->Resize(Rect.right-Rect.left,Rect.bottom-Rect.top);
};
//---------------------------------------------------------------------------

void __stdcall ts::WindowsSCREEN::Capture()
{
Sleep(100);
GetWindowRect(Hwnd,&Rect);
Context->Resize(Rect.right - Rect.left,Rect.bottom - Rect.top);
BitBlt(Context->Hdc,
    	 0,0,Rect.right,Rect.bottom,
    	 Hdc,
    	 0,0,SRCCOPY);
}
//---------------------------------------------------------------------------
__stdcall  ts::WindowsSCREEN::~WindowsSCREEN() {ReleaseDC(0,Hdc); DeleteObject(Hdc); delete Context; }
//---------------------------------------------------------------------------

void __stdcall ts::WindowsWINDOW::Set(HWND AHwnd)
{
Hwnd = AHwnd;
GetWindowTextA(Hwnd,Text,128-1);
GetWindowRect(Hwnd,&Rect);
Visible = IsWindowVisible(Hwnd);
//Context->Resize(Rect.left-Rect.right,Rect.bottom-Rect.top);
// ::PrintWindow(Handle,Context->Bitmap.Handle,0);
Minimized = IsIconic(Hwnd);
}

//---------------------------------------------------------------------------

void __stdcall ts::WindowsWINDOW::Capture()
{
//const int WM_PRINT = 0x0317;
//conar inr WM_PRINTCLIENT = 0x0318;
//__int32 PRF_CLIENT = 4;
//__int32 PRF_CHILDREN = 0x10;
//__int32 PRF_NON_CLIENT = 2;
//__int32 COMBINED_PRINTFLAGS = PRF_CLIENT | PRF_CHILDREN | PRF_NON_CLIENT;
//SendMessage(Handle, WM_PRINTCLIENT, (int)Context->Bitmap.Handle, COMBINED_PRINTFLAGS);
//Context->Resize(Rect.left-Rect.right,Rect.bottom-Rect.top);
//BitBlt(Context->Hdc, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, Context->Bitmap.Handle, 0, 0, (int)SRCCOPY);

// ::PrintWindow(Handle,Context->Bitmap.Handle,0);
//GetWindowRect(Screen->Hwnd,&Virtual[Active]->Rect);
//Context->Resize(Rect.right - Rect.left,Rect.bottom - Rect.top);
//BitBlt(Context->Hdc,
//    	 0,0,Rect.right,Rect.bottom,
//    	 Screen->Hdc,
//    	 0,0,SRCCOPY);
}

