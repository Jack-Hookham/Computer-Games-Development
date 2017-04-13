#version 150

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out Vertex	{
	vec4 vertexColor;
	vec2 vertexUV;
} OUT;

void main(void) {
	gl_Position.xy = position;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	
	OUT.vertexColor		= vertexColor;
	OUT.vertexUV 	= vertexUV;
}