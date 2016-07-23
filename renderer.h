/**
 * renderer.h
 *
 * Este header contiene los prototipos de las funciones necesarias para
 * hacer un render de una escena de una superficie con una pelota.
 *
 * Programacion: #00-32749 Ciro Duran
 *               #00-33151 David Ojeda
 *
 * Fecha: 2004-11-11
 *
 * Universidad Simon Bolivar
 * Departamento de Computacion y Tecnologia de la Informacion
 * CI-4321 - Computacion Grafica I
 */

#ifndef RENDERER_H
#define RENDERER_H
#define BUFSIZE 256

#include "data.h"

/** FUNCIONES **/

void init_renderer(void);
void resize(int width, int height);

void render(void);
void render_level();
void render_plane(PLANE *plane);
void render_surface(SURFACE *surface);
void render_control_points(SURFACE *surface);
void render_ball(BALL *ball, SURFACE *surface);
void render_text(void);
void drawstr(GLuint x, GLuint y, GLdouble scale, char* format, ...);
void drawstr2(GLfloat x, GLfloat y, GLfloat z, GLdouble scale, char* format, ...);
void set_material(MATERIAL *material);
void pick_points(int x,int y);
void processHits(GLint hits, GLuint buffer[], int sw);

#endif
