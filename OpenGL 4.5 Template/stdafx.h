// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// reference additional headers your program requires here
#include "gl_galogen.h" // this was generated using https://galogen.gpfault.net/galogen-web.html
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>
#include "MathHelper.h"

// window size etc, stuff that's unlikely to change during development
#define WINDOW_XPOS			500
#define WINDOW_YPOS			200
#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600
