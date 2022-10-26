#include "stdafx.h"
#include "Shape_Rect.h"

// constructor
Shape_Rect::Shape_Rect(float x, float y, unsigned int VAO_toUse, unsigned int shaderProgram_toUse, float r, float g, float b)
{
	VAOid = VAO_toUse;
	shaderProgram = shaderProgram_toUse;
	xPos = x;
	yPos = y;
	zPos = 0.5f; // fixed value because the shape is 2d
	zRot = 0.0f;
	colourR = r;
	colourG = g;
	colourB = b;
}
void Shape_Rect::handleInput(void)
{

}
void Shape_Rect::update(void)
{
	//zRot += 0.1f;	// update rotation

	float objRotationZ[16];
	float objTranslation[16];

	createIdentity(localToWorld); // reset local transformation matrix

	createRotationZ(objRotationZ, zRot);								// work out a rotation matrix based on zRot
	createTranslation(objTranslation, xPos, yPos, zPos * sin(zRot));	// work out a translation matrix based on x,y,z position of object

	multiply4x4(localToWorld, objTranslation, objRotationZ);			// update the object's local matrix with the translation and rotation
}

void Shape_Rect::draw(void)
{

	// enable our shader combo
	glUseProgram(shaderProgram);

	// recall VBO setup in one step using our VAO
	glBindVertexArray(VAOid);

	// transform into position
	int matrixLocation = glGetUniformLocation(shaderProgram, "localToWorld");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, localToWorld);

	// colour the fagment
	int colourLocation = glGetUniformLocation(shaderProgram, "fragControl");
	glUniform4f(colourLocation, colourR, colourG, colourB, 1.0f);

	// DRAW SHAPE HERE!
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}