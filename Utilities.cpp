#include "Utilities.h"
#include "stdio.h"

// Show error box with file name and line number
void Trace(const CHAR* strErrorMsg, const CHAR* strFileName, DWORD iLineNumber)
{
	// Buffer to hold the error text which include error message, file name and line number
	CHAR errorBuffer[100] ;

	// Append error message, file name and line number to error buffer
	_snprintf(errorBuffer, sizeof(errorBuffer), "%s\nfile: %s\nline: %u", strErrorMsg, strFileName, iLineNumber);

	// Show error box
	MessageBoxA(0, errorBuffer, "Error", 0) ;
}

// Output a float
void OutputFloat(float f)
{
	CHAR textBuffer[100] ;

	_snprintf(textBuffer, sizeof(textBuffer), "%s%f\n", "f:", f);

	OutputDebugStringA(textBuffer) ;
}

// Output a point
void OutputPoint(const POINT& point)
{
	CHAR textBuffer[100] ;

	_snprintf(textBuffer, sizeof(textBuffer), "%s%d %s%d\n", "x:", point.x, "y:", point.y);

	OutputDebugStringA(textBuffer) ;
}

void OutputVector(const D3DXVECTOR3& vec3)
{
	CHAR textBuffer[100] ;
	_snprintf(textBuffer, sizeof(textBuffer), "%s%f %s%f %s%f \n", "x:", vec3.x, "y:", vec3.y, "z:", vec3.z);
	OutputDebugStringA(textBuffer) ;
}

void OutputQuaternion(const D3DXQUATERNION& quat)
{
	CHAR textBuffer[100] ;
	_snprintf(textBuffer, sizeof(textBuffer), "%s%f %s%f %s%f %s%f \n", "x:", quat.x, "y:", quat.y, "z:", quat.z, "w:", quat.w);
	OutputDebugStringA(textBuffer) ;
}

void OutputPosition(float x, float y, float z)
{
	CHAR textBuffer[100] ;
	_snprintf(textBuffer, sizeof(textBuffer), "%s%f %s%f %s%f \n", "x:", x, "y:", y, "z:", z);
	OutputDebugStringA(textBuffer) ;
}

// Output a matrix
void OutputMatrix(const D3DXMATRIX& matrix)
{
	CHAR textBuffer[300] ;

	_snprintf(textBuffer, sizeof(textBuffer), "%s%f %s%f %s%f %s%f\n%s%f %s%f %s%f %s%f\n%s%f %s%f %s%f %s%f\n%s%f %s%f %s%f %s%f\n", 
		"_11:", matrix._11, "_12:", matrix._12, "_13:", matrix._13, "_14:", matrix._14,
		"_21:", matrix._21, "_22:", matrix._22, "_23:", matrix._23, "_24:", matrix._24,
		"_31:", matrix._31, "_32:", matrix._32, "_33:", matrix._33, "_34:", matrix._34,
		"_41:", matrix._41, "_42:", matrix._42, "_43:", matrix._43, "_44:", matrix._44);

	OutputDebugStringA(textBuffer) ;
}

// Initialize materials
D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

// Convert float to DWORD
DWORD FloatToDword(float f)
{
	return *((DWORD*)&f);
}

// Interploration
float Lerp(float a, float b, float t)
{
	return a - (a * t) + (b * t);
}

// Generate random int between minValue and maxValue(inclusive)
int GetRandomInt(int minValue, int maxValue)
{
	int range = maxValue - minValue + 1;

	srand((unsigned int)time(0));
	return rand() % range + minValue ;
}

float GetRandomFloat(float lowBound, float highBound)
{
	if( lowBound >= highBound ) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound; 
}

// Generate random vector
void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

D3DXVECTOR3 RandomVector(D3DXVECTOR3 *min, D3DXVECTOR3* max)
{
	D3DXVECTOR3 v ;

	v.x = GetRandomFloat(min->x, max->x);
	v.y = GetRandomFloat(min->y, max->y);
	v.z = GetRandomFloat(min->z, max->z);

	return v ;
}