/**
 * resources.c
 *
 * Implementa las funciones prototipadas en resources.h. Para mas informacion
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
 
#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "resources.h"
#include "physics.h"

/** VARIABLES GLOBALES **/
PLANE   *plane;
SURFACE *surface;
BALL    *ball;
CAMERA  *camera;

LIGHT_POINT *light_point;
MATERIAL *ball_mat;
MATERIAL *plane_mat;

//Propiedades del punto de luz
GLfloat light_position[] = { 10.0f, 10.0f, 10.0f, 0.0f };
GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//Propiedades del material de la pelota
GLfloat ball_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat ball_diffuse[] = { 0.2f, 0.2f, 1.0f, 1.0f };
GLfloat ball_specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat ball_shininess[] = { 25.0f };

//Propiedades del material del plano
GLfloat plane_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat plane_diffuse[] = { 0.8f, 0.1f, 0.1f, 1.0f };
GLfloat plane_specular[] = { 1.0f, 1.0f, 1.0f, 0.5f };
GLfloat plane_shininess[] = { 90.0f };

void init_resources() {
  POINTPARAM ballStartPos;


  //Creando la camara
  POINT3D *position = create_point3d( 15.0, 15.0, 15.0);
  POINT3D *center   = create_point3d( 0.0, 0.0,  0.0);
  POINT3D *up       = create_point3d( 0.0, 1.0,  0.0);
  camera = create_camera(*position, *center, *up, 45.0f);

  free(position);
  free(center);
  free(up);

  //Creando los materiales
  ball_mat = create_material(ball_ambient,
			     ball_diffuse,
			     ball_specular,
			     ball_shininess);
	
  plane_mat = create_material(plane_ambient,
			      plane_diffuse,
			      plane_specular,
			      plane_shininess);
	
  //Creando el punto de luz
  light_point = create_light_point(light_position,
				   light_ambient,
				   light_diffuse,
				   light_specular);
	
  //Creando los objetos
  plane = create_plane(5.0f, 5.0f,
		       -5.0f, 5.0f,
		       -5.0f, -5.0f,
		       5.0f, -5.0f,
		       plane_mat);
	
  ballStartPos.u = 0.5f;
  ballStartPos.v = 0.5f;
	
  ball = create_ball(ballStartPos, 0.33f, ball_mat);

  // creo la superficie
  POINT3D *ptscontrol = create_control_points();
  surface = create_surface(ptscontrol);
}
/*
void destroy_resources() {
	
}
*/

void ball_update() {
  

  if (ball->movement_type == BALL_ON_SURFACE) {
    float distance[2] = { 0,0 };
    calculate_distance(ball, surface, distance);

    ball->position.u += distance[0];
    ball->position.v += distance[1];
  } else {
    float distance[1] = { 0 };
    calculate_falling(ball, distance);

    ball->outside_position.x += ball->velocity.u;
    ball->outside_position.z += ball->velocity.v;
    ball->outside_position.y += distance[0];
  }

}
