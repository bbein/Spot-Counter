#include "stdafx.h"
#include "Controller.h"

//////////////////////
// Controller Class //
//////////////////////

//Contstructor for the basic Controller sets hwnd to NULL
Controller::Controller() {
	this->_hwnd = NULL;
}

//Contstructor for the basic Controller sets hwnd
Controller::Controller (HWND hwnd) {
	this->_hwnd = hwnd;
}

//Destructor for the Controller
Controller::~Controller () {
	
}

//handels the menu controll of the window
void Controller::Command(WPARAM wParam, LPARAM lParam) {
	
}

///////////////////////////////////
// ImageSettingsController Class //
///////////////////////////////////

ImageSettingsController::ImageSettingsController() {

}

//Constructor for SpotsInfoController sets hwnd
ImageSettingsController::ImageSettingsController(HWND hwnd) {
	this->_hwnd = hwnd;
}

//Handels the Commmand Messages
void ImageSettingsController::Command(WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDOK:	{
			if (GetParent(this->_hwnd)) {
				DataController * Parent = WinGetLong<DataController *> (GetParent(this->_hwnd));
				if (Parent) {
					//Get Range
					int len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_RANGE));
					if(len > 0){
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_RANGE, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Range(sub *1e-9);
						delete[] buf;
					}
					//Get Offset
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_OFFSET));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_OFFSET, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Offset(sub *1e-9);
						delete[] buf;
					}
					//Get 3D Range
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_3D_RANGE));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_3D_RANGE, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Range_3D(sub);
						delete[] buf;
					}
					Parent->Paint();
				}
			}
		break;
		}
		case IDCANCEL:
			SendMessage(this->_hwnd, WM_CLOSE, wParam, lParam);	
		break;
	}
}



//Sets the Caption of the Image Controller
void ImageSettingsController::Activate() {
	if (GetParent(this->_hwnd)) {
		DataController * Parent = WinGetLong<DataController *> (GetParent(this->_hwnd));
		if (Parent) {
			Spots * Daten = Parent->GetData();
			wchar_t * buf = NULL;
			buf = Daten->Get_Filename();
			SetDlgItemText(this->_hwnd, IDC_CAPTION, buf);
			delete buf;
		}
	}
}

////////////////////////////////////////
// HistogrammSettingsController Class //
////////////////////////////////////////

//Constructor for SpotsInfoController
HistogrammSettingsController::HistogrammSettingsController() {

}

//Constructor for SpotsInfoController sets hwnd
HistogrammSettingsController::HistogrammSettingsController(HWND hwnd) {
	this->_hwnd = hwnd;
}

//Handels the Commmand Messages
void HistogrammSettingsController::Command(WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDOK:	{
			if (GetParent(this->_hwnd)) {
				HistogrammController * Parent = WinGetLong<HistogrammController *> (GetParent(this->_hwnd));
				if (Parent) {
					bool Y_Range_changed = false;
					double Y_Range;
					//Get Max Number/Y-Range
					int len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_YRANGE));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_YRANGE, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						if (sub == Parent->Get_Max_Number()) {
							Y_Range_changed = false;
						} else {
							Y_Range_changed = true;
							Y_Range = sub;
						}
						delete[] buf;
					}
					//Get Bin Number
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_BIN_NUMBER));
					if(len > 0){
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_BIN_NUMBER, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Bin_Number((int)sub);
						delete[] buf;
					}
					//Get Min Height
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_MINIMUM_HEIGHT));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_MINIMUM_HEIGHT, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Min_Height(sub *1e-9);
						delete[] buf;
					}
					//Get Max Height
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_MAXIMUM_HEIGHT));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_MAXIMUM_HEIGHT, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Max_Height(sub *1e-9);
						delete[] buf;
					}
					//Set new Y-Range if it changed
					if (Y_Range_changed) Parent->Set_Max_Number((int)Y_Range);
					this->Activate();
					Parent->Paint();
				}
			}
		break;
		}
		case IDCANCEL:
			SendMessage(this->_hwnd, WM_CLOSE, wParam, lParam);	
		break;
	}
}

//Sets the Caption of the Image Controller
void HistogrammSettingsController::Activate() {
	if (GetParent(this->_hwnd)) {
		HistogrammController * Parent = WinGetLong<HistogrammController *> (GetParent(this->_hwnd));
		if (Parent) {
			Spots * Daten = Parent->GetData();
			wchar_t * buf = NULL;
			buf = Daten->Get_Filename();
			SetDlgItemText(this->_hwnd, IDC_CAPTION, buf);
			WCHAR s[20];
			swprintf_s(s,20,TEXT("%.0f"),(double) Parent->Get_Bin_Number());
			SetDlgItemText(this->_hwnd, IDC_BIN_NUMBER, s);
			swprintf_s(s,20,TEXT("%.2f"),Parent->Get_Min_Height()*1e9);
			SetDlgItemText(this->_hwnd, IDC_MINIMUM_HEIGHT, s);
			swprintf_s(s,20,TEXT("%.2f"),Parent->Get_Max_Height()*1e9);
			SetDlgItemText(this->_hwnd, IDC_MAXIMUM_HEIGHT, s);
			swprintf_s(s,20,TEXT("%.0f"),(double) Parent->Get_Max_Number());
			SetDlgItemText(this->_hwnd, IDC_YRANGE, s);
			delete buf;
		}
	}
}

////////////////////////////////////////
// AreaHeightSettingsController Class //
////////////////////////////////////////

//Constructor for SpotsInfoController
AreaHeightSettingsController::AreaHeightSettingsController() {

}

//Constructor for SpotsInfoController sets hwnd
AreaHeightSettingsController::AreaHeightSettingsController(HWND hwnd) {
	this->_hwnd = hwnd;
}

//Handels the Commmand Messages
void AreaHeightSettingsController::Command(WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDOK:	{
			if (GetParent(this->_hwnd)) {
				AreaHeightController * Parent = WinGetLong<AreaHeightController *> (GetParent(this->_hwnd));
				if (Parent) {
					//Get Max Height
					int len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_AREAHEIGHT_MAX_HEIGHT));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_MAX_HEIGHT, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Max_Height(sub *1e-9);
						delete[] buf;
					}
					//Get Min Height
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_AREAHEIGHT_MIN_HEIGHT));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_MIN_HEIGHT, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Min_Height(sub *1e-9);
						delete[] buf;
					}
					//Get Max Area
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_AREAHEIGHT_MAX_AREA));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_MAX_AREA, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Max_Area((int)sub);
						delete[] buf;
					}
					//Get Min Area
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_AEAHEIGHT_MIN_AREA));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_AEAHEIGHT_MIN_AREA, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Min_Area((int)sub);
						delete[] buf;
					}
					//Get Fit Area
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_AREAHEIGHT_FIT_AREA));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_FIT_AREA, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Fit_Area((int)sub);
						delete[] buf;
					}
					//Get error multiplier
					len = GetWindowTextLength(GetDlgItem(this->_hwnd, IDC_AREAHEIGHT_ERROR_MULTIPLIER));
					if(len > 0)	{
						PWSTR buf;
						buf = new WCHAR[len+1];
						std::string text;
						GetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_ERROR_MULTIPLIER, buf, len + 1);
						cvtPW2stdstring(text,buf);
						std::istringstream iss(text);
    					double sub=0;
						iss >> sub;
						Parent->Set_Error_Multiplier(sub);
						delete[] buf;
					}
					
				}
			}
		break;
		}
		case IDCANCEL:
			SendMessage(this->_hwnd, WM_CLOSE, wParam, lParam);	
		break;
	}
}

