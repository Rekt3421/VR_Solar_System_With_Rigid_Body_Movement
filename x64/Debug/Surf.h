#ifndef __SURF_H__
#define __SURF_H__

#include <windows.h>
#include <GL/glew.h>

//Call this in initOpenGl() 
GLuint create_surf_vao();
GLuint create_surf_ibo();
GLuint  create_asteroids_vbo();
GLuint create_asteroid_vao();
//Call this in display()
void draw_surf(GLuint vao, GLuint ibo, int draw);
void read_mesh();
#endif
