/**
 * renderer.c
 *
 * Implementa las funciones prototipadas en renderer.h. Para mas informacion
 * referirse a dicho header.
 *
 * Programacion: #00-32749 Ciro Duran
 *               #00-33151 David Ojeda
 *
 * Fecha: 2004-11-14
 *
 * Universidad Simon Bolivar
 * Departamento de Computacion y Tecnologia de la Informacion
 * CI-4321 - Computacion Grafica I
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "data.h"
#include "renderer.h"
#include "mathutil.h"

//Variables importadas desde main.c
extern int screen_height;
extern int screen_width;
extern int selected;
extern int resolucion;
extern int edit_mode;
extern int wire_mode;
extern int cam_mode;
extern int camz_mode;


//Variables importadas desde resources.c
extern PLANE *plane;
extern SURFACE *surface;
extern BALL  *ball;
extern LIGHT_POINT *light_point;
extern CAMERA *camera;

extern POINT3D normal_vector;

//Fuentes
GLvoid *font_style = GLUT_STROKE_ROMAN;

void init_renderer() {
  //Seleccionando color de fondo
  glClearColor(0.0, 0.0, 0.0, 0.0);
	
  //Tipo de shading
  glShadeModel(GL_SMOOTH);
	
  //Disponiendo del viewport
  glViewport(0,0,screen_width,screen_height);
	
  //Configurando el frustrum
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(camera->aperture,(GLfloat)screen_width/(GLfloat)screen_height,1.0f,1000.0f);
	
  //Necesitamos probar profundidad
  glEnable(GL_DEPTH_TEST);
	
  //La manera como mostramos los poligonos
  glPolygonMode (GL_FRONT, GL_FILL);
	
  //Prendiendo las luces
  glLightfv (GL_LIGHT1, GL_AMBIENT, light_point->ambient);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, light_point->diffuse);
  glLightfv (GL_LIGHT1, GL_SPECULAR, light_point->specular);
  glLightfv (GL_LIGHT1, GL_POSITION, light_point->position);

  glEnable (GL_LIGHTING);	
  glEnable (GL_LIGHT1);
}

#define WIDTH  800
#define HEIGHT 600
void resize(int width, int height) {
  screen_width=width;
  screen_height=height;
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,screen_width,screen_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(camera->aperture,(GLfloat)screen_width/(GLfloat)screen_height,1.0f,1000.0f);
  glutPostRedisplay ();
}

void render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Configurando el frustrum
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(camera->aperture,(GLfloat)screen_width/(GLfloat)screen_height,1.0f,1000.0f);
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  gluLookAt(camera->position.x, camera->position.y, camera->position.z,
	    camera->center.x,   camera->center.y,   camera->center.z,
	    camera->up.x,       camera->up.y,       camera->up.z);

  /*
  //Rendering light point
  glPushMatrix();
  glDisable(GL_LIGHTING);
  glColor3f(0, 0.7, 0);
  glTranslatef(light_point->position[0], 
	      light_point->position[1],
	      light_point->position[2]);
  glutSolidSphere(0.1, 10, 10);
  glEnable(GL_LIGHTING);
  glPopMatrix();
  */
  glPushMatrix();
  glRotatef(surface->alphax,1.0,0.0,0.0);
  glRotatef(surface->alphaz,0.0,0.0,1.0);
 
	
  render_surface(surface);
  render_control_points(surface);
  render_ball(ball, surface);
  glPopMatrix();
  render_level();
  render_text();


  glFlush();
  glutSwapBuffers();
}

void render_level() {
  glDisable(GL_LIGHTING);
  //Eje X
  glBegin(GL_LINES);
  glColor3f(1.0, 1.0, 1.0);
  glVertex3f(0, 0, 0);
  glVertex3f(100, 0, 0);
  glEnd();
  
  //Eje Y
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 100, 0);
  glEnd();
  
  //Eje Z
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 100);
  glEnd();

  glEnable(GL_LIGHTING);
}

void render_plane(PLANE *plane) {
  int i = 0;
  int j = 0;
  
  set_material(plane->material);
  
  for (i = 0; i != SUBDIVISION_STEPS; i++) {
    for (j = 0; j != SUBDIVISION_STEPS; j++) {
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, 0.0f );
      glVertex3f(plane->inter_points[i][j].x, 0.0, plane->inter_points[i][j].z);
      glVertex3f(plane->inter_points[i+1][j].x, 0.0, plane->inter_points[i+1][j].z);
      glVertex3f(plane->inter_points[i+1][j+1].x, 0.0, plane->inter_points[i+1][j+1].z);
      glVertex3f(plane->inter_points[i][j+1].x, 0.0, plane->inter_points[i][j+1].z);
      glEnd();
    }
  }
}