//Sets the Caption of the Image Controller
void AreaHeightSettingsController::Activate() {
	if (GetParent(this->_hwnd)) {
		AreaHeightController * Parent = WinGetLong<AreaHeightController *> (GetParent(this->_hwnd));
		if (Parent) {
			Spots * Daten = Parent->GetData();
			wchar_t * buf = NULL;
			buf = Daten->Get_Filename();
			SetDlgItemText(this->_hwnd, IDC_CAPTION, buf);
			WCHAR s[20];
			swprintf_s(s,20,TEXT("%.2f"), Parent->Get_Max_Height()*1e9);
			SetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_MAX_HEIGHT, s);
			swprintf_s(s,20,TEXT("%.2f"),Parent->Get_Min_Height()*1e9);
			SetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_MIN_HEIGHT, s);
			swprintf_s(s,20,TEXT("%.0f"),(double)Parent->Get_Max_Area());
			SetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_MAX_AREA, s);
			swprintf_s(s,20,TEXT("%.0f"),(double)Parent->Get_Min_Area());
			SetDlgItemText(this->_hwnd, IDC_AEAHEIGHT_MIN_AREA, s);
			swprintf_s(s,20,TEXT("%.0f"),(double)Parent->Get_Fit_Area());
			SetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_FIT_AREA, s);
			swprintf_s(s,20,TEXT("%.2f"),Parent->Get_Error_Multiplier());
			SetDlgItemText(this->_hwnd, IDC_AREAHEIGHT_ERROR_MULTIPLIER, s);
			delete buf;
		}
	}
}


///////////////////////////////
// SpotsInfoController Class //
///////////////////////////////

//Constructor for SpotsInfoController 
SpotsInfoController::SpotsInfoController() {

}

//Constructor for SpotsInfoController sets hwnd
SpotsInfoController::SpotsInfoController(HWND hwnd) {
	this->_hwnd = hwnd;
}

//Handels the Commmand Messages
void SpotsInfoController::Command(WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDOK:
			SendMessage(this->_hwnd, WM_CLOSE, wParam, lParam);	
		break;
		case IDC_SPOTINFO_UPDATE:
			SendMessage(this->_hwnd, WM_ACTIVATE, wParam, lParam);	
		break;
	}
}

//Sets all the lables to the right data
void SpotsInfoController::Activate() {
	WCHAR s[200];
	if (GetParent(this->_hwnd)) {
		DataController * Parent = WinGetLong<DataController *> (GetParent(this->_hwnd));
		if (Parent) {
			Spots * Daten = Parent->GetData();
			//Set Countet Spots
			swprintf_s(s,200,TEXT("%.0f"),(double)Daten->Get_Spot_Number());
			SetDlgItemText(this->_hwnd, IDC_SPOT_INFO_COUNTED_SPOTS, s);
			//Set Aktive Spots
			swprintf_s(s,200,TEXT("%.0f"),(double)Daten->Get_Aktiv_Spots());
			SetDlgItemText(this->_hwnd, IDC_SPOT_INFO_AKTIV_SPOTS, s);
			//Set Averige Area
			swprintf_s(s,200,TEXT("%.2f"),Daten->Get_Averige_Area(MODE_AKTIV));
			SetDlgItemText(this->_hwnd, IDC_SPOT_INFO_AVeRIGE_AREA, s);
			//Set Averige Hight
			swprintf_s(s,200,TEXT("%.2f nm"),Daten->Get_Averige_Hight(MODE_AKTIV)*1e9);
			SetDlgItemText(this->_hwnd, IDC_SPOT_INFO_AVeRIGE_HIGHT, s);
			//Set volum
			swprintf_s(s,200,TEXT("%.2f nm^3"),Daten->Get_Aktiv_Volumen(MODE_NORMAL)*1e27);
			SetDlgItemText(this->_hwnd, IDC_SPOTSINFO_VOLUM, s);
			//Set reduced volum
			swprintf_s(s,200,TEXT("%.2f nm^3"),Daten->Get_Aktiv_Volumen(MODE_REDUCED)*1e27);
			SetDlgItemText(this->_hwnd, IDC_REDUCEDVOLME, s);
			//Set Caption
			wchar_t * buf = NULL;
			buf = Daten->Get_Filename();
			SetDlgItemText(this->_hwnd, IDC_CAPTION, buf);
			delete buf;
		}
	}
}


//////////////////////////////
// SpotInfoController Class //
//////////////////////////////

//Constructor for SpotsInfoController 
SpotInfoController::SpotInfoController() {

}

//Constructor for SpotsInfoController sets hwnd
SpotInfoController::SpotInfoController(HWND hwnd) {
	this->_hwnd = hwnd;
}

//Handels the Commmand Messages
void SpotInfoController::Command(WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDOK:
			SendMessage(this->_hwnd, WM_CLOSE, wParam, lParam);	
		break;
		case IDC_SPOTINFO_UPDATE:
			SendMessage(this->_hwnd, WM_ACTIVATE, wParam, lParam);	
		break;
	}
}

//Sets all the lables to the right data
void SpotInfoController::Activate() {
	WCHAR s[200];
	if (GetParent(this->_hwnd)) {
		DataController * Parent = WinGetLong<DataController *> (GetParent(this->_hwnd));
		if (Parent) {
			Spots * Daten = Parent->GetData();
			Spot * spot = Daten->Get_Spot(Daten->Get_Selected_Spot());
			if (spot) {
				//Set ID
				swprintf_s(s,200,TEXT("%.0f"),(double)spot->id);
				SetDlgItemText(this->_hwnd, IDC_SPOTINFO_ID, s);
				//Set Height
				swprintf_s(s,200,TEXT("%.2f"),spot->hight*1e9);
				SetDlgItemText(this->_hwnd, IDC_SPOTINFO_HIGHT, s);
				//Set Area
				swprintf_s(s,200,TEXT("%.0f"),(double)spot->area);
				SetDlgItemText(this->_hwnd, IDC_SPOTINFO_AREA, s);
				delete spot;
			}
			//Set Caption
			wchar_t * buf = NULL;
			buf = Daten->Get_Filename();
			SetDlgItemText(this->_hwnd, IDC_CAPTION, buf);
			delete buf;
		}
	}
}

////////////////////////
// GLController Class //
////////////////////////

//Constructor for GLController sets all pointers to NULL
GLController::GLController() {
	this->Init_GLController();
}

//Constructor for GLController sets all pointers to NULL and inits hwnd
GLController::GLController (HWND hwnd) {
	this->Init_GLController();
	this->_hwnd = hwnd;
}

//GLController destructor
GLController::~GLController () {
	
}
//Initilizes the GLController
void GLController::Init_GLController() {
	this->_hwnd = NULL;
	this->_view = NULL;
	this->Daten = NULL;
}
//Resizes the drawing area
void GLController::Size(int width, int hight) {
	this->_view->ReSizeGLScene(width, hight);
}
//Draws the image on the screen
void GLController::Paint() {
	this->_view->DrawGLScene();
}

//Handels the selection
void GLController::Selection(int mouse_x, int mouse_y) {
	this->_view->Selection(mouse_x, mouse_y);
}

//handels the menu controll of the window 
void GLController::Command(WPARAM wParam, LPARAM lParam) {

}

//Sets up the data of the controller
void GLController::setData(Spots *Data) {
	this->_view->SetData(Data);
	this->Daten = Data;
}
//Returns the pointer to the Dataset coresponding to this controller
Spots * GLController::GetData() {
	return this->Daten;
}
//changes the perspectiv to view the GL scene
void GLController::change_perspective(WPARAM Key) {
	this->_view->change_perspective(Key);
}
//////////////////////////
// DataController Class //
//////////////////////////

//Initializes the DataController
DataController::DataController() {
	this->D_ImageSettings = NULL;
	this->D_SpotInfo = NULL;
	this->D_SpotsInfo = NULL;
	this->_range = 0;
	this->_offset = 0;
}

