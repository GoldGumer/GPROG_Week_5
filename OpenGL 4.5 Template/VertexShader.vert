#version 450 core


// https://learnopengl.com/Getting-started/Shaders
layout (location = 0) in vec3 aPos;		// the position variable has attribute position 0

uniform mat4 localToWorld;				// the matrix containing this object's position
uniform mat4 worldToView;				// the matrix containing the camera's position
uniform mat4 viewToProjection;			// the matrix containing the projection frustum

//out vec4 vertexColor;					// not used any more so re-enable if you want to send the vertex colour to the vert shader

void main()
{
	gl_Position = (viewToProjection * worldToView * localToWorld) * vec4(aPos,1.0);
    // vertexColor = vec4(0.5, 0.5, 0.0, 1.0); // set the output variable to a dark-yellow color (not used any more, re-enable if needed)
}