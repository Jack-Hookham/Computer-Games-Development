#version 150

uniform sampler2D mySampler;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 colour;

void main() {
    
    vec4 textureColor = texture(mySampler, IN.texCoord);
    colour = IN.colour * textureColor;
}