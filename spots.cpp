
#include "stdafx.h"
#include "spots.h"


////////////////////////
// Structure RGBColor //
////////////////////////

RGBColor::RGBColor() {
	this->Red = 0;
	this->Green = 0;
	this->Blue = 0;
}

RGBColor::RGBColor(BYTE Red, BYTE Blue, BYTE Green) {
	this->Red = Red;
	this->Green = Blue;
	this->Blue = Green;
}

/////////////////////
// Structure Point //
/////////////////////

//Sets the x,y ans z value for the point 
void Point::Init(int x, int y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->next = NULL;
}

//Initialises the Point
Point::Point() {
	this->next = NULL;
}
//creates a copy of the list
Point* Point::Copy(void) {
	Point* returns = new Point();
	returns->Init(this->x, this->y, this->z);
	Point* walk;
	Point* next;
	next = new Point();
	returns->next = next;
	walk = this;
	while (walk->next) {
		next->Init(walk->next->x, walk->next->y, walk->next->z);
		walk = walk->next;
		if (walk->next) {
			Point* neu = new Point();
			next->next = neu;
			next = neu;
		} else {
			next->next = NULL;
		}
	}
	return returns;
}


//Destroys the Point List
Point::~Point() {

}

//Delets all points in the point list
void Point::Free() {
	if(this->next) this->next->Free();
	delete this;
}

////////////////////
// Structure Spot //
////////////////////

Spot::Spot() {
	this->first = NULL;
}
//Initialises the spot
void Spot::Init(int SpotId, int area, double hight, Point* Points, int split, bool aktiv, double volume) {
	this->id = SpotId;
	this->area = area;
	this->hight = hight;
	this->first = Points;
	this->split = split;
	this->aktiv = aktiv;
}

//creates a copy of the spot
Spot* Spot::Copy() {
	Spot* returns = new Spot(); 
	returns->Init(this->id, this->area, this->hight, this->first->Copy(), this->split, this->aktiv,this->volume);
	return returns;
}

//Destroys the spot
Spot::~Spot() {
	first->Free();
}
////////////////
//Spots Class //
////////////////

//Constructor: Initialises the Spot class
Spots::Spots(int size) {
	Init(size);
}
//Constructor: Initialises the Spot class and loads a file at pth Path
Spots::Spots(int size, PWSTR Path) {
	Init(size);
	this->Data_Loaded = Load_Data(size, Path);
}

//Destructor: Destroys all arrays and pointer values
Spots::~Spots() {
	if (Data && Spot_Data) Delete_Data();
	if (spots) Delete_Spots();
	if (this->Path) delete this->Path;
}

//Initialises the Spots class
void Spots::Init(int size) {
	this->Path = NULL;
	length = size;
	selected_spot = -1;
	spot_number = 0;
	aktiv_spots = 0;
	spots=NULL;
	min_area= 1;
	Create_Data();
	this->Data_Loaded = false;
}
//Creates the Data and Spot_Data arrays and loads the Data from a file at the path Path. If it can't open the file it returns false
bool Spots::Load_Data(int size, PWSTR Path) {
	if (size != length) {
		Delete_Data();
		Delete_Spots();
		length = size;
		Create_Data();
	}
	return Load_Data(Path);
}
//Loads the Data from a file at the path Path. If it can't open the file it returns false
bool Spots::Load_Data(PWSTR Path) {
	std::ifstream ifile;
	if (Path) {
		ifile.open(Path);
		if (ifile.is_open()) {
			int n = 0;
			int x = 0;
			int y = 0;
			double z=0;
			ifile >> z;
			while (ifile.good()) {
				Data[x][y] = z;
				ifile >> z;
				n++;
				y++;
				if (y == length) {
					y = 0;
					x++;
				}	
			}
			ifile.close();
			Data_Loaded = true;
			this->Set_Path(Path);
			return true;
		}
	}
	return false;
}

//Sets the value of the cut parameter
void Spots::Set_Cut(double Cut) {
	cut = Cut;
}
//Sets the value of surrounding parameter
void Spots::Set_Surrounding(int Surrounding) {
	surrounding = Surrounding;
}