//Initializes the DataController
DataController::DataController (HWND hwnd) {
	this->GLController::GLController(hwnd);
	this->_view = new DataView(hwnd);
	this->GLController::_view = this->_view;
	this->D_SpotsInfo = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SPOTS_INFO), this->_hwnd, SpotsInfoDlgProc);
	this->D_SpotInfo = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SPOTINFO), this->_hwnd, SpotInfoDlgProc);
	this->D_ImageSettings = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_IMAGE_SETTINGS), this->_hwnd, IImageSettingsDlgProc);
	this->_range = 0.5e-9;
	this->_offset = 0;
	if(D_ImageSettings != NULL) {
		WCHAR s[20];
		swprintf_s(s,20,TEXT("%.2f"),_range*1e9);
		SetDlgItemText(D_ImageSettings, IDC_RANGE, s);
		swprintf_s(s,20,TEXT("%.2f"),_offset*1e9);
		SetDlgItemText(D_ImageSettings, IDC_OFFSET, s);
		swprintf_s(s,20,TEXT("%.2f"),this->_view->Get_Range_3D());
		SetDlgItemText(D_ImageSettings, IDC_3D_RANGE, s);
	}
}

//Destroys the DataController
DataController::~DataController() {
	delete this->_view;
	DestroyWindow(this->D_SpotsInfo);
	DestroyWindow(this->D_SpotInfo);
	DestroyWindow(this->D_ImageSettings);
}

//handels the menu controll of the window
void DataController::Command(WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case ID_DATA_IMAGE_SETTINGS:
			ShowWindow(this->D_ImageSettings, SW_SHOW);
		break;
		case ID_DATA_SPOTSINFO32791:
			ShowWindow(this->D_SpotsInfo, SW_SHOW);
		break;
		case ID_DATA_SPOTINFO32792:
			ShowWindow(this->D_SpotInfo, SW_SHOW);
		break;
		case ID_DISPLAY_COUNTETSPOTS32789:
			if(this->_view->Get_Display_Spots()) {
				this->_view->Set_Display_Spots(false);
				CheckMenuItem(GetMenu(this->_hwnd), ID_DISPLAY_COUNTETSPOTS32789, MF_UNCHECKED);
			} else {
				this->_view->Set_Display_Spots(true);
				CheckMenuItem(GetMenu(this->_hwnd), ID_DISPLAY_COUNTETSPOTS32789, MF_CHECKED);
			}
		break;
		case ID_DISPLAY_MASK32790:
			if(this->_view->Get_Display_Mask()) {
				this->_view->Set_Display_Mask(false);
				CheckMenuItem(GetMenu(this->_hwnd), ID_DISPLAY_MASK32790, MF_UNCHECKED);
			} else {
				this->_view->Set_Display_Mask(true);
				CheckMenuItem(GetMenu(this->_hwnd), ID_DISPLAY_MASK32790, MF_CHECKED);
			}
			this->_view->CreateDataTexture(this->_range,this->_offset);
		break;
		case ID_DISPLAY_3D:
			if(this->_view->Get_Display_3D()) {
				this->_view->Set_Display_3D(false);
				CheckMenuItem(GetMenu(this->_hwnd), ID_DISPLAY_3D, MF_UNCHECKED);
			} else {
				this->_view->Set_Display_3D(true);
				CheckMenuItem(GetMenu(this->_hwnd), ID_DISPLAY_3D, MF_CHECKED);
			}
		break;
		case ID_SAVE_AKTIVEHEIGHTS:
			if(this->Daten) {
				PWSTR pszFilePath = Save_File();
				if (pszFilePath) {
					this->Daten->Save_Aktiv_Hights(pszFilePath);
					delete pszFilePath;
				}
			}
			break;
		case ID_SAVE_HEIGHTS:
			if(this->Daten) {
				PWSTR pszFilePath = Save_File();
				if (pszFilePath) {
					this->Daten->Save_Hights(pszFilePath);
					delete pszFilePath;
				}
			}
			break;
		case ID_SAVE_AKTIVEAREAS:
			if(this->Daten) {
				PWSTR pszFilePath=Save_File();
				if (pszFilePath) {
					Daten->Save_Areas(pszFilePath, MODE_AKTIV);
					delete pszFilePath;
				}
			}
			break;
		case ID_SAVE_ALLAREAS:
			if(this->Daten) {
				PWSTR pszFilePath=Save_File();
				if (pszFilePath) {
					Daten->Save_Areas(pszFilePath, MODE_ALL);
					delete pszFilePath;
				}
			}
			break;
	}
}

//returns the Display_Mask value
bool DataController::Get_Display_Mask() {
	return this->_view->Get_Display_Mask();
}
//Sets the Display_Mask parameter
void DataController::Set_Display_Mask(bool Display_Mask) {
	this->_view->Set_Display_Mask(Display_Mask);
}
//returns the Display_Spots value
bool DataController::Get_Display_Spots() {
	return this->_view->Get_Display_Spots();
}
//Sets the Display_Spots parameter
void DataController::Set_Display_Spots(bool Display_Spots) {
	this->_view->Set_Display_Spots(Display_Spots);
}
//returns the Display_3D value
bool DataController::Get_Display_3D() {
	return this->_view->Get_Display_3D();
}
//Sets the Display_3D parameter
void DataController::Set_Display_3D(bool Display_3D) {
	this->_view->Set_Display_3D(Display_3D);
}
//returns the Range_3D value
double DataController::Get_Range_3D() {
	return this->_view->Get_Range_3D();
}
//Sets the Range_3D parameter
void DataController::Set_Range_3D(double Range_3D) {
	this->_view->Set_Range_3D(Range_3D);
}
//Sets the offset value and creates a new bitmap
void DataController::Set_Offset(double offset) {
	this->_offset = offset;
	this->_view->CreateDataTexture(this->_range, this->_offset);
}
//Sets the range value and creates a new bitmap
void DataController::Set_Range(double range) {
	this->_range = range;
	this->_view->CreateDataTexture(this->_range, this->_offset);
}
//Gets the offset value for this data
double DataController::Get_Offset() {
	return this->_offset;
}
//Gets the range value for this data
double DataController::Get_Range() {
	return this->_range;
}

//Updates the Spotinfo dialog
void DataController::Update_Dialogs() {
	SendMessage(this->D_SpotInfo, WM_ACTIVATE, 0, 0);
}

////////////////////////////////
// HistogrammController Class //
////////////////////////////////

//Basic Constructor Initilizes all members to NULL
HistogrammController::HistogrammController() {
	this->_view = NULL;
	this->_hwnd = NULL;
	this->D_HistogrammSettings =NULL;
	
}
//Constructor Initilazes all Member functions
HistogrammController::HistogrammController(HWND hwnd) {
	this->GLController::GLController(hwnd);
	this->_view = new HistogrammView(hwnd);
	this->GLController::_view = this->_view;
	this->D_HistogrammSettings = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_HISTOGRAMMSETTINGS), this->_hwnd, HistogrammSettingsDlgProc);
}
//Destructor destroys all members
HistogrammController::~HistogrammController() {
	delete this->_view;
}
//Handels the Comand event for the window
void HistogrammController::Command(WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case ID_DATA_HISTOGRAMMSETTING: {
			ShowWindow(this->D_HistogrammSettings, SW_SHOW);
		}
		break;
	}
}
//Returns the nuber of bins of the histogramm
int HistogrammController::Get_Bin_Number() {
	return this->_view->Get_Bin_Number();
}
//Returns the heighest count of elements in one bin
int HistogrammController::Get_Max_Number() {
	return this->_view->Get_Max_Number();
}
//Returns the minimal height at which the histogramm starts
double HistogrammController::Get_Min_Height() {
	return this->_view->Get_Min_Height();
}
//Retuns the maximum height at which the histogramm ends
double HistogrammController::Get_Max_Height() {
	return this->_view->Get_Max_Height();
}
//sets the maximum count against which the bins are plotted
void HistogrammController::Set_Max_Number(int Max_Number) {
	this->_view->Set_Max_Number(Max_Number);
}
//Sets the number of bins of the histogramm
void HistogrammController::Set_Bin_Number(int Bin_Number) {
	this->_view->Set_Bin_Number(Bin_Number);
}
//Sets the minimum height where the first bin starts
void HistogrammController::Set_Min_Height(double Min_Height) {
	this->_view->Set_Min_Height(Min_Height);
}
//Sets the maximum height at which the last bin ends
void HistogrammController::Set_Max_Height(double Max_Height) {
	this->_view->Set_Max_Height(Max_Height);
	
}
////////////////////////////////
// AreaHeightController Class //
////////////////////////////////

