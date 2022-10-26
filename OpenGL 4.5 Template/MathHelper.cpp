#include "stdafx.h"

// some basic vector and matrix math functions, see header file for details. formulae freely available online

void multiply4x4(float* result, float* m1, float* m2)
{
	result[0] = (m1[0] * m2[0]) + (m1[4] * m2[1]) + (m1[8] * m2[2]) + (m1[12] * m2[3]);
	result[1] = (m1[1] * m2[0]) + (m1[5] * m2[1]) + (m1[9] * m2[2]) + (m1[13] * m2[3]);
	result[2] = (m1[2] * m2[0]) + (m1[6] * m2[1]) + (m1[10] * m2[2]) + (m1[14] * m2[3]);
	result[3] = (m1[3] * m2[0]) + (m1[7] * m2[1]) + (m1[11] * m2[2]) + (m1[15] * m2[3]);
	result[4] = (m1[0] * m2[4]) + (m1[4] * m2[5]) + (m1[8] * m2[6]) + (m1[12] * m2[7]);
	result[5] = (m1[1] * m2[4]) + (m1[5] * m2[5]) + (m1[9] * m2[6]) + (m1[13] * m2[7]);
	result[6] = (m1[2] * m2[4]) + (m1[6] * m2[5]) + (m1[10] * m2[6]) + (m1[14] * m2[7]);
	result[7] = (m1[3] * m2[4]) + (m1[7] * m2[5]) + (m1[11] * m2[6]) + (m1[15] * m2[7]);
	result[8] = (m1[0] * m2[8]) + (m1[4] * m2[9]) + (m1[8] * m2[10]) + (m1[12] * m2[11]);
	result[9] = (m1[1] * m2[8]) + (m1[5] * m2[9]) + (m1[9] * m2[10]) + (m1[13] * m2[11]);
	result[10] = (m1[2] * m2[8]) + (m1[6] * m2[9]) + (m1[10] * m2[10]) + (m1[14] * m2[11]);
	result[11] = (m1[3] * m2[8]) + (m1[7] * m2[9]) + (m1[11] * m2[10]) + (m1[15] * m2[11]);
	result[12] = (m1[0] * m2[12]) + (m1[4] * m2[13]) + (m1[8] * m2[14]) + (m1[12] * m2[15]);
	result[13] = (m1[1] * m2[12]) + (m1[5] * m2[13]) + (m1[9] * m2[14]) + (m1[13] * m2[15]);
	result[14] = (m1[2] * m2[12]) + (m1[6] * m2[13]) + (m1[10] * m2[14]) + (m1[14] * m2[15]);
	result[15] = (m1[3] * m2[12]) + (m1[7] * m2[13]) + (m1[11] * m2[14]) + (m1[15] * m2[15]);

}


void normaliseFloat3(float* in)
{
	float mag = sqrt(in[0] * in[0] + in[1] * in[1] + in[2] * in[2]);
	in[0] /= mag;
	in[1] /= mag;
	in[2] /= mag;
}

void crossFloat3(float* result, float* in1, float* in2)
{
	result[0] = in1[1] * in2[2] - in1[2] * in2[1];
	result[1] = in1[2] * in2[0] - in1[0] * in2[2];
	result[2] = in1[0] * in2[1] - in1[1] * in2[0];
}

void createRotationZ(float* out, float rad)
{
	createIdentity(out);
	out[0] = cos(rad);
	out[1] = sin(rad);
	out[4] = -sin(rad);
	out[5] = cos(rad);
}

void createTranslation(float* out, float x, float y, float z)
{
	createIdentity(out);
	out[12] = x;
	out[13] = y;
	out[14] = z;
}

void createIdentity(float* out)
{
	for (int i = 0; i < 16; i++)
	{
		if (i % 5 == 0)
		{
			out[i] = 1;
		}
		else
		{
			out[i] = 0;
		}
	}
}

// https://www.3dgep.com/understanding-the-view-matrix/#Memory_Layout_of_Column-Major_Matrices
void computeView(GLfloat* result)
{
	// ASSUMES result is an empty array of 16 floats
	float eye[] = { 0.0f, 0.0f, 1.0f };
	float target[] = { 0.0f, 0.0f, 0.0f };
	float up[] = { 0.0f, 1.0f, 0.0f };


	float zaxis[] = { eye[0] - target[0], eye[1] - target[1], eye[2] - target[2] };
	normaliseFloat3(zaxis);	// The "forward" vector.

	float xaxis[3];
	crossFloat3(xaxis, up, zaxis);
	normaliseFloat3(xaxis);	// The "right" vector.

	float yaxis[3];
	crossFloat3(yaxis, zaxis, xaxis);     // The "up" vector.

	// Create a 4x4 orientation matrix from the right, up, and forward vectors
	// This is transposed which is equivalent to performing an inverse 
	// if the matrix is orthonormalized (in this case, it is).
	float orientation[16] = {
	   xaxis[0], xaxis[1], xaxis[2], 0,
	   yaxis[0], yaxis[1], yaxis[2], 0,
	   zaxis[0], zaxis[1], zaxis[2], 0,
	   0,       0,       0,		1
	};

	// Create a 4x4 translation matrix.
	// The eye position is negated which is equivalent
	// to the inverse of the translation matrix. 
	// T(v)^-1 == T(-v)
	float translation[16] = {
		1,      0,      0,   0,
		0,      1,      0,   0,
		0,      0,      1,   0,
		-eye[0], -eye[1], -eye[2], 1
	};

	// Combine the orientation and translation to compute 
	// the final view matrix. Note that the order of 
	// multiplication is reversed because the matrices
	// are already inverted.
	multiply4x4(result, orientation, translation);
}

// https://stackoverflow.com/questions/1638355/function-for-perspective-projection-of-a-matrix-in-c
void computeProjection(float fov, float aspect, float nearcp, float farcp, GLfloat* mret) 
{
	float D2R = 3.14159f / 180.0f;
	float yScale = 1.0f / tan(D2R * fov / 2);
	float xScale = yScale / aspect;
	float nearmfar = nearcp - farcp;
	GLfloat m[] = {
		xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, (farcp + nearcp) / nearmfar, -1,
		0, 0, 2 * farcp*nearcp / nearmfar, 0
	};
	memcpy(mret, m, sizeof(float) * 16);
}