void render_control_points(SURFACE *surface) {
  if (!edit_mode)
    return;

  int i = 0, j = 0;

  // dibujo los puntos de control como esferitas
  glDisable(GL_LIGHTING);
 
  for (i=0;i<11;i++) {
    for (j=0;j<11;j++) {
      POINT3D punto = *(surface->ptscontrol+i*11+j);
      glPushMatrix();
      glTranslatef(punto.x,
		   punto.y +2.0,
		   punto.z);
      if (selected == 11*i + j)
	glColor3f(1,0.5,0.5);
      else
	glColor3f(0.5,0.5,0);

      glutSolidSphere(0.1, 10, 10);
      glPopMatrix();
      
      if (selected != -1) {
	glColor3f(0,0,0.8);
	// linea guia de ayuda
	glBegin(GL_LINES);
	glVertex3f(punto.x, 0, punto.z);
	glVertex3f(punto.x, punto.y, punto.z);
	glEnd();
      }
    }
  }
  glEnable(GL_LIGHTING);
}

void render_surface(SURFACE *surface) {
  int i,j;
  // dibujo los parches de bezier
  glPushMatrix();
  glScalef(1.0,1.0,1.0);
  set_material(plane->material);
  for (i=0;i<8;i++) {
    for (j=0;j<8;j++) {
      GLfloat parche[4][4][3];
      bspline2bezier(surface,i,j,parche);
      glMap2f(GL_MAP2_VERTEX_3,
	      0.0,1.0,3,4,
	      0.0,1.0,12,4,
	      &parche[0][0][0]);
      glEnable(GL_MAP2_VERTEX_3);
      glEnable(GL_AUTO_NORMAL);
      glEnable(GL_NORMALIZE);
      glMapGrid2f(resolucion,0.0,1.0,
		  resolucion,0.0,1.0);
      // si estoy seleccionando entonces dibujo modo wireframe
      if (selected != -1 || wire_mode)
	glEvalMesh2(GL_LINE,0,resolucion,0,resolucion);
      else
	glEvalMesh2(GL_FILL,0,resolucion,0,resolucion);
    }
  }
  glPopMatrix();

  // pintar normal para ver que tal 
  POINT3D mipunto, minormal;
  calculateNormalNotRotated(surface,ball->position.u,ball->position.v,&mipunto,&minormal);
  glBegin(GL_LINES); 
  set_material(plane->material); 
  glVertex3f(mipunto.x,mipunto.y,mipunto.z); 
  glVertex3f(mipunto.x+5*minormal.x,mipunto.y+5*minormal.y,mipunto.z+5*minormal.z);  
  glEnd(); 

}

void render_ball(BALL *ball, SURFACE *surface) {

  glPushMatrix();
  

  POINT3D point, normal;
  calculateNormalNotRotated(surface,ball->position.u,ball->position.v,&point,&normal);
  glTranslatef(point.x + normal.x * ball->radius, 
	       point.y + normal.y * ball->radius, 
	       point.z + normal.z * ball->radius);
  
  set_material(ball->material);
  
  glutSolidSphere(ball->radius, 20, 20);
  glPopMatrix();

}

void render_text() {
  glDisable(GL_LIGHTING);
  POINT3D *point = (POINT3D *)malloc(sizeof(POINT3D));
  calculate_plane_point3d(plane, ball->position, point);

  drawstr(10, 50, 0.12, "Ball position");
  drawstr(10, 65, 0.12, "u: %f v: %f", ball->position.u, ball->position.v);

#define M_PI		3.14159265358979323846
  drawstr(10, 85, 0.12, "Ball velocity");
  drawstr(10, 100, 0.12, "u: %f v: %f", ball->velocity.u*180/M_PI, ball->velocity.v*180/M_PI);

  drawstr(10, 120, 0.12, "Ball position (world coordinates)");
  if (ball->movement_type == BALL_ON_SURFACE) {
    drawstr(10, 135, 0.12, "x: %f y: %f z: %f", point->x, point->y, point->z);
  } else {
    drawstr(10, 135, 0.12, "x: %f y: %f z: %f", ball->outside_position.x, ball->outside_position.y, ball->outside_position.z);
  }
  
  drawstr(10, 155, 0.12, "Plane inclination");
  drawstr(10, 170, 0.12, "alphax: %f alphaz: %f", surface->alphax, surface->alphaz);

  drawstr(10, 190, 0.12, "Normal vector @ ball position");
  drawstr(10, 205, 0.12, "x: %f y: %f z: %f", normal_vector.x, normal_vector.y, normal_vector.z);

  if (edit_mode)
    drawstr(10, 525, 0.12, "Edit mode: move the control points");
  else
    drawstr(10, 525, 0.12, "Game mode: have fun!");
  if (wire_mode)
    drawstr(10, 540, 0.1, "Wireframe mode");
  if (cam_mode)
    drawstr(10, 555, 0.1, "Free XY mouse cam mode: horizontal moves X coordinate, vertical moves Y coordinate");
  if (camz_mode)
    drawstr(10, 570, 0.1, "Free Z mouse cam mode: vertical moves Z coordinate");


  free(point);
  glEnable(GL_LIGHTING);
}

