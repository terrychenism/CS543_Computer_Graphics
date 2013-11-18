#ifdef USER_INPUT
// Include the C++ iostreams classes for use when collecting user input
#include <iostream>
#endif 


#include "Angel.h"
#include "math.h"
#include <cassert>
#include "bmpread.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <complex>


using namespace std;
//----------------------------------------------------------------------------
int width = 0;
int height = 0;
int keyboard_value='W';
float ztrans;
bool shadowsON = false;
bool textureON = false;
bool fogON = false; 
bool reflectON = false;
bool Xaction=true;
bool xaction=true;
bool Yaction=true;
bool yaction=true;
bool Zaction=true;
bool zaction=true;
bool rotateaction=true;
bool scaleaction=true;
bool normalaction=true;
bool boundaction=true;
bool pulsingaction=true;
int keyboardState=2;
int numclick=0;
int Lsysnum=0;
int iTreeNum=0;
vec4 iColor;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef std::complex<float>  Complex;

inline float Random() { return float(rand())/RAND_MAX; }

GLuint Projection;
//GLuint tex ;
GLuint texMapLocation;

/* default data*/
/* can enter other values via command line arguments */

#define CENTERX -0.5
#define CENTERY 0.5
#define HEIGHT 0.5
#define WIDTH 0.5
#define MAX_ITER 100

/* N x M array to be generated */

#define N 512
#define M 512

float theight = HEIGHT;        
float twidth = WIDTH;


int n = N;
int m = M;


bmpread_t bitmap;

static  GLuint  texture = 0;
GLuint tex;
//----------------------------------------------------------------------------


float stepX=0,stepY=0,stepZ=0;
// remember to prototype
void generateGeometry( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
void quad( int a, int b, int c, int d );
void colorcube(void);
void drawCube(void);
void ReadPlyFile(char *FileName,int i);
void idle( void );
void doIterate(int num,int iteratenum);
void generatelinetree(int Lsysnum);
void cube( void );


// handle to program
GLuint program;

vec2    tex_coords[6];
vec4 points[6] = {
    point4( -10,0,-50, 1.0 ),
    point4( -10,0,14, 1.0 ),
    point4( 10,0,14,1.0 ),
	point4( 10,0,14,1.0 ),
    point4( 10,0,-50, 1.0 ),
	point4( -10,0,-50, 1.0 )

};
point4 mpoints[36];
point4 normals[36];
color4 mcolors[36];
point4 cube_vertices[8] = {
	point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
	};
	// RGBA olors
color4 vertex_colors[8] = {
		color4( 0.0, 0.0, 0.0, 1.0 ),  // black
		color4( 1.0, 0.0, 0.0, 1.0 ),  // red
		color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
		color4( 0.0, 1.0, 0.0, 1.0 ),  // green
		color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
		color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
		color4( 1.0, 1.0, 1.0, 1.0 ),  // white
		color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
	};

void Ground(){
	
	tex_coords[0] = vec2( 0.0, 10.0 );
	tex_coords[1] = vec2( 0.0, 0.0 );
	tex_coords[2] = vec2( 10.0, 0.0 );
	tex_coords[3] = vec2( 10.0, 0.0 );
	tex_coords[4] = vec2( 10.0, 10.0 );
	tex_coords[5] = vec2( 0.0, 10.0 );

	 }	

void init( void )
{
	Ground();

	printf("Loading grass.bmp\n");
	if(textureON){
		if(!bmpread("grass.bmp", 0, &bitmap))
			{
				fprintf(stderr, "%s:error loading bitmap file\n", "grass.bmp");
				exit(1);
			}
	}
	else {
		if(!bmpread("stones.bmp", 0, &bitmap))
			{
				fprintf(stderr, "%s:error loading bitmap file\n", "stones.bmp");
				exit(1);
			}
	}
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data );
	bmpread_free(&bitmap);


    // Create a vertex array object
    GLuint t_vao;
    glGenVertexArrays( 1, &t_vao );
    glBindVertexArray( t_vao );

    // Create and initialize a buffer object
    GLuint buffer10;
    glGenBuffers( 1, &buffer10 );
    glBindBuffer( GL_ARRAY_BUFFER, buffer10 );



	glBufferData( GL_ARRAY_BUFFER,sizeof(points) + sizeof(tex_coords), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(tex_coords), tex_coords );


    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );
	
    // Since our texture coordinates match our vertex positions, we
    //   can reuse the position data for our texture coordinates.
    GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" ); 
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points))  );

    //Projection = glGetUniformLocation( program, "Projection" );
	//Projection = glGetUniformLocation( program, "projection_matrix");
    // Set our texture samples to the active texture unit
    glUniform1i( glGetUniformLocation(program, "texture"), 0 );
    glBindTexture(GL_TEXTURE_2D, texture);

	GLuint vGroundOn = glGetUniformLocation( program, "groundOn" );
	glUniform1i(vGroundOn, true);
	
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable( GL_DEPTH_TEST );
    glDrawArrays( GL_TRIANGLES, 0, 6  );
	
	glDisable( GL_DEPTH_TEST ); 
}

class vertexdata
{
public:
		int numvertex;
		int numface; 
		int* Faces_Triangles;
		float* Vertex_Buffer;
		float* colors;
		float* Normals;
		float* vertexNormals;
		float* refNormals;
		GLfloat* currentNormal;
		GLuint m_VBO;
		GLuint n_VBO;
		GLuint m_IBO;
		GLuint n_IBO;
		GLfloat xMAX,xMIN,yMAX,yMIN,zMAX,zMIN;
		float stepN;
		float thetaY;
		float vr[3];

	vertexdata()
	{
		xMAX=0;
		xMIN=0;
		yMAX=0;
		yMIN=0;
		zMAX=0;
		zMIN=0;
		stepN=0;
		thetaY=0;

	}

	~vertexdata()
	{
		delete Faces_Triangles;
		delete Vertex_Buffer;
		delete Normals;
		delete currentNormal;
		delete vertexNormals;
		delete refNormals;
	}

};

vertexdata picture[2];

class token
{
	public:		
		int len,iter;
		GLfloat	rotx, roty, rotz;
		char start;
		char leftstart[40];
		char* rightsystem;
		char* leftsystem;
		int initlength;
		int length;
		int count;
		vec4 currentposition;
		vec4 PointoDraw[50000];
		mat4 rotateMat[50000];
		size_t s;
	token()
	{
		initlength=0;
		length=0;
		count= 0;
		s=0;
		currentposition = (0,0,0,1);
	}
	~token()
	{
		delete rightsystem;
		delete leftsystem;
	}

};
token element[5];

class Lsystem
{
	public:
		mat4 rotateX;
		mat4 rotateY;
		mat4 rotateZ;
};
Lsystem lsys[50];

