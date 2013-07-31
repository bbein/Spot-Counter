#pragma once
#include "spots.h"
#include "resource.h"
#include "Window_classes.h" //for the help functions and WNDProcs
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "gl_graphics.h"    //My own GL functions

//////////////////
// GLView Class //
//////////////////

class GLView {

public:
	GLView(HWND hwnd);
	GLView();
	virtual ~GLView();
	void GLViewInit(HWND hwnd);
	virtual void DrawGLScene(void) = 0;
   	void ReSizeGLScene(GLsizei width, GLsizei height);
	virtual void SetData(Spots *Data);
	GLint Selection(int mouse_x, int mouse_y);
	void Create_Font(wchar_t * Font_Name);
	void change_perspective(WPARAM Key);

protected:
	void Init_GLView();
	bool InitGL();
	void FreeDCRC();
	bool selection;

	HDC		_hdc;			// Private GDI Device Context
	HGLRC	_hrc;			// Permanent Rendering Context
	HWND	_hwnd;			// Holds the handle to the window which owns this view object
	Spots* Daten;			//saves the pointer to the conectet Data 
	GLuint	_Font;			//Basis for the font of ths object 
	GLYPHMETRICSFLOAT* _gmf;// Storage For Information About The Font
	
	double _pos_x;
	double _pos_y;
	double _pos_z;
	double _rot_x;
	double _rot_y;
	double _rot_z;

private:
	
};

////////////////////
// DataView Class //
////////////////////

class DataView : public GLView {

	public:
		DataView();
		DataView(HWND hwnd);
		virtual void DrawGLScene(void);
		void DrawSpots();
		void CreateDataTexture(double range, double offset);
		virtual void SetData(Spots* Data);
		bool Get_Display_Spots();
		void Set_Display_Spots(bool Display_Spots);
		bool Get_Display_Mask();
		void Set_Display_Mask(bool Display_Mask);
		bool Get_Display_3D();
		void Set_Display_3D(bool Display_3D);
		double Get_Range_3D();
		void Set_Range_3D(double Range_3D);

	
	protected:
		
	private:
		void DataViewInit();
		GLuint texture[1];
		bool textured;
		bool _Display_Spots;
		bool _Display_Mask;
		bool _Display_3D;
		double _Range_3D;
};



//////////////////////////
// AreaHeightView Class //
//////////////////////////

class AreaHeightView : public GLView {
	public:
		AreaHeightView();
		AreaHeightView(HWND hwnd);
		~AreaHeightView();
		virtual void DrawGLScene(void);
		virtual void SetData(Spots *Data);
		void Set_Max_Height(double max_height);
		void Set_Min_Height(double min_height);
		void Set_Max_Area(int max_area);
		void Set_Min_Area(int min_area);
		void Set_Fit_Area(int fit_area);
		void Set_Error_Multiplier(double error_multiplier);
		double Get_Max_Height();
		double Get_Min_Height();
		int Get_Max_Area();
		int Get_Min_Area();
		int Get_Fit_Area();
		double Get_Error_Multiplier();
	
	protected:
		
	private:
		void AreaHeightView_Init();
		void Set_Data();
		void Draw_Axes();
		void Draw_Fit();
		void Draw_Line(double slope, double intercept);
		void Calc_Lin_Fit();
		
		double * _Heights;	//Array which saves the heigts from the data
		int * _Areas;		//Array which saves the areas from the data
		double _max_Height; //max height value of the plot
		double _min_Height; //min height value of the plot
		int _max_Area;		//max area value of the plot
		int _min_Area;		//min area value of the plot
		int _fit_Area;		//value above which data points are excluded from the fit
		double _intercept;	//intercept of the fit
		double _slope;		//slope of the fit
		double _error_intercept;	//one sigma error of the intercept
		double _error_slope;		//one sigma error of the slope
		double _error_multiplier;	//multiplier to select how many sigmas should be used

};


//////////////////////////
// HistogrammView Class //
//////////////////////////

class HistogrammView : public GLView {
	public:
		HistogrammView();
		HistogrammView(HWND hwnd);
		~HistogrammView();
		virtual void DrawGLScene(void);
		virtual void SetData(Spots *Data);
		int Get_Bin_Number();
		int Get_Max_Number();
		double Get_Min_Height();
		double Get_Max_Height();
		void Set_Bin_Number(int Bin_Number);
		void Set_Max_Number(int Max_Number);
		void Set_Min_Height(double Min_Height);
		void Set_Max_Height(double Max_Height);	
	
	protected:
		
	private:
		void HistogrammView_Init();
		void Calc_Histogramm_Data();
		void Draw_Axes();
		void Init_Bins();
		int Find_Max_Number();
		void Count_Bin_Elements();
		void Calc_Mean();
		double Poisson(int n);

		void Sort();			//Sorts the elemts array small elments at the beginning
		int _min_number;		//minimal number of elements per bin
		int _bin_number;		//number of bins
		int _bin_elements;		//avarige number of elemts per bin
		int _max_number;		//Number of elements in the most filled bin
		int _element_number;	//Totall number of elements
		double _min_height;		//Saves the smalest height to display in the histogramm
		double _max_height;		//Saves the heighest height to display in the hisogramm
		int * _bins;			//Array which holds the number of elements in the bins
		double * _elements;		//Array which holds all the elemts of the data
		double _mean;
	
};

