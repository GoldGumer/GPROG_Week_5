#pragma once
#include "DrawableObject.h"
class Shape_Hex : public DrawableObject
{
public:
	// constructor 
	Shape_Hex(float x, float y, unsigned int VAO_toUse, unsigned int shaderProgram_toUse, float r, float g, float b);

	// we must supply these (polymorphism)
	void handleInput(void);
	void update(void);
	void draw(void);

};

