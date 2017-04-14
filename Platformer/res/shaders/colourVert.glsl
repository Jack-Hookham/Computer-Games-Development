#version 150

uniform mat4 projMatrix;

in vec2 position;
in vec4 colour;
in vec2 texCoord;

out Vertex	{
	vec4 colour;
	vec2 texCoord;
} OUT;

void main(void) {
	gl_Position.xy = (projMatrix * vec4(position, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	
	OUT.colour		= colour;
	
	//flip the texCoord
	OUT.texCoord 	= vec2(texCoord.x, 1.0 - texCoord.y);
}