void colorcube()
{

	float boundpoints[24]={ picture[numclick].xMIN, picture[numclick].yMIN,  picture[numclick].zMAX ,
						    picture[numclick].xMIN, picture[numclick].yMAX,  picture[numclick].zMAX ,
							picture[numclick].xMAX, picture[numclick].yMAX, picture[numclick].zMAX ,
							picture[numclick].xMAX, picture[numclick].yMIN, picture[numclick].zMAX ,
							picture[numclick].xMIN, picture[numclick].yMIN,  picture[numclick].zMIN,
							picture[numclick].xMIN,  picture[numclick].yMAX, picture[numclick].zMIN ,
							picture[numclick].xMAX,  picture[numclick].yMAX, picture[numclick].zMIN ,
							picture[numclick].xMAX,  picture[numclick].yMIN, picture[numclick].zMIN ,
							};

	unsigned int indices[24] = {1,5,6,2,
								1,2,3,0,
								7,4,0,3,
								5,4,7,6,
								2,6,7,3,
								1,0,4,5
	};


		glGenBuffers( 1, &picture[numclick].n_VBO );
		glBindBuffer( GL_ARRAY_BUFFER, picture[numclick].n_VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(float)*24, boundpoints, GL_STATIC_DRAW );

		//create IBO
		glGenBuffers( 1, &picture[numclick].n_IBO );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  picture[numclick].n_IBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER,  sizeof(int)*24, indices, GL_STATIC_DRAW );


}

vec4 randomPoint( float min, float max)
{
        Angel::vec4 point(0, 0, 0, 1);
    double random(double,double);
    
    
        
                point.x = GLfloat(random(min,max));
				point.y = 0.0f;
                point.z = GLfloat(random(min,max));       
        
     return point;
}
double random(double start, double end)
{ 
        return start+(end-start)*rand()/(RAND_MAX);
}

void reflection( void )
{
	bmpread_t bitmapnegx,bitmapnegy,bitmapnegz,bitmapposx,bitmapposy,bitmapposz;
	//if(!bmpread("nvnegx.bmp", 0, &bitmapnegx))
 //                       {
 //                               fprintf(stderr, "%s:error loading bitmap file\n", "grass.bmp");
 //                               exit(1);
 //                       }
	//if(!bmpread("nvnegy.bmp", 0, &bitmapnegy))
 //                       {
 //                               fprintf(stderr, "%s:error loading bitmap file\n", "grass.bmp");
 //                               exit(1);
 //                       }
	//if(!bmpread("nvnegz.bmp", 0, &bitmapnegz))
 //                       {
 //                               fprintf(stderr, "%s:error loading bitmap file\n", "grass.bmp");
 //                               exit(1);
 //                       }
	//if(!bmpread("nvposx.bmp", 0, &bitmapposx))
 //                       {
 //                               fprintf(stderr, "%s:error loading bitmap file\n", "grass.bmp");
 //                               exit(1);
 //                       }
	//if(!bmpread("nvposy.bmp", 0, &bitmapposy))
 //                       {
 //                               fprintf(stderr, "%s:error loading bitmap file\n", "grass.bmp");
 //                               exit(1);
 //                       }
	//if(!bmpread("nvposz.bmp", 0, &bitmapposz))
 //                       {
 //                               fprintf(stderr, "%s:error loading bitmap file\n", "grass.bmp");
 //                               exit(1);
 //                       }
	bmpread("nvnegx.bmp", 0, &bitmapnegx);
	bmpread("nvnegy.bmp", 0, &bitmapnegy);
	bmpread("nvnegz.bmp", 0, &bitmapnegz);
	bmpread("nvposx.bmp", 0, &bitmapposx);
	bmpread("nvposy.bmp", 0, &bitmapposy);
	bmpread("nvposz.bmp", 0, &bitmapposz);

	glActiveTexture(GL_TEXTURE1); 
	glGenTextures(1,&tex); 
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X ,0,GL_RGB,1,1,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapposx.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,0,GL_RGB,1,1,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapnegx.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y ,0,GL_RGB,1,1,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapposy.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ,0,GL_RGB,1,1,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapnegy.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z ,0,GL_RGB,1,1,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapposz.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z ,0,GL_RGB,1,1,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapnegz.rgb_data);


	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


   //GLuint texMapLocation;
   texMapLocation = glGetUniformLocation(program, "texMap");
   glUniform1i(texMapLocation, 1); // corresponding to unit 1

}

//int Index = 0;
//void quad( int a, int b, int c, int d )
//{
//    mcolors[Index] = vertex_colors[a]; mpoints[Index] = cube_vertices[a]; Index++;
//    mcolors[Index] = vertex_colors[b]; mpoints[Index] = cube_vertices[b]; Index++;
//    mcolors[Index] = vertex_colors[c]; mpoints[Index] = cube_vertices[c]; Index++;
//    mcolors[Index] = vertex_colors[a]; mpoints[Index] = cube_vertices[a]; Index++;
//    mcolors[Index] = vertex_colors[c]; mpoints[Index] = cube_vertices[c]; Index++;
//    mcolors[Index] = vertex_colors[d]; mpoints[Index] = cube_vertices[d]; Index++;
//}

void quad(int a, int b, int c, int d)
{
	//point4 normals[6];
	point4 normal;

	point4 vertices[8] = {
		point4( -0.5, -0.5,  0.5, 1.0 ),
		point4( -0.5,  0.5,  0.5, 1.0 ),
		point4(  0.5,  0.5,  0.5, 1.0 ),
		point4(  0.5, -0.5,  0.5, 1.0 ),
		point4( -0.5, -0.5, -0.5, 1.0 ),
		point4( -0.5,  0.5, -0.5, 1.0 ),
		point4(  0.5,  0.5, -0.5, 1.0 ),
		point4(  0.5, -0.5, -0.5, 1.0 )
    };
        static int i =0;
        normal = normalize(cross(vertices[b]-vertices[a], vertices[c]-vertices[b]));
        normals[i] = normal;
        mpoints[i] = vertices[a];
        i++;
        normals[i] = normal;
		mpoints[i] = vertices[b];

        i++;
        normals[i] = normal;
        mpoints[i] = vertices[c];
        i++;
        normals[i] = normal;
        mpoints[i] = vertices[a];
        i++;
        normals[i] = normal;
        mpoints[i] = vertices[c];
        i++;
        normals[i] = normal;
        mpoints[i] = vertices[d];
        i++;

        /*GLuint buffer11;
		glGenBuffers( 1, &buffer11);
        glBindBuffer(GL_ARRAY_BUFFER, buffer11);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
        NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
        sizeof(normals), normals);

	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

	GLuint vNormal = glGetAttribLocation(program, "Normal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );


	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	glUniform1i(texMapLocation, 1);*/


}

void cube_test( void )
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

GLuint vao;
void generateGeometry( void )
{	
	
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );

    glClearColor( 1.0, 1.0,1.0,1.0 ); // white background
}
//void drawCube(void)
//{
//	//colorcube();
//	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // white background
//	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//	glEnable( GL_DEPTH_TEST );
//	//glBindBuffer( GL_ARRAY_BUFFER, picture[numclick].n_VBO );
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  picture[numclick].n_IBO );
//	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
//	glEnableVertexAttribArray( vPosition );
//	glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
//			   BUFFER_OFFSET(0) );	
//	//glDrawArrays( GL_QUADS, 0, 24 );
//	glDrawElements( GL_QUADS, 24, GL_UNSIGNED_INT,  0);//violation
//	glDisable( GL_DEPTH_TEST ); 
//}