/**
 * dibuja un string. Tomado de shapes.c
 */
void drawstr(GLuint x, GLuint y, GLdouble scale, char* format, ...)
{
  va_list args;
  char buffer[255], *s;

  glPushMatrix();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, screen_width, screen_height, 0.0);

  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();
  glTranslatef(x,y+2,0);
  glScalef(scale,scale,scale);
  glRotatef(180,1,0,0);
  glColor3f(1.0, 1.0, 1.0);

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
    
  for (s = buffer; *s; s++)
    glutStrokeCharacter(font_style, *s);

  glPopMatrix();
}

/**
 * dibuja un string. Tomado de shapes.c
 */
void drawstr2(GLfloat x, GLfloat y, GLfloat z, GLdouble scale, char* format, ...)
{
  va_list args;
  char buffer[255], *s;

  glPushMatrix();

  glTranslatef(x,y,z);
  glScalef(scale,scale,scale);
  glRotatef(180,1,0,0);
  glColor3f(1.0, 1.0, 1.0);

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
    
  for (s = buffer; *s; s++)
    glutStrokeCharacter(font_style, *s);

  glPopMatrix();
}

void set_material(MATERIAL *material) {
  glMaterialfv (GL_FRONT, GL_AMBIENT, material->ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, material->diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, material->specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, material->shininess);
}


/**
 * base de este codigo tomado de la pagina que 
 * hace picking con hombres de nieve
 */
void pick_points(int x, int y) {
  int i,j;
  int cursorX = x;
  int cursorY = y;
  GLint hits;

  // inicializo el picking
  GLint viewport[4];
  float ratio;

  GLuint selectBuf[BUFSIZE];
  glSelectBuffer(BUFSIZE,selectBuf);

  glGetIntegerv(GL_VIEWPORT,viewport);

  glRenderMode(GL_SELECT);

  glInitNames();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  gluPickMatrix(cursorX,viewport[3]-cursorY,1,1,viewport);
  ratio = (viewport[2]+0.0) / viewport[3];
  gluPerspective(camera->aperture,ratio,0.1,1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(camera->position.x, camera->position.y, camera->position.z,
	    camera->center.x,   camera->center.y,   camera->center.z,
	    camera->up.x,       camera->up.y,       camera->up.z);
  glRotatef(surface->alphax,1.0,0.0,0.0);
  glRotatef(surface->alphaz,0.0,0.0,1.0);
  for (i=0;i<11;i++) {
    for (j=0;j<11;j++) {
      POINT3D punto = *(surface->ptscontrol+i*11+j);
      glPushMatrix();
      glPushName(11*i + j);
      glTranslatef(punto.x,
		   punto.y+2.0,
		   punto.z);
      glRectf(-0.25f,0.25f,0.25f,-0.25f);
      glRotatef(90,1,0,0);
      glRectf(-0.25f,0.25f,0.25f,-0.25f);
      glRotatef(90,0,1,0);
      glRectf(-0.25f,0.25f,0.25f,-0.25f);
      glPopName();
      glPopMatrix();
    }
  }

  // termino de pickear y testeo
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glFlush();
  hits = glRenderMode(GL_RENDER);

  if (hits != 0){
    processHits(hits,selectBuf,0);
  }
}

/**
 * codigo tomado de la pagina que hace picking con
 * hombres de nieve
 */
void processHits (GLint hits, GLuint buffer[], int sw)
{
  unsigned int i, j;
  GLuint names, *ptr;

  ptr = (GLuint *) buffer;
  for (i = 0; i < hits; i++) {  /* for each hit  */
    names = *ptr;
     ptr++;
     ptr++;
     ptr++;
    for (j = 0; j < names; j++) {  /* for each name */
      selected = *ptr;
      ptr++;
    }
  }

}
