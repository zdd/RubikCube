#include <time.h>

#include "RubikCube.h"

RubikCube rubikCube;

int initWindowPosX	 = rubikCube.GetWindowPosX();
int initWindowPosY	 = rubikCube.GetWindowPosY();
int InitWindowWidth	 = rubikCube.GetWindowWidth() ;
int InitWindowHeight = rubikCube.GetWindowHeight() ;

// Message process
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)   
{
	rubikCube.HandleMessages(hwnd, message, wParam, lParam) ;
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

// Main entry point of program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	WNDCLASSEX winClass ;

	winClass.lpszClassName = L"MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WndProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = NULL;
	winClass.hIconSm	   = NULL ;
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW) ; 
	winClass.hbrBackground = NULL ;
	winClass.lpszMenuName  = NULL ;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	RegisterClassEx (&winClass) ;  

	HWND hWnd = CreateWindowEx(NULL,  
		L"MY_WINDOWS_CLASS",        // window class name
		L"Rubik Cube",				// window caption
		WS_OVERLAPPEDWINDOW, 		// window style
		initWindowPosX,				// initial x position
		initWindowPosY,				// initial y position
		InitWindowWidth,			// initial x size
		InitWindowHeight,			// initial y size
		NULL,						// parent window handle
		NULL,						// window menu handle
		hInstance,					// program instance handle
		NULL) ;						// creation parameters

	// Initialize rubik cube
	rubikCube.Initialize(hWnd);

	ShowWindow(hWnd, iCmdShow) ;
	UpdateWindow(hWnd) ;
	//SendMessage(hWnd, WM_KEYDOWN, 'F', 0);
	
	MSG    msg ; 
	ZeroMemory( &msg, sizeof(msg) );
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while (msg.message != WM_QUIT)  
	{
		if( PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0)
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
		else // Render game scene if no message to process
		{
			rubikCube.Render() ;
		}
	}

	return msg.wParam ;
}
