#include <vector>
#include <glm/glm.hpp>
#include "Surf.h"
#include <string>
#include "LoadMesh.h"
#include <iostream>
#include <windows.h>
#include <GL/glew.h>

#include <GL/freeglut.h>

#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#define BUFFER_OFFSET(offset) ((void *)(offset))
static const int N = 600;
static const int NUM_VERTICES = 300*168;
static const std::string texture_name = "earth.jpg";
int count = 0;
//GLuint texture_id = 1;
//The sinc surface
static glm::vec3 surf(int i, int j)
{
   const float center = 0.5f*N;
   const float xy_scale = 20.0f / N;
   const float z_scale = 10.0f;

   float x = xy_scale * (i - center);
   float y = xy_scale * (j - center);

   float r = sqrt(x*x + y * y);
   float z = 1.0f;

   if (r != 0.0f)
   {
      z = sin(r) / r;
   }
   z = z * z_scale;

   return 0.05f*glm::vec3(x, y, z);
}

//Sinc surface normals
static glm::vec3 normal(int i, int j)
{
   glm::vec3 du = surf(i+1, j) - surf(i-1, j);
   glm::vec3 dv = surf(i, j+1) - surf(i, j-1);
   return glm::normalize(glm::cross(du, dv));
}

GLuint create_surf_vbo(int draw)
{
   //Declare a vector to hold N*N vertices
   //std::vector<glm::vec3> surf_verts;
   
   //surf_verts.reserve(NUM_VERTICES*3);
   std::vector<GLfloat> ballVerts;
   std::vector<GLfloat> norms;
   if (draw == 1) {
	   float radius = 1.0f;
	   float M_PI = 3.1415629;
	   for (int i = 0; i <= 300; i++)
	   {
		   double lat0 = M_PI * (-0.5 + (double)(i - 1) / 40);
		   double z0 = radius*sin(lat0);
		   double zr0 = radius*cos(lat0);

		   double lat1 = M_PI * (-0.5 + (double)i / 40);
		   double z1 = radius*sin(lat1);
		   double zr1 = radius*cos(lat1);

		   for (int j = 0; j <= 168; j++)
		   {
			   double lng = 2 * M_PI * (double)(j - 1) / 40;
			   double x = cos(lng);
			   double y = sin(lng);

			   ballVerts.push_back(x * zr0); //X
			   ballVerts.push_back(y * zr0); //Y
			   ballVerts.push_back(z0);      //Z
			   ballVerts.push_back(y * zr0);
			   ballVerts.push_back(x * zr0);
			   ballVerts.push_back(z0);
			   count++;
			   ballVerts.push_back( (count % (300*int(radius*radius))));
			   ballVerts.push_back( (count % (168*int(radius*radius))));





		   }
	   }

   }
   
   glUnmapBuffer(GL_ARRAY_BUFFER);
   GLuint vbo=2;
   glGenBuffers(1, &vbo); //Generate vbo to hold vertex attributes for triangle.

   glBindBuffer(GL_ARRAY_BUFFER, vbo); //Specify the buffer where vertex attribute data is stored.
   
   //Upload from main memory to gpu memory.
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*ballVerts.size(), ballVerts.data(), GL_STATIC_DRAW);

   return vbo;

}