void drawCube(void)
{

	cube_test();
	//reflection();
	GLuint cubevao;
    glGenVertexArrays( 1, &cubevao );
    glBindVertexArray( cubevao );
	GLuint buffercube;
    glGenBuffers( 1, &buffercube );
    glBindBuffer( GL_ARRAY_BUFFER, buffercube );
   /*glBufferData( GL_ARRAY_BUFFER, sizeof(mpoints),mpoints,
		   GL_STATIC_DRAW );*/
	glBufferData( GL_ARRAY_BUFFER, sizeof(mpoints) + sizeof(normals),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(mpoints), mpoints );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(mpoints), sizeof(normals), normals );

	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );
	
	 /*GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(mpoints)) );*/

	//glClearColor( 1.0, 1.0,1.0, 1.0 ); // white background
	GLuint vNormal = glGetAttribLocation(program, "Normal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(mpoints)) );


	texMapLocation = glGetUniformLocation(program, "texMap");
    glUniform1i(texMapLocation, 1); // corresponding to unit 1

	//glActiveTexture(GL_TEXTURE1); 
	//glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	//glUniform1i(texMapLocation, 1);
	GLuint vcubeOn = glGetUniformLocation( program, "cubeOn");
	if ( reflectON == true )
		glUniform1i (vcubeOn, true);
	else
		glUniform1i (vcubeOn, false);



	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_TRIANGLES, 0, 36 );
	glDisable( GL_DEPTH_TEST ); 
}
void drawpic(int picnum)
{
	int i= picnum;
	for(int u=0; u<picture[i].numvertex;u++){
			picture[i].colors[u*3]=iColor.x;
			picture[i].colors[u*3+1]=iColor.y;
			picture[i].colors[u*3+2]=iColor.z;

		}
		//glGenBuffers( 1, &picture[i].m_VBO );
		glBindBuffer( GL_ARRAY_BUFFER, picture[i].m_VBO );
		//glBufferData( GL_ARRAY_BUFFER, sizeof(float)*picture[i].numvertex*3, picture[i].Vertex_Buffer, GL_STATIC_DRAW );
		glBufferData( GL_ARRAY_BUFFER, sizeof(float)*picture[i].numvertex*3 + sizeof(float)*picture[i].numvertex*3, NULL, GL_STATIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(float)*picture[i].numvertex*3, picture[i].Vertex_Buffer );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(float)*picture[i].numvertex*3, sizeof(float)*picture[i].numvertex*3, picture[i].colors );


	GLuint vGroundOn = glGetUniformLocation( program, "groundOn" );
	glUniform1i(vGroundOn, false);


    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glEnable( GL_DEPTH_TEST );
	//glBindBuffer( GL_ARRAY_BUFFER, picture[picnum].m_VBO );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  picture[picnum].m_IBO );
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );
	GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(float)*picture[picnum].numvertex*3));
	glDrawElements( GL_TRIANGLES, picture[picnum].numface*3, GL_UNSIGNED_INT,  0);
	glDisable( GL_DEPTH_TEST ); 
}

void drawRefPic(int picnum)
{
	int i= picnum;

		glBindBuffer( GL_ARRAY_BUFFER, picture[i].m_VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(float)*picture[i].numvertex*3 + sizeof(float)*picture[i].numvertex*3, NULL, GL_STATIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(float)*picture[i].numvertex*3, picture[i].Vertex_Buffer );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(float)*picture[i].numvertex*3, sizeof(float)*picture[i].numvertex*3, picture[i].refNormals );


    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glEnable( GL_DEPTH_TEST );
	//glBindBuffer( GL_ARRAY_BUFFER, picture[picnum].m_VBO );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  picture[picnum].m_IBO );
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );
	GLuint vNormal = glGetAttribLocation( program, "Normal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(float)*picture[picnum].numvertex*3));
	glDrawElements( GL_TRIANGLES, picture[picnum].numface*3, GL_UNSIGNED_INT,  0);
	glDisable( GL_DEPTH_TEST ); 
}

