#pragma once
#include "stdafx.h"


class DrawableObject
{
public:
	
	// all DrawableObjects will need to implement these functions...
	virtual void handleInput(void) = 0;
	virtual void update(void) = 0;
	virtual void draw(void) = 0;
	
	// local space transformation matrix (initialised as identity)
	GLfloat localToWorld[16] =
	{
		 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	};

protected: // because inheritance

	unsigned int VAOid;			// the VAO that this object will use
	unsigned int shaderProgram;	// the set of shaders that this object will use
	
	float zRot;					// current rotation angle
	float zRotationMatrix[16];	// stores the obj's rotation transformation, based on the angle

	// position of object
	float xPos;
	float yPos;
	float zPos;

	// colour of object (0-1)
	float colourR;
	float colourG;
	float colourB;
};



	