GLuint create_asteroids_vbo() {
	std::vector< float > ballVerts;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	//FILE * file = fopen("rock.obj", "r");
	FILE* file = fopen("rock1.obj", "r");
	//FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		//return false;
	}
	int done = 0;
	while (!done) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			done = 1;
			break;
			// EOF = End Of File. Quit the loop.
		}
		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			//std::cout << "in";

			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			ballVerts.push_back(vertex.x);
			ballVerts.push_back(vertex.y);
			ballVerts.push_back(vertex.z);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			ballVerts.push_back(uv.x);
			ballVerts.push_back(uv.y);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			ballVerts.push_back(normal.x);
			ballVerts.push_back(normal.y);
			ballVerts.push_back(normal.z);
		}



	}
	GLuint vbo2 = 1;
	glGenBuffers(1, &vbo2); //Generate vbo to hold vertex attributes for triangle.

	glBindBuffer(GL_ARRAY_BUFFER, vbo2); //Specify the buffer where vertex attribute data is stored.

	//Upload from main memory to gpu memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*ballVerts.size(), ballVerts.data(), GL_STATIC_DRAW);

	return vbo2;
}
GLuint create_asteroid_vao() {
	GLuint vao2;
	glPointSize(0.01);
	//Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenVertexArrays(1, &vao2);


	//Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
	glBindVertexArray(vao2);

	GLuint vbov = create_asteroids_vbo();

	GLint pos_loc = 0;
	GLint color_loc = 4;
	GLint nor_loc = 2;
	GLint tex_loc = 1;

	//Enable the position attribute.

	//Tell opengl how to get the attribute values out of the vbo (stride and offset).
	//In this case, the vertices are at the beginning of the VBO and are tightly packed.
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glVertexAttribDivisor(pos_loc, 0);
	glEnableVertexAttribArray(pos_loc);

	glVertexAttribPointer(nor_loc, 3, GL_FLOAT, false, 8 * sizeof(float), BUFFER_OFFSET((3 * sizeof(GLfloat))));
	glVertexAttribDivisor(nor_loc, 0);
	glEnableVertexAttribArray(nor_loc);

	glVertexAttribPointer(tex_loc, 2, GL_FLOAT, false, 8 * sizeof(float), BUFFER_OFFSET((2 * sizeof(GLfloat))));
	glVertexAttribDivisor(tex_loc, 0);
	glEnableVertexAttribArray(tex_loc);
	
	glm::vec3 colors[] = {
	   glm::vec3(1.0,0.5,0.7),
	   glm::vec3(0.2, 1.0, 0.7),
	   glm::vec3(0.2, 0.5, 1.0),
	   glm::vec3(1.0, 0.5, 1.0),
	   glm::vec3(0.5, 1.0, 0.7),
	   glm::vec3(0.9, 0.3, 0.4),
	   glm::vec3(0.1, 0.3, 0.9),
	   glm::vec3(0.5,0.9,0.5),
	   glm::vec3(0.2,0.2,0.7)
	};
	glm::mat4 first = glm::scale(glm::vec3(-10.00f, -0.00f, -0.00f));
	glm::mat4 second = glm::translate(glm::vec3(-1.0f, 0, 0));
	glm::mat4 third = glm::translate(glm::vec3(0, 0, 0));
	glm::mat4 fourth = glm::translate(glm::vec3(1.0f, 1.0f, 0));
	glm::mat4 fifth = glm::translate(glm::vec3(0, 1.0f, 0));
	glm::mat4 sixth = glm::translate(glm::vec3(-1, 1.0f, 0));
	glm::mat4 seventh = glm::translate(glm::vec3(-1, 2, 0));
	glm::mat4 eighth = glm::translate(glm::vec3(0, 2, 0));
	glm::mat4 nineth = glm::translate(glm::vec3(1, 2, 0));

	glm::mat4 offsets[] = {
					first,
					second,
					third,
					fourth,
					fifth,
					sixth,
					seventh,
					eighth,
					nineth

	};

	GLuint coloursID;
	glGenBuffers(1, &coloursID);
	glBindBuffer(GL_ARRAY_BUFFER, coloursID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(GL_FLOAT) * 3, 0);
	glVertexAttribDivisor(4, 1);

	
	GLuint offsetsID;



	glGenBuffers(1, &offsetsID);
	glBindBuffer(GL_ARRAY_BUFFER, offsetsID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), offsets, GL_STATIC_DRAW);
	
	//glVertexAttribPointer(4, 4, GL_FLOAT, false,  sizeof(GL_FLOAT)*16, 0);
	//glVertexAttribDivisor(4, 1);
	for (int i = 0; i < 4; i++)
	{
		// Set up the vertex attribute
		glVertexAttribPointer(7 + i, // Location
			4, GL_FLOAT, GL_FALSE, // vec4
			sizeof(glm::mat4), // Stride
			(void *)(sizeof(glm::vec4) * i)); // Start offset
			// Enable it
		glEnableVertexAttribArray(7 + i);
		// Make it instanced
		glVertexAttribDivisor(7 + i, 1);
	}
	GLuint type;
	std::vector<float> type1;
	type1.push_back(0);
	glGenBuffers(1, &type);
	glBindBuffer(GL_ARRAY_BUFFER, type);
	glBufferData(GL_ARRAY_BUFFER, 1, type1.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, false, sizeof(GL_FLOAT), 0);
	glVertexAttribDivisor(5, 3);


	//GLuint vbon = create_surf_vbo_n();

  //  glVertexAttribPointer(nor_loc, 3, GL_FLOAT, false, 6 * sizeof(float), (GL *void));

	//glVertexAttribDivisor(nor_loc, 0);
	//glEnableVertexAttribArray(nor_loc);
	glPointSize(100);
	glBindVertexArray(0);
	return vao2;

}
GLuint create_surf_vao()
{

	
   GLuint vao;

   //Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
   glGenVertexArrays(1, &vao);


   //Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
   glBindVertexArray(vao);

   GLuint vbov = create_surf_vbo(1);
   
    GLint pos_loc = 0; 
	GLint color_loc = 4;
	GLint nor_loc = 2;
	GLint tex_loc = 1;

   //Enable the position attribute.

   //Tell opengl how to get the attribute values out of the vbo (stride and offset).
   //In this case, the vertices are at the beginning of the VBO and are tightly packed.
   glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 8*sizeof(float), 0);
   glVertexAttribDivisor(pos_loc, 0);
   glEnableVertexAttribArray(pos_loc);

   glVertexAttribPointer(nor_loc, 3, GL_FLOAT, false,8*sizeof(float) ,BUFFER_OFFSET(( 3*sizeof(GLfloat))));
   glVertexAttribDivisor(nor_loc, 0	);
   glEnableVertexAttribArray(nor_loc);

   glVertexAttribPointer(tex_loc, 2, GL_FLOAT, false,  8*sizeof(float), BUFFER_OFFSET((2*sizeof(GLfloat))));
   glVertexAttribDivisor(tex_loc, 0);
   glEnableVertexAttribArray(tex_loc);
   //glUnmapBuffer(vao);

   glm::vec3 colors[] = {
		glm::vec3(1.0,0.5,0.7),
		glm::vec3(0.2, 1.0, 0.7),
		glm::vec3(0.2, 0.5, 1.0),
		glm::vec3(1.0, 0.5, 1.0),
		glm::vec3(0.5, 1.0, 0.7),
		glm::vec3(0.9, 0.3, 0.4),
		glm::vec3(0.1, 0.3, 0.9),
		glm::vec3(0.5,0.9,0.5),
		glm::vec3(0.2,0.2,0.7)
   };
   glm::mat4 first = glm::translate(glm::vec3(0, 0, 0.0));
   glm::mat4 second = glm::translate(glm::vec3(-1.0f, 0, 0));
   glm::mat4 third = glm::translate(glm::vec3(0, 0, 0));
   glm::mat4 fourth = glm::translate(glm::vec3(1.0f, 1.0f, 0));
   glm::mat4 fifth = glm::translate(glm::vec3(0, 1.0f, 0));
   glm::mat4 sixth = glm::translate(glm::vec3(-1, 1.0f, 0));
   glm::mat4 seventh = glm::translate(glm::vec3(-1, 2, 0));
   glm::mat4 eighth = glm::translate(glm::vec3(0, 2, 0));
   glm::mat4 nineth = glm::translate(glm::vec3(1, 2, 0));

   glm::mat4 offsets[] = {
				   first,
				   second,
				   third,
				   fourth,
				   fifth,
				   sixth,
				   seventh,
				   eighth,
				   nineth

   };

   GLuint coloursID;
   glGenBuffers(1, &coloursID);
   glBindBuffer(GL_ARRAY_BUFFER, coloursID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
   glEnableVertexAttribArray(4);
   glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(GL_FLOAT) * 3, 0);
   glVertexAttribDivisor(4, 1);
   GLuint type;
   std:: vector<float> type1;
   type1.push_back(1);
   glGenBuffers(1, &type);
   glBindBuffer(GL_ARRAY_BUFFER, type);
   glBufferData(GL_ARRAY_BUFFER, 1, type1.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(5);
   glVertexAttribPointer(5, 1, GL_FLOAT, false, sizeof(GL_FLOAT), 0);
   glVertexAttribDivisor(5, 3);
   GLuint offsetsID;



   glGenBuffers(1, &offsetsID);
   glBindBuffer(GL_ARRAY_BUFFER, offsetsID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), offsets, GL_STATIC_DRAW);
   glEnableVertexAttribArray(4);
   //glVertexAttribPointer(4, 4, GL_FLOAT, false,  sizeof(GL_FLOAT)*16, 0);
   //glVertexAttribDivisor(4, 1);
   for (int i = 0; i < 4; i++)
   {
	   // Set up the vertex attribute
	   glVertexAttribPointer(7 + i, // Location
		   4, GL_FLOAT, GL_FALSE, // vec4
		   sizeof(glm::mat4), // Stride
		   (void *)(sizeof(glm::vec4) * i)); // Start offset
		   // Enable it
	   glEnableVertexAttribArray(7 + i);
	   // Make it instanced
	   glVertexAttribDivisor(7 + i, 1);
   }
   
  
   glBindVertexArray(0);
   //GLuint vbon = create_surf_vbo_n();

 //  glVertexAttribPointer(nor_loc, 3, GL_FLOAT, false, 6 * sizeof(float), (GL *void));

   //glVertexAttribDivisor(nor_loc, 0);
   //glEnableVertexAttribArray(nor_loc);

   return vao;
}