//Basic Constructor Initilizes all members to NULL
AreaHeightController::AreaHeightController() {
	this->_view = NULL;
	this->_hwnd = 0;
	this->D_AreaHeightSettings = NULL;
	
}
//Constructor Initilazes all Member functions
AreaHeightController::AreaHeightController(HWND hwnd) {
	this->GLController::GLController(hwnd);
	this->_view = new AreaHeightView(hwnd);
	this->GLController::_view = this->_view;
	this->D_AreaHeightSettings = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_AREAHEIGHT_SETTINGS), this->_hwnd, AreaHeightSettingsDlgProc);
}
//Destructor destroys all members
AreaHeightController::~AreaHeightController() {
	delete this->_view;
}
//Handels the Comand event for the window
void AreaHeightController::Command(WPARAM wParam, LPARAM lParam) {

switch(LOWORD(wParam)) {
		case ID_FILE_AREAHEIGHT_SETTINGS: {
			ShowWindow(this->D_AreaHeightSettings, SW_SHOW);
		}
		break;
	}
}
//Sets the max Height value for the plot
void AreaHeightController::Set_Max_Height(double max_height) {
	this->_view->Set_Max_Height(max_height);
}
//Sets the min height value for the plot
void AreaHeightController::Set_Min_Height(double min_height) {
	this->_view->Set_Min_Height(min_height);
}
//Sets the max area value for the plot
void AreaHeightController::Set_Max_Area(int max_area) {
	this->_view->Set_Max_Area(max_area);
}
//Sets the min area value for the plot
void AreaHeightController::Set_Min_Area(int min_area) {
	this->_view->Set_Min_Area(min_area);
}
//Sets cut of area for the fit values and recalculates the fit
void AreaHeightController::Set_Fit_Area(int fit_area) {
	this->_view->Set_Fit_Area(fit_area);
}
//Sets the Error multipleier for the fit
void AreaHeightController::Set_Error_Multiplier(double error_multiplier) {
	this->_view->Set_Error_Multiplier(error_multiplier);
}
//returns the max height value of the plot
double AreaHeightController::Get_Max_Height() {
	return this->_view->Get_Max_Height();
}
//returns the min height value of the plot
double AreaHeightController::Get_Min_Height() {
	return this->_view->Get_Min_Height();
}
//returns the max area value of the plot
int AreaHeightController::Get_Max_Area() {
	return this->_view->Get_Max_Area();
}
//returns the min area value of the plot
int AreaHeightController::Get_Min_Area() {
	return this->_view->Get_Min_Area();
}
//retunrs the area above which the data is no longer used for the fit
int AreaHeightController::Get_Fit_Area() {
	return this->_view->Get_Fit_Area();
}
//returns the error multiplier of the fit
double AreaHeightController::Get_Error_Multiplier() {
	return this->_view->Get_Error_Multiplier();
}
//////////////////
// GLView Class //
//////////////////

//Constructor: initializes the GLView class and sets the pointers of the DC and RC to NULL
GLView::GLView (HWND hwnd) {
	this->Init_GLView();
	this->GLViewInit(hwnd);
}

//Constructor: sets the pointers of the DC and RC to NULL
GLView::GLView () {
	this->Init_GLView();
}

//Destructor: frees the DC and RC
GLView::~GLView () {
	FreeDCRC();
	if (this->_gmf) delete this->_gmf;
	glDeleteLists(this->_Font, 256);
}

