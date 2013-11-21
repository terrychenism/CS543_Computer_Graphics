#version 150

uniform sampler2D texture;
uniform bool groundOn;
uniform bool refOn;
uniform bool fogOn;
uniform bool shadowOn;
uniform samplerCube texMap;
uniform bool refractOn;
uniform bool reflectOn;

in vec3 R;
in  vec4  interpolatedColor;
in  vec2 texCoord;
in  vec4 Position ;
out vec4  fColor;


void main() 
{ 
	//R = normalize(R);
	if ( groundOn ) 
	{
		fColor = texture2D( texture, texCoord );
	}
	else if(shadowOn )
	{
		fColor = vec4(0.0, 0.0,0.0,1.0);	
	}
	else if( refractOn)
	{
		fColor =  mix(textureCube(texMap, R),vec4(1.0, 1.0,1.0,1.0),0.3f);
	}
	else if( reflectOn )
	{
		fColor = textureCube(texMap, R);
	}
	else 
	{
		//fColor = interpolatedColor;	
		fColor = vec4(0.5, 1.0,0.5,1.0);	
	}



	
		float dist = abs(Position.z);
		
		float fogFactor = (300 - dist) / (300 - 0);
		if (fogOn) fogFactor = exp(-dist*0.0125);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		fColor = mix(vec4(1.0, 1.0,1.0,1.0), fColor,fogFactor);
	
	
	//fColor = vec4(1.0, 0.0,0.0,1.0);
	//fColor = textureCube(texMap, R);
} 

