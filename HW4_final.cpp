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
bool refRandom = false;
bool refractionON = false;
bool reflectionON = false; 
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
void generateGeometry( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
void ReadPlyFile(char *FileName,int i);
void doIterate(int num,int iteratenum);
void generatelinetree(int Lsysnum);


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
		delete Vertex_Buffer;
		delete colors;
		delete Faces_Triangles;
		delete Normals;
		delete currentNormal;
		delete vertexNormals;
		delete refNormals;
	}

};

vertexdata picture[10];

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

	bmpread("nvnegx.bmp", 0, &bitmapnegx);
	bmpread("nvnegy.bmp", 0, &bitmapnegy);
	bmpread("nvnegz.bmp", 0, &bitmapnegz);
	bmpread("nvposx.bmp", 0, &bitmapposx);
	bmpread("nvposy.bmp", 0, &bitmapposy);
	bmpread("nvposz.bmp", 0, &bitmapposz);

	glActiveTexture(GL_TEXTURE1); 
	glGenTextures(1,&tex); 
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X ,0,GL_RGB,512,512,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapposx.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,0,GL_RGB,512,512,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapnegx.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y ,0,GL_RGB,512,512,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapposy.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ,0,GL_RGB,512,512,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapnegy.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z ,0,GL_RGB,512,512,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapposz.rgb_data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z ,0,GL_RGB,512,512,0,GL_RGB,
        GL_UNSIGNED_BYTE, bitmapnegz.rgb_data);


	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


   //GLuint texMapLocation;
   texMapLocation = glGetUniformLocation(program, "texMap");
   glUniform1i(texMapLocation, 1); // corresponding to unit 1

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


	GLuint vRefOn = glGetUniformLocation( program, "refOn");
	if ( reflectON == true )
		glUniform1i (vRefOn, true);
	else
		glUniform1i (vRefOn, false);



	glDrawElements( GL_TRIANGLES, picture[picnum].numface*3, GL_UNSIGNED_INT,  0);
	glDisable( GL_DEPTH_TEST ); 
}

void drawCar( void )
{
	int i= 2;
	int picnum = 2;
	int randNumb = 3;
	if (refRandom) 
		randNumb = rand()%2;
	if(refractionON) randNumb = 0;
	else if(reflectionON) randNumb =1;
	GLuint vRefraction = glGetUniformLocation(program, "refractOn");
	GLuint vReflection = glGetUniformLocation(program, "reflectOn");

	if(randNumb==0 )
		{
			glUniform1i(vRefraction, true);
		}

	if(randNumb==1)
		{
			glUniform1i(vReflection, true);
		}

		glBindBuffer( GL_ARRAY_BUFFER, picture[i].m_VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(float)*picture[i].numvertex*3 + sizeof(float)*picture[i].numvertex*3, 
				NULL, GL_STATIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(float)*picture[i].numvertex*3, picture[i].Vertex_Buffer );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(float)*picture[i].numvertex*3, sizeof(float)*picture[i].numvertex*3, picture[i].refNormals );


    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background

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

	texMapLocation = glGetUniformLocation(program, "texMap");
    glUniform1i(texMapLocation, 1);

	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_DEPTH_TEST );

	glDrawElements( GL_TRIANGLES, picture[picnum].numface*3, GL_UNSIGNED_INT,  0);
	glDisable( GL_DEPTH_TEST ); 

	glUniform1i(vRefraction, false);
	glUniform1i(vReflection, false);
}

