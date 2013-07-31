#include "stdafx.h"
#include "hight map gl.h"
#include "spots.h"
#include "Data_Collection.h"
#include "histogram.h"
#include "Window_classes.h"
#include "Controller.h"

using namespace std;

HWND		D_Settings=NULL;	// Window handel for the Settings dialog //move to main window controller?

//initial constants
double cut = 3e-9;
double min_hight = cut;
int min_area = 1;
int surrounding = 1;
double offset= 0;
double range= 5e-10;
int max_files = 10;

DataCollector * Daten = NULL;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

HMENU Create_File_Menu(int id) {
	HMENU hMenu;
	hMenu = CreatePopupMenu();
	int countspots, showdata, close, showhistogramm, showareaheight;
	switch (id) {
		case 1:
			countspots = ID_DATA_COUNTSPOTS_1;
			showdata = ID_DATA_SHOWDATA_1;
			close = ID_DATA_CLOSE_1;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_1;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_1;
		break;
		case 2:
			countspots = ID_DATA_COUNTSPOTS_2;
			showdata = ID_DATA_SHOWDATA_2;
			close = ID_DATA_CLOSE_2;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_2;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_2;
		break;
		case 3:
			countspots = ID_DATA_COUNTSPOTS_3;
			showdata = ID_DATA_SHOWDATA_3;
			close = ID_DATA_CLOSE_3;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_3;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_3;
		break;
		case 4:
			countspots = ID_DATA_COUNTSPOTS_4;
			showdata = ID_DATA_SHOWDATA_4;
			close = ID_DATA_CLOSE_4;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_4;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_4;
		break;
		case 5:
			countspots = ID_DATA_COUNTSPOTS_5;
			showdata = ID_DATA_SHOWDATA_5;
			close = ID_DATA_CLOSE_5;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_5;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_5;
		break;
		case 6:
			countspots = ID_DATA_COUNTSPOTS_6;
			showdata = ID_DATA_SHOWDATA_6;
			close = ID_DATA_CLOSE_6;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_6;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_6;
		break;
		case 7:
			countspots = ID_DATA_COUNTSPOTS_7;
			showdata = ID_DATA_SHOWDATA_7;
			close = ID_DATA_CLOSE_7;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_7;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_7;
		break;
		case 8:
			countspots = ID_DATA_COUNTSPOTS_8;
			showdata = ID_DATA_SHOWDATA_8;
			close = ID_DATA_CLOSE_8;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_8;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_8;
		break;
		case 9:
			countspots = ID_DATA_COUNTSPOTS_9;
			showdata = ID_DATA_SHOWDATA_9;
			close = ID_DATA_CLOSE_9;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_9;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_9;
		break;
		case 10:
			countspots = ID_DATA_COUNTSPOTS_10;
			showdata = ID_DATA_SHOWDATA_10;
			close = ID_DATA_CLOSE_10;
			showhistogramm = ID_DATA_SHOWHISTOGRAMM_10;
			showareaheight = ID_DATA_SHOWAREAHEIGHT_10;
		break;
	}
	AppendMenu(hMenu, MF_STRING, countspots, L"Count Spots");
	AppendMenu(hMenu, MF_STRING, showdata, L"Show Data");
	AppendMenu(hMenu, MF_STRING | MF_DISABLED, showhistogramm, L"Show Histogramm"); //has to stay at position 2 (counted from 0) to avoid errors
	AppendMenu(hMenu, MF_STRING | MF_DISABLED, showareaheight, L"Show AreaHeight");	//has to stay at position 3 (counted from 0) to avoid errors
	AppendMenu(hMenu, MF_STRING, close, L"Close Data");
	return hMenu;
}
void FIX_MENU (HWND hwnd, DataCollector * Daten) {
	//Get the right menu
	HMENU hMenu, hSubMenu, hSubSubMenu;
	hMenu = GetMenu(hwnd);
	hSubMenu = GetSubMenu(hMenu,1);
	//Delete all Menu items
	for (int i = 0; i < Daten->Get_Last()+1; i++) {
		DeleteMenu(hSubMenu, 1, MF_BYPOSITION);
	}
	//Create the menu again
	wchar_t* buf = NULL;
	for (int i = 0; i < Daten->Get_Last(); i++) {
		hSubSubMenu = Create_File_Menu(i+1);
		buf = Daten->Get_Data(i)->Get_Filename();
		AppendMenu(hSubMenu, MF_STRING | MF_POPUP, (UINT)hSubSubMenu, buf );
		if ( Daten->Get_Data(i)->Get_Spots_Counted() ) {
			EnableMenuItem(hSubSubMenu, 2, MF_ENABLED |MF_BYPOSITION); //Enables the Histogramm
			EnableMenuItem(hSubSubMenu, 3, MF_ENABLED |MF_BYPOSITION); //Enables the Area Height plot
		}
	}
	delete buf;	
}


