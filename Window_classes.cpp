#include "stdafx.h"
#include "window_classes.h"


//////////////////////////
// WinSimpleClass Class //
//////////////////////////
//WinSimpleClass Constructor sets defaults for the window class and sets the WNDProc function
//resId is the resId to the clss name
WinSimpleClass::WinSimpleClass (int resId, HINSTANCE hInst): _hInstance (hInst) {
	ResString resStr (hInst, resId);
	_name = resStr;
}

//Returns the running window if there is a window ranning of this class otherwise it returns NULL
HWND WinSimpleClass::GetRunningWindow () {
    HWND hwnd = ::FindWindow (GetName(), 0);
    if (::IsWindow (hwnd))
    {
        HWND hwndPopup = ::GetLastActivePopup (hwnd);
        if (::IsWindow (hwndPopup))
            hwnd = hwndPopup;
    }
    else 
        hwnd = 0;

    return hwnd;
}

////////////////////
// WinClass Class //
////////////////////

//WinClass Constructor sets defaults for the window class and sets the WNDProc function
//Class name is the name of the class
WinClass::WinClass (wchar_t const * className, HINSTANCE hInst, WNDPROC wndProc): WinSimpleClass (className, hInst) {
    _class.lpfnWndProc = wndProc;
	SetDefaults ();
}
//WinClass Constructor sets defaults for the window class and sets the WNDProc function
//resId is the resId to the class name
WinClass::WinClass (int resId, HINSTANCE hInst, WNDPROC wndProc): WinSimpleClass (resId, hInst) {
    _class.lpfnWndProc = wndProc;
	SetDefaults ();
}

//Sets the default valus for a window
void WinClass::SetDefaults () {
    // Provide reasonable default values
    _class.cbSize = sizeof (WNDCLASSEX);
    _class.style = CS_HREDRAW | CS_VREDRAW;
    _class.lpszClassName = GetName();
    _class.hInstance = GetInstance();
    _class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    _class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    _class.lpszMenuName = 0;
    _class.cbClsExtra = 0;
    _class.cbWndExtra = 0;
    _class.hbrBackground = reinterpret_cast<HBRUSH> (COLOR_WINDOW + 1);
    _class.hCursor = ::LoadCursor (0, IDC_ARROW);
}
//Sets the Small and big Icon of the Window class to resId
void WinClass::SetResIcons (int resId)	{
    _class.hIcon = reinterpret_cast<HICON> (
        ::LoadImage (
            _class.hInstance, 
            MAKEINTRESOURCE (resId), 
            IMAGE_ICON, 
            ::GetSystemMetrics (SM_CXICON),
            ::GetSystemMetrics (SM_CYICON),
            0));
    // Small icon can be loaded from the same resource
    _class.hIconSm = reinterpret_cast<HICON> (
        ::LoadImage (
            _class.hInstance, 
            MAKEINTRESOURCE (resId), 
            IMAGE_ICON, 
            ::GetSystemMetrics (SM_CXSMICON),
            ::GetSystemMetrics (SM_CYSMICON),
            0));
}

//Registers the Class to windows
void WinClass::Register ()
{
     if (::RegisterClassEx (&_class) == 0)
        throw WinException (L"Internal error: RegisterClassEx failed.");
}

//Sets the Menu of the window class to ResId
void WinClass::SetMenu(int ResId) {
	this->_class.lpszMenuName = MAKEINTRESOURCE(ResId);
}

//////////////////////
// WinGLClass Class //
//////////////////////

//WinClass Constructor sets defaults for the window class and sets the WNDProc function
//Class name is the name of the class
WinGLClass::WinGLClass(const wchar_t *className, HINSTANCE hInst, WNDPROC wndProc) : WinClass(className, hInst, wndProc) {
	this->SetDefaults();
}
//WinClass Constructor sets defaults for the window class and sets the WNDProc function
//resId is the resId to the class name
WinGLClass::WinGLClass(int resId, HINSTANCE hInst, WNDPROC wndProc) : WinClass(resId, hInst, wndProc) {
	this->SetDefaults();
}
//Sets the default valus for a window
void WinGLClass::SetDefaults() {
	 _class.style |= CS_OWNDC;
	 _class.hbrBackground = NULL;
}
////////////////////
// WinMaker Class //
////////////////////

