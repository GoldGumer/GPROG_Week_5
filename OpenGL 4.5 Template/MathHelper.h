#pragma once


// some handy functions we will use throughout

void multiply4x4(float* result, float* m1, float* m2);			// multiplies 2 4x4 matices together and stores result
void normaliseFloat3(float* in);								// normalise a 3d vector
void crossFloat3(float* result, float* in1, float* in2);		// cross product of 2 3d vectors, stores result
void createRotationZ(float* out, float rad);					// given a rotation in radians, creates and stores a 4x4 z-rotation matrix
void createTranslation(float* out, float x, float y, float z);	// given x,y,z movement, creates and stores a 4x4 translation matrix
void createIdentity(float* out);								// fills up an array of 16 floats with an identity matrix

// https://www.3dgep.com/understanding-the-view-matrix/#Memory_Layout_of_Column-Major_Matrices
void computeView(GLfloat* result); 
// fills up an array of 16 floats with a basic fixed-camera view matrix 

// https://stackoverflow.com/questions/1638355/function-for-perspective-projection-of-a-matrix-in-c
void computeProjection(float fov, float aspect, float nearcp, float farcp, GLfloat* mret); 
// fills up an array of 16 floats with a basic perspective projection using information supplied
