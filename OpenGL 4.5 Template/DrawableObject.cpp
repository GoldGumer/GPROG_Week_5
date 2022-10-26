#include "stdafx.h"
#include "DrawableObject.h"

//DrawableObject::DrawableObject(unsigned int VAO_toUse, unsigned int shaderProgram_toUse)
//{
//	VAO = VAO_toUse;
//	shaderProgram = shaderProgram_toUse;
//	xPos = 0.0f;
//	yPos = 0.0f;
//	zPos = 0.0f;
//	zRot = 0.0f;
//}
//void DrawableObject::handleInput(void)
//{
//
//}
//void DrawableObject::update(void)
//{
//	zRot += 0.1f;
//
//	float objRotationZ[16];
//	float objTranslation[16];
//	createIdentity(localToWorld);
//
//	createRotationZ(objRotationZ, zRot);
//	createTranslation(objTranslation, 0.0f, 0.0f, 0.5* sin(zRot));
//
//	multiply4x4(localToWorld, objTranslation, objRotationZ);
//}
//
//void DrawableObject::draw(void)
//{
//
//	// enable our shader combo
//	glUseProgram(shaderProgram);
//
//	// Select VBO setup in one step using our VAO
//	glBindVertexArray(VAO);
//
//	// transform into position
//	int matrixLocation = glGetUniformLocation(shaderProgram, "localToWorld");
//	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, localToWorld);
//
//
//	// DRAW SHAPE HERE!
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//}