bool loadfile(DataCollector* Daten) {
	PWSTR pszFilePath=Open_File();
	if (pszFilePath) {
		//Determine File Format
		string endung;
		cvtPW2stdstring(endung,pszFilePath);
		endung = endung.substr(endung.length() -3);
		if(endung == "afm") {
			if(!Daten->Add_Data(pszFilePath)) {	
				MessageBox(NULL, TEXT("Couldn't load data!"), TEXT("Warning!"), MB_OK | MB_ICONINFORMATION);
			} else {
				delete pszFilePath;
				return true;
			}
		}
		delete pszFilePath;
	}
	return false;
}

bool savefile(DataCollector* Daten) {
	PWSTR pszFilePath=Save_File();
	if (!pszFilePath) return false;
	if (!Daten->Save_To_File(pszFilePath)) {	
		MessageBox(NULL, TEXT("Couldn't Save data! All files counted?"), TEXT("Warning!"), MB_OK | MB_ICONINFORMATION);
		delete pszFilePath;
		return false;
	}
	delete pszFilePath;
	return true;
}

BOOL CALLBACK AboutDlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg)
    {
        case WM_INITDIALOG:
	        return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hWnd, IDOK);
                break;
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK SettingsDlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg)
    {	
		case WM_COMMAND:
            switch(LOWORD(wParam))
            {
				case IDC_SETTINGS_CLOSE:
					SendMessage(hWnd, WM_CLOSE, wParam, lParam);	
				break;
				case IDC_SETTINGS_SET:
				{
					//Get surrounding
					BOOL bSuccess;
					int value = GetDlgItemInt(hWnd, IDC_SETTINGS_SURROUNDING, &bSuccess, FALSE);
					if (value > 0) surrounding = value;
					Daten->Set_Surrounding(value);
					//Get minimal area
					value = GetDlgItemInt(hWnd, IDC_SETTINGS_MIN_AREA, &bSuccess, FALSE);
					if (value > 0) min_area = value;
					Daten->Set_Min_Area(value);
					//Get minimal hight
					int len = GetWindowTextLength(GetDlgItem(hWnd, IDC_SETTINGS_HIGHT));
					if(len > 0)
					{
						PWSTR buf;
						buf = new WCHAR[len+1];
						string text;
						GetDlgItemText(hWnd, IDC_SETTINGS_MIN_HIGHT, buf, len + 1);
						cvtPW2stdstring(text,buf);
						istringstream iss(text);
    					double sub=0;
						iss >> sub;
						min_hight = sub *1e-9;
						Daten->Set_Min_Hight(sub *1e-9);
						delete[] buf;
					}
					//Get Cutoff
					len = GetWindowTextLength(GetDlgItem(hWnd, IDC_SETTINGS_HIGHT));
					if(len > 0)
					{
						PWSTR buf;
						buf = new WCHAR[len+1];
						string text;
						GetDlgItemText(hWnd, IDC_SETTINGS_HIGHT, buf, len + 1);
						cvtPW2stdstring(text,buf);
						istringstream iss(text);
    					double sub=0;
						iss >> sub;
						cut = sub *1e-9;
						Daten->Set_Cut(sub *1e-9);
						delete[] buf;
					}
					FIX_MENU(GetParent(hWnd),Daten);
					SendMessage(GetParent(hWnd),WM_PAINT,wParam,lParam);
				break;
				}
			}
		break;
		case WM_CLOSE:
		{
			ShowWindow(hWnd, SW_HIDE);	
			break;
		}
		default:
			return FALSE;
    }
	return TRUE;
}





