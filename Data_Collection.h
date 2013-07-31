#include "spots.h"
#include "Window_classes.h"
#include "Controller.h"

/////////////////////////
// DataCollector Class //
/////////////////////////

class DataCollector
{
public:
	DataCollector();
	DataCollector(double cut, double min_hight, int surrounding, int min_area,int max_files, int line_length);
	~DataCollector();
	bool Add_Data(wchar_t* path);
	bool Remove_Data(int id);
	void Count_Spots(int id);
	void Count_All_Spots();
	void Set_Surrounding(int surrounding);
	void Set_Min_Area(int min_area);
	void Set_Min_Hight(double min_hight);
	void Set_Cut(double cut);
	bool Set_Line_Length(int line_length);
	bool Set_Max_Files(int max_files);
	int Get_Surrounding();
	int Get_Min_Area();
	double Get_Min_Hight();
	double Get_Cut();
	int Get_Line_Length();
	int Get_Max_Files();
	int Get_Last();
	Spots* Get_Data(int id);
	void Set_Data_Win(WinMaker * Win);
	HWND Create_Data_Win(int id);
	bool remove_Data_Win(HWND hwnd);
	void Set_Histogramm_Win(WinMaker * Win);
	HWND Create_Histogramm_Win(int id);
	bool remove_Histogramm_Win(HWND hwnd);
	void Set_AreaHeight_Win(WinMaker * Win);
	HWND Create_AreaHeight_Win(int id);
	bool remove_AreaHeight_Win(HWND hwnd);
	void Close_All_Windows();
	void Update_Connected(int id);
	void Update_Connected(HWND hwnd);
	bool Save_To_File(wchar_t* Path);
	

protected:
	Spots ** Daten;			//Araay of pointers to handle the different files
	HWND * Show_Daten;		//Array which holds the pointers to the show data windows
	WinMaker * Win_Data;	//Holds a pointer to a winmaker to create new Data windows
	HWND * Show_Histogramm;	//Array which holds the pointers to the show Histogramm windows
	WinMaker * Win_Histogramm;	//Holds a pointer to a winmaker to create new Histogramm windows
	HWND * Show_AreaHeight;	//Array which holds the pointers to the show Histogramm windows
	WinMaker * Win_AreaHeight;	//Holds a pointer to a winmaker to create new AreaHight windows

private:
	int _last;			//saves the position of the last used data
	int _max_files;		//maximum number of open data files
	int _line_length;	//length of one line in the afm files 
	int _surrounding;	//surrounding for all data spots
	int _min_area;		//minimal area for all Spots
	double _min_hight;	//minimum hight for all spots
	double _cut;		//cutofhight for all spots

	void init();
	void init_Data();
	void init_Show_Data();
	void init_Show_Histogramm();
	void init_Show_AreaHeight();
	void delete_Data();
	void delete_Show_Data();
	void delete_Show_Histogramm();
	void delete_Show_AreaHeight();
	int Get_id(HWND hwnd);
};
