#version 150

//uniform float timeMod;
uniform sampler2D sampler;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 colour;

void main() {
    
    vec4 textureColour = texture(sampler, IN.texCoord);
    //colour = IN.colour * textureColour;

	colour = IN.colour * textureColour;
}

