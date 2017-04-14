#version 150

uniform float timeMod;
uniform sampler2D mySampler;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 colour;

void main() {
    
    vec4 textureColour = texture(mySampler, IN.texCoord);
    //colour = IN.colour * textureColour;

	colour = vec4(IN.colour.r * (cos(4.0 + timeMod) + 1.0) * 0.5,
				  IN.colour.g * (cos(8.0 + timeMod) + 1.0) * 0.5,
				  IN.colour.g * (cos(2.0 + timeMod) + 1.0) * 0.5,
				  IN.colour.a) * textureColour;
}