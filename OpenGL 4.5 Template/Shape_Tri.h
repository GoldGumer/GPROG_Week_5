#pragma once
#include "stdafx.h"
#include "DrawableObject.h"

class Shape_Tri : public DrawableObject
{
public:

	// overloaded constructor allows you to spawn shapes with custom settings...
	Shape_Tri(float x, float y, unsigned int VAO_toUse, unsigned int shaderProgram_toUse, float r, float g, float b);

	// we must supply these (polymorphism)
	void handleInput(void);
	void update(void);
	void draw(void);

};