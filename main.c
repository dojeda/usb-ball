/**
 * main.c
 *
 * Contiene la rutina de entrada al programa de la pelota. Pelota es un
 * demo/juego que muestra las capacidades de una computadora para simular
 * la física aplicada a una pelota rodando por una superficie.
 *
 * Ejecuta las rutinas de inicializacion de la ventana grafica, del area de
 * dibujo de OpenGL, y los recursos del juego.
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

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "renderer.h"
#include "resources.h"
#include "main.h"
#include "mathutil.h"
#include "physics.h"

int screen_height = 600;
int screen_width  = 800;

int timer_delay = 100;

int ventana;

int mousex, mousey;

int selected = -1;
int resolucion = 7;
int rotate_mode = 0;
int edit_mode = 0;
int wire_mode = 0;
int cam_mode  = 0;
int camz_mode  = 0;

extern CAMERA *camera;
extern PLANE *plane;
extern SURFACE *surface;

int main(int argc, char *argv[]) {
	
  init_resources();
  init_math();
  init_window(argc, argv);
  init_renderer();
	
  glutTimerFunc(timer_delay, update, 1);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutKeyboardFunc(func_keyboard);
  glutSpecialFunc(func_arrows);
  glutMouseFunc(func_mouse);
  glutMotionFunc(func_motion);
  glutPassiveMotionFunc(func_passive_motion);
	
  glutMainLoop();
	
  return 0;
}

void init_window(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowSize(screen_width, screen_height);
  glutInitWindowPosition(50, 50);

  ventana = glutCreateWindow("Pelota. Brought to you by Ojeda & Duran Games");
}

void func_keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'a':
  case 'A':
    camera->position.x += 1.0;
    break;
  case 'z':
  case 'Z':
    camera->position.x -= 1.0;
    break;
  case 's':
  case 'S':
    camera->position.y += 1.0;
    break;
  case 'x':
  case 'X':
    camera->position.y -= 1.0;
    break;
  case 'd':
  case 'D':
    camera->position.z += 1.0;
    break;
  case 'c':
  case 'C':
    camera->position.z -= 1.0;
    break;
  case 'q':
  case 'Q':
    if (camera->aperture < 170)
      camera->aperture += 1.0;
    break;
  case 'w':
  case 'W':
    if (camera->aperture > 1)
      camera->aperture -= 1.0;
    break;
  case 43:
    if (resolucion < 20)
      resolucion++;
    break;
  case 45:
    if (resolucion > 1)
      resolucion--;
    break;
  case 'r':
  case 'R':
    if (edit_mode)
      resetPoints();
    break;
  case 32:
    if (edit_mode)
      randomizePoints();
    break;
  case 27:
    exit(0);
    break;
  }
  resize(screen_width,screen_height);
}

void func_arrows(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_UP:
    if (!edit_mode && surface->alphax > -90)
      surface->alphax--;
    break;
  case GLUT_KEY_DOWN:
    if (!edit_mode && surface->alphax < 90)
      surface->alphax++;
    break;
  case GLUT_KEY_LEFT:
    if (!edit_mode && surface->alphaz < 90)
      surface->alphaz++;
    break;
  case GLUT_KEY_RIGHT:
    if (!edit_mode && surface->alphaz > -90)
      surface->alphaz--;
    break;
  case GLUT_KEY_F1:
    edit_mode ^= 1;
    break;
  case GLUT_KEY_F2:
    wire_mode ^= 1;
    break;
  }
}

void func_mouse(int button, int state, int x, int y) {

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    if (edit_mode)
      pick_points(x,y);
    else
      rotate_mode = 1;
  } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    selected = -1;
    rotate_mode = 0;
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    cam_mode = 1;
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
    cam_mode = 0;
  } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    camz_mode = 1;
  } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
    camz_mode = 0;
  } else if (glutDeviceGet(GLUT_NUM_MOUSE_BUTTONS) > 2 &&
	     button == 3 && state == GLUT_DOWN) {
    if (camera->aperture < 170)
      camera->aperture += 1.0;
  } else if (glutDeviceGet(GLUT_NUM_MOUSE_BUTTONS) > 2 &&
	     button == 4 && state == GLUT_DOWN) {
    if (camera->aperture > 1)
      camera->aperture -= 1.0;
  }

}

void func_motion(int x, int y) {
  if (selected != -1) {
    (surface->ptscontrol+selected)->y += (float)(mousey-y)/10.0;
  }
  if (cam_mode) {
    camera->position.x += (float)(mousex-x)/10.0;
    camera->position.y += (float)(mousey-y)/10.0;
  }
  if (camz_mode) {
    camera->position.z += (float)(mousey-y)/10.0;
  }
  if (rotate_mode) {
    if (surface->alphax - (float)(mousey-y)/5.0 < 90 &&
	surface->alphax - (float)(mousey-y)/5.0 > -90)
      surface->alphax -= (float)(mousey-y)/5.0;
    if (surface->alphaz + (float)(mousex-x)/5.0 < 90 &&
	surface->alphaz + (float)(mousex-x)/5.0 > -90)
      surface->alphaz += (float)(mousex-x)/5.0;
  }
  

  mousex = x;
  mousey = y;
}

void func_passive_motion(int x, int y) {
  mousex = x;
  mousey = y;
}

void update(int value) {
  if (!edit_mode)
    ball_update();

  glutPostRedisplay();
  glutTimerFunc(timer_delay, update, value);
}

void randomizePoints() {
  int i, j;

  for (i = 0; i != 11; i++) {
    for (j = 0; j != 11; j++) {
      POINT3D *p = surface->ptscontrol+i*11+j;
      int numero = random();
      p->y = (float)((numero - (RAND_MAX>>2))*10) / RAND_MAX; 
    }
  }
}

void resetPoints() {
  int i, j;

  for (i = 0; i != 11; i++) {
    for (j = 0; j != 11; j++) {
      POINT3D *p = surface->ptscontrol+i*11+j;
      p->y = 0;
    }
  }
}