void display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

	if (keyboardState == 3){	
		 glutSwapBuffers();
	}

	//----light source for shadow --------------
	float light[3]={-50,100,-30};//location of light 
	mat4 mlight;
	mlight[3][1]=-1.0/light[1];
	mlight[3][3] = 0;
	//--------------------------------------

	mat4 viewMat = LookAt( vec4(0,30,100,1), vec4(0,0,0,1), vec4(0,1,0,0) );
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)60.0, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 9998.0);
	mat4 modelMat;
	ztrans = sqrt(pow(picture[numclick].xMAX-picture[numclick].xMIN,2)+pow(picture[numclick].yMAX-picture[numclick].yMIN,2)+pow(picture[numclick].zMAX-picture[numclick].zMIN,2));

	mat4 viewMat_Tree = viewMat;
	mat4 viewMat_Plane = viewMat;
	mat4 viewMat_Car = viewMat;


	mat4 modelMatCar = modelMat;
	mat4 modelMatPlane = modelMat;
	mat4 modelMatGround = modelMat;
	mat4 modelMatCube = modelMat;

	//-----------draw cube----------


	if ( reflectON == true ){
		mat4 mcube =  modelMatCube * Scale(0.3,0.3,0.3)*Translate(0.0, 0.0, -2.0f) * Angel::RotateY(45.0f) * Angel::RotateX(35.0f);
		GLuint modelMatrixCube = glGetUniformLocationARB(program, "model_matrix");
		glUniformMatrix4fv( modelMatrixCube, 1, GL_TRUE, mcube );
		drawCube();
	
	}
	

	//--------------------------- 

	GLuint vfogOn = glGetUniformLocation( program, "fogOn" );
	if( fogON == true ){
		
		glUniform1i(vfogOn, true);
	}
	else 
		{
		glUniform1i(vfogOn, false);
	}


	
	//------bind shadow color--------------
	GLuint vshadowsOn = glGetUniformLocation( program, "shadowOn" );
	if( shadowsON == true ){		
		glUniform1i(vshadowsOn, true);
	}
	else 
		{
		glUniform1i(vshadowsOn, false);
	}
	//--------------------------------------------------
	if(keyboardState == 0)
	{
		element[iTreeNum].currentposition.x = 0;
		element[iTreeNum].currentposition.y = -10;
		element[iTreeNum].currentposition.z = 0;
		generatelinetree(iTreeNum);

		
				for(int i = 0; i < element[iTreeNum].count; i++ )
						{	
						
					
								modelMat = Angel::Translate(0,0,-ztrans ) ;

								modelMat = element[iTreeNum].rotateMat[i] * modelMat;
					
								modelMat = Angel::Translate(element[iTreeNum].PointoDraw[i].x,element[iTreeNum].PointoDraw[i].y,element[iTreeNum].PointoDraw[i].z) * modelMat;
					
								//viewMat  = viewMat * Translate(light[0],light[1], light[2])*mlight*Translate(-light[0],-light[1],-light[2]); 
								 //set up projection matricies
								mat4 m = viewMat * modelMat;
								GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");
								glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, m );
								drawpic(0);
								drawpic(1);

							//------draw tree shadow------
								/*viewMat_Tree  = viewMat_Tree * Translate(light[0],light[1], light[2])*mlight*Translate(-light[0],-light[1],-light[2]); 
								mat4 mmtree = viewMat_Tree * modelMat;
								GLuint modelMatrixShadowTree = glGetUniformLocationARB(program, "model_matrix");
								glUniformMatrix4fv( modelMatrixShadowTree, 1, GL_TRUE, mmtree );	
								drawpic(0);
								drawpic(1);*/

						}	
				
			}	
	else if (keyboardState == 3){
		iTreeNum=int(random(0,4));
		iColor.x =  float(random(0,1));
		iColor.y =  float(random(0,1));
		iColor.z =  float(random(0,1));
		element[iTreeNum].currentposition.x = randomPoint(-50.0f,50.0f).x;
		element[iTreeNum].currentposition.y = randomPoint(-200.0f,200.0f).y;
		element[iTreeNum].currentposition.z = randomPoint(-50.0f,50.0f).z;
		generatelinetree(iTreeNum);

						for(int i = 0; i < element[iTreeNum].count; i++ )
						{	
						
					
								modelMat = Angel::Translate(0,0,-ztrans ) ;

								modelMat = element[iTreeNum].rotateMat[i] * modelMat;
					
								modelMat = Angel::Translate(element[iTreeNum].PointoDraw[i].x,element[iTreeNum].PointoDraw[i].y,element[iTreeNum].PointoDraw[i].z) * modelMat;

								//viewMat  = viewMat* Translate(light[0],light[1], light[2])*m*Translate(-light[0],-light[1],-light[2]); 

								// set up projection matricies
								mat4 m = viewMat * modelMat;
								GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");
								glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, m );
								drawpic(0);
								drawpic(1);

								//------draw tree shadow------
								if(shadowsON){
									viewMat_Tree  = viewMat_Tree * Translate(light[0],light[1], light[2])*mlight*Translate(-light[0],-light[1],-light[2]); 
									mat4 mmtree = viewMat_Tree * modelMat;
									GLuint modelMatrixShadowTree = glGetUniformLocationARB(program, "model_matrix");
									glUniformMatrix4fv( modelMatrixShadowTree, 1, GL_TRUE, mmtree );	
									drawpic(0);
									drawpic(1);
								}

						}	
	modelMatGround= Angel::Scale(5,5,5)*modelMatGround; 
	mat4 m2 = viewMat * modelMatGround;
	GLuint modelMatrixGround = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrixGround, 1, GL_TRUE, m2 );	
	//drawGround();
	init();
	}

	else if (keyboardState == 1){

			int count=0;
			for (int i = 1; i <= 2; i++)
			{

				count++;
				cout<<count<<endl;


			
				iTreeNum=int(random(0,4));
				iColor.x =  float(random(0,1));
				iColor.y =  float(random(0,1));
				iColor.z =  float(random(0,1));

	
				switch (iTreeNum){
					case 0:
						element[iTreeNum].currentposition.x = randomPoint(-1000.0f,1000.0f).x;
						element[iTreeNum].currentposition.y = randomPoint(-200.0f,200.0f).y;
						element[iTreeNum].currentposition.z = randomPoint(-50.0f,50.0f).z;
						break;
					case 1:
						element[iTreeNum].currentposition.x = randomPoint(-1000.0f,0.0f).x;
						element[iTreeNum].currentposition.y = randomPoint(-200.0f,200.0f).y;
						element[iTreeNum].currentposition.z = randomPoint(-50.0f,50.0f).z;
						break;
					case 2:
						element[iTreeNum].currentposition.x = randomPoint(-50.0f,10.0f).x;
						element[iTreeNum].currentposition.y = randomPoint(-200.0f,200.0f).y;
						element[iTreeNum].currentposition.z = randomPoint(-50.0f,50.0f).z;
						break;
					case 3:
						element[iTreeNum].currentposition.x = randomPoint(-150.0f,10.0f).x;
						element[iTreeNum].currentposition.y = randomPoint(-200.0f,200.0f).y;
						element[iTreeNum].currentposition.z = randomPoint(-50.0f,50.0f).z;
						break;
					case 4:
						element[iTreeNum].currentposition.x = randomPoint(-300.0f,10.0f).x;
						element[iTreeNum].currentposition.y = randomPoint(-200.0f,200.0f).y;
						element[iTreeNum].currentposition.z = randomPoint(-50.0f,50.0f).z;
						break;
					}
				
				generatelinetree(iTreeNum);

		
				for(int i = 0; i < element[iTreeNum].count; i++ )
						{	
						
					
								modelMat = Angel::Translate(0,0,-ztrans ) ;

								modelMat = element[iTreeNum].rotateMat[i] * modelMat;
					
								modelMat = Angel::Translate(element[iTreeNum].PointoDraw[i].x,element[iTreeNum].PointoDraw[i].y,element[iTreeNum].PointoDraw[i].z) * modelMat;

								//viewMat  = viewMat * Translate(light[0],light[1], light[2]) * mlight * Translate(-light[0],-light[1],-light[2]); 
								 //set up projection matricies
								mat4 m = viewMat * modelMat;
								GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");
								glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, m );
								drawpic(0);
								drawpic(1);

								//------draw tree shadow------
								if(shadowsON){
									viewMat_Tree  = viewMat_Tree * Translate(light[0],light[1], light[2])*mlight*Translate(-light[0],-light[1],-light[2]); 
									mat4 mmtree = viewMat_Tree * modelMat;
									GLuint modelMatrixShadowTree = glGetUniformLocationARB(program, "model_matrix");
									glUniformMatrix4fv( modelMatrixShadowTree, 1, GL_TRUE, mmtree );	
									drawpic(0);
									drawpic(1);
								}

						}	
				
			}	

	//------draw plane-------
	modelMatPlane = modelMatPlane* Angel::Scale(2,2,2)*Angel::RotateX(15.0f)* Angel::Translate(-10,20,0);
	mat4 m3= viewMat * modelMatPlane;
	GLuint modelMatrixPlane = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrixPlane, 1, GL_TRUE, m3 );	
	drawpic(3);

	//------draw plane shadow------
	viewMat_Plane = viewMat_Plane* Translate(light[0],light[1], light[2])*mlight*Translate(-light[0],-light[1],-light[2]); 
	mat4 mm3 = viewMat_Plane * modelMatPlane;
	GLuint modelMatrixShadowPlane = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrixShadowPlane, 1, GL_TRUE, mm3 );	
	drawpic(3);
	//---------------------

	//-----draw car-------------
	modelMatCar = modelMatCar* Angel::Scale(4,4,4)*Angel::RotateX(-10.0f)* Angel::Translate(5,0,0);
	mat4 m1 = viewMat * modelMatCar;
	GLuint modelMatrixCar = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrixCar, 1, GL_TRUE, m1 );	
	drawpic(2);

	//------draw car shadow------
	viewMat_Car  = viewMat_Car * Translate(light[0],light[1], light[2])*mlight*Translate(-light[0],-light[1],-light[2]); 
	mat4 mm1 = viewMat_Car * modelMatCar;
	GLuint modelMatrixShadowCar = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrixShadowCar, 1, GL_TRUE, mm1 );	
	drawpic(2);



	//------draw ground-------
	modelMatGround= Angel::Scale(5,5,5)*modelMatGround; 
	mat4 m2 = viewMat * modelMatGround;
	GLuint modelMatrixGround = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrixGround, 1, GL_TRUE, m2 );	
	//drawGround();
	init();
	}
	//-------------------------
		
	GLuint projectionMatrix = glGetUniformLocationARB(program, "projection_matrix");
	glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, perspectiveMat);
	


	glFlush(); // force output to graphics hardware

	 //use this call to double buffer
	glutSwapBuffers();