WinMaker::WinMaker (WinClass & winClass)
  : _hwnd (0),
    _class (winClass),
    _exStyle (WS_EX_APPWINDOW | WS_EX_WINDOWEDGE),       // extended window style
    _windowName (0),    // pointer to window name
    _style (WS_OVERLAPPEDWINDOW| WS_CLIPSIBLINGS | WS_CLIPCHILDREN), // window style
    _x (0), // horizontal position of window
    _y (0),             // vertical position of window
    _width (400), // window width  
    _height (400),        // window height
    _hWndParent (0),    // handle to parent or owner window
    _hMenu (0),         // handle to menu, or child-window identifier
    _data (0)           // pointer to window-creation data
{
}

void WinMaker::Create () {
    _hwnd = ::CreateWindowEx (
        _exStyle,
        _class.GetName (),
        _windowName,
        _style,
        _x,
        _y,
        _width,
        _height,
        _hWndParent,
        _hMenu,
        _class.GetInstance (),
        _data);
    if (_hwnd == 0)
        throw WinException (L"Internal error: Window Creation Failed.");
}

void WinMaker::Show (int nCmdShow)
{
	::ShowWindow(_hwnd, nCmdShow);
	::SetForegroundWindow(_hwnd);
	::UpdateWindow (_hwnd);
}



/////////////////////
// ResString Class //
/////////////////////

ResString::ResString (HINSTANCE hInst, int resId){
     if (!::LoadString (hInst, resId, _buf, MAX_RESSTRING + 1))
        throw WinException (L"Load String failed");
}

////////////////////
// Help Functions //
////////////////////