//Deletes the list of Points with first as the starting point of the list
void Spots::Delete_Spots() {
	for (int i = 0; i <spot_number; i++) {
		delete spots[i];
	}
	spots = NULL;
}
//Deletes the Data and Spot_Data arrays
void Spots::Delete_Data() {
	Delete_Array(length, Data);
	Delete_Array(length, Spot_Data);
}

//Creates and Initilises the Data and Spot_Data arrays
void Spots::Create_Data() {
	Data = Create_Array(length, Data);
	Init_Array(length, Data);
	Spot_Data = Create_Array(length, Spot_Data);
	Init_Array(length, Spot_Data);
}


//Returns the number of spots if no Data is loaded it returns -1
int  Spots::Count_Spots() {
	if (Data_Loaded) {
		int n = 0;
		int next = 0;
		Init_Array(length, Spot_Data);
		for (int y = 0; y < length; y++) {
			for (int x = 0; x < length; x++) {
				if (Data[x][y] > cut) {
					next = 0;
					for (int y2 = y-surrounding; y2 < y+1; y2++) {
						for (int x2 = x-surrounding; x2 < x+surrounding; x2++) {
							if (x2 > -1 && x2 < length && y2 > -1 && ((x2-x)*(x2-x)+(y2-y)*(y2-y)) <= (surrounding*surrounding) && Spot_Data[x2][y2] > 0) {
								if (next == 0) {
									next = Spot_Data[x2][y2];
								} else {
									if (Spot_Data[x2][y2] != next) {
										Merge_Spots(Spot_Data[x2][y2], next, n);
										n--;
										next = Spot_Data[x2][y2];
									}
								}
							}
						}
					}
					if (next != 0) {
						Spot_Data[x][y] = next;
					} else {
						n++;
						Spot_Data[x][y] = n;
					}
				} else {
					Spot_Data[x][y] = 0;
				}
			}
		}
		if (spots) Delete_Spots();
		spots = new Spot*[n];
		for (int i = 0; i < n; i++) {
			spots[i] = Collect_Spot_Data(i+1);
		}
		spot_number = n;
		return n;
	} else {
		return -1;
	}
}

//Returns the number of aktiv spots if no Data is loaded or the spots aren't countet it returns -1
int Spots::Count_Aktiv_Spots() {
	int aktiv_spots = 0;
	if (Data_Loaded && spots) {
		for (int i = 0; i < this->spot_number; i++) {
			if (this->spots[i]->hight > this->min_hight && this->spots[i]->area >= this->min_area) {
				this->spots[i]->aktiv = true;
				aktiv_spots++;
			} else {
				this->spots[i]->aktiv = false;
			}
		}
	} else {
		return -1;
	}
	this->aktiv_spots = aktiv_spots;
	return aktiv_spots;
}

//Returns a Spot witch is initialised with the data of the spot ID SpotId
Spot* Spots::Collect_Spot_Data(int SpotId) {
	Spot* spot;
	spot = new Spot();
	spot->id=SpotId;
	int area = 0;	
	double max = 0;
	int xmax = 0;
	int ymax =0;
	int xmin = length;
	int ymin = length;
	spot->first = new Point();
	Point* last = spot->first;
	for (int y = 0; y < length; y++) {
		for (int x = 0; x < length; x++) {
			if (Spot_Data[x][y] == SpotId) {
				if (area) {
					Point* next = new Point();
					next->Init(x, y, Data[x][y]);
					last->next = next;
					last = next;
				} else {
					last->Init(x, y, Data[x][y]);
				}
				area++;
				if (x > xmax) xmax = x;
				if (y > ymax) ymax = y;
				if (x < xmin) xmin = x;
				if (y < ymin) ymin = y;
				if (Data[x][y] > max) {
					max = Data[x][y];
				}
			}
		}
	}  
	spot->hight=max;
	spot->area=area;
	spot->volume = this->integration(5000,(int)(spot->hight*5e9))*2/3*M_PI*0.2*0.2*0.2*1e-27;
	if (area >= min_area && max > min_hight) {
		spot->aktiv = true;
	} else {
		spot->aktiv = false;
	}
	return spot;
}
//Returns the surrounding Points of the spot with id SpotId in the area defined by xmax ymax xmin ymin
Point* Spots::Collect_Points(int xmax, int ymax, int xmin, int ymin, int SpotId) {
	int xdif = xmax-xmin;
	int ydif = ymax-ymin;
	Point *first;
	Point *last;
	Point *next;
	
	first = new Point();
	next = first;
	next->x = xmin;
	next->y = ymin;
	next->z = 0;
	last = next;

	next = new Point();
	last->next = next;
	next->x = xmax;
	next->y = ymin;
	next->z = 0;
	last = next;

	next = new Point();
	last->next = next;
	next->x = xmax;
	next->y = ymax;
	next->z = 0;
	last = next;

	next = new Point();
	last->next = next;
	next->x = xmin;
	next->y = ymax;
	next->z = 0;
	last = next;

	last->next = NULL;

	return first;
}

