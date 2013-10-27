#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform vec3 face_normal;
uniform float step;
uniform bool pulsing;

in  vec4 vPosition;
in  vec4 vColor;
out vec4 interpolatedColor;

void main() 
{
  if( pulsing )
  {
	vec4 pnew = vPosition + vec4(face_normal, 0.0)*step;
	gl_Position = projection_matrix*model_matrix*pnew;
  }
  else
  {
	gl_Position = projection_matrix*model_matrix*vPosition;
  }

  if(vColor==vec4(0.0,0.0,0.0,1.0))
	 {
		interpolatedColor=vec4(0.0,1.0,0.0,1.0);
	}
  else{
		interpolatedColor = vColor;
		}
} 