void drawPlane( void )
{
	int i= 3;
	int picnum = 3;

	int randNumb = 3;
	
	if (refRandom) 
		randNumb = rand()%2;
	if(refractionON) randNumb = 0;
	else if(reflectionON) randNumb = 1;
	GLuint vRefraction = glGetUniformLocation(program, "refractOn");
	GLuint vReflection = glGetUniformLocation(program, "reflectOn");
	if(randNumb==0 )
		{
			
			glUniform1i(vRefraction, true);
		}

	if(randNumb==1)
		{

			glUniform1i(vReflection, true);
		}


		glBindBuffer( GL_ARRAY_BUFFER, picture[i].m_VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(float)*picture[i].numvertex*3 + sizeof(float)*picture[i].numvertex*3, 
				NULL, GL_STATIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(float)*picture[i].numvertex*3, picture[i].Vertex_Buffer );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(float)*picture[i].numvertex*3, sizeof(float)*picture[i].numvertex*3, picture[i].refNormals );


    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background

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

	texMapLocation = glGetUniformLocation(program, "texMap");
    glUniform1i(texMapLocation, 1);

	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_DEPTH_TEST );

	glDrawElements( GL_TRIANGLES, picture[picnum].numface*3, GL_UNSIGNED_INT,  0);
	glDisable( GL_DEPTH_TEST ); 
	glUniform1i(vRefraction, false);
	glUniform1i(vReflection, false);
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
	if(refractionON||reflectionON) drawPlane();
	else if (refRandom) drawPlane();
	else drawpic(3);


	//------draw plane shadow------
	if(shadowsON){
	viewMat_Plane = viewMat_Plane* Translate(light[0],light[1], light[2])*mlight*Translate(-light[0],-light[1],-light[2]); 
	mat4 mm3 = viewMat_Plane * modelMatPlane;
	GLuint modelMatrixShadowPlane = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrixShadowPlane, 1, GL_TRUE, mm3 );	
	drawpic(3);
	}
	//---------------------

	//-----draw car-------------
	modelMatCar = modelMatCar* Angel::Scale(4,4,4)*Angel::RotateX(-10.0f)* Angel::Translate(5,2,0);
	mat4 m1 = viewMat * modelMatCar;
	GLuint modelMatrixCar = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrixCar, 1, GL_TRUE, m1 );	
	if(refractionON||reflectionON) drawCar();
	else if (refRandom) drawCar();
	//drawRefPic(2);
	else drawpic(2);

	//------draw car shadow------
	if(shadowsON){
	viewMat_Car  = viewMat_Car * Translate(light[0],light[1], light[2])*mlight*Translate(-light[0],-light[1],-light[2]); 
	mat4 mm1 = viewMat_Car * modelMatCar;
	GLuint modelMatrixShadowCar = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrixShadowCar, 1, GL_TRUE, mm1 );	
	drawpic(2);

	}

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
		display();
		break;
	case 'T':{
		keyboard_value = 'T';
		reflectionON = !reflectionON;
		refractionON = false;
		display();
		break;}
	case 'V':
		{
		keyboard_value = 'V';
		refractionON = !refractionON;
		reflectionON = false;
		display();
		break;
		}
	case 'K':
		keyboard_value = 'K';
		reflectON = !reflectON;
		refRandom = true;
		reflectionON = false;
		refractionON = false;
		display();
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

			for(int i = 0; i < element[Lsysnum].length; i++ )
			{
		
				if(element[Lsysnum].leftsystem[i] == 'F')
				{

					
					element[Lsysnum].PointoDraw[element[Lsysnum].count] = Angel::RotateY(-90.0f) * element[Lsysnum].currentposition ;
					element[Lsysnum].rotateMat[element[Lsysnum].count] = Angel::RotateY(-90.0f)*  Direction ;
					element[Lsysnum].currentposition = element[Lsysnum].currentposition + currentdirection*element[Lsysnum].len;
					element[Lsysnum].count++;
				}

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
			picture[i].vertexNormals = new float[picture[i].numface*9];
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

		Angel::mat4 modelMat = Angel::identity();
		
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

			for( int k = 0; k < picture[i].numvertex*3; k++ )
			{
				picture[i].vertexNormals[k] = 0.0f;
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
						normals = normalize(normals);

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
			vec3  mNormal = vec3 (picture[i].vertexNormals[iterator*3],picture[i].vertexNormals[iterator*3+1],picture[i].vertexNormals[iterator*3+2]);

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
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 800,800 );
	srand(unsigned(time(0)));
	width = 512;
	height = 512;
    glutInitContextVersion( 3, 3 );
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
	//ReadPlyFile("box.ply",2);
	ReadPlyFile("f16.ply",3);
	
	generateGeometry();
	reflection();
	
    glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
    glutMainLoop();
	return 0;
}