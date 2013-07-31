#include "stdafx.h"
#include "Data_Collection.h"

/////////////////////////
// DataCollector Class //
/////////////////////////

//Basic Constructor Initilazes all members to 0
DataCollector::DataCollector() {
	this->init();
}
//Constructor Initilazes all members to
DataCollector::DataCollector(double cut, double min_hight, int surrounding, int min_area, int max_files, int line_length) {
	this->init();
	this->_cut = cut;
	this->_min_hight = min_hight;
	this->_surrounding = surrounding;
	this->_min_area = min_area;
	this->_max_files = max_files;
	this->_line_length = line_length;
	this->init_Data();
	this->init_Show_Data();
	this->init_Show_Histogramm();
	this->init_Show_AreaHeight();
}

//Destructor destroys all members
DataCollector::~DataCollector() {
	this->Close_All_Windows();
	this->delete_Data();
	this->delete_Show_Data();
	this->delete_Show_Histogramm();
	this->delete_Show_AreaHeight();
	if (this->Win_Data) delete Win_Data;
	if (this->Win_Histogramm) delete Win_Histogramm;
	if (this->Win_AreaHeight) delete Win_AreaHeight;
}

//Initilazes all members to 0
void DataCollector::init() {
	this->_cut = 0;
	this->_min_hight = 0;
	this->_surrounding = 0;
	this->_min_area = 0;
	this->_max_files = 0;
	this->_line_length = 0;
	this->_last = 0;
	this->Win_Histogramm = NULL;
	this->Win_Data = NULL;
	this->Daten = NULL;
	this->Show_Daten = NULL;
	this->Show_Histogramm = NULL;
	this->Show_AreaHeight= NULL;
}
//initilizes the Data array with NULL pointers
void DataCollector::init_Data() {
	if (this->Daten) this->delete_Data();
	this->Daten = new Spots*[this->_max_files];
	for (int i=0; i < this->_max_files; i++) {
		this->Daten[i] = NULL;
	}
}
//initilizes the Show Data array with NULL pointers
void DataCollector::init_Show_Data() {
	if (this->Show_Daten) this->delete_Show_Data();
	this->Show_Daten = new HWND[this->_max_files];
	for (int i=0; i < this->_max_files; i++) {
		this->Show_Daten[i] = NULL;
	}
}
//initilizes the Histogramm Data array with NULL pointers
void DataCollector::init_Show_Histogramm() {
	if (this->Show_Histogramm) this->delete_Show_Histogramm();
	this->Show_Histogramm = new HWND[this->_max_files];
	for (int i=0; i < this->_max_files; i++) {
		this->Show_Histogramm[i] = NULL;
	}
}
//initilizes the Show AreaHight array with NULL pointers
void DataCollector::init_Show_AreaHeight() {
	if (this->Show_AreaHeight) this->delete_Show_AreaHeight();
	this->Show_AreaHeight = new HWND[this->_max_files];
	for (int i=0; i < this->_max_files; i++) {
		this->Show_AreaHeight[i] = NULL;
	}
}
//deletes the Data array
void DataCollector::delete_Data() {
	for (int i=0; i < this->_max_files; i++) {
		delete this->Daten[i];
	}
	delete[] this->Daten;
}
//deletes the Show Data array
void DataCollector::delete_Show_Data() {
	delete[] this->Show_Daten;
}
//deletes the Show Histogramm array
void DataCollector::delete_Show_Histogramm() {
	delete[] this->Show_Histogramm;
}
//deletes the Show Histogramm array
void DataCollector::delete_Show_AreaHeight() {
	delete[] this->Show_AreaHeight;
}
//Adds a new data file at the end
bool DataCollector::Add_Data(wchar_t* path) {
	if ( this->_last < this->_max_files) {
		this->Daten[this->_last] = new Spots(this->_line_length, path);
		this->Daten[this->_last]->Set_Cut(this->_cut);
		this->Daten[this->_last]->Set_Min_Area(this->_min_area);
		this->Daten[this->_last]->Set_Min_Hight(this->_min_hight);
		this->Daten[this->_last]->Set_Surrounding(this->_surrounding);
		this->_last++;
		return true;
	}
	return false;
}