//	 you can implement your own buffers with textures
}

void keyboard( unsigned char key, int x, int y )
{
	
    switch ( key ) {

    case 033:
        exit( EXIT_SUCCESS );
        break;

	case 'a':
		//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		keyboardState = 0;
		iTreeNum=0;
		display();
		break;
	case 'b':
		
		keyboardState = 0;
		iTreeNum=1;
		display();
		break;
	case 'c':
		keyboardState = 0;
		iTreeNum=2;
		display();
		break;
	case 'd':
		keyboardState = 0;
		iTreeNum=3;
		display();
		break;
	case 'e':
		keyboardState = 0;
		iTreeNum=4;
		display();
		break;
	case 'f':
		keyboardState=1;

		display();
		break;
	case 'r':
		keyboardState=3;

		display();
		break;
	

	case 'D':
		keyboard_value = 'D';
		shadowsON=!shadowsON; 
		display();
		break;
	case 'A':
		keyboard_value = 'A';
		textureON=!textureON;
		display();
		break;
	case 'F':
		keyboard_value = 'F';
		fogON = !fogON;
		break;
	case 'T':
		keyboard_value = 'T';
		break;
	case 'V':
		keyboard_value = 'V';
		
		break;
	case 'K':
		keyboard_value = 'K';
		reflectON = !reflectON;
		display();
		break;
	}
		
}

void idle( void )
{
	
	switch(keyboard_value)
	{
		case'X':
			if(!Xaction){
				stepX += (picture[numclick].xMAX-picture[numclick].xMIN)/5000;
				if ( stepX > (picture[numclick].xMAX-picture[numclick].xMIN) ) {
					//stepX -= (picture[numclick].xMAX-picture[numclick].xMIN);
					display();
					}
				glutPostRedisplay();
			}
		break;

		case 'x':
			if(!xaction){
				stepX -= (picture[numclick].xMAX-picture[numclick].xMIN)/5000;
				if ( stepX < 0.0 ) {
					//stepX += (picture[numclick].xMAX-picture[numclick].xMIN);
					display();
					}
				glutPostRedisplay();
			}
		break;	

		case'Y':
			if(!Yaction){
				stepY += (picture[numclick].yMAX-picture[numclick].yMIN)/5000;
				if ( stepY > (picture[numclick].yMAX-picture[numclick].yMIN) ) {
					//stepY -= (picture[numclick].yMAX-picture[numclick].yMIN);
					display();
					}
				glutPostRedisplay();
			}
		break;

		case 'y':
			if(!yaction){
				stepY -= (picture[numclick].yMAX-picture[numclick].yMIN)/5000;
				if ( stepY < 0.0 ) {
					//stepY += (picture[numclick].yMAX-picture[numclick].yMIN);
					display();
					}
				glutPostRedisplay();
			}
		break;

		case'Z':
			if(!Zaction){
				stepZ += (picture[numclick].zMAX-picture[numclick].zMIN)/5000;
				if ( stepZ > (picture[numclick].zMAX-picture[numclick].zMIN) ) {
					//stepZ -= (picture[numclick].zMAX-picture[numclick].zMIN);
					display();
					}
				glutPostRedisplay();
			}
		break;

		case 'z':
			if(!zaction){
				stepZ -= (picture[numclick].zMAX-picture[numclick].zMIN)/5000;
				if ( stepZ < 0.0 ) {
					//stepZ += (picture[numclick].zMAX-picture[numclick].zMIN);
					display();
					}
				glutPostRedisplay();
			}
		break;

		case 'R':
			if(!rotateaction){
				if(numclick%2==0){
					picture[numclick].thetaY += 0.1;
					if(picture[numclick].thetaY >= 360)
						numclick++;
				}

				else {
					picture[numclick].thetaY -= 0.1;
					if(picture[numclick].thetaY < -360)
						numclick++;
				}
				glutPostRedisplay();

				if(numclick == 2)
					numclick = 0;

			}


		break;

		case 'B':	
			
				
			

			if(!pulsingaction)
			{
				picture[numclick].stepN -= ztrans/10000;
				
			}
			else {
				picture[numclick].stepN += ztrans/10000;
			}

			if( picture[numclick].stepN >= ztrans/10 ){
				pulsingaction =! pulsingaction;
			}
			if(picture[numclick].stepN < 0){

				pulsingaction =! pulsingaction;
			}
			//display();
			glutPostRedisplay();
			break;

	}
	
}

void ReadTxtFile(char *FileName,int numtoken)
{
	
	FILE *inStream;

	char buffer[999];
	if((inStream = fopen(FileName, "rt")) == NULL) // Open The File
	{
		printf("File does not exist!");
		exit(0);
	}

			fgets(buffer,10,inStream);			// txt

			while (  strncmp( "len: ", buffer, 5 ))
			{
				fgets(buffer,10,inStream);			// format
			}
			strcpy(buffer, buffer+strlen("len: "));
			sscanf(buffer,"%d", &element[numtoken].len);
 
			
			while (  strncmp( "iter: ", buffer, 6  ))
			{
				fgets(buffer,10,inStream);			// format
			}
			strcpy(buffer, buffer+strlen("len: "));
			sscanf(buffer,"%d", &element[numtoken].iter);


			while (  strncmp( "rot: ", buffer,5  ))
					{
						fgets(buffer,30,inStream);			// format
					}

			strcpy(buffer, buffer+strlen("rot: "));
			sscanf(buffer,"%f %f %f", &element[numtoken].rotx,&element[numtoken].roty,&element[numtoken].rotz);


			while (  strncmp( "start: ", buffer, 7  ))
					{
						fgets(buffer,10,inStream);			// format
					}
			strcpy(buffer, buffer+strlen("start: "));
			sscanf(buffer,"%s", &element[numtoken].start);


			

			while (  strncmp( "F: ", buffer, 3  ))
					{
						fgets(buffer,35,inStream);			// format
					}
			strcpy(buffer, buffer+strlen("F: "));
			sscanf(buffer,"%s", &element[numtoken].leftstart);
			
		fclose(inStream);

		//copy to R+L system
		element[numtoken].length = strlen(element[numtoken].leftstart);

		element[numtoken].rightsystem = new char[element[numtoken].length+1];
		element[numtoken].leftsystem = new char[element[numtoken].length+1];
		
		for(int i=0; i < element[numtoken].length; i++)
		{
			element[numtoken].rightsystem[i] = element[numtoken].leftstart[i];
		}
		element[numtoken].rightsystem[element[numtoken].length] = '\0';

		doIterate(numtoken,element[numtoken].iter-1);
		
}

