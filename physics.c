#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mathutil.h"
#include "physics.h"

#define U_PLANE 1
#define V_PLANE 2

extern float sin_values[361];
extern float cos_values[361];

POINT3D normal_vector = { 0, 0, 0 };

void calculate_distance(BALL *ball, SURFACE *surface, float resp[2]) {
  POINT3D *normal = (POINT3D *)malloc(sizeof(POINT3D));
  POINT3D *punto = (POINT3D *)malloc(sizeof(POINT3D));

  calculateNormal(surface,ball->position.u,ball->position.v,punto,normal);
  
  normal_vector.x = normal->x;
  normal_vector.y = normal->y;
  normal_vector.z = normal->z;

  //Calculando la distancia en U
  resp[0] = calculate_distance_plane(ball, normal, U_PLANE);
  resp[0] = ball->radius * asin(resp[0]/ball->radius);

  //Calculando la distancia en V
  resp[1] = calculate_distance_plane(ball, normal, V_PLANE);
  resp[1] = ball->radius * asin(resp[1]/ball->radius);

  check_on_surface(ball, surface, resp);
  
  free(normal);
  free(punto);

  convert_distance(resp, ball, surface);
}

float calculate_distance_plane(BALL *ball, POINT3D *normal, int plane) {
  float theta = 0.0f;  //Inclinacion de la normal con respecto al eje y
  float alpha = 0.0f;  //Aceleracion angular
  float phi = 0.0f;    //Angulo recorrido en un tick de tiempo 
                       //y respuesta de la funcion
  
  float F = 0.0f;

  float normaly = normal->y;
  float normalx = (plane == U_PLANE? normal->z: normal->x);
  
  if (plane != U_PLANE && plane != V_PLANE) {
    perror("calculate_distance_plane: Usando un plano no valido.");
    exit(-1);
  }

  if (normaly == 0) {
    theta = 90.0f;
  } else if (normalx == 0) {
    theta = 0.0f;
  } else {
    theta = (float)atan2(normalx, normaly);
    theta = (float)(theta*180/M_PI);
  }

  
  if (theta != 0) {
    F = GRAVITY * 2 * SIN(theta);
    

    alpha = F / ball->radius;
    
    if (plane == U_PLANE) {
      phi = (ball->velocity.u * TICK) + (alpha * TICK * TICK)/2;
      ball->velocity.u += alpha * TICK;
    } else {
      phi = (ball->velocity.v * TICK) + (alpha * TICK * TICK)/2;
      ball->velocity.v += alpha * TICK;
    }
  } else {
    //No hay pendiente, asi que la pelota va con velocidad constante
    if (plane == U_PLANE) {
      phi = (ball->velocity.u * TICK);
    } else {
      phi = (ball->velocity.v * TICK);
    }
  }
  return phi;
}

void check_on_surface(BALL *ball, SURFACE *surface, float distance[2]) {
  if (ball->position.u + distance[0] < 0 || 
      ball->position.u + distance[0] > 1) {
    ball->velocity.u = 0;
    ball->position.u = (ball->position.u < 0.5? 0.001: 1);
    distance[0] = 0;

  }
  if (ball->position.v + distance[1] < 0 ||
      ball->position.v + distance[1]> 1) {
    
    ball->velocity.v = 0;
    ball->position.v = (ball->position.v < 0.5? 0.001: 1);

    distance[1] = 0;
  }
}

void convert_distance(GLfloat distance[2], BALL *ball, SURFACE *surface) {

  POINT3D p1 = calculateSpline(surface, 
			       ball->position.u, 
			       ball->position.v);
  POINT3D p2 = calculateSpline(surface, 
			       ball->position.u+distance[0],
			       ball->position.v+distance[1]);

  ANGLE angle;
  COORDINATE normalx;
  COORDINATE normaly;

  //Calculando en base al plano U (ZY)
  normalx = fabsf(p2.z - p1.z);
  normaly = fabsf(p2.y - p1.y);
  

  angle = (float)atan2(normaly, normalx);

  angle = (float)(angle*180/M_PI);


  distance[0] = distance[0] * COS(angle);
  
  //Calculando en base al plano V (XY)
  normalx = fabsf(p2.x - p1.x);
  normaly = fabsf(p2.y - p1.y);

  angle = (float)atan2(normaly, normalx);
  angle = (float)(angle*180/M_PI);

  distance[1] = distance[1] * COS(angle);
}

void calculate_falling(BALL *ball, float distance[1]) {
  ball->vertical_velocity = GRAVITY *0.2f;
  distance[0] = ball->vertical_velocity * 0.2f + ((GRAVITY * 0.04f)/2);
}
