// OpenGL 4.5 Template.cpp : Defines the entry point for the application.
//

// Neil Dansey 27/11/19

// requires graphics card compatible with OpenGL 4.5!

// uses help from:
// https://learnopengl.com/Getting-started/Hello-Triangle
// https://galogen.gpfault.net/galogen-web.html
// https://gamedev.stackexchange.com/questions/26759/best-way-to-get-elapsed-time-in-miliseconds-in-windows
// https://stackoverflow.com/questions/18404890/how-to-build-perspective-projection-matrix-no-api
// plus others (cited in the correct places)


#include "stdafx.h"					// need this for common windows stuff
#include "OpenGL 4.5 Template.h"	// created by template code
#include <mmsystem.h>				// for high-res timing
#include "Game.h"					// the game class

#pragma comment( lib, "opengl32.lib" )	// openGL
#pragma comment( lib, "winmm.lib" )		// for hi-res timing

#define MAX_LOADSTRING		100
#define IDEAL_FPS			60
#define IDEAL_DELTA			1000/IDEAL_FPS		// assuming we are running at the ideal FPS the frame time will be this.

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HGLRC renderContext;							// the openGL rendering context

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
long long			milliseconds_now();		//	high resolution function to get the current time


// main function
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	// standard window creation stuff supplied by template...
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OPENGL45TEMPLATE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGL45TEMPLATE));
    MSG msg;
	HWND hWnd = FindWindowEx(NULL, NULL, szWindowClass, NULL);	
	
	// create a Game instance...
	Game* game = new Game(renderContext);
	
	// main game loop...
	bool needToQuit = false;
	while (!needToQuit)
	{
		// get time at start of loop:
		long long frameStart = milliseconds_now();

		// Process message if one exists:
		// (I've changed this to "PeekMessage" so it fires constantly rather than just when messages are received,
		// otherwise it halts the real-time rendering loop and you have to wiggle the mouse to get the game to update!)
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{

			if (msg.message == WM_KEYDOWN)
			{
				// if they press escape, quit the game
				if (msg.wParam == VK_ESCAPE)
				{
					needToQuit = true;
				}
			}
			else if (msg.message == WM_QUIT)
			{
				needToQuit = true;
			}

			
			// process messages as normal...
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		
		}
		
		// this stuff runs every frame regardless of Windows messages
		game->handleInput();
		game->update();
		game->draw(hWnd);

		// work out the current frame time and sleep for any spare time we have until the ideal delta
		// note that a flaw with this approach is that if the PC cannot handle the ideal frame rate, the game will lag
		long long deltaTime = milliseconds_now() - frameStart;

		if (deltaTime < IDEAL_DELTA)
		{
			Sleep((DWORD)(IDEAL_DELTA - deltaTime));
		}

	}

	// if we get here, they have quit the game, so clean up.
	delete game;
	game = nullptr;

    return (int) msg.wParam;
}

// some more standard Windows app stuff...


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // edited this line to include OWNDC for OpenGL compatibility
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OPENGL45TEMPLATE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OPENGL45TEMPLATE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


// more standard Windows app stuff...

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP | WS_BORDER,
      WINDOW_XPOS, WINDOW_YPOS, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   SetMenu(hWnd, NULL);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//	WM_CREATE	- window has been created but not displayed yet
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_CREATE:
		{
			// this is the process for creating a very simple OpenGL context only
			// for anything a bit more flexible the process is much more complex, 
			// and can be found here: https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)#Proper_Context_Creation

			// anyway...

			// here is the ideal pixel format we would like...
			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
				PFD_TYPE_RGBA,			// The kind of framebuffer. RGBA or palette.
				32,						// Colordepth of the framebuffer.
				0, 0, 0, 0, 0, 0,		// ignore all of these zeroes for now (not relevant) 
				0,
				0,
				0,
				0, 0, 0, 0,
				24,						// Number of bits for the depthbuffer
				8,						// Number of bits for the stencilbuffer
				0,						// Number of Aux buffers in the framebuffer.
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};

			HDC hdc = GetDC(hWnd);		// get the "device context" information from our Windows window, ready to link it up with OpenGL

			// find the best pixel format based on our ideal, and then set that to be the current setting
			int  pxf = ChoosePixelFormat(hdc, &pfd);
			SetPixelFormat(hdc, pxf, &pfd);

			// get Windows to tell openGL to make a rendering context that can link to our window.
			renderContext = wglCreateContext(hdc);
			// make this combo "current" so we can draw to it
			wglMakeCurrent(hdc, renderContext);
		}
		break;

		// more standard stuff...
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            EndPaint(hWnd, &ps);
        }
        break;

		// in this case, if they quit or close the game we need to clean up the openGL context.
		// first we set "NULL" as the current context, so our openGL context becomes "un-current", and therefore not being used
		// so then we can delete it
		case WM_DESTROY: case WM_CLOSE:
		{
			wglMakeCurrent(GetDC(hWnd), NULL);
			wglDeleteContext(renderContext);

			// standard Windows quit code...
			PostQuitMessage(0);
		}
        break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Standard Windows stuff...
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


// borrowed from the internet - returns hi-res time, or low res time if hi-res was not available (see link at top)
long long milliseconds_now() 
{
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount();
	}
}