LRESULT CALLBACK WndProcMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)									
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_EXIT:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					return 0;
				case ID_DATEI_OPEN: {
					if(!loadfile(Daten)) return 0;
					//add menu item

					HMENU hMenu, hSubMenu, hSubSubMenu;
					hMenu = GetMenu(hWnd);
					hSubMenu = GetSubMenu(hMenu,1);
					hSubSubMenu = Create_File_Menu(Daten->Get_Last());
					wchar_t* buf = NULL;
					buf = Daten->Get_Data(Daten->Get_Last()-1)->Get_Filename();
					AppendMenu(hSubMenu, MF_STRING | MF_POPUP, (UINT)hSubSubMenu, buf );
					delete buf;	

					PostMessage(hWnd, WM_PAINT, 0, 0);
					return 0;
									}
				case IDM_ABOUT:
					DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
					return 0;
				case ID_DATA_SETTINGS:
					ShowWindow(D_Settings, SW_SHOW);
					return 0;
				case ID_ALL_SAVEALLDATA:
					savefile(Daten);
					return 0;
				//Count spots
				case ID_DATA_COUNT_ALL:
					Daten->Count_All_Spots();
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_1:
					Daten->Count_Spots(0);
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_2:
					Daten->Count_Spots(1);
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_3:
					Daten->Count_Spots(2);
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_4:
					Daten->Count_Spots(3);
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_5:
					Daten->Count_Spots(4);
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_6:
					Daten->Count_Spots(5);
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_7:
					Daten->Count_Spots(6);
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_8:
					Daten->Count_Spots(7);
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_9:
					Daten->Count_Spots(8);
					FIX_MENU(hWnd,Daten);
					return 0;
				case ID_DATA_COUNTSPOTS_10:
					Daten->Count_Spots(9);
					FIX_MENU(hWnd,Daten);
					return 0;
				//Show data
				case ID_DATA_SHOWDATA_1:{
					Daten->Create_Data_Win(0);
					return 0;
				}
				case ID_DATA_SHOWDATA_2: {
					Daten->Create_Data_Win(1);
					return 0;
				}
				case ID_DATA_SHOWDATA_3: {
					Daten->Create_Data_Win(2);
					return 0;
				}
				case ID_DATA_SHOWDATA_4: {
					Daten->Create_Data_Win(3);
					return 0;
				}
				case ID_DATA_SHOWDATA_5: {
					Daten->Create_Data_Win(4);
					return 0;
				}
				case ID_DATA_SHOWDATA_6: {
					Daten->Create_Data_Win(5);
					return 0;
				}
				case ID_DATA_SHOWDATA_7: {
					Daten->Create_Data_Win(6);
					return 0;
				}
				case ID_DATA_SHOWDATA_8: {
					Daten->Create_Data_Win(7);
					return 0;
				}
				case ID_DATA_SHOWDATA_9: {
					Daten->Create_Data_Win(8);
					return 0;
				}
				case ID_DATA_SHOWDATA_10: {
					Daten->Create_Data_Win(9);
					return 0;
				}
				//Show Histogramm
				case ID_DATA_SHOWHISTOGRAMM_1:{
					Daten->Create_Histogramm_Win(0);
					return 0;
				}
				case ID_DATA_SHOWHISTOGRAMM_2: {
					Daten->Create_Histogramm_Win(1);
					return 0;
				}
				case ID_DATA_SHOWHISTOGRAMM_3: {
					Daten->Create_Histogramm_Win(2);
					return 0;
				}
				case ID_DATA_SHOWHISTOGRAMM_4: {
					Daten->Create_Histogramm_Win(3);
					return 0;
				}
				case ID_DATA_SHOWHISTOGRAMM_5: {
					Daten->Create_Histogramm_Win(4);
					return 0;
				}
				case ID_DATA_SHOWHISTOGRAMM_6: {
					Daten->Create_Histogramm_Win(5);
					return 0;
				}
				case ID_DATA_SHOWHISTOGRAMM_7: {
					Daten->Create_Histogramm_Win(6);
					return 0;
				}
				case ID_DATA_SHOWHISTOGRAMM_8: {
					Daten->Create_Histogramm_Win(7);
					return 0;
				}
				case ID_DATA_SHOWHISTOGRAMM_9: {
					Daten->Create_Histogramm_Win(8);
					return 0;
				}
				case ID_DATA_SHOWHISTOGRAMM_10: {
					Daten->Create_Histogramm_Win(9);
					return 0;						  
				}
				//Show AreaHeight
				case ID_DATA_SHOWAREAHEIGHT_1:{
					Daten->Create_AreaHeight_Win(0);
					return 0;
				}
				case ID_DATA_SHOWAREAHEIGHT_2: {
					Daten->Create_AreaHeight_Win(1);
					return 0;
				}
				case ID_DATA_SHOWAREAHEIGHT_3: {
					Daten->Create_AreaHeight_Win(2);
					return 0;
				}
				case ID_DATA_SHOWAREAHEIGHT_4: {
					Daten->Create_AreaHeight_Win(3);
					return 0;
				}
				case ID_DATA_SHOWAREAHEIGHT_5: {
					Daten->Create_AreaHeight_Win(4);
					return 0;
				}
				case ID_DATA_SHOWAREAHEIGHT_6: {
					Daten->Create_AreaHeight_Win(5);
					return 0;
				}
				case ID_DATA_SHOWAREAHEIGHT_7: {
					Daten->Create_AreaHeight_Win(6);
					return 0;
				}
				case ID_DATA_SHOWAREAHEIGHT_8: {
					Daten->Create_AreaHeight_Win(7);
					return 0;
				}
				case ID_DATA_SHOWAREAHEIGHT_9: {
					Daten->Create_AreaHeight_Win(8);
					return 0;
				}
				case ID_DATA_SHOWAREAHEIGHT_10: {
					Daten->Create_AreaHeight_Win(9);
					return 0;						  
				}
				//Close data
				case ID_DATA_CLOSE_1: {
					Daten->Remove_Data(0);
					FIX_MENU(hWnd,Daten);
					return 0;
				}
				case ID_DATA_CLOSE_2: {
					Daten->Remove_Data(1);
					FIX_MENU(hWnd,Daten);
					return 0;
				}
				case ID_DATA_CLOSE_3: {
					Daten->Remove_Data(2);
					FIX_MENU(hWnd,Daten);
					return 0;
				}
				case ID_DATA_CLOSE_4: {
					Daten->Remove_Data(3);
					FIX_MENU(hWnd,Daten);
					return 0;
				}
				case ID_DATA_CLOSE_5: {
					Daten->Remove_Data(4);
					FIX_MENU(hWnd,Daten);
					return 0;
				}
				case ID_DATA_CLOSE_6: {
					Daten->Remove_Data(5);
					FIX_MENU(hWnd,Daten);
					return 0;
				}
				case ID_DATA_CLOSE_7: {
					Daten->Remove_Data(6);
					FIX_MENU(hWnd,Daten);
					return 0;
				}
				case ID_DATA_CLOSE_8: {
					Daten->Remove_Data(7);
					FIX_MENU(hWnd,Daten);
					return 0;
				}
				case ID_DATA_CLOSE_9: {
					Daten->Remove_Data(8);
					FIX_MENU(hWnd,Daten);
					return 0;
				}
				case ID_DATA_CLOSE_10: {
					Daten->Remove_Data(9);
					FIX_MENU(hWnd,Daten);
					return 0;
				}				
			}
			return 0;
		case WM_CREATE:
			D_Settings = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SETTINGS), hWnd, SettingsDlgProc);
			if(D_Settings != NULL) { 
				SetDlgItemInt(D_Settings, IDC_SETTINGS_SURROUNDING, (UINT)surrounding, FALSE);
				SetDlgItemInt(D_Settings, IDC_SETTINGS_MIN_AREA, (UINT)min_area, FALSE);
				WCHAR s[20];
				swprintf_s(s,20,TEXT("%.2f"),cut*1e9);
				SetDlgItemText(D_Settings, IDC_SETTINGS_HIGHT, s);
				swprintf_s(s,20,TEXT("%.2f"),min_hight*1e9);
				SetDlgItemText(D_Settings, IDC_SETTINGS_MIN_HIGHT, s);
			} else {
				MessageBox(hWnd, TEXT("CreateDialog returned NULL"), TEXT("Warning!"), MB_OK | MB_ICONINFORMATION);
			}
		return 0;

		case WM_CLOSE:								// Did We Receive A Close Message?
		/* Destroy all windows when the main window is closed */
		{
			DestroyWindow(D_Settings);
			delete Daten;
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}                
	}
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}



