//---------------------------------------------------------------------------
//---------------------Stanislaw Stasiak sstsoft.pl 2001-2017----------------
//---------------------------------------------------------------------------
#include "tsoft_WindowsMover.h"
//---------------------------------------------------------------------------
#define mbLeft	0
#define mbRight	1
//---------------------------------------------------------------------------

__stdcall ts::WindowsMover::WindowsMover(void) : f_mouse_is_down(false), f_resized(0)
{
}
//---------------------------------------------------------------------

__stdcall ts::WindowsMover::~WindowsMover(void)
{
}
//---------------------------------------------------------------------

void __stdcall ts::WindowsMover::MouseDown(HWND handle, int X, int Y, int a_button_id)
{
if (a_button_id==mbLeft)
   {f_old_X = X; f_old_Y = Y; f_mouse_is_down = true;
    ::GetWindowRect(handle,&f_old_rect);
   }
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsMover::MouseMove(HWND handle, int X, int Y, bool a_move, bool a_snap, RECT *a_snap_rect, int a_snap_min_X,int a_snap_min_Y, bool resize,int a_res_min_X,int a_res_min_Y)
{
if (f_mouse_is_down==false)
    return;
if (f_resized==false && a_snap==false && a_move==true)
   {
    int X1 = f_old_rect.left + (X - f_old_X);
    f_new_rect.left = X1;
    f_new_rect.right = X1 + (f_old_rect.right-f_old_rect.left);
    int Y1 = f_old_rect.top + (Y - f_old_Y);
    f_new_rect.top = Y1;
    f_new_rect.bottom = Y1 + (f_old_rect.bottom-f_old_rect.top);
   }
else
if (f_resized==false && a_snap==true && a_move==true)
   {
    int X1 = f_old_rect.left + (X - f_old_X), X2 = f_old_rect.right + (X - f_old_X);
    if (a_snap_rect->right - X2 > X1 - a_snap_rect->left)
       {if (X1 < a_snap_rect->left + a_snap_min_X)
            X1 = a_snap_rect->left;
       }
    else
       {if (X2 > a_snap_rect->right - a_snap_min_X)
            X1 = a_snap_rect->right - (f_old_rect.right-f_old_rect.left);
       }
    f_new_rect.left = X1;
    f_new_rect.right = X1 + (f_old_rect.right-f_old_rect.left);

    int Y1 = f_old_rect.top + (Y - f_old_Y), Y2 = f_old_rect.bottom + (Y - f_old_Y);
    if (a_snap_rect->bottom - Y2 > Y1 - a_snap_rect->top)
       {if (Y1 < a_snap_rect->top + a_snap_min_Y)
            Y1 = a_snap_rect->top;
       }
    else
       {if (Y2 > a_snap_rect->bottom - a_snap_min_Y)
            Y1 = a_snap_rect->bottom - (f_old_rect.bottom-f_old_rect.top);
       }
    f_new_rect.top = Y1;
    f_new_rect.bottom = Y1 + (f_old_rect.bottom-f_old_rect.top);
   }
else
if (resize==true)
   {
    if (f_old_X>(f_old_rect.right-f_old_rect.left)-a_res_min_X)
       {
        f_resized = true;
        f_new_rect.right = f_old_rect.right + X - f_old_X;
        f_old_X = X;
       }
    if (f_old_Y>(f_old_rect.bottom-f_old_rect.top)-a_res_min_Y)
       {
        f_resized = true;
        f_new_rect.bottom = f_old_rect.bottom + Y - f_old_Y;
        f_old_Y = Y;
       }
   }
if (f_resized==true)
   {
    ::SetWindowPos(handle,NULL,
             0,0,f_new_rect.right-f_old_rect.left,f_new_rect.bottom-f_old_rect.top,
             SWP_NOCOPYBITS|SWP_NOZORDER|SWP_NOMOVE);
    ::GetWindowRect(handle,&f_old_rect);
    return;
   }
if (a_move==true)
   {
    ::SetWindowPos(handle,NULL,
             f_new_rect.left,f_new_rect.top,0,0,
             SWP_NOZORDER|SWP_NOSIZE);
    ::GetWindowRect(handle,&f_old_rect);
    return;
   }
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsMover::MouseMove(HWND handle, int X, int Y, bool a_move, RECT *a_snap_rect, int a_snap_min_X,int a_snap_min_Y)
{
this->MouseMove(handle,X,Y,a_move,true,a_snap_rect,a_snap_min_X,a_snap_min_Y,false,0,0);
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsMover::MouseMove(HWND handle, int X, int Y, bool a_move)
{
this->MouseMove(handle,X,Y,a_move,false,NULL,0,0,false,0,0);
}
//---------------------------------------------------------------------------

void __stdcall ts::WindowsMover::MouseUp(HWND handle, int X, int Y, int a_button_id)
{
if (a_button_id==mbLeft)
   {f_old_X = X; f_old_Y = Y; f_mouse_is_down = false; f_resized = 0;
   }
}
//---------------------------------------------------------------------------