//Removes the data file saved at position id
bool DataCollector::Remove_Data(int id) {
	if (id > this->_last) { // excludes also negativ id's and enshures that last doesn't get negativ
		return false;
	}
	delete this->Daten[id];
	if(Show_Daten[id]) PostMessage(Show_Daten[id], WM_CLOSE, 0, 0);
	if(Show_Histogramm[id]) PostMessage(Show_Histogramm[id], WM_CLOSE, 0, 0);
	if(Show_AreaHeight[id]) PostMessage(Show_AreaHeight[id], WM_CLOSE, 0, 0);
	for (int i = id; i < this->_max_files-1; i++) {
		this->Daten[i] = this->Daten[i+1];
		this->Show_Daten[i] = this->Show_Daten[i+1];
		this->Show_Histogramm[i] = this->Show_Histogramm[i+1];
		this->Show_AreaHeight[i] = this->Show_AreaHeight[i+1];
	}
	this->Daten[this->_max_files-1] = NULL;
	this->Show_Daten[this->_max_files-1] = NULL;
	this->Show_Histogramm[this->_max_files-1] = NULL;
	this->Show_AreaHeight[this->_max_files-1] = NULL;
	this->_last--;
	return true;
}

//Counts the number of spots for the datafile at id
void DataCollector::Count_Spots(int id) {
	if (id < this->_last){
		this->Daten[id]->Count_Spots();
		this->Daten[id]->Count_Aktiv_Spots();
	}
}
//Counts the number of spots for all the loaded data files
void DataCollector::Count_All_Spots() {
	for ( int i = 0; i < this->_last; i++) {
		this->Count_Spots(i);
	}
}

//Sets the sourounding for all data files
void DataCollector::Set_Surrounding(int surrounding) {
	this->_surrounding = surrounding;
	for (int i=0; i < this->_last; i++) {
		this->Daten[i]->Set_Surrounding(surrounding);
	}
}

//Sets the minimum area of a spot for all data files
void DataCollector::Set_Min_Area(int min_area) {
	this->_min_area = min_area;
	for (int i=0; i < this->_last; i++) {
		this->Daten[i]->Set_Min_Area(min_area);
		this->Daten[i]->Count_Aktiv_Spots();
	}
}

//Sets the minimum hight of a spot for all data files
void DataCollector::Set_Min_Hight(double min_hight) {
	this->_min_hight = min_hight;
	for (int i=0; i < this->_last; i++) {
		this->Daten[i]->Set_Min_Hight(min_hight);
		this->Daten[i]->Count_Aktiv_Spots();
	}
} 

//Sets the cut value for all data files
void DataCollector::Set_Cut(double cut) {
	this->_cut = cut;
	for (int i=0; i < this->_last; i++) {
		this->Daten[i]->Set_Cut(cut);
	}
} 

//Sets the line_length for the data files returns fals if there is already a file opened
bool DataCollector::Set_Line_Length(int line_length) {
	if (this->Daten) return false;
	this->_line_length = line_length;
	return true;
} 

//Sets the number of Maximumfiles it returns fals if there are more files loaded than the new maximim number of files
bool DataCollector::Set_Max_Files(int max_files) {
	if (this->_last > max_files) return false;
	Spots** newData = new Spots*[max_files];
	for (int i = 0; i < this->_last; i++) {
		newData[i] = this->Daten[i];
	}
	for (int i = this->_last; i < max_files; i++) {
		newData[i] = NULL;
	}
	delete[] this->Daten;
	this->Daten = newData;
	return true;
}

//returns the value of surrounding for all data files
int DataCollector::Get_Surrounding() {
	return this->_surrounding;
}

//returns the value of minimum area for all spots
int DataCollector::Get_Min_Area() {
	return this->_min_area;
}

//returns the value for the minimum hight of all spots
double DataCollector::Get_Min_Hight() {
	return this->_min_hight;
}

//returns the value for the cutof hight for all data files
double DataCollector::Get_Cut() {
	return this->_cut;
}

//returns the line length value for all data files
int DataCollector::Get_Line_Length() {
	return this->_line_length;
}

//returns the value for the maximum number of files
int DataCollector::Get_Max_Files() {
	return this->_max_files;
}

