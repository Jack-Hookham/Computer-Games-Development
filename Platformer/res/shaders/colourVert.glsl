#version 150

in vec2 position;
in vec4 colour;
in vec2 texCoord;

out Vertex	{
	vec4 colour;
	vec2 texCoord;
} OUT;

void main(void) {
	gl_Position.xy = position;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	
	OUT.colour		= colour;
	OUT.texCoord 	= texCoord;
}