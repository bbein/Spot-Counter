#pragma once

#define MODE_AKTIV 0
#define MODE_ALL 1

#define MODE_NORMAL 0
#define MODE_REDUCED 1

#define _USE_MATH_DEFINES 
#include <math.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

////////////////////////
// Structure RGBColor //
////////////////////////

struct RGBColor {
	BYTE Red;
	BYTE Green;
	BYTE Blue;
	RGBColor(void);
	RGBColor(BYTE Red, BYTE Green, BYTE Blue);
};

/////////////////////
// Structure Point //
/////////////////////

//Defines a Point with x, y, z value
struct Point {
    int x;
	int y;
	double z;
    Point *next;
	void Init(int x, int y, double z);
	Point(void);
	~Point(void);
	Point* Copy(void);
	void Free(void);
};

////////////////////
// Structure Spot //
////////////////////

//Defines a Spot 
struct Spot {
	int id;
	int area;
	double hight;
	double volume;
	Point* first;
	int split;
	bool aktiv;
	void Init(int SpotId, int area, double hight, Point* Points, int split, bool aktiv, double volume);
	Spot* Copy(void);
	~Spot(void);
	Spot(void);

};

////////////////
//Spots Class //
////////////////

//stores the number of spots all thier information
class Spots {

public:
	//Konstruktor
	Spots(int size);
	Spots(int size, PWSTR Path);
	//Destruktor
	~Spots(void);
	
	//Functions
	bool Load_Data(int size, PWSTR Path);
	void Set_Cut(double Cut);
	void Set_Surrounding(int Surrounding);
	void Set_Min_Hight(double Hight);
	void Set_Min_Area(int Area);
	int Get_Min_Area(void);
	int Get_Surrounding(void);
	double Get_Cut(void);
	double Get_Min_Hight(void);
	int Get_Spot_Number(void);
	int Get_Length(void);
	Spot* Get_Spot(int SpotId);
	int Count_Spots(void);
	int Count_Aktiv_Spots(void);
	bool Save_Hights(PWSTR Path);
	bool Save_Aktiv_Hights(PWSTR Path);
	bool Save_Areas(PWSTR Path, int mode);
	int Get_Aktiv_Spots(void);
	double* Get_Hights(int mode);
	int* Get_Areas(int mode);
	double Get_Averige_Hight(int mode);
	double Get_Averige_Area(int mode);
	double Get_Aktiv_Volumen(int mode);
	int Get_Area_Max(void);
	double Get_Hight_Max(void);
	int Get_Selected_Spot(void);
	void Set_Selected_Spot(int spot);
	bool Get_Data_Loaded(void);
	bool Get_Spots_Counted(void);
	RGBColor GetPixelColor(double range, double offset, int point, bool masked);
	wchar_t * Get_Path();
	wchar_t * Get_Filename();

private:

	//Variables
	int length;			//width and hight of the map
	int spot_number;
	int aktiv_spots;
	int surrounding;
	double cut;
	double min_hight;
	int min_area;
	bool Data_Loaded;
	bool Spots_Counted;
	int selected_spot;
	double** Data;		//map of the z values of the loaded data
	int** Spot_Data;	//map of the points above cut
	Spot** spots;		//pointer to the array of pointers to all spots
	wchar_t * Path;		//Saves the Path of the opend file

	//Functions
	void Init(int size);
	void Delete_Data(void);
	void Create_Data(void);
	void Delete_Spots(void);
	bool Load_Data(PWSTR Path);
	void Set_Path(wchar_t * Path);
	void Merge_Spots(int spot1, int spot2, int spot_max);
	double Hight(int SpotId);
	Spot* Collect_Spot_Data(int SpotId);
	Point* Collect_Points(int xmax, int ymax, int xmin, int ymin, int SpotId);
	double integration(int steps, int rmax);
};

///////////////////
// Help Funtions //
///////////////////

void Init_Array(int length, double** data);
void Init_Array(int length, int** data);
double** Create_Array(int length, double** data);
int** Create_Array(int length, int** data);
void Delete_Array(int length, double** data);
void Delete_Array(int length,int** data);