GLuint create_surf_ibo() {
	GLuint ibo = 0;
	

	std::vector< unsigned short> indexes;

	indexes.reserve(NUM_VERTICES);
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			indexes.push_back(N*i + j);
			indexes.push_back(N*(i+1)  + j);
			
		}
		indexes.push_back(1000000);
		//std:: cout<< (N*(i + 1) + 1 + 49);
	}

	
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*NUM_VERTICES ,indexes.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,);

	return ibo;


}

void draw_surf(GLuint vao ,GLuint ibo, int draw)
{
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, ibo);
	
	
	glBindVertexArray(vao);
	glEnable(GL_PRIMITIVE_RESTART);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLE_STRIP);
	glPrimitiveRestartIndex(1000000);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	if(draw == 0)
	glDrawElementsInstanced(GL_TRIANGLES, 300*300, GL_UNSIGNED_SHORT, 0,1);
	if (draw == 1)
		glDrawElementsInstanced(GL_TRIANGLE_STRIP, count/500
			, GL_UNSIGNED_SHORT, 0, 100001);
	
	//glDrawArrays(GL_TRIANGLE_STRIP_ADJACENCY, 0, NUM_VERTICES);
}

void read_mesh() {
	std::cout << "Haha";
	//string mesh_name = "rock.obj";
	//std::string file = "rock.obj";
	//MeshData mesh_data = LoadMesh("rock.obj");
	
	
}