//////////////////////
// Controller Class //
//////////////////////

class Controller {

public:
	Controller();
    Controller(HWND hwnd);
    ~Controller ();
	void Command(WPARAM wParam, LPARAM lParam);

protected:

    HWND        _hwnd;
};

///////////////////////////////////
// ImageSettingsController Class //
///////////////////////////////////

class ImageSettingsController: public Controller {

public:
	ImageSettingsController();
	ImageSettingsController(HWND hwnd);
	void Command(WPARAM wParam, LPARAM lParam);
	void Activate();
protected:

private:

};

////////////////////////////////////////
// HistogrammSettingsController Class //
////////////////////////////////////////

class HistogrammSettingsController: public Controller {

public:
	HistogrammSettingsController();
	HistogrammSettingsController(HWND hwnd);
	void Command(WPARAM wParam, LPARAM lParam);
	void Activate();
protected:

private:

};


////////////////////////////////////////
// AreaHeightSettingsController Class //
////////////////////////////////////////

class AreaHeightSettingsController: public Controller {

public:
	AreaHeightSettingsController();
	AreaHeightSettingsController(HWND hwnd);
	void Command(WPARAM wParam, LPARAM lParam);
	void Activate();
protected:

private:

};


///////////////////////////////
// SpotsInfoController Class //
///////////////////////////////

class SpotsInfoController: public Controller {

public:
	SpotsInfoController();
	SpotsInfoController(HWND hwnd);
	void Command(WPARAM wParam, LPARAM lParam);
	void Activate();
protected:

private:

};

//////////////////////////////
// SpotInfoController Class //
//////////////////////////////

class SpotInfoController: public Controller {

public:
	SpotInfoController();
	SpotInfoController(HWND hwnd);
	void Command(WPARAM wParam, LPARAM lParam);
	void Activate();
protected:

private:

};

////////////////////////
// GLController Class //
////////////////////////

class GLController: public Controller {

public:
	GLController();
    GLController(HWND hwnd);
    ~GLController ();
	void Selection(int mouse_x, int mouse_y);
	void Command(WPARAM wParam, LPARAM lParam);
    void    Size (int width, int hight);
    void    Paint ();
	void setData(Spots * Data);
	Spots * GetData();
	void change_perspective(WPARAM Key);

protected:
	
	void Init_GLController();
    GLView * _view;
	Spots * Daten;
};

//////////////////////////
// DataController Class //
//////////////////////////

class DataController: public GLController {
public:
	DataController();
	DataController(HWND hwnd);
	~DataController();
	void Command(WPARAM wParam, LPARAM lParam);
	bool Get_Display_Spots();
	void Set_Display_Spots(bool Display_Spots);
	bool Get_Display_Mask();
	void Set_Display_Mask(bool Display_Mask);
	bool Get_Display_3D();
	void Set_Display_3D(bool Display_3D);
	double Get_Range_3D();
	void Set_Range_3D(double Range_3D);
	void Set_Range(double range);
	double Get_Range();
	void Set_Offset(double offset);
	double Get_Offset();
	void Update_Dialogs();

protected:
	HWND		D_SpotsInfo;		// Window handel for the Spots Info dialog
	HWND		D_SpotInfo;			// Window handel for the Spot Info dialog
	HWND		D_ImageSettings;	// Window handel for the Spot Info dialog
	DataView *	_view;
	double _range, _offset;			//Saves the cut and offset value for the image;

};

////////////////////////////////
// HistogrammController Class //
///////////////////////////////

class HistogrammController: public GLController {
public:
	HistogrammController();
	HistogrammController(HWND hwnd);
	~HistogrammController();
	void Command(WPARAM wParam, LPARAM lParam);
	
	int Get_Bin_Number();
	int Get_Max_Number();
	double Get_Min_Height();
	double Get_Max_Height();
	void Set_Bin_Number(int Bin_Number);
	void Set_Max_Number(int Max_Number);
	void Set_Min_Height(double Min_Height);
	void Set_Max_Height(double Max_Height);
protected:
	HistogrammView *	_view;
	HWND		D_HistogrammSettings;		// Window handel for the Histogramm Settings dialog
};


////////////////////////////////
// AreaHeightController Class //
///////////////////////////////

class AreaHeightController: public GLController {
public:
	AreaHeightController();
	AreaHeightController(HWND hwnd);
	~AreaHeightController();
	void Command(WPARAM wParam, LPARAM lParam);
	void Set_Max_Height(double max_height);
	void Set_Min_Height(double min_height);
	void Set_Max_Area(int max_area);
	void Set_Min_Area(int min_area);
	void Set_Fit_Area(int fit_area);
	void Set_Error_Multiplier(double error_multiplier);
	double Get_Max_Height();
	double Get_Min_Height();
	int Get_Max_Area();
	int Get_Min_Area();
	int Get_Fit_Area();
	double Get_Error_Multiplier();

protected:
	AreaHeightView *	_view;
	HWND		D_AreaHeightSettings;		// Window handel for the AreaHeight Settings dialog
};