#version 450 core
// https://learnopengl.com/Getting-started/Shaders


out vec4 fragColor;

// in vec4 vertexColor; // not used any more, so re-enable if you want to receive the vertex colour from the vert shader

uniform vec4 fragControl;	// the user controls the colour of each object  

void main()
{
    fragColor = fragControl;
}