//Merges two spots into the spot with the lower Id and fixes the following spot ID's
void Spots::Merge_Spots(int spot1, int spot2, int spot_max) {
	int spot_bigger;
	int spot_smaller;
	if (spot1 > spot2) {
		spot_bigger = spot1;
		spot_smaller = spot2;
	} else {
		spot_bigger = spot2;
		spot_smaller = spot1;
	}
	for (int n = spot_bigger; n <= spot_max; n++) {
		for (int y = 0; y < length; y++) {
			for (int x = 0; x < length; x++) {
				if(Spot_Data[x][y] == n) {
					if (n == spot_bigger) {
						Spot_Data[x][y] = spot_smaller; 
					} else {
						Spot_Data[x][y] = n-1;
					}
				}
			} 
		}
	}
}


//Returns the highest z value of all Spots with ID SpotId
double Spots::Hight(int SpotId) {
	double max = 0;
	for (int y = 0; y < length; y++) {
		for (int x = 0; x < length; x++) {
			if (Spot_Data[x][y] == SpotId && Data[x][y] > max) max = Data[x][y];
		}
	}
	return max;
}



//Saves the hights of all Spots into a file at path Path if it can't open the file it returns false
bool Spots::Save_Hights(PWSTR Path) {
	if (Path && spots) {
		std::ofstream os;
		os.precision(5);
		os.open(Path);
		if (os.is_open()) {
			for(int i=0; i<spot_number; i++) {
				os << spots[i]->hight << std::endl;
			}
			os.close();
			return true;
		}
	}
	return false;
}

//Returns the number of countet spots
int Spots::Get_Spot_Number() {
	return spot_number;
}

//Returns a spot with the valus of the spot with ID SpotId
Spot* Spots::Get_Spot(int SpotId) {	
	return spots[SpotId-1]->Copy();
}

//Returns the length of the spot data
int Spots::Get_Length() {
	return this->length;
}

//Sets the minimum hight
void Spots::Set_Min_Hight(double Hight) {
	this->min_hight = Hight;
}

//Sets the minimum area
void Spots::Set_Min_Area(int Area) {
	this->min_area = Area;
}

//Returns the minimum area
int Spots::Get_Min_Area(void) {
	return this->min_area;
}

//Returns the surrounding length
int Spots::Get_Surrounding(void) {
	return this->surrounding;
}

//Returns the Cut off filter
double Spots::Get_Cut(void) {
	return this->cut;
}

//Returns the minimum Hight to display spots
double Spots::Get_Min_Hight(void) {
	return this->min_hight;
}