void doIterate(int num,int iteratenum)
{
	unsigned int count=0;
	unsigned int i,a;
	unsigned int j=0;
	unsigned int b=0;
	
	element[num].initlength = strlen(element[num].leftstart);
	for(int itime=1; itime <= iteratenum; itime++)
	{
		count=0;
		element[num].length = strlen(element[num].rightsystem);//iterate length

			for( i=0; i<element[num].length; i++)
			{
				if(element[num].rightsystem[i]=='F')
				{
					count++;
				}
			}
			
			element[num].s = element[num].initlength*count+(element[num].length-count);
			size_t s = element[num].s;
			size_t mb = s / 1024 / 1024;
			element[num].leftsystem = (char *)malloc(element[num].s+1);

			//new leftsystem
			for( j = 0 ; j < element[num].s;)
			{
				for(i=0;i< element[num].length;i++)
				{
					if(element[num].rightsystem[i]=='F')
					{
						for(a=0;a < element[num].initlength;a++)
						{
							element[num].leftsystem[j]=element[num].leftstart[a];
							j++;
						}

					}
					else
					{
						element[num].leftsystem[j]=element[num].rightsystem[i];
						j++;
					}
					
				}
				
			}
			element[num].leftsystem[j] = '\0';

			//new rightsystem
			element[num].rightsystem = (char *)malloc(element[num].s+1);

			for( b=0; b < element[num].s; b++ )
			{
				element[num].rightsystem[b]=element[num].leftsystem[b];
				
			}
			element[num].rightsystem[element[num].s] = '\0';
	}
	element[num].length = strlen(element[num].leftsystem);
	Lsysnum = num;
	//generatelinetree(num);
}

struct TurtleState
{
	vec4 currentposition;
	vec4 currentdirection;
	mat4 Direction;
};

void generatelinetree(int Lsysnum)
{
	std::vector<TurtleState> stateStack;
	vec4 currentdirection;
	currentdirection.x = 0.0f;
	currentdirection.y = 1.0f;
	currentdirection.z = 0.0f;
	currentdirection.w = 0.0f;
	mat4 Direction = Angel::identity();

	//for(Lsysnum = 0; Lsysnum < 5; Lsysnum++){
		

		/*switch (Lsysnum){
			case 0:
				element[Lsysnum].currentposition = randomPoint(-100.0f,100.0f);
				break;
			case 1:
				element[Lsysnum].currentposition = randomPoint(-50.0f,-50.0f);
				break;
			case 2:
				element[Lsysnum].currentposition = randomPoint(-20.0f,30.0f);
				break;
			case 3:
				element[Lsysnum].currentposition = randomPoint(-30.0f,60.0f);
				break;
			case 4:
				element[Lsysnum].currentposition = randomPoint(-60.0f,100.0f);
				break;
			}*/


			for(int i = 0; i < element[Lsysnum].length; i++ )
			{
		
				if(element[Lsysnum].leftsystem[i] == 'F')
				{

					
					element[Lsysnum].PointoDraw[element[Lsysnum].count] = Angel::RotateY(-90.0f) * element[Lsysnum].currentposition ;
					element[Lsysnum].rotateMat[element[Lsysnum].count] = Angel::RotateY(-90.0f)*  Direction ;
					element[Lsysnum].currentposition = element[Lsysnum].currentposition + currentdirection*element[Lsysnum].len;
					element[Lsysnum].count++;
				}
				//else if(element[Lsysnum].leftstart[i] == 'f')

				else if(element[Lsysnum].leftsystem[i] == '+')
				{
					lsys[Lsysnum].rotateX = RotateX(element[Lsysnum].rotx);

					Direction = RotateX(element[Lsysnum].rotx) * Direction ;
					currentdirection = lsys[Lsysnum].rotateX * currentdirection;
					
				}
				else if(element[Lsysnum].leftsystem[i] == '-')
				{
					lsys[Lsysnum].rotateX = RotateX(-element[Lsysnum].rotx);

					Direction = RotateX(-element[Lsysnum].rotx) * Direction ;
					currentdirection = lsys[Lsysnum].rotateX * currentdirection;
				}
				else if(element[Lsysnum].leftsystem[i] == '&')
				{
					lsys[Lsysnum].rotateY = RotateY(element[Lsysnum].roty);

					Direction = RotateY(element[Lsysnum].roty) * Direction ;
					currentdirection = lsys[Lsysnum].rotateY * currentdirection;
				}
				else if(element[Lsysnum].leftsystem[i] == '^')
				{
					lsys[Lsysnum].rotateY = RotateY(-element[Lsysnum].roty);

					Direction = RotateY(-element[Lsysnum].roty) * Direction ;
					currentdirection = lsys[Lsysnum].rotateY * currentdirection;
				}
				else if(element[Lsysnum].leftsystem[i] == '\\')
				{
					lsys[Lsysnum].rotateZ = RotateZ(element[Lsysnum].rotz);

					Direction =  RotateZ(element[Lsysnum].rotz)  * Direction;
					currentdirection = lsys[Lsysnum].rotateZ*currentdirection;
				}
				else if(element[Lsysnum].leftsystem[i] == '/')
				{
					lsys[Lsysnum].rotateZ = RotateZ(-element[Lsysnum].rotz);

					Direction =  RotateZ(-element[Lsysnum].rotz) * Direction;
					currentdirection = lsys[Lsysnum].rotateZ *currentdirection;
				}
				else if(element[Lsysnum].leftsystem[i] == '|')
				{

					Direction = Direction;
					currentdirection = -currentdirection;
				}
				else if(element[Lsysnum].leftsystem[i] == '[')
				{
					TurtleState s;
					s.currentdirection = currentdirection;
					s.currentposition = element[Lsysnum].currentposition;
					s.Direction = Direction;
					stateStack.push_back(s);
					
				}
				else if(element[Lsysnum].leftsystem[i] == ']')
				{
					TurtleState s;
					s = stateStack.back();
					stateStack.pop_back();
					currentdirection = s.currentdirection;
					element[Lsysnum].currentposition = s.currentposition;
					Direction = s.Direction;
				}
				else
					cout<< "ERROR!"<<endl;



		
			}
	//}

}

