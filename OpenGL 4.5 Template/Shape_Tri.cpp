#include "stdafx.h"
#include "Shape_Tri.h"

// note that this class is almost identical to Shape_Rect, refer to that .cpp file for notes.
// roatates with different direction/speed
// draws fewer vertices, because triangle

Shape_Tri::Shape_Tri(float x, float y, unsigned int VAO_toUse, unsigned int shaderProgram_toUse, float r, float g, float b)
{
	VAOid = VAO_toUse;
	shaderProgram = shaderProgram_toUse;
	xPos = x;
	yPos = y;
	zPos = 0.5f;
	zRot = 0.0f;
	colourR = r;
	colourG = g;
	colourB = b;
}
void Shape_Tri::handleInput(void)
{

}
void Shape_Tri::update(void)
{
	zRot -= 0.05f;

	float objRotationZ[16];
	float objTranslation[16];
	createIdentity(localToWorld);

	createRotationZ(objRotationZ, zRot);
	createTranslation(objTranslation, xPos, yPos, zPos * sin(zRot));

	multiply4x4(localToWorld, objTranslation, objRotationZ);
}

void Shape_Tri::draw(void)
{

	// enable our shader combo
	glUseProgram(shaderProgram);

	// recall VBO setup in one step using our VAO
	glBindVertexArray(VAOid);

	// transform into position
	int matrixLocation = glGetUniformLocation(shaderProgram, "localToWorld");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, localToWorld);

	// colour the fragment
	int colourLocation = glGetUniformLocation(shaderProgram, "fragControl");
	glUniform4f(colourLocation, colourR, colourG, colourB, 1.0f);

	// DRAW SHAPE HERE!
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}