//returns the value of last
int DataCollector::Get_Last() {
	return this->_last;
}
//returns the pointer to the data file with at position id
Spots* DataCollector::Get_Data(int id) {
	return this->Daten[id];
}
//Sets the WinMaker class to create the Data Windows
void DataCollector::Set_Data_Win(WinMaker *Win) {
	this->Win_Data = Win;
}
//Creates a Data Window for the Data at id and returns the HWND to the Window
HWND DataCollector::Create_Data_Win(int id) {
	//make shure that there is a data file
	if (id > this->_last) return NULL;
	//check if there is window already opened if it is return the handle
	if (this->Show_Daten[id]) {
		::SetFocus(this->Show_Daten[id]);
		return this->Show_Daten[id];
	}
	//create a new window
	try {
		wchar_t * caption = NULL;
		caption = this->Daten[id]->Get_Filename();
		this->Win_Data->AddCaption(caption);
		this->Win_Data->Create();
		//Conect the data to the controller for the window
		this->Show_Daten[id] = (HWND)*Win_Data;
		DataController * pCtrl = WinGetLong<DataController *> (this->Show_Daten[id]);
		pCtrl->setData(this->Daten[id]);
		//show the window
		this->Win_Data->Show(SW_SHOW);
		delete caption;
		return this->Show_Daten[id];
	} catch (WinException e)	{
            ::MessageBox(NULL, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
         return NULL;
    }
}
//removes the pointer to the hwnd from the Show Daten array
bool DataCollector::remove_Data_Win(HWND hwnd) {
	for (int i = 0 ; i <this->_last; i++) {
		if (this->Show_Daten[i] = hwnd) {
			this->Show_Daten[i] = NULL;
			return true;
		}
	}
	return false;
}


//Sets the WinMaker class to create the Histogramm Windows
void DataCollector::Set_Histogramm_Win(WinMaker *Win) {
	this->Win_Histogramm = Win;
}
//Creates a Histogramm Window for the Data at id and returns the HWND to the Window
HWND DataCollector::Create_Histogramm_Win(int id) {
	//make shure that there is a data file
	if (id > this->_last) return NULL;
	//check if there is window already opened if it is return the handle
	if (this->Show_Histogramm[id]) {
		::SetFocus(this->Show_Histogramm[id]);
		return this->Show_Histogramm[id];
	}
	//create a new window
	try {
		wchar_t * caption = NULL;
		caption = this->Daten[id]->Get_Filename();
		this->Win_Histogramm->AddCaption(caption);
		this->Win_Histogramm->Create();
		//Conect the data to the controller for the window
		this->Show_Histogramm[id] = (HWND)*this->Win_Histogramm;
		HistogrammController * pCtrl = WinGetLong<HistogrammController *> (this->Show_Histogramm[id]);
		pCtrl->setData(this->Daten[id]);
		//show the window
		this->Win_Histogramm->Show(SW_SHOW);
		delete caption;
		return this->Show_Histogramm[id];
	} catch (WinException e)	{
            ::MessageBox(NULL, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
         return NULL;
    }
}
//removes the pointer to the hwnd from the Show Histogramm array
bool DataCollector::remove_Histogramm_Win(HWND hwnd) {
	for (int i = 0 ; i <this->_last; i++) {
		if (this->Show_Histogramm[i] = hwnd) {
			this->Show_Histogramm[i] = NULL;
			return true;
		}
	}
	return false;
}
//Sets the WinMaker class to create the Histogramm Windows
void DataCollector::Set_AreaHeight_Win(WinMaker *Win) {
	this->Win_AreaHeight = Win;
}
//Creates a AreaHeight Window for the Data at id and returns the HWND to the Window
HWND DataCollector::Create_AreaHeight_Win(int id) {
	//make shure that there is a data file
	if (id > this->_last) return NULL;
	//check if there is window already opened if it is return the handle
	if (this->Show_AreaHeight[id]) {
		::SetFocus(this->Show_AreaHeight[id]);
		return this->Show_AreaHeight[id];
	}
	//create a new window
	try {
		wchar_t * caption = NULL;
		caption = this->Daten[id]->Get_Filename();
		this->Win_AreaHeight->AddCaption(caption);
		this->Win_AreaHeight->Create();
		//Conect the data to the controller for the window
		this->Show_AreaHeight[id] = (HWND)*this->Win_AreaHeight;
		AreaHeightController * pCtrl = WinGetLong<AreaHeightController *> (this->Show_AreaHeight[id]);
		pCtrl->setData(this->Daten[id]);
		//show the window
		this->Win_AreaHeight->Show(SW_SHOW);
		delete caption;
		return this->Show_AreaHeight[id];
	} catch (WinException e)	{
            ::MessageBox(NULL, e.GetMessage(), L"Initialization",
                MB_ICONEXCLAMATION | MB_OK);
         return NULL;
    }
}
//removes the pointer to the hwnd from the Show AreaHeight array
bool DataCollector::remove_AreaHeight_Win(HWND hwnd) {
	for (int i = 0 ; i <this->_last; i++) {
		if (this->Show_AreaHeight[i] = hwnd) {
			this->Show_AreaHeight[i] = NULL;
			return true;
		}
	}
	return false;
}
//Sends a close message to all opened windows
void DataCollector::Close_All_Windows() {
	for (int i = 0; i < this->_max_files; i++) {
		if(Show_Daten[i]) PostMessage(Show_Daten[i], WM_CLOSE, 0, 0);
		if(Show_Histogramm[i]) PostMessage(Show_Histogramm[i], WM_CLOSE, 0, 0);
		if(Show_AreaHeight[i]) PostMessage(Show_AreaHeight[i], WM_CLOSE, 0, 0);
	}
}
//sends an update message to all the windows connected to the id
void DataCollector::Update_Connected(int id) {
	if(Show_Daten[id]) PostMessage(Show_Daten[id], WM_PAINT, 0, 0);
	if(Show_Histogramm[id]) PostMessage(Show_Histogramm[id], WM_PAINT, 0, 0);
	if(Show_AreaHeight[id]) PostMessage(Show_AreaHeight[id], WM_PAINT, 0, 0);
}
//sends an update message to all the windows connected to the id coresponding to the hwnd
void DataCollector::Update_Connected(HWND hwnd) {
	this->Update_Connected(this->Get_id(hwnd));
}
//returns the id corresponding to the given hwnd
int DataCollector::Get_id(HWND hwnd) {
	int id = 0;
	for (int i = 0; i < this->_last; i++) {
		if(Show_Daten[i] == hwnd || Show_Histogramm[i] == hwnd || Show_AreaHeight[i] == hwnd) {
			id = i;
			break;
		}
	}
	return id;
}
//Saves all the information of the data in a Tab seperated file
bool DataCollector::Save_To_File(wchar_t* Path) {
	//check if there is a path and if all data is counted
	if (!Path) return false;
	for (int i = 0; i < this->_last; i++) {
		if ( !this->Daten[i]->Get_Spots_Counted() ) return false;
	}
	//open of stream
	std::ofstream os;
	os.precision(10);
	os.open(Path);
	//check if the stream could be opened
	if (!os.is_open()) return false;
	//write the head line
	os << "Sample \t Spots \t Height \t Area(" << this->_min_area << ") \t Volume \t Reduced" << std::endl;
	//write data
	for (int i = 0; i < this->_last; i++) {
		std::string filename;
		cvtPW2stdstring(filename,this->Daten[i]->Get_Filename());
		os  << filename << "\t" 
			<< this->Daten[i]->Get_Aktiv_Spots() << "\t"
			<< this->Daten[i]->Get_Averige_Hight(MODE_AKTIV)*1e9 << "\t" 
			<< this->Daten[i]->Get_Averige_Area(MODE_AKTIV) << "\t" 
			<< this->Daten[i]->Get_Aktiv_Volumen(MODE_NORMAL)*1e27 << "\t"
			<< this->Daten[i]->Get_Aktiv_Volumen(MODE_REDUCED)*1e27 << std::endl;
	}
	//free line
	os << std::endl;
	//write stats
	//Avarige
	os << "Averige: \t"; 
	for (int j = 1; j < 6; j++) { //six is the number o colums we use
		os << "=(" << ( (char)( (int)( 'A' ) +j ) ) << "2"; //makes it posible to increase the character in a for loop
		for (int i = 1; i < this->_last; i++) {
			os << "+" << ( (char)( (int)('A') +j ) ) << i+2;
		}
		os << ")/" << this->_last << "\t";
	}
	os << std::endl;
	//Varianz
	os << "Relativ Varianz: \t"; 
	for (int j = 1; j < 6; j++) { //six is the number of colums we use
		int A = (int)( 'A' ); //makes it posible to increase the character in a for loop
		int avg = this->_last+3;
		os  << "=SQRT( ("
			<< "(" << (char)( A +j ) << "2-" << (char)( A +j ) << avg << ") * ("
			<< (char)( A +j ) << "2-" << (char)( A +j ) << avg << ")"; 
		for (int i = 1; i < this->_last; i++) {
			os  << "+" 
				<< "(" << (char)( A +j ) << i+2 << "-" << (char)( A +j ) << avg << ") * ("
				<< (char)( A +j ) << i+2 << "-" << (char)( A +j ) << avg << ")"; 
		}
		os << ")/" << this->_last << ")/" << (char)( A +j ) << avg << "\t";
	}
	os << std::endl;
	//Error
	os << "Relativ Error: \t"; 
	for (int j = 1; j < 6; j++) { //six is the number o colums we use
		os << "=" << ( (char)( (int)( 'A' ) +j ) ) << this->_last+4 << "/SQRT(" << this->_last << ")" << "\t"; //makes it posible to increase the character in a for loop
	}
	//close  
	os.close();
	return true;
}