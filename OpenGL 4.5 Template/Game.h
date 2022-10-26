#pragma once
#include "stdafx.h"
#include "Shape_Rect.h"		// so we can draw a shape!
#include "Shape_Tri.h"		// so we can draw a different shape!
#include <vector>			// so we can have a vector of mixed shapes
using namespace std;

#define GRIDHEIGHT	4
#define GRIDWIDTH	3

class Game
{
public:
	// rectangle vertex array
	float vertices_rect[12] = {
		0.008f,  0.008f, 0.0f,		// top right of rect
		0.008f, -0.008f, 0.0f,		// bottom right of rect
		-0.008f, -0.008f, 0.0f,		// bottom left of rect
		-0.008f,  0.008f, 0.0f		// top left of rect
	};

	// rectangle index array, first vert is 0
	unsigned int indices_rect[6] = {
		0, 1, 3,	// first triangle of rect
		1, 2, 3    // second triangle of rect
	};

	// triangle vertex array
	float vertices_tri[9] = {
		-0.1f, -0.1f, 0.0f,		// bottom left of tri
		0.1f, -0.1f, 0.0f,		// bottom right of tri
		0.0f, 0.1f, 0.0f		// top of tri
	};

	// triangle index array, first vert is 0
	unsigned int indices_tri[3] = {
		0, 1, 2
	};




	unsigned int VBO[2];				// one vertex buffer for each shape
	unsigned int EBO[2];				// one index/element buffer for each shape
	unsigned int VAO[2];				// one vertex array object for each shape
	unsigned int vertexShader, fragmentShader;	// single set of shaders for now
	unsigned int shaderProgram;					// single shader program for now

	GLfloat worldToView[16];			// this array will hold the view matrix when it is generated

	// this array holds our projection matrix once it is generated, but our function requires an identity matrix to be passed in so set that up now.
	GLfloat viewToProjection[16] =		
	{
		 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	};

	DrawableObject* gameGrid[GRIDWIDTH][GRIDHEIGHT];



	Game(HGLRC& renderContext);	// constructor requires information about the render context otherwise game class cannot set up openGL for us

	// the usual functions we'd expect in a game
	void handleInput();
	void update();
	void draw(HWND hWnd);

	~Game(); // to clear up the shape vector

}; 