BOOL CALLBACK SpotsInfoDlgProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	SpotsInfoController * pCtrl = WinGetLong<SpotsInfoController *> (hwnd);
	switch(uMsg) {
		case WM_INITDIALOG:
        // Have to catch exception in case new throws!
        try
        {
            pCtrl = new SpotsInfoController(hwnd);
            WinSetLong<SpotsInfoController *> (hwnd, pCtrl);
			
        }
        catch (WinException e)
        {
            ::MessageBox(hwnd, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        catch (...)
        {
            ::MessageBox (hwnd, L"Unknown Error", L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        break;
		case WM_COMMAND:
           pCtrl->Command(wParam, lParam);
		break;	
		case WM_CLOSE:
		{
			ShowWindow(hwnd, SW_HIDE);	
			break;
		}
		case WM_ACTIVATE: {
			 pCtrl->Activate();
		break;
		}
		default:
			return FALSE;
    }
	return TRUE;
}

BOOL CALLBACK SpotInfoDlgProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
SpotInfoController * pCtrl = WinGetLong<SpotInfoController *> (hwnd);
	switch(uMsg) {
		case WM_INITDIALOG:
        // Have to catch exception in case new throws!
        try
        {
            pCtrl = new SpotInfoController(hwnd);
            WinSetLong<SpotInfoController *> (hwnd, pCtrl);
			
        }
        catch (WinException e)
        {
            ::MessageBox(hwnd, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        catch (...)
        {
            ::MessageBox (hwnd, L"Unknown Error", L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        break;
		case WM_COMMAND:
           pCtrl->Command(wParam, lParam);
		break;	
		case WM_CLOSE:
		{
			ShowWindow(hwnd, SW_HIDE);	
			break;
		}
		case WM_ACTIVATE: {
			 pCtrl->Activate();
		break;
		}
		default:
			return FALSE;
    }
	return TRUE;
}

BOOL CALLBACK IImageSettingsDlgProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	ImageSettingsController * pCtrl = WinGetLong<ImageSettingsController *> (hwnd);
	switch(uMsg) {
		case WM_INITDIALOG:
        // Have to catch exception in case new throws!
        try
        {
            pCtrl = new ImageSettingsController(hwnd);
            WinSetLong<ImageSettingsController *> (hwnd, pCtrl);
        }
        catch (WinException e)
        {
            ::MessageBox(hwnd, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        catch (...)
        {
            ::MessageBox (hwnd, L"Unknown Error", L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        break;
		case WM_COMMAND:
           pCtrl->Command(wParam, lParam);
		break;	
		case WM_CLOSE:
		{
			ShowWindow(hwnd, SW_HIDE);	
			break;
		}
		case WM_ACTIVATE: {
			 pCtrl->Activate();
		break;
		}
		default:
			return FALSE;
    }
	return TRUE;
}

BOOL CALLBACK HistogrammSettingsDlgProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HistogrammSettingsController * pCtrl = WinGetLong<HistogrammSettingsController *> (hwnd);
	switch(uMsg) {
		case WM_INITDIALOG:
        // Have to catch exception in case new throws!
        try
        {
            pCtrl = new HistogrammSettingsController(hwnd);
            WinSetLong<HistogrammSettingsController *> (hwnd, pCtrl);
        }
        catch (WinException e)
        {
            ::MessageBox(hwnd, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        catch (...)
        {
            ::MessageBox (hwnd, L"Unknown Error", L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        break;
		case WM_COMMAND:
           pCtrl->Command(wParam, lParam);
		break;	
		case WM_CLOSE:
		{
			ShowWindow(hwnd, SW_HIDE);	
			break;
		}
		case WM_ACTIVATE: {
			 pCtrl->Activate();
		break;
		}
		default:
			return FALSE;
    }
	return TRUE;
}

BOOL CALLBACK AreaHeightSettingsDlgProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	AreaHeightSettingsController * pCtrl = WinGetLong<AreaHeightSettingsController *> (hwnd);
	switch(uMsg) {
		case WM_INITDIALOG:
        // Have to catch exception in case new throws!
        try
        {
            pCtrl = new AreaHeightSettingsController(hwnd);
            WinSetLong<AreaHeightSettingsController *> (hwnd, pCtrl);
        }
        catch (WinException e)
        {
            ::MessageBox(hwnd, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        catch (...)
        {
            ::MessageBox (hwnd, L"Unknown Error", L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            break;
        }
        break;
		case WM_COMMAND:
           pCtrl->Command(wParam, lParam);
		break;	
		case WM_CLOSE:
		{
			ShowWindow(hwnd, SW_HIDE);	
			break;
		}
		case WM_ACTIVATE: {
			 pCtrl->Activate();
		break;
		}
		default:
			return FALSE;
    }
	return TRUE;
}

bool cvtPW2stdstring(std::string& s, const PWSTR pw, UINT codepage) {
	bool res = false;
	char* p = 0;
	int bsz;
 
	bsz = WideCharToMultiByte(codepage,
		0,
		pw,-1,
		0,0,
		0,0);
	if (bsz > 0) {
		p = new char[bsz];
		int rc = WideCharToMultiByte(codepage,
			0,
			pw,-1,
			p,bsz,
			0,0);
		if (rc != 0) {
			p[bsz-1] = 0;
			s = p;
			res = true;
		}
	}
	delete [] p;
	return res;
}            
wchar_t* Open_File() {
	PWSTR pszFilePath=NULL;
	wchar_t* returns = NULL;
	//Open file Dialog
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr)) {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr)) {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr)) {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    pItem->Release();
                }
            }
            pFileOpen->Release();
			//copy the string so that it can be destroyed with delete
			if(pszFilePath) {
				int length = wcslen(pszFilePath)+1;
				returns = new wchar_t[length];
				wcscpy_s(returns,length,pszFilePath);
				CoTaskMemFree(pszFilePath);
			}
        }
        CoUninitialize();
    }
	return returns;
}

wchar_t* Save_File() {
	PWSTR pszFilePath=NULL;
	wchar_t* returns = NULL;
	//Save file Dialog
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileSaveDialog *pFileSave;

        // Create the SaveDialog object.
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, 
                IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

        if (SUCCEEDED(hr))
        {
            // Show the Save dialog box.
            hr = pFileSave->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileSave->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    pItem->Release();
                }
            }
            pFileSave->Release();
			//copy the string so that it can be destroyed with delete
			if (pszFilePath) {
				int length = wcslen(pszFilePath)+1;
				returns = new wchar_t[length];
				wcscpy_s(returns,length,pszFilePath);
				CoTaskMemFree(pszFilePath);
			}
        }
        CoUninitialize();
    }
	return returns;
}

double factorial(int n)
{
	if ( n== 1 || n == 0) return 1;
	double result = n;
	for (int i = 1; i < n; i++) {
		result *= (n-i);
	}
	return result;
}