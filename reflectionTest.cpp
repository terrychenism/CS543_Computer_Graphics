void reflection( void )
{
	cube();
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


	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);


   //GLuint texMapLocation;
   texMapLocation = glGetUniformLocation(program, "texMap");
   glUniform1i(texMapLocation, 1); // corresponding to unit 1

}

void quad(int a, int b, int c, int d)
{
	point4 normals[6];
	point4 normal;
	point4 cubePoints[8];
	point4 vertices[8] = {
        point4( -5, -5,  5, 1.0 ),
        point4( -5,  5,  5, 1.0 ),
        point4(  5,  5,  5, 1.0 ),
        point4(  5, -5,  5, 1.0 ),
        point4( -5, -5, -5, 1.0 ),
        point4( -5,  5, -5, 1.0 ),
        point4(  5,  5, -5, 1.0 ),
        point4(  5, -5, -5, 1.0 )
    };
        static int i =0;
        normal = normalize(cross(vertices[b]-vertices[a], vertices[c]-vertices[b]));

        normals[i] = normal;
        cubePoints[i] = vertices[a];
        i++;

        normals[i] = normal;
        cubePoints[i] = vertices[b];
        i++;

        normals[i] = normal;
        cubePoints[i] = vertices[c];
        i++;

        normals[i] = normal;
        cubePoints[i] = vertices[a];
        i++;

        normals[i] = normal;
        cubePoints[i] = vertices[c];
        i++;

        normals[i] = normal;
        cubePoints[i] = vertices[d];
        i++;

        GLuint buffer1;
		glGenBuffers( 1, &buffer1 );
        glBindBuffer(GL_ARRAY_BUFFER, buffer1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals),
        NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices),
        sizeof(normals), normals);

	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

	GLuint vNormal = glGetAttribLocation(program, "Normal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(vertices)) );


	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	glUniform1i(texMapLocation, 1);

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable( GL_DEPTH_TEST );
    glDrawArrays( GL_QUADS, 0, 6  );

}

void cube( void )
 {
	quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
 }