void ReadPlyFile(char *FileName,int i)
{
	
	FILE *inStream;
	GLfloat	x, y, z;
	int triangle_index = 0;
	char buffer[999999];
	if((inStream = fopen(FileName, "rt")) == NULL) // Open The File
	{
		printf("File does not exist!");
		exit(0);
	}

			fgets(buffer,300,inStream);			// ply

			while (  strncmp( "element vertex", buffer,strlen("element face")) != 0  )
			{
				fgets(buffer,300,inStream);			// format
			}
			strcpy(buffer, buffer+strlen("element vertex"));
			sscanf(buffer,"%i", &picture[i].numvertex);
 
			
			while (  strncmp( "element face", buffer,strlen("element face")) != 0  )
			{
				fgets(buffer,300,inStream);			// format
			}
			strcpy(buffer, buffer+strlen("element face"));
			sscanf(buffer,"%i", &picture[i].numface);

			
 
				// go to end_header
			while (  strncmp( "end_header", buffer,strlen("end_header")) != 0  )
					{
						fgets(buffer,300,inStream);			// format
					}

			picture[i].Vertex_Buffer = new float[picture[i].numvertex*3];
			picture[i].colors = new float[picture[i].numvertex*3];
			picture[i].Faces_Triangles = new int[picture[i].numface*3];
			picture[i].Normals = new float[picture[i].numface*6];			
			picture[i].currentNormal = new float[picture[i].numface*3];
			picture[i].vertexNormals = new float[picture[i].numvertex*3];
			picture[i].refNormals = new float[picture[i].numvertex*3];
		
			// read vertexs
			for (int iterator = 0; iterator < picture[i].numvertex; iterator++)
			{
				fgets(buffer,300,inStream);
				
				sscanf(buffer,"%f %f %f", &picture[i].Vertex_Buffer[iterator*3], &picture[i].Vertex_Buffer[iterator*3+1], &picture[i].Vertex_Buffer[iterator*3+2]);


				if(picture[i].Vertex_Buffer[iterator*3]>=picture[i].xMAX){
					picture[i].xMAX=picture[i].Vertex_Buffer[iterator*3];
					}
				if(picture[i].Vertex_Buffer[iterator*3]<picture[i].xMIN){
					picture[i].xMIN=picture[i].Vertex_Buffer[iterator*3];
				}
				if(picture[i].Vertex_Buffer[iterator*3+1]>=picture[i].yMAX){
					picture[i].yMAX=picture[i].Vertex_Buffer[iterator*3+1];
				}
				if(picture[i].Vertex_Buffer[iterator*3+1]<picture[i].yMIN){
					picture[i].yMIN=picture[i].Vertex_Buffer[iterator*3+1];
				}
				if(picture[i].Vertex_Buffer[iterator*3+2]>=picture[i].zMAX){
					picture[i].zMAX=picture[i].Vertex_Buffer[iterator*3+2];
				}
				if(picture[i].Vertex_Buffer[iterator*3+2]<picture[i].zMIN){
					picture[i].zMIN=picture[i].Vertex_Buffer[iterator*3+2];
				}				
			}

	
		//cout<<picture[i].xMAX<<","<<picture[i].xMIN<<endl;	
		//cout<<picture[i].yMAX<<","<<picture[i].yMIN<<endl;
		//cout<<picture[i].zMAX<<","<<picture[i].zMIN<<endl;

		Angel::mat4 modelMat = Angel::identity();
		//modelMat = modelMat *Angel::Translate(-(picture[i].xMAX + picture[i].xMIN)/2,-(picture[i].yMAX + picture[i].yMIN)/2,-(picture[i].zMAX + picture[i].zMIN)/2);
			//modelMat = modelMat* Angel::RotateX(90.0f);	
		//	modelMat = modelMat* Angel::Scale(0.1,0.1,0.1)*Angel::RotateX(90.0f) *Angel::Translate(-(picture[i].xMAX + picture[i].xMIN)/2,-(picture[i].yMAX + picture[i].yMIN)/2,-(picture[i].zMAX + picture[i].zMIN)/2) ;//* Angel::RotateX(90.0f)
		
		/*if( i==0 )
			modelMat = modelMat* Angel::Scale(1/(picture[i].yMAX-picture[i].yMIN)/20,1/(picture[i].yMAX-picture[i].yMIN),(picture[i].yMAX-picture[i].yMIN)/10)* Angel::RotateX(90.0f) *	Angel::Translate(0,(picture[i].yMAX-picture[i].yMIN)/2,0);
		else modelMat = modelMat* Angel::Scale(1/(picture[i].yMAX-picture[i].yMIN)/20,1/(picture[i].yMAX-picture[i].yMIN)/20,(picture[i].yMAX-picture[i].yMIN)/10);*/
		
			//modelMat = modelMat* Angel::Translate(0,(picture[i].yMAX-picture[i].yMIN)/2,0)* Angel::RotateX(90.0f)* Angel::Scale(1/(picture[i].yMAX-picture[i].yMIN)/20,1/(picture[i].yMAX-picture[i].yMIN),(picture[i].yMAX-picture[i].yMIN)/10);	
		if (i == 0)     //sphere
			modelMat = modelMat* Angel::Scale((picture[0].xMAX-picture[0].xMIN)/20,(picture[i].yMAX-picture[i].yMIN)/20,(picture[i].zMAX-picture[i].zMIN)/20);
		else if( i == 1 )  //cylinder
			modelMat = modelMat* Angel::Translate(0,(picture[i].yMAX-picture[i].yMIN)/5,0)* Angel::RotateX(90.0f)* Angel::Scale((picture[0].xMAX-picture[0].xMIN)/(picture[i].xMAX-picture[i].xMIN),(picture[0].yMAX-picture[0].yMIN)/(picture[i].yMAX-picture[i].yMIN),1/(picture[i].zMAX-picture[i].zMIN));	
		else 
			modelMat = modelMat* Angel::Scale(1,1,1) *Angel::Translate(-(picture[i].xMAX + picture[i].xMIN)/2,-(picture[i].yMAX + picture[i].yMIN)/2,-(picture[i].zMAX + picture[i].zMIN)/2);	

		for(int iterator = 0; iterator < picture[i].numvertex; iterator++){
			Angel::vec4 modelvertex(
			picture[i].Vertex_Buffer[iterator*3],
			picture[i].Vertex_Buffer[iterator*3+1],
			picture[i].Vertex_Buffer[iterator*3+2],
			1.0
			);
			modelvertex=modelMat*modelvertex;
			
			picture[i].Vertex_Buffer[iterator*3] = modelvertex.x;
			picture[i].Vertex_Buffer[iterator*3+1] = modelvertex.y;
			picture[i].Vertex_Buffer[iterator*3+2] = modelvertex.z;
		}

		picture[i].xMAX=0;
		picture[i].xMIN=0;
		picture[i].yMAX=0;
		picture[i].yMIN=0;
		picture[i].zMAX=0;
		picture[i].zMIN=0;
		

		for (int iterator = 0; iterator < picture[i].numvertex; iterator++)
			{
				if(picture[i].Vertex_Buffer[iterator*3]>=picture[i].xMAX){
					picture[i].xMAX=picture[i].Vertex_Buffer[iterator*3];
					}
				if(picture[i].Vertex_Buffer[iterator*3]<picture[i].xMIN){
					picture[i].xMIN=picture[i].Vertex_Buffer[iterator*3];
				}
				if(picture[i].Vertex_Buffer[iterator*3+1]>=picture[i].yMAX){
					picture[i].yMAX=picture[i].Vertex_Buffer[iterator*3+1];
				}
				if(picture[i].Vertex_Buffer[iterator*3+1]<picture[i].yMIN){
					picture[i].yMIN=picture[i].Vertex_Buffer[iterator*3+1];
				}
				if(picture[i].Vertex_Buffer[iterator*3+2]>=picture[i].zMAX){
					picture[i].zMAX=picture[i].Vertex_Buffer[iterator*3+2];
				}
				if(picture[i].Vertex_Buffer[iterator*3+2]<picture[i].zMIN){
					picture[i].zMIN=picture[i].Vertex_Buffer[iterator*3+2];
				}
				
			}


		

			// read faces
			for (int iterator = 0; iterator < picture[i].numface; iterator++)
			{
				fgets(buffer,300,inStream);
 
					if (buffer[0] == '3')
					{
 						int vertex1 = 0, vertex2 = 0, vertex3 = 0;
						buffer[0] = ' ';
						sscanf(buffer,"%i%i%i", &vertex1, &vertex2, &vertex3 );

						picture[i].Faces_Triangles[iterator*3    ] = vertex1;
						picture[i].Faces_Triangles[iterator*3 + 1] = vertex2;
						picture[i].Faces_Triangles[iterator*3 + 2] = vertex3;
					

						//collect every three points of triangle into Normal to be calculated

						float coord1[3] = {picture[i].Vertex_Buffer[vertex1*3], picture[i].Vertex_Buffer[vertex1*3+1], picture[i].Vertex_Buffer[vertex1*3+2]};
						float coord2[3] = {picture[i].Vertex_Buffer[vertex2*3], picture[i].Vertex_Buffer[vertex2*3+1], picture[i].Vertex_Buffer[vertex2*3+2]};
						float coord3[3] = {picture[i].Vertex_Buffer[vertex3*3], picture[i].Vertex_Buffer[vertex3*3+1], picture[i].Vertex_Buffer[vertex3*3+2]};
						

						/* calculate Vector1 and Vector2 */
						   float va[3], vb[3], val; 
						   va[0] = coord1[0] - coord2[0];
						   va[1] = coord1[1] - coord2[1];
						   va[2] = coord1[2] - coord2[2];

						  vec3 normpoint1 = (0.0,0.0,0.0);
						  normpoint1.x = va[0];
						  normpoint1.y = va[1];
						  normpoint1.z = va[2];

						   vb[0] = coord1[0] - coord3[0];
						   vb[1] = coord1[1] - coord3[1];
						   vb[2] = coord1[2] - coord3[2];

						  vec3 normpoint2 = (0.0,0.0,0.0);
						  normpoint2.x = vb[0];
						  normpoint2.y = vb[1];
						  normpoint2.z = vb[2];

						/* cross product */

						vec3 normals=(0.0,0.0,0.0);
						normals = cross(normpoint1,normpoint2);

						//don't forget to initialize vertexNormal
						picture[i].vertexNormals[vertex1*3] += normals.x;
						picture[i].vertexNormals[vertex1*3+1] += normals.y;
						picture[i].vertexNormals[vertex1*3+2] += normals.z;
						picture[i].vertexNormals[vertex2*3] += normals.x;
						picture[i].vertexNormals[vertex2*3+1] += normals.y;
						picture[i].vertexNormals[vertex2*3+2] += normals.z;
						picture[i].vertexNormals[vertex3*3] += normals.x;
						picture[i].vertexNormals[vertex3*3+1] += normals.y;
						picture[i].vertexNormals[vertex3*3+2] += normals.z;


					}				
			}	
		
		for (int iterator = 0; iterator < picture[i].numvertex; iterator++)
		{
			vec3 mNormal = vec3 (picture[i].vertexNormals[iterator*3],picture[i].vertexNormals[iterator*3+1],picture[i].vertexNormals[iterator*3+2]);

			vec3 mmNormal = normalize(mNormal);

			picture[i].refNormals[ iterator*3  ] = mmNormal.x;
			picture[i].refNormals[ iterator*3+1] = mmNormal.y;
			picture[i].refNormals[ iterator*3+2] = mmNormal.z;

		}


		// Create VBO.
		//GLuint m_VBO; 
		glGenBuffers( 1, &picture[i].m_VBO );
		glBindBuffer( GL_ARRAY_BUFFER, picture[i].m_VBO );


		glGenBuffers( 1, &picture[i].m_IBO );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  picture[i].m_IBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*picture[i].numface*3, picture[i].Faces_Triangles, GL_STATIC_DRAW );
		fclose(inStream);
}

