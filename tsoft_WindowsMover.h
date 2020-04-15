//---------------------tsoftware 2001----------------------
#ifndef sstsoft_WindowsMoverH
#define sstsoft_WindowsMoverH
//---------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------
namespace ts { class WindowsMover
{
//---------------------------------------------------------------------
private:
//---------------------------------------------------------------------
int		f_old_X,f_old_Y;
bool	f_mouse_is_down, f_resized;
RECT	f_old_rect, f_new_rect;
int		f_new_cursor, f_old_cursor;
//---------------------------------------------------------------------
public:
//---------------------------------------------------------------------
             __stdcall WindowsMover     (void);
//---------------------------------------------------------------------
        void __stdcall MouseDown        (HWND handle, int X, int Y, int a_button_id);
virtual void __stdcall MouseMove        (HWND handle, int X, int Y, bool a_move, bool a_snap, RECT *a_snap_rect, int a_snap_min_X,int a_snap_min_Y, bool a_resize,int a_res_min_X,int a_res_min_Y);
virtual void __stdcall MouseMove        (HWND handle, int X, int Y, bool a_move, RECT *a_snap_rect, int a_snap_min_X,int a_snap_min_Y);
virtual void __stdcall MouseMove        (HWND handle, int X, int Y, bool a_move);
        void __stdcall MouseUp          (HWND handle, int X, int Y, int a_button_id);
//---------------------------------------------------------------------
             __stdcall ~WindowsMover    (void);
//---------------------------------------------------------------------
};
//---------------------------------------------------------------------------
}
#endif
