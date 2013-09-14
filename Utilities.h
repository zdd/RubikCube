#ifndef UTILITIES_H
#define UTILITIES_H

#include <windows.h>
#include <tchar.h>
#include <d3dx9.h>
#include <time.h>
#include <DxErr.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p) = NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

#define HR(x)                                      \
{                                                  \
	HRESULT hr = x;                                \
	if(FAILED(hr))                                 \
	{                                              \
		DXTrace(__FILE__, __LINE__, hr, _T(#x), TRUE); \
	}                                              \
}

// Show error box with file name and line number
void Trace(const char* strErrorMsg, const char* strFileName, DWORD iLineNumber) ;

// This Macro wraps the Trace function to show error box
#define ERRORBOX(msg) Trace((msg), (__FILE__), __LINE__) ;

void OutputFloat(float f) ;
void OutputPoint(const POINT& point) ;
void OutputVector(const D3DXVECTOR3& vec3) ;
void OutputPosition(float x, float y, float z) ;
void OutputQuaternion(const D3DXQUATERNION& quat);
void OutputMatrix(const D3DXMATRIX& matrix) ;

// Colors
const D3DXCOLOR	WHITE(D3DCOLOR_XRGB(255, 255, 255));
const D3DXCOLOR	GRAY(D3DCOLOR_XRGB(192, 192, 192)) ;		
const D3DXCOLOR	BLACK(D3DCOLOR_XRGB(0, 0, 0));
const D3DXCOLOR	RED(D3DCOLOR_XRGB(255, 0, 0));
const D3DXCOLOR	GREEN(D3DCOLOR_XRGB(0, 255, 0));
const D3DXCOLOR	BLUE(D3DCOLOR_XRGB(0, 0, 255));
const D3DXCOLOR	YELLOW(D3DCOLOR_XRGB(255, 255, 0));
const D3DXCOLOR	CYAN(D3DCOLOR_XRGB(0, 255, 255));
const D3DXCOLOR	MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

// Initialize materials
D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p) ;

// Materials
const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

BOOL FileExsit(WCHAR *strFileName) ;

// File system operation
void ValidateFileExsit(CHAR *strFileName) ;

// Convert float to DWORD
DWORD FloatToDword(float f) ;

// interpolation
float Lerp(float a, float b, float t) ;

// Generate random int
int GetRandomInt(int minValue, int maxValue) ;

// Generate random float in range of [lowBound, highBound] 
float GetRandomFloat(float lowBound, float highBound) ;

// Generate random vector
void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max) ;

D3DXVECTOR3 RandomVector(D3DXVECTOR3 *min, D3DXVECTOR3* max) ;

#endif // UTILITIES_H