//initilizes all variables of GLView
void GLView::Init_GLView() {
	this->_hdc = NULL;
	this->_hrc = NULL;
	this->_gmf = NULL;
	this->_hwnd = NULL;
	this->selection = false;
	this->_pos_x = 0;
	this->_pos_y = 0;
	this->_pos_z = -2.0;
	this->_rot_x = 0;
	this->_rot_y = 0;
	this->_rot_z = 0;
}
//initializes the GLView class by choosing the right pixel format and creating the DC and RC
void GLView::GLViewInit(HWND hwnd) {
	this->_hwnd = hwnd;
	GLuint		PixelFormat;						// Holds The Results After Searching For A Match
	
	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		16,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(this->_hdc=GetDC(this->_hwnd)))			// Did We Get A Device Context?
	{
		int x = GetLastError();
		MessageBox(this->_hwnd,TEXT("Can't Create A GL Device Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
	}
	if (!(PixelFormat=ChoosePixelFormat(this->_hdc,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		MessageBox(this->_hwnd,TEXT("Can't Find A Suitable PixelFormat."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
	}

	if(!SetPixelFormat(this->_hdc,PixelFormat,&pfd))// Are We Able To Set The Pixel Format?
	{
		MessageBox(this->_hwnd,TEXT("Can't Set The PixelFormat."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
	}

	if (!(this->_hrc=wglCreateContext(this->_hdc)))	// Are We Able To Get A Rendering Context?
	{
		MessageBox(this->_hwnd,TEXT("Can't Create A GL Rendering Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
	}
	if(!wglMakeCurrent(this->_hdc,this->_hrc))		// Try To Activate The Rendering Context
	{
		MessageBox(this->_hwnd,TEXT("Can't Activate The GL Rendering Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
	}
	if (!this->InitGL())									// Initialize Our Newly Created GL Window
	{
		MessageBox(this->_hwnd,TEXT("Initialization Failed."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
	}
}

//Frees the DC and RC of the GLView
void GLView::FreeDCRC() {
	if (this->_hrc)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,TEXT("Release Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(this->_hrc))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,TEXT("Release Rendering Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		this->_hrc=NULL;										// Set RC To NULL
	}

	if (this->_hdc && !ReleaseDC(this->_hwnd ,this->_hdc))					// Are We Able To Release The DC
	{
		MessageBox(NULL,TEXT("Release Device Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		this->_hdc=NULL;										// Set DC To NULL
	}
}
//Resizes the GL scene to the new size of the display
GLvoid GLView::ReSizeGLScene(GLsizei width, GLsizei height) {	// Resize And Initialize The GL Window
	if(!wglMakeCurrent(this->_hdc,this->_hrc)) {
		MessageBox(NULL,TEXT("Set Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
	}

	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}
//Virtual, Initilizes the GL Scene
bool GLView::InitGL(GLvoid)								// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(1.0f, 1.0f, 1.0f, 0.5f);				// Black Background
	glClearDepth(10.0f);									// Depth Buffer Setup
	glDepthRange(0.0f, 10.0f);
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return TRUE;										// Initialization Went OK
}
//Sets the Data for the window
void GLView::SetData(Spots* Data) {
	this->Daten = Data;
}
//handels the selection of objects
GLint GLView::Selection(int mouse_x, int mouse_y) {
	if(!wglMakeCurrent(this->_hdc,this->_hrc)) {
		MessageBox(NULL,TEXT("Set Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
	}
	GLuint	buffer[512];						// Set Up A Selection Buffer
	GLint	hits;								// The Number Of Objects That We Selected
	// The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>
	GLint	viewport[4];

	// This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);				// Tell OpenGL To Use Our Array For Selection
	// Puts OpenGL In Selection Mode. Nothing Will Be Drawn. Object ID's and Extents Are Stored In The Buffer.
	(void) glRenderMode(GL_SELECT);

	glInitNames();								// Initializes The Name Stack
	glPushName(0);								// Push 0 (At Least One Entry) Onto The Stack
	glMatrixMode(GL_PROJECTION);						// Selects The Projection Matrix
	glPushMatrix();								// Push The Projection Matrix
	glLoadIdentity();							// Resets The Matrix

	// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
	gluPickMatrix((GLdouble) mouse_x, (GLdouble) (viewport[3]-mouse_y), 5.0f, 5.0f, viewport);
	// Apply The Perspective Matrix
	gluPerspective(45.0f, (GLfloat) (viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]), 0.1f, 5.0f);
	glMatrixMode(GL_MODELVIEW);					// Select The Modelview Matrix
	this->selection = true;
	DrawGLScene();								// Render The Targets To The Selection Buffer
	this->selection = false;
	glMatrixMode(GL_PROJECTION);				// Select The Projection Matrix
	glPopMatrix();								// Pop The Projection Matrix
	glMatrixMode(GL_MODELVIEW);					// Select The Modelview Matrix
	hits=glRenderMode(GL_RENDER);				// Switch To Render Mode, Find Out How Many
	if (hits > 0)								// If There Were More Than 0 Hits
	{
		this->Daten->Set_Selected_Spot(buffer[(hits-1)*4+3]);
		return buffer[(hits-1)*4+3];
	}
	this->Daten->Set_Selected_Spot(0);
	return 0;	
}
//Creates a font with the name Font_Name to use with the glPrint function
void GLView::Create_Font(wchar_t* Font_Name) {
	HFONT	font;						// Windows Font ID
	this->_gmf = new GLYPHMETRICSFLOAT[256];
	this->_Font = glGenLists(256);			// Storage For 256 Characters
	font = CreateFont(	-12,				// Height Of Font

							0,				// Width Of Font
				0,							// Angle Of Escapement
				0,							// Orientation Angle
				FW_BOLD,					// Font Weight
				FALSE,						// Italic
				FALSE,						// Underline
				FALSE,						// Strikeout
				ANSI_CHARSET,				// Character Set Identifier
				OUT_TT_PRECIS,				// Output Precision
				CLIP_DEFAULT_PRECIS,		// Clipping Precision
				ANTIALIASED_QUALITY,		// Output Quality
				FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
				Font_Name);					// Font Name
	SelectObject(this->_hdc, font);			// Selects The Font We Want
	wglUseFontOutlines(this->_hdc,			// Select The Current DC
				0,							// Starting Character
				256-1,						// Number Of Display Lists To Build
				this->_Font,				// Starting Display Lists
				0.0f,						// Deviation From The True Outlines
				0.0f,						// Font Thickness In The Z Direction
				WGL_FONT_POLYGONS,			// Use Polygons, Not Lines
				this->_gmf);
}

//changes the perspectiv to view the GL scene
void GLView::change_perspective(WPARAM Key) {
	switch (Key) {
		case 'W':
			this->_pos_y += 0.1;
			break;
		case 'S':
			this->_pos_y -= 0.1;
			break;
		case 'A':
			this->_pos_x += 0.1;
			break;
		case 'D':
			this->_pos_x -= 0.1;
			break;
		case 'Q':
			this->_pos_z += 0.1;
			break;
		case 'E':
			this->_pos_z -= 0.1;
			break;
		case VK_UP:
			this->_rot_x += 0.5;
			break;
		case VK_DOWN:
			this->_rot_x -= 0.5;
			break;
		case VK_RIGHT:
			this->_rot_y += 0.5;
			break;
		case VK_LEFT:
			this->_rot_y -= 0.5;
			break;
		case VK_PRIOR:
			this->_rot_z += 0.5;
			break;
		case VK_NEXT:
			this->_rot_z -= 0.5;
			break;
	}
	this->DrawGLScene();
}
////////////////////
// DataView Class //
////////////////////

//Basic constructor calls the Init function of Dataview
DataView::DataView() {
	this->DataViewInit();
}
//Constructor calls the Init function of DataView and GLview
DataView::DataView(HWND hwnd) {
	this->GLViewInit(hwnd);
	this->DataViewInit();
}
//Init function to Initialice all members
void DataView::DataViewInit() {
	this->Daten = NULL;
	this->textured = false;
	this->_Display_Mask = true;
	this->_Display_Spots = false;
	this->_Display_3D = false;
	this->_Range_3D = 0.004;
}
//Draws the background and the Background square
void DataView::DrawGLScene() {
	//Check if we can set the DC and RC
	if (!selection) {
		if(!wglMakeCurrent(this->_hdc,this->_hrc)) { //set the hdc and hrc only f selection is false
			MessageBox(NULL,TEXT("Set Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	this->DrawSpots();
	if (!selection) {
		SwapBuffers(this->_hdc);
	}
}

//Draws all the Active spots on top of the background
void DataView::DrawSpots() {
	glPushMatrix();
		glTranslated(this->_pos_x,this->_pos_y, this->_pos_z);
		glRotated(this->_rot_x,1,0,0);
		glRotated(this->_rot_y,0,1,0);
		glRotated(this->_rot_z,0,0,1);
		glTranslated(-0.5,-0.5,0);
		glPushMatrix();
			if (this->textured) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, this->texture[0]);               // Select Our Texture			
				glColor3f(1.0f, 1.0f, 1.0f);
			} else {
				glColor3f(0.0f, 0.0f, 0.0f);
			}
			glLoadName(0);	
			glPushMatrix();
				glTranslatef(0.5f, 0.5f, 0.0f);
				glSquare3d(0.5,0.5,0.0);
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		//Draw Spots
		
		Spot* aktuell;
		Point* point;
		double length_1 = 1.0/Daten->Get_Length();
		for (int n = 1; n <= Daten->Get_Spot_Number(); n++) {
			if (this->selection || this->_Display_Spots) {
				glLoadName(n);	
				glColor3f(1.0f,0.0f,0.0f);
				if (n==Daten->Get_Selected_Spot()) glColor3f(0.0f,1.0f,0.0f);
				aktuell = Daten->Get_Spot(n);
				point = aktuell->first;
				if (aktuell->aktiv) {
					while(point) {
						glPushMatrix();
							glTranslated(point->x*length_1, point->y*length_1, 0.0f);
							if (this->_Display_3D) {
								glTranslated(0.0, 0.0, point->z*0.5*1e9*this->_Range_3D);
								glCuboid3d(0.5f*length_1,0.5f*length_1,point->z*0.5*1e9*this->_Range_3D);
							} else {
								glSquare3d(0.5f*length_1,0.5f*length_1,0.001f);
							}
						glPopMatrix();	
						point = point->next;
					}
				}
				delete aktuell;
			} else {
				if (n==Daten->Get_Selected_Spot()) { 
					glColor3f(0.0f,1.0f,0.0f);
					aktuell = Daten->Get_Spot(n);
					point = aktuell->first;
					if (aktuell->aktiv) {
						while(point) {
							glPushMatrix();
								glTranslated(point->x*length_1, point->y*length_1, 0.0f);
								if (this->_Display_3D) {
									glTranslated(0.0, 0.0, point->z*0.5*1e9*this->_Range_3D);
									glCuboid3d(0.5f*length_1,0.5f*length_1,point->z*0.5*1e9*this->_Range_3D);
								} else {
									glSquare3d(0.5f*length_1,0.5f*length_1,0.001f);
								}
							glPopMatrix();	
							point = point->next;
						}
					}
					delete aktuell;
				}
			
			}
		}
	glPopMatrix();
}

//Creates the Texture of the AFM image, if displaymask is true it draws all points higher than offset+range in blue
void DataView::CreateDataTexture(double range, double offset) {
	if (this->Daten->Get_Data_Loaded()) {
		if(!wglMakeCurrent(this->_hdc,this->_hrc)) {
			MessageBox(NULL,TEXT("Set Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		glGenTextures(1, &this->texture[0]);                  // Create The Texture
 
		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, this->texture[0]);
		// Generate The Texture
		BYTE data[512*512*3];
		for (int n = 0; n <512*512; n++) {
			RGBColor color = this->Daten->GetPixelColor(range,offset,n,this->_Display_Mask);
			data[n*3  ] = color.Red;
			data[n*3+1] = color.Green;
			data[n*3+2] = color.Blue;
		}
		glPixelStorei ( GL_UNPACK_ALIGNMENT,   4 );
		glPixelStorei ( GL_UNPACK_ROW_LENGTH,  0 );
		glPixelStorei ( GL_UNPACK_SKIP_ROWS,   0 );
		glPixelStorei ( GL_UNPACK_SKIP_PIXELS, 0 );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		this->textured = true;
	}
}

//Sets the Data for the window
void DataView::SetData(Spots* Data) {
	GLView::SetData(Data);
	this->CreateDataTexture(5e-10,0);
}

//returns the Display_Mask value
bool DataView::Get_Display_Mask() {
	return this->_Display_Mask;
}
//Sets the Display_Mask parameter
void DataView::Set_Display_Mask(bool Display_Mask) {
	this->_Display_Mask = Display_Mask;
}
//returns the Display_Spots value
bool DataView::Get_Display_Spots() {
	return this->_Display_Spots;
}
//Sets the Display_Spots parameter
void DataView::Set_Display_Spots(bool Display_Spots) {
	this->_Display_Spots = Display_Spots;
}

//returns the Display_3D value
bool DataView::Get_Display_3D() {
	return this->_Display_3D;
}
//Sets the Display_3D parameter
void DataView::Set_Display_3D(bool Display_3D) {
	this->_Display_3D = Display_3D;
}

//returns the Range_3D value
double DataView::Get_Range_3D() {
	return this->_Range_3D;
}
//Sets the Range_3D parameter
void DataView::Set_Range_3D(double Range_3D) {
	this->_Range_3D = Range_3D;
}

//////////////////////////
// HistogrammView Class //
//////////////////////////

//Constructs a HistogrammView object
HistogrammView::HistogrammView() {
	this->HistogrammView_Init();
}
//Constructs a HistogrammView object
HistogrammView::HistogrammView(HWND hwnd) {
	this->GLViewInit(hwnd);
	this->Create_Font(L"Times New Roman");
	this->HistogrammView_Init();
}
//Destroys all objects created by HistogrammView
HistogrammView::~HistogrammView() {
	if (this->_bins) delete this->_bins;
	if (this->_elements) delete this->_elements;
}
//Initilazes all HistogrammView members
void HistogrammView::HistogrammView_Init() {
	this->_min_number = 5;
	this->_bin_number = 0;
	this->_bin_elements = 30;
	this->_max_number = 0;
	this->_min_height = 0;
	this->_max_height = 0;
	this->_bins = NULL;
	this->_elements = NULL;
}
//Draws the Histogramm
void HistogrammView::DrawGLScene() {
	if(!wglMakeCurrent(this->_hdc,this->_hrc)) { //set the hdc and hrc only f selection is false
		MessageBox(NULL,TEXT("Set Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	glPushMatrix();
		glTranslatef(-0.0f, -0.0f, -2.0f);	
		glTranslatef(-0.7f, -0.7f, -0.0f);
		this->Draw_Axes();	
		for (int i = 0; i < this->_bin_number; i++) {
			double width = 0.65/this->_bin_number;
			double height = 0.65/this->_max_number*this->_bins[i];
			double height_p = 0.65/this->_max_number*this->Poisson(i)*this->_element_number;
			glPushMatrix();
				glColor3f(0.0f, 0.0f, 1.0f);
				glTranslated((i*2+1)*width,height,0.0);
				glSquare3d(width,height,0.0);
			glPopMatrix();
			glPushMatrix();
				glColor3f(1.0f, 0.0f, 0.0f);
				glTranslated((i*2+1)*width,height_p,0.0);
				glSquare3d(width,height_p,0.0);
			glPopMatrix();
		}
	glPopMatrix();
	
	SwapBuffers(this->_hdc);
}

//Loads the Data and creates the histogram if the data is already loaded
void HistogrammView::SetData(Spots *Data) {
	GLView::SetData(Data);
	Calc_Histogramm_Data();
	Calc_Mean();
}

//Calculates the histogramm from the given data
void HistogrammView::Calc_Histogramm_Data() {
	if (this->Daten && this->Daten->Get_Spots_Counted()) {
		//init all members that depent on the loaded data
		this->_element_number = this->Daten->Get_Aktiv_Spots();
		this->_elements = Daten->Get_Hights(MODE_AKTIV);
		this->_bin_number = (int) ( (double) this->_element_number/this->_bin_elements);
		//Sort the elements
		this->Sort();
		//Get the minimum and muximum heights
		this->_max_height = this->_elements[this->_element_number-1-this->_min_number];
		this->_min_height = this->_elements[0];
		//create the bins array
		this->Init_Bins();
		//count the number of elemts for each bin
		this->Count_Bin_Elements();
		//find max number in one bin
		this->_max_number = this->Find_Max_Number();
	}
}
//Calculates the mean from the given data
void HistogrammView::Calc_Mean() {
	if (this->_bins) {
		int temp = 0;
		for (int i = 0; i < this->_bin_number; i++) {
			temp += this->_bins[i]*(i+1);
		}
		this->_mean = (double)temp/this->_element_number;
	}
}
//Sorts the elmenets array the smallest value is at the beginning
void HistogrammView::Sort() {
	double * temp = new double[this->_element_number];
	for (int i = 0; i < this->_element_number; i++) {
		int smaller = 0;
		int same = 0;  
		for (int j = 0; j < this->_element_number; j++) {
			if (this->_elements[j] < this->_elements[i]) smaller++;
			if (this->_elements[j] == this->_elements[i]) same++;
		}
		for (int j = 0; j < same; j++) {
			temp[smaller+j] = this->_elements[i];
			if (smaller==0) {
				int test = 0;
			}
		}
	}
	delete[] this->_elements;
	this->_elements = temp;
}
//Draws the Axes labels and scale for the plot
void HistogrammView::Draw_Axes() {
	glColor3d(0.0,0.0,0.0);
	
	//Draw the lines
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		glVertex3d(-0.05, 0, 0);
		glVertex3d(1.4, 0, 0);

		glVertex3d(0, -0.05, 0);
		glVertex3d(0, 1.4, 0);
	glEnd();
	//Draw arrow heads and lables
	//x Achses
	glPushMatrix();
		glTranslated(1.4,0,0);
		glBegin(GL_TRIANGLES);
			glVertex3d(0, 0, 0);
			glVertex3d(-0.05, 0.025, 0);
			glVertex3d(-0.05, -0.025, 0);
		glEnd();
		glTranslated(0.1,0,0);
		glScaled(0.05,0.05,0.05);
		glPrint("r [nm]", this->_Font, this->_gmf);
	glPopMatrix();
	//y Achses
	glPushMatrix();
		glTranslated(0,1.4,0);
		glBegin(GL_TRIANGLES);
			glVertex3d(0, 0, 0);
			glVertex3d(0.025, -0.05, 0);
			glVertex3d(-0.025, -0.05, 0);
		glEnd();
		glTranslated(0,0.03,0);
		glScaled(0.05,0.05,0.05);
		glPrint("N", this->_Font, this->_gmf);
	glPopMatrix();
	//Draw Scale
	//x-Achses
	double range_x = -(this->_min_height-this->_max_height)/this->_bin_number;
	for (int i = 0; i < this->_bin_number; i++) {
		glPushMatrix();
			glTranslated(1.3/this->_bin_number*i,0,0);
			//Draw the Lines
			glBegin(GL_LINES);
				glVertex3d(0, 0.025, 0);
				glVertex3d(0, -0.025, 0);
			glEnd();
			glTranslated(0,-0.1,0);
			char s[20];
			sprintf_s(s,20,"%.2f",(this->_min_height+range_x*i)*1e9);
			glScaled(0.05,0.05,0.05);
			glPrint(s,this->_Font, this->_gmf);
		glPopMatrix();
	}
	//x-Achses
	double range_y = this->_max_number/5;
	for (int i = 0; i < 6; i++) {
		glPushMatrix();
			glTranslated(0,1.3/5*i,0);
			//Draw the Lines
			glBegin(GL_LINES);
				glVertex3d( 0.025, 0, 0);
				glVertex3d(-0.025, 0, 0);
			glEnd();
			glTranslated(-0.1,0,0);
			char s[20];
			sprintf_s(s,20,"%.0f",(range_y*i));
			glScaled(0.05,0.05,0.05);
			glPrint(s,this->_Font, this->_gmf);
		glPopMatrix();
	}
}
//Initilizes the Bins array
void HistogrammView::Init_Bins() {
	if (this->_bin_number > 0) {
		if (this->_bins) delete[] this->_bins;
		this->_bins = new int[this->_bin_number];
		for (int i = 0; i < this->_bin_number; i++) {
			this->_bins[i] = 0;
		}
	}
}

//Returns the number of counts in the most filled bin if bins is not initilized it returns 0
int HistogrammView::Find_Max_Number() {
	if (!this->_bins) return 0;
	int max = this->_bins[0];
	for (int i = 1; i < this->_bin_number; i++) {
		if (this->_bins[i] > max) max = this->_bins[i];
	}
	return max;
}
//Counts the number of elements for each bin
void HistogrammView::Count_Bin_Elements() {
	double range = (this->_max_height - this->_min_height)/(this->_bin_number-1); //the width in height of each bin 
	int j = 0; //counts the bin into which the elements are added
	double scope = range + this->_min_height; //the maximum hight of the next bin
	for (int i = 0; i < this->_element_number; i++) {
		if (this->_elements[i] < scope) {
			this->_bins[j]++;
		} else {
			j++;
			scope = range*(1+j) + this->_min_height;
			if (j == this->_bin_number-1) {
				this->_bins[j] = this->_element_number-i;
				break;
			}
		}
	}
}
//Calculates th Poisson value coresbonding to the nth bin
double HistogrammView::Poisson(int n) {
	return (pow(this->_mean/this->_element_number,n) * pow(1-this->_mean/this->_element_number,this->_element_number-n) * factorial(this->_element_number)/factorial(n)/factorial(this->_element_number-n));
}
//Returns the nuber of bins of the histogramm
int HistogrammView::Get_Bin_Number() {
	return this->_bin_number;
}
//Returns the heighest count of elements in one bin
int HistogrammView::Get_Max_Number() {
	return this->_max_number;
}
//Returns the minimal height at which the histogramm starts
double HistogrammView::Get_Min_Height() {
	return this->_min_height;
}
//Retuns the maximum height at which the histogramm ends
double HistogrammView::Get_Max_Height() {
	return this->_max_height;
}
//sets the maximum count against which the bins are plotted
void HistogrammView::Set_Max_Number(int Max_Number) {
	this->_max_number = Max_Number;
}
//Sets the number of bins of the histogramm
void HistogrammView::Set_Bin_Number(int Bin_Number) {
	this->_bin_number = Bin_Number;
	this->Init_Bins();
	this->Count_Bin_Elements();
	this->_max_number = this->Find_Max_Number();
	this->Calc_Mean();
}
//Sets the minimum height where the first bin starts
void HistogrammView::Set_Min_Height(double Min_Height) {
	this->_min_height = Min_Height;
	this->Init_Bins();
	this->Count_Bin_Elements();
	this->_max_number = this->Find_Max_Number();
	this->Calc_Mean();
}
//Sets the maximum height at which the last bin ends
void HistogrammView::Set_Max_Height(double Max_Height) {
	this->_max_height = Max_Height;
	this->Init_Bins();
	this->Count_Bin_Elements();
	this->_max_number = this->Find_Max_Number();
	this->Calc_Mean();
}
//////////////////////////
// AreaHeightView Class //
//////////////////////////

//Constructs a AreaHeightView object
AreaHeightView::AreaHeightView() {
	this->AreaHeightView_Init();
}
//Constructs a AreaHeightView object
AreaHeightView::AreaHeightView(HWND hwnd) {
	this->GLViewInit(hwnd);
	this->Create_Font(L"Times New Roman");
	this->AreaHeightView_Init();
}
//Destroys all objects created by AreaHeightView
AreaHeightView::~AreaHeightView() {
	if (this->_Heights) delete this->_Heights;
	if (this->_Areas)	delete this->_Areas; 
}
//Initilazes all AreaHeightView members
void AreaHeightView::AreaHeightView_Init() {
	this->_Areas = NULL;
	this->_Heights = NULL;
	this->_max_Area = 0;
	this->_min_Area = 0;
	this->_max_Height = 0;
	this->_min_Height = 0;
	this->_fit_Area = 0;
	this->_intercept = 0;
	this->_slope = 0;
	this->_error_intercept = 0;
	this->_error_slope = 0;
	this->_error_multiplier = 3;
}
//Draws the AreaHight Plot
void AreaHeightView::DrawGLScene() {
	if (!selection) {
		if(!wglMakeCurrent(this->_hdc,this->_hrc)) { //set the hdc and hrc only f selection is false
			MessageBox(NULL,TEXT("Set Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	Spot * aktuell;	
	glPushMatrix();
		glTranslatef(-0.7f, -0.7f, -2.0f);
		this->Draw_Axes();
		this->Draw_Fit();
		double range_x = this->_max_Height - this->_min_Height;
		double range_y = this->_max_Area - this->_min_Area;
		for (int n = 1; n <= Daten->Get_Spot_Number(); n++) {
			aktuell = Daten->Get_Spot(n);
			if (aktuell->aktiv) {	
				glLoadName(n);	
				glColor3f(0.0f,0.0f,0.0f);
				if (n==Daten->Get_Selected_Spot()) glColor3f(0.0f,1.0f,0.0f);
				glPushMatrix();
					glTranslated((aktuell->hight-this->_min_Height)/range_x*1.3,(double)(aktuell->area-this->_min_Area)/range_y*1.3,0.0);
					glSquare3d(0.01,0.01,0);
				glPopMatrix();
			}
		}		
	glPopMatrix();
	
	if (!selection) {
		SwapBuffers(this->_hdc);
	}
}
//Loads the Data and creates the AreaHight plot if the data is already loaded
void AreaHeightView::SetData(Spots *Data) {
	GLView::SetData(Data);
	Set_Data();
}
//Sets the Data for the object
void AreaHeightView::Set_Data() {
	if (this->Daten && this->Daten->Get_Spots_Counted()) {
		this->_Heights = this->Daten->Get_Hights(MODE_AKTIV);
		this->_Areas   = this->Daten->Get_Areas(MODE_AKTIV);
		this->_max_Height = this->Daten->Get_Hight_Max();
		this->_max_Area	  = this->Daten->Get_Area_Max();
		this->_min_Height = this->Daten->Get_Min_Hight();
		this->_min_Area	  = this->Daten->Get_Min_Area();
		this->_fit_Area   = this->_max_Area;
		this->Calc_Lin_Fit();
	}
}
//Draws the Axes labels and scale for the plot
void AreaHeightView::Draw_Axes() {
	glColor3d(0.0,0.0,0.0);
	
	//Draw the lines
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		glVertex3d(-0.05, 0, 0);
		glVertex3d(1.4, 0, 0);

		glVertex3d(0, -0.05, 0);
		glVertex3d(0, 1.4, 0);
	glEnd();
	//Draw arrow heads and lables
	//x Achses
	glPushMatrix();
		glTranslated(1.4,0,0);
		glBegin(GL_TRIANGLES);
			glVertex3d(0, 0, 0);
			glVertex3d(-0.05, 0.025, 0);
			glVertex3d(-0.05, -0.025, 0);
		glEnd();
		glTranslated(0.1,0,0);
		glScaled(0.05,0.05,0.05);
		glPrint("r [nm]", this->_Font, this->_gmf);
	glPopMatrix();
	//y Achses
	glPushMatrix();
		glTranslated(0,1.4,0);
		glBegin(GL_TRIANGLES);
			glVertex3d(0, 0, 0);
			glVertex3d(0.025, -0.05, 0);
			glVertex3d(-0.025, -0.05, 0);
		glEnd();
		glTranslated(0,0.03,0);
		glScaled(0.05,0.05,0.05);
		glPrint("Area[pixel]", this->_Font, this->_gmf);
	glPopMatrix();
	//Draw Scale
	//x-Achses
	double range_x = (this->_max_Height-this->_min_Height)/5;
	for (int i = 0; i < 6; i++) {
		glPushMatrix();
			glTranslated(1.3/5*i,0,0);
			//Draw the Lines
			glBegin(GL_LINES);
				glVertex3d(0, 0.025, 0);
				glVertex3d(0, -0.025, 0);
			glEnd();
			glTranslated(0,-0.1,0);
			char s[20];
			sprintf_s(s,20,"%.2f",(this->_min_Height+range_x*i)*1e9);
			glScaled(0.05,0.05,0.05);
			glPrint(s,this->_Font, this->_gmf);
		glPopMatrix();
	}
	//y-Achses
	double range_y = (this->_max_Area-this->_min_Area)/5;
	for (int i = 0; i < 6; i++) {
		glPushMatrix();
			glTranslated(0,1.3/5*i,0);
			//Draw the Lines
			glBegin(GL_LINES);
				glVertex3d( 0.025, 0, 0);
				glVertex3d(-0.025, 0, 0);
			glEnd();
			glTranslated(-0.1,0,0);
			char s[20];
			sprintf_s(s,20,"%.0f",(this->_min_Area+range_y*i));
			glScaled(0.05,0.05,0.05);
			glPrint(s,this->_Font, this->_gmf);
		glPopMatrix();
	}
}

//Fits a line to the data
void AreaHeightView::Calc_Lin_Fit() {
	int * y = this->_Areas;
	double * x = this->_Heights;
	int m = this->Daten->Get_Aktiv_Spots(); 
	int n = 0;
	double a,b,c,d,s;			//Sum variables a=x b=x^2 c=y d=x*y; s = (ye - yf)^2
	a=b=c=d=s=0;
	for (int i= 0; i < m; i++) {
		if (y[i] < this->_fit_Area) {
			a += x[i]*1e9;
			b += x[i]*x[i]*1e9*1e9;
			c += y[i];
			d += x[i]*1e9*y[i];
			n++;
		}
	}
	this->_intercept = (c*b-a*d)/(n*b-a*a);
	this->_slope = (d*n-c*a)/(n*b-a*a);
	for (int i= 0; i < m; i++) {
		if (y[i] < this->_fit_Area) {
			double y_dif = (y[i] - (this->_slope * x[i]*1e9 + this->_intercept));
			s += y_dif * y_dif;
		}
	}
	this->_error_intercept = sqrt(s/(n-3)*b/(n*b-a*a));
	this->_error_slope = sqrt(s/(n-3)*n/(n*b-a*a));
}
//Draws the fit Line and shows the fit parameters
void AreaHeightView::Draw_Fit() {
	//Draw Fit
	glColor3d(1.0,0,0);
	this->Draw_Line(this->_slope, this->_intercept);
	//Draw Error estimation
	glColor3d(0.0,1.0,0);
	this->Draw_Line(this->_slope - this->_error_multiplier*this->_error_slope, this->_intercept - this->_error_multiplier*this->_error_intercept);
	this->Draw_Line(this->_slope + this->_error_multiplier*this->_error_slope, this->_intercept + this->_error_multiplier*this->_error_intercept);
	//Print Parameters
	glColor3d(0.0,0.0,0.0);
	glPushMatrix();
		glTranslated(0.3,1.3,0);
		char s[50];
		int i = 0;
		//slope
		i = sprintf_s(s,50,"slope: %.2f",(this->_slope));
		i += sprintf_s(s+i,50-i," ± %.2f",(this->_error_slope));
		glScaled(0.05,0.05,0.05);
		glPrint(s,this->_Font, this->_gmf);
	glPopMatrix();
	glPushMatrix();
		//intercept
		glTranslated(0.3,1.25,0);
		i = sprintf_s(s,50,"intercept: %.2f",(this->_error_multiplier*this->_intercept));
		i += sprintf_s(s+i,50-i," ± %.2f",(this->_error_multiplier*this->_error_intercept));
		glScaled(0.05,0.05,0.05);
		glPrint(s,this->_Font, this->_gmf);
	glPopMatrix();
}
//Draws a line into the graph
void AreaHeightView::Draw_Line(double slope, double intercept) {
	double x0,y0,x1,y1;
	double range_x = (this->_max_Height - this->_min_Height)*1e9;
	double range_y = (this->_max_Area - this->_min_Area);
	if (slope*this->_min_Height*1e9+intercept < this->_min_Area) {
		x0 = 1.3/range_x * ( (this->_min_Area - intercept)/slope - this->_min_Height * 1e9);
		y0 = 0;
	} else {
		x0 = 0;
		y0 = 1.3/range_y * (slope * this->_min_Height * 1e9 + intercept - this->_min_Area);
	}
	if (slope * this->_max_Height * 1e9 + intercept < this->_max_Area) {
		x1 = 1.3;
		y1 = 1.3/range_y * (slope * this->_max_Height * 1e9 + intercept - this->_min_Area);
	} else {
		x1 = 1.3/range_x * ( ( this->_max_Area - intercept )/slope - this->_min_Height * 1e9 );
		y1 = 1.3;
	}
	//Draw the Fit
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		glVertex3d(x0, y0, 0);
		glVertex3d(x1, y1, 0);
	glEnd();
}
//Sets the max Height value for the plot
void AreaHeightView::Set_Max_Height(double max_height) {
	this->_max_Height = max_height;
}
//Sets the min height value for the plot
void AreaHeightView::Set_Min_Height(double min_height) {
	this->_min_Height = min_height;
}
//Sets the max area value for the plot
void AreaHeightView::Set_Max_Area(int max_area) {
	this->_max_Area = max_area;
}
//Sets the min area value for the plot
void AreaHeightView::Set_Min_Area(int min_area) {
	this->_min_Area = min_area;
}
//Sets cut of area for the fit values and recalculates the fit
void AreaHeightView::Set_Fit_Area(int fit_area) {
	this->_fit_Area = fit_area;
	this->Calc_Lin_Fit();
}
//Sets the Error multipleier for the fit
void AreaHeightView::Set_Error_Multiplier(double error_multiplier) {
	this->_error_multiplier = error_multiplier;
}
//returns the max height value of the plot
double AreaHeightView::Get_Max_Height() {
	return this->_max_Height;
}
//returns the min height value of the plot
double AreaHeightView::Get_Min_Height() {
	return this->_min_Height;
}
//returns the max area value of the plot
int AreaHeightView::Get_Max_Area() {
	return this->_max_Area;
}
//returns the min area value of the plot
int AreaHeightView::Get_Min_Area() {
	return this->_min_Area;
}
//retunrs the area above which the data is no longer used for the fit
int AreaHeightView::Get_Fit_Area() {
	return this->_fit_Area;
}
//returns the error multiplier of the fit
double AreaHeightView::Get_Error_Multiplier() {
	return this->_error_multiplier;
}