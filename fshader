#version 150

uniform sampler2D texture;
uniform bool groundOn;
uniform bool fogOn;

in  vec4  interpolatedColor;
in  vec2 texCoord;
in  vec4 Position ;
out vec4  fColor;


void main() 
{ 

	

	if ( groundOn ) 
		fColor = texture2D( texture, texCoord );
	else
	{
		fColor = interpolatedColor;	
	}

	if (fogOn){
		float dist = abs(Position.z);
		float fogFactor = (100 - dist) / (100 - 0);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		fColor = mix(vec4(1.0, 1.0,1.0,1.0), fColor,fogFactor);
	}
} 