LRESULT CALLBACK WndProcDataView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    DataController * pCtrl = WinGetLong<DataController *> (hwnd);
    switch (message)
    {
    case WM_CREATE:
        // Have to catch exception in case new throws!
        try
        {
            pCtrl = new DataController (hwnd);
            WinSetLong<DataController *> (hwnd, pCtrl);			
        }
        catch (WinException e)
        {
            ::MessageBox(hwnd, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        catch (...)
        {
            ::MessageBox (hwnd, L"Unknown Error", L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        return 0;
    case WM_SIZE:
        pCtrl->Size(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_PAINT:
        pCtrl->Paint();
        return ::DefWindowProc (hwnd, message, wParam, lParam);
    case WM_DESTROY:
        WinSetLong<DataController *> (hwnd, 0);
        delete pCtrl;
		Daten->remove_Data_Win(hwnd);
        return 0;
	case WM_LBUTTONDOWN:    
		pCtrl->Selection(LOWORD(lParam),HIWORD(lParam));
		Daten->Update_Connected(hwnd);
		pCtrl->Update_Dialogs();
		return 0;
	case WM_COMMAND:
		pCtrl->Command(wParam, lParam);	
		return 0;
	case WM_KEYDOWN:							
		pCtrl->change_perspective(wParam);			
		return 0;
	}
    return ::DefWindowProc (hwnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProcHistogrammView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    HistogrammController * pCtrl = WinGetLong<HistogrammController *> (hwnd);
    switch (message)
    {
    case WM_CREATE:
        // Have to catch exception in case new throws!
        try
        {
            pCtrl = new HistogrammController (hwnd);
            WinSetLong<HistogrammController *> (hwnd, pCtrl);			
        }
        catch (WinException e)
        {
            ::MessageBox(hwnd, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        catch (...)
        {
            ::MessageBox (hwnd, L"Unknown Error", L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        return 0;
    case WM_SIZE:
        pCtrl->Size(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_PAINT:
        pCtrl->Paint();
        return ::DefWindowProc (hwnd, message, wParam, lParam);
    case WM_DESTROY:
        WinSetLong<HistogrammController *> (hwnd, 0);
        delete pCtrl;
		Daten->remove_Histogramm_Win(hwnd);
        return 0;
	case WM_COMMAND:
		pCtrl->Command(wParam, lParam);	
		return 0;
	case WM_LBUTTONDOWN:    
		Daten->Update_Connected(hwnd);
		return 0;
    }
    return ::DefWindowProc (hwnd, message, wParam, lParam);
}
LRESULT CALLBACK WndProcAreaHeightView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    AreaHeightController * pCtrl = WinGetLong<AreaHeightController *> (hwnd);
    switch (message)
    {
    case WM_CREATE:
        // Have to catch exception in case new throws!
        try
        {
            pCtrl = new AreaHeightController (hwnd);
            WinSetLong<AreaHeightController *> (hwnd, pCtrl);			
        }
        catch (WinException e)
        {
            ::MessageBox(hwnd, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        catch (...)
        {
            ::MessageBox (hwnd, L"Unknown Error", L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        return 0;
    case WM_SIZE:
        pCtrl->Size(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_PAINT:
        pCtrl->Paint();
        return ::DefWindowProc (hwnd, message, wParam, lParam);
    case WM_DESTROY:
        WinSetLong<AreaHeightController *> (hwnd, 0);
        delete pCtrl;
		Daten->remove_AreaHeight_Win(hwnd);
        return 0;
	case WM_COMMAND:
		pCtrl->Command(wParam, lParam);	
		return 0;
	case WM_LBUTTONDOWN:    
		pCtrl->Selection(LOWORD(lParam),HIWORD(lParam));
		Daten->Update_Connected(hwnd);
		return 0;
    }
    return ::DefWindowProc (hwnd, message, wParam, lParam);
}

int WINAPI WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow) {
	MSG		msg;								// Windows Message Structure
	BOOL	done=FALSE;							// Bool Variable To Exit Loop
	//Init Data
	Daten = new DataCollector(cut, min_hight, surrounding, min_area, max_files, 512);
	//Create Main Window
	WinMaker * Win;
	WinClass * winClass;
	winClass = new WinClass(L"Main", hInstance, WndProcMain);
	winClass->SetMenu(IDC_HIGHTMAPGL);
	winClass->Register();
	Win = new WinMaker(*winClass);
	Win->AddCaption(L"Spot Counter");
	Win->Create();
	Win->Show(nCmdShow);
	delete winClass;
	delete Win;
	//create Dataview windows
	WinGLClass* winGLClassData = new WinGLClass(L"DataView", hInstance, WndProcDataView);
	winGLClassData->SetMenu(IDC_SPOT_DISPLAY_MENU);
	winGLClassData->Register();
	WinMaker * Win_Data = new WinMaker(*winGLClassData);
	Daten->Set_Data_Win(Win_Data);
	
	//create Histogramm windows
	WinGLClass* winGLClassHistogramm = new WinGLClass(L"HistogrammView", hInstance, WndProcHistogrammView);
	winGLClassHistogramm->SetMenu(IDR_HISTOGRAMM_MENU);
	winGLClassHistogramm->Register();
	WinMaker * Win_Histogramm = new WinMaker(*winGLClassHistogramm);
	Daten->Set_Histogramm_Win(Win_Histogramm);

	//create AreaHight windows
	WinGLClass* winGLClassAreaHeight = new WinGLClass(L"AreaHeightView", hInstance, WndProcAreaHeightView);
	winGLClassAreaHeight->SetMenu(IDR_AREAHEIGHT_MENU);
	winGLClassAreaHeight->Register();
	WinMaker * Win_AreaHeight = new WinMaker(*winGLClassAreaHeight);
	Daten->Set_AreaHeight_Win(Win_AreaHeight);
	
	//Message Loop
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
	}

	// Shutdown
	delete winGLClassData;
	delete winGLClassHistogramm;
	delete winGLClassAreaHeight;
	return (msg.wParam);							// Exit The Program
}
