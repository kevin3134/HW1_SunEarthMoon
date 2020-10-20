#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

uniform mat4 projMatrix;
uniform mat4 mvMatrix;
uniform mat4 view;

void main()
{
	gl_Position = projMatrix * view * mvMatrix * vec4(vertexPosition_modelspace, 1);
}
