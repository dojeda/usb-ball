/**
 * data.c
 *
 * Implementa las funciones prototipadas en data.h. Para mas informacion
 * acerca de las estructuras de datos ver inicio de data.h.
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
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "mathutil.h"

POINT3D *create_point3d(COORDINATE x, COORDINATE y, COORDINATE z) {
  POINT3D *point = (POINT3D *)malloc(sizeof(POINT3D));

  point->x = x;
  point->y = y;
  point->z = z;

  return point;
}

void destroy_point3d(POINT3D *point) {
	free(point);
}

POINTPARAM *create_pointparam(COORDINATE u, COORDINATE v) {
	POINTPARAM *point = (POINTPARAM *)malloc(sizeof(POINTPARAM));

	point->u = u;
	point->v = v;

	return point;
}

void destroy_pointparam(POINTPARAM *point) {
	free(point);
}

PLANE *create_plane(COORDINATE x1, COORDINATE z1,
		    COORDINATE x2, COORDINATE z2,
		    COORDINATE x3, COORDINATE z3,
		    COORDINATE x4, COORDINATE z4,
		    MATERIAL *material) {
	PLANE *plane = (PLANE *)malloc(sizeof(PLANE));
	int i = 0;
	int j = 0;
	GLfloat step = 0.0f;

	plane->point[0].x = x1;
	plane->point[0].y = 0;
	plane->point[0].z = z1;

	plane->point[1].x = x2;
	plane->point[0].y = 0;
	plane->point[1].z = z2;

	plane->point[2].x = x3;
	plane->point[0].y = 0;
	plane->point[2].z = z3;

	plane->point[3].x = x4;
	plane->point[0].y = 0;
	plane->point[3].z = z4;

	plane->alphax = 0;
	plane->alphaz = 0;

	plane->material = material;
	
	step = (x1-x2)/SUBDIVISION_STEPS;

	for (j = 0; j != SUBDIVISION_STEPS+1; j++) {
	  for (i = 0; i != SUBDIVISION_STEPS+1; i++) {
	    plane->inter_points[j][i].y = 0;
	    plane->inter_points[j][i].x = x2 + (step*i);
	    plane->inter_points[j][i].z = x2 + (step*j);
	  }
	}
	return plane;
}

void destroy_plane(PLANE *plane) {
  free(plane);
}

void calculate_plane_point3d(PLANE *plane, POINTPARAM param, POINT3D *point) {
  point->y = 0;
  point->x = (plane->point[0].x - plane->point[1].x)*param.u - plane->point[0].x;
  point->z = (plane->point[0].z - plane->point[3].z)*param.v - plane->point[0].z;

  //printf("point->x: %f\npoint->z: %f\n", point->x, point->z);
}

//Falta implementar create_surface y destroy_surface

BALL *create_ball(POINTPARAM startPos, RADIUS radius, MATERIAL *material) {
  BALL *ball = (BALL *)malloc(sizeof(BALL));

  ball->position.u = startPos.u;
  ball->position.v = startPos.v;

  ball->velocity.u = 0;
  ball->velocity.v = 0;
  
  ball->radius = radius;
  
  ball->material = material;
  
  ball->movement_type = BALL_ON_SURFACE;
  ball->outside_position.x = 0;
  ball->outside_position.y = 0;
  ball->outside_position.z = 0;
  ball->vertical_velocity = 0;

  return ball;
}

void destroy_ball(BALL *ball) {
  free(ball);
}

CAMERA *create_camera(POINT3D position, POINT3D center, POINT3D up, COORDINATE aperture) {
  CAMERA *camera = (CAMERA *)malloc(sizeof(CAMERA));
  
  camera->position.x = position.x;
  camera->position.y = position.y;
  camera->position.z = position.z;

  camera->center.x = center.x;
  camera->center.y = center.y;
  camera->center.z = center.z;

  camera->up.x = up.x;
  camera->up.y = up.y;
  camera->up.z = up.z;

  camera->aperture = aperture;

  return camera;
}

void destroy_camera(CAMERA *camera) {
  free(camera);
}

LIGHT_POINT *create_light_point(GLfloat position[], 
                                GLfloat ambient[],
                                GLfloat diffuse[],
                                GLfloat specular[]) {
	LIGHT_POINT *point = (LIGHT_POINT *)malloc(sizeof(LIGHT_POINT));
	
	point->position[0] = position[0];
	point->position[1] = position[1];
	point->position[2] = position[2];
	point->position[3] = position[3];
	
	point->ambient[0] = ambient[0];
	point->ambient[1] = ambient[1];
	point->ambient[2] = ambient[2];
	point->ambient[3] = ambient[3];
	
	point->diffuse[0] = diffuse[0];
	point->diffuse[1] = diffuse[1];
	point->diffuse[2] = diffuse[2];
	point->diffuse[3] = diffuse[3];
	
	point->specular[0] = specular[0];
	point->specular[1] = specular[1];
	point->specular[2] = specular[2];
	point->specular[3] = specular[3];
	
	return point;
}

void destroy_light_point(LIGHT_POINT *light_point) {
	free(light_point);
}

MATERIAL *create_material(GLfloat ambient[],
                          GLfloat diffuse[],
                          GLfloat specular[],
                          GLfloat shininess[]) {
	MATERIAL *material = (MATERIAL *)malloc(sizeof(MATERIAL));
	
	material->ambient[0] = ambient[0];
	material->ambient[1] = ambient[1];
	material->ambient[2] = ambient[2];
	material->ambient[3] = ambient[3];
	
	material->diffuse[0] = diffuse[0];
	material->diffuse[1] = diffuse[1];
	material->diffuse[2] = diffuse[2];
	material->diffuse[3] = diffuse[3];
	
	material->specular[0] = specular[0];
	material->specular[1] = specular[1];
	material->specular[2] = specular[2];
	material->specular[3] = specular[3];
	
	material->shininess[0] = shininess[0];
	
	return material;
}
void destroy_material(MATERIAL *material) {
	free(material);
}

SURFACE *create_surface(POINT3D *ptscontrol) {
  SURFACE *surface = (SURFACE*) malloc(sizeof(SURFACE));
  surface->ptscontrol = ptscontrol;
  surface->alphax = 0;
  surface->alphaz = 0;
  return surface;
}

void destroy_surface(SURFACE *surface) {
  destroy_control_points(surface->ptscontrol);
  free(surface);
}

POINT3D *create_control_points() {
  int i, j;
  POINT3D *puntos = (POINT3D*) malloc(11*11 * sizeof(POINT3D));
  COORDINATE x = -5;
  COORDINATE y =  0;
  COORDINATE z = -5;
  for (i=0; i<11; i++) {
    for (j=0; j<11; j++) {
      (puntos+11*i+j)->x = x;
      (puntos+11*i+j)->y = y;
      (puntos+11*i+j)->z = z;
      x += 1;
    }
    z += 1;
    x = -5;
  }
  return puntos;
}

void destroy_control_points(POINT3D *puntos) {
  free(puntos);
}