int main( int argc, char **argv )
{
	// init glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 800,800 );
	srand(unsigned(time(0)));
	width = 512;
	height = 512;
	//glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	//glutInitWindowSize( N, M );
    glutInitContextVersion( 3, 3 );
    //glutInitContextVersion( 3, 1 );
   // glutInitContextProfile( GLUT_CORE_PROFILE );

	// create window
	// opengl can be incorperated into other packages like wxwidgets, fltoolkit, etc.
    /*glutCreateWindow( "Color Cube" );*/
	glutCreateWindow( "Image Manipulation" );
	ReadTxtFile("lsys1.txt",0);
	ReadTxtFile("lsys2.txt",1);
	ReadTxtFile("lsys3.txt",2);
	ReadTxtFile("lsys4.txt",3);
	ReadTxtFile("lsys5.txt",4);


	glewExperimental = GL_TRUE;

	// init glew
    glewInit();
	ReadPlyFile("sphere.ply",0);
	ReadPlyFile("cylinder.ply",1);
	ReadPlyFile("big_dodge.ply",2);
	//ReadPlyFile("pickup_big.ply",2);
	ReadPlyFile("f16.ply",3);
	/*ReadPlyFile("weathervane.ply",0);
	ReadPlyFile("airplane.ply",1);
	ReadPlyFile("ant.ply",2);
	ReadPlyFile("apple.ply",3);
	ReadPlyFile("balance.ply",4);
	ReadPlyFile("beethoven.ply",5);
	ReadPlyFile("big_atc.ply",6);
	ReadPlyFile("big_dodge.ply",7);
	ReadPlyFile("teapot.ply",8);
	ReadPlyFile("big_porsche.ply",9);
	ReadPlyFile("big_spider.ply",10);
	ReadPlyFile("canstick.ply",11);
	ReadPlyFile("chopper.ply",12);
	ReadPlyFile("cow.ply",13);
	ReadPlyFile("dolphins.ply",14);
	ReadPlyFile("egret.ply",15);
	ReadPlyFile("f16.ply",16);
	ReadPlyFile("footbones.ply",17);
	ReadPlyFile("fracttree.ply",18);
	ReadPlyFile("galleon.ply",19);
	ReadPlyFile("hammerhead.ply",20);
	ReadPlyFile("helix.ply",21);
	ReadPlyFile("hind.ply",22);
	ReadPlyFile("kerolamp.ply",23);
	ReadPlyFile("ketchup.ply",24);
	ReadPlyFile("mug.ply",25);
	ReadPlyFile("part.ply",26);
	ReadPlyFile("pickup_big.ply",27);
	ReadPlyFile("pump.ply",28);
	ReadPlyFile("pumpa_tb.ply",29);
	ReadPlyFile("sandal.ply",30);
	ReadPlyFile("saratoga.ply",31);
	ReadPlyFile("scissors.ply",32);
	ReadPlyFile("shark.ply",33);
	ReadPlyFile("steeringweel.ply",34);
	ReadPlyFile("stratocaster.ply",35);
	ReadPlyFile("street_lamp.ply",36);
	ReadPlyFile("tennis_shoe.ply",37);
	ReadPlyFile("tommygun.ply",38);
	ReadPlyFile("trashcan.ply",39);
	ReadPlyFile("turbine.ply",40);
	ReadPlyFile("urn2.ply",41);
	ReadPlyFile("walkman.ply",42);*/

	
	
	generateGeometry();

	//init();
	reflection();
	// assign handlers
	
    glutDisplayFunc( display );
	//glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutIdleFunc(idle);
    glutMainLoop();
	return 0;
}