//Returns the number of aktiv spots
int Spots::Get_Aktiv_Spots(void) {
	return this->aktiv_spots;
}
//Returns the Averige spot hight mode can be MODE_ALL or MODE_AKTIV
double Spots::Get_Averige_Hight(int mode) {
	double returns = 0;
	if (this->spots) {
		switch (mode) {
			case MODE_AKTIV: {
				for (int i = 0; i < spot_number; i++) {
					if (this->spots[i]->aktiv) returns += this->spots[i]->hight;
				}
				returns /=this->aktiv_spots;
			break;
			}
			case MODE_ALL: {
				for (int i = 0; i < spot_number; i++) {
					returns += this->spots[i]->hight;
				}
				returns /= this->spot_number;
			break;
			}
		}
		return returns;
	} else {
		return 0.0;
	}
}
//Returns the Averige spot area mode can be MODE_ALL or MODE_AKTIVE
double Spots::Get_Averige_Area(int mode){
double returns = 0;
	if (this->spots) {
		switch (mode) {
			case MODE_AKTIV: {
				for (int i = 0; i < spot_number; i++) {
					if (this->spots[i]->aktiv) returns += this->spots[i]->area;
				}
				returns /=this->aktiv_spots;
			break;
			}
			case MODE_ALL: {
				for (int i = 0; i < spot_number; i++) {
					returns += this->spots[i]->area;
				}
				returns /= this->spot_number;
			break;
			}
		}
		return returns;
	} else {
		return 0.0;
	}
}

//Saves all aktiv spot hights to the file in Path
bool Spots::Save_Aktiv_Hights(PWSTR Path) {
	if (Path && spots) {
		std::ofstream os;
		os.precision(5);
		os.open(Path);
		if (os.is_open()) {
			for(int i=0; i<spot_number; i++) {
				if (spots[i]->aktiv) os << spots[i]->hight << std::endl;
			}
			os.close();
			return true;
		}
	}
	return false;
}

//Returns the volum of the aktiv spots
double Spots::Get_Aktiv_Volumen(int mode) {
	double volumen = 0;
	switch(mode) {
		case MODE_NORMAL: {
			for (int i = 0; i < this->spot_number; i++) {
				if (this->spots[i]->aktiv) volumen += this->spots[i]->hight*this->spots[i]->hight*this->spots[i]->hight*2/3*M_PI;
			}	
		} 
		break;
		case MODE_REDUCED: {
			for (int i = 0; i < this->spot_number; i++) {
				if (this->spots[i]->aktiv) volumen += this->spots[i]->volume;
			}
		}
		break;
	}
	return volumen;
}


//Saves spot Area to the file in Path mode can chose between all countet and only activ spot
bool Spots::Save_Areas(PWSTR Path , int mode) {
	if (Path && spots) {
		std::ofstream os;
		os.precision(5);
		os.open(Path);
		if (os.is_open()) {
			for(int i=0; i<spot_number; i++) {
				switch (mode) {
					case MODE_AKTIV:
						if (spots[i]->aktiv) os << spots[i]->area << std::endl;
					break;
					case MODE_ALL:
						os << spots[i]->area << std::endl;
					break;
				}
			}
			os.close();
			return true;
		}
	}
	return false;
}

//Returns the spot which is selected right now returns -1 if no spot is selected
int Spots::Get_Selected_Spot(void) {
	return this->selected_spot;
}
//Sets the selected spot to the given spot id
void Spots::Set_Selected_Spot(int spot) {
	if (spot <= this->spot_number) {
		this->selected_spot = spot;
	}
}
//Returns array with the hights of all/aktiv spots ordered by spot id
double* Spots::Get_Hights(int mode) {
	int n = -1;
	double* Hights;
	switch (mode) {
		case MODE_AKTIV:
			Hights = new double[this->aktiv_spots];
			for(int i=0; i<this->spot_number; i++) {
				if (spots[i]->aktiv) Hights[n++] = spots[i]->hight;
			}
		break;
		case MODE_ALL:
			Hights = new double[this->spot_number];
			for(int i=0; i<this->spot_number; i++) {
				Hights[i] = spots[i]->hight;
			}
		break;
	}
	return Hights;
}


//Returns array with the area of all/aktiv spots ordered by spot id
int* Spots::Get_Areas(int mode) {
	int n = -1;
	int* Area;
	switch (mode) {
		case MODE_AKTIV:
			Area = new int[this->aktiv_spots];
			for(int i=0; i<spot_number; i++) {
				if (spots[i]->aktiv) Area[n++] = spots[i]->area;
			}
		break;
		case MODE_ALL:
			Area = new int[spot_number];
			for(int i=0; i<spot_number; i++) {
				Area[i] = spots[i]->area;
			}
		break;
	}
	return Area;
}

