#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform vec3 face_normal;
uniform float step;
uniform bool refractOn;
uniform bool reflectOn;

in  vec4 vTexCoord;
in  vec4 vPosition;
in  vec4 Normal;
in  vec4 vColor;
out vec4 interpolatedColor;
out vec2 texCoord;
out vec4 Position;
out vec3 R;

void main() 
{

  if(refractOn){

	vec4 viewPos = vPosition;
	vec3 eyeDirection = viewPos.xyz;
	vec3 mNormal = Normal.xyz;
	vec4 mmNormal = vec4(Normal.xyz,0);
	vec4 viewN = model_matrix*mmNormal;
	vec3 N = normalize(viewN.xyz);
	R = refract(eyeDirection, N, 0.3);
	}

  if(reflectOn)
  {
	vec4 viewPos = model_matrix*vPosition;
	vec3 eyeDirection = viewPos.xyz;
	vec3 mNormal = Normal.xyz;
	vec4 mmNormal = vec4(Normal.xyz,0);
	vec4 viewN = model_matrix*mmNormal;
	vec3 N = normalize(viewN.xyz);
	R = reflect(eyeDirection, N);
  }



  

  gl_Position = projection_matrix*model_matrix*vPosition;

  texCoord = vTexCoord.st;

  interpolatedColor = vColor;

  Position = gl_Position; 
  }
  //