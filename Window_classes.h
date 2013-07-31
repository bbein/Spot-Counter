#pragma once

#include "resource.h"

#include "spots.h"
#include "Controller.h"

#include <windows.h>		// Header File For Windows
#include <shobjidl.h>		// Header for more Windows functions




//////////////////////////
// WinSimpleClass Class //
//////////////////////////

class WinSimpleClass
{
public:
	WinSimpleClass (wchar_t const * name, HINSTANCE hInst)
		: _name (name), _hInstance (hInst)
	{}
	WinSimpleClass (int resId, HINSTANCE hInst);
	wchar_t const * GetName () const { return _name.c_str(); }
	HINSTANCE GetInstance () const { return _hInstance; }
    HWND GetRunningWindow ();
protected:
	HINSTANCE	_hInstance;
	std::wstring	_name;
};

////////////////////
// WinClass Class //
////////////////////

class WinClass: public WinSimpleClass
{
public:
    WinClass (wchar_t const * className, HINSTANCE hInst, WNDPROC wndProc);
    WinClass (int resId, HINSTANCE hInst, WNDPROC wndProc);
    void SetResIcons (int resId);
    void Register ();
	void SetMenu (int ResId);
protected:
    void SetDefaults ();
    WNDCLASSEX _class;
};

//////////////////////
// WinGLClass Class //
//////////////////////

class WinGLClass: public WinClass
{
public:
    WinGLClass (wchar_t const * className, HINSTANCE hInst, WNDPROC wndProc);
    WinGLClass (int resId, HINSTANCE hInst, WNDPROC wndProc);

protected:
	void SetDefaults ();
};

////////////////////
// WinMaker Class //
////////////////////

class WinMaker
{
public:
    WinMaker (WinClass & winClass);
    operator HWND () { return _hwnd; }
    void AddCaption (wchar_t const * caption) { _windowName = caption; }
    void AddSysMenu ()    { _style |= WS_SYSMENU; }
    void AddVScrollBar () { _style |= WS_VSCROLL; }
    void AddHScrollBar () { _style |= WS_HSCROLL; }
    void Create ();
    void Show (int nCmdShow = SW_SHOWNORMAL);
protected:
    WinClass   & _class;
    HWND         _hwnd;

    DWORD			_exStyle;       // extended window style
    wchar_t const * _windowName;    // pointer to window name
    DWORD			_style;         // window style
    int				_x;             // horizontal position of window
    int				_y;             // vertical position of window
    int				 _width;        // window width  
    int				_height;        // window height
    HWND			_hWndParent;    // handle to parent or owner window
    HMENU			_hMenu;         // handle to menu, or child-window ID
    void		  * _data;          // pointer to window-creation data
};

/////////////////////
// ResString Class //
/////////////////////

class ResString
{
    enum { MAX_RESSTRING = 255 };
public:
    ResString (HINSTANCE hInst, int resId);
    operator wchar_t const * () { return _buf; }
private:
    wchar_t _buf [MAX_RESSTRING + 1];
};

////////////////////////
// WinException Class //
////////////////////////

class WinException
{
public:
    WinException (wchar_t* msg)
    : _err (::GetLastError()), _msg(msg)
    {}
    DWORD GetError() const { return _err; }
    wchar_t const * GetMessage () const { return _msg; }
private:
    DWORD  _err;
    wchar_t * _msg;
};

////////////////////
// Help Functions //
////////////////////

//Used to get the pointer of a controler for the window from the window userdata
template <class T>
inline T WinGetLong (HWND hwnd, int which = GWL_USERDATA)
{
    return reinterpret_cast<T> (::GetWindowLong (hwnd, which));
}

//Used to save the pointer of a controler for the window to the window userdata
template <class T>
inline void WinSetLong (HWND hwnd, T value, int which = GWL_USERDATA)
{
    ::SetWindowLong (hwnd, which, reinterpret_cast<long> (value));
}

BOOL CALLBACK IImageSettingsDlgProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SpotInfoDlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SpotsInfoDlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK HistogrammSettingsDlgProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AreaHeightSettingsDlgProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool cvtPW2stdstring(std::string& s, const PWSTR pw, UINT codepage = CP_ACP);
wchar_t* Open_File();
wchar_t* Save_File();
double factorial(int n);