//Returns the integrated volum for a spot with radius rmax and uses steps integration steps
double Spots::integration(int steps, int rmax) {
	double sum = 0;
	double dr = (double)rmax/steps;
	for (int i = 1; i < steps; i++) {
		double x = i*dr;
		sum += dr*x*x*pow(x,-0.58)*exp(-pow(x/(7+0.7*rmax),4.8));
	}
	return sum;
}


//Returns the area of the biggest spot of all counted spots
int Spots::Get_Area_Max() {
	int returns = 0;
	for (int n=0; n<this->spot_number; n++) {
		if (this->spots[n]->aktiv && this->spots[n]->area > returns) {
			returns = this->spots[n]->area;
		}
	}
	return returns;
}

//returns the hight of the highest spot in the data 
double Spots::Get_Hight_Max() {
	double returns = 0;
	for (int n=0; n<this->spot_number; n++) {
		if (this->spots[n]->aktiv && this->spots[n]->hight > returns) {
			returns = this->spots[n]->hight;
		}
	}
	return returns;
}


//returns a black and white pixel color of the data at point point where ofset is the ight ofset, 
// range is range of hight from black to white and masked makes all points ble which are higher than range + offset
RGBColor Spots::GetPixelColor(double range, double offset, int point, bool masked) {
	RGBColor returns;
	if (masked && Data[point%length][point/length] > this->cut+offset) {
		returns.Red = 0;
		returns.Green = 0;
		returns.Blue = 255;
	} else {
		int color;
		color = (int)((Data[point%length][point/length]+offset)/range*128+128);
		if (color > 255) color = 255;
		if (color < 0) color = 0;
		returns.Red = color;
		returns.Green = color;
		returns.Blue = color;
	}
	return returns;
}

bool Spots::Get_Data_Loaded(void) {
	return this->Data_Loaded;
}
//returns a pointer to a copy of the path where the file is saved
wchar_t* Spots::Get_Path() {
	wchar_t* target = new wchar_t[wcslen(this->Path)+1];
	wcscpy_s(target,wcslen(this->Path)+1,this->Path);
	return target;
}
//Sets the path where the loaded file is saved
void Spots::Set_Path(wchar_t * Path) {
	/*int length = wcslen(Path);
	this->Path = new wchar_t(length);
	for ( int i = 0; i < length; i++) {
		this->Path[i] = Path[i];
	}*/
	if (this->Path) delete this->Path;
	int length = wcslen(Path)+1;
	this->Path = new wchar_t[length];
	wcscpy_s(this->Path,wcslen(Path)+1, Path);	
}

//Returns the Spots_Counted variable
bool Spots::Get_Spots_Counted(void) {
	return this->Spots_Counted;
}

wchar_t * Spots::Get_Filename(void) {
	wchar_t* target = new wchar_t[wcslen(this->Path)+1];
	wcscpy_s(target,wcslen(this->Path)+1,this->Path);
	return target;
}

///////////////////
// Help Funtions //
///////////////////


double** Create_Array(int length, double** data) {
	data = new double*[length];
	for (int i=0; i < length; i++) {
		data[i] = new double[length];
	}
	return data;
}

int** Create_Array(int length, int** data) {
	data = new int*[length];
	for (int i=0;i < length; i++) {
		data[i] = new int[length];
	}
	return data;
}

void Init_Array(int length, double** data) {
	for(int i=0; i < length; i++) {
		for(int j=0;j < length; j++) {
			data[i][j] = 0;
		}
	}
}

void Init_Array(int length, int** data) {
	for(int i= 0; i < length; i++) {
		for(int j=0;j < length; j++) {
			data[i][j] = 0;
		}
	}
}

void Delete_Array(int length, double** data) {
	for (int i=0; i < length; i++) {
		delete[] data[i];
	}
	delete[] data;
}

void Delete_Array(int length, int** data) {
	for (int i=0; i < length; i++) {
		delete[] data[i];
	}
	delete[] data;
}
