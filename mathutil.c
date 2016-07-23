#include <stdio.h>
#include "mathutil.h"

#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962

GLfloat m_bspline_bezier[4][4] = {
  {1.0/6.0, 4.0/6.0, 1.0/6.0, 0.0    },
  {    0.0, 4.0/6.0, 2.0/6.0, 0.0    },
  {    0.0, 2.0/6.0, 4.0/6.0, 0.0    },
  {    0.0, 1.0/6.0, 4.0/6.0, 1.0/6.0}
};

GLfloat m_bspline_bezierT[4][4] = {
  {1.0/6.0,     0.0,     0.0, 0.0    },
  {4.0/6.0, 4.0/6.0, 2.0/6.0, 1.0/6.0},
  {1.0/6.0, 2.0/6.0, 4.0/6.0, 4.0/6.0},
  {    0.0,     0.0,     0.0, 1.0/6.0}
};

GLfloat m_bspline[4][4] = {
  {-1.0/6.0, 3.0/6.0,-3.0/6.0, 1.0/6.0},
  { 3.0/6.0,-6.0/6.0, 3.0/6.0, 0.0    },
  {-3.0/6.0, 0.0    , 3.0/6.0, 0.0    },
  { 1.0/6.0, 4.0/6.0, 1.0/6.0, 0.0    }
};

GLfloat m_bsplineT[4][4] = {
  {-1.0/6.0, 3.0/6.0,-3.0/6.0, 1.0/6.0},
  { 3.0/6.0,-6.0/6.0, 0.0    , 4.0/6.0},
  {-3.0/6.0, 3.0/6.0, 3.0/6.0, 1.0/6.0},
  { 1.0/6.0, 0.0    , 0.0    , 0.0    } 
};

float sin_values[360];
float cos_values[360];

void init_math() {
  int i = 0;

  for (i = 0; i != 360; i++) {
    sin_values[i] = (float)sin(i*M_PI/180);
    cos_values[i] = (float)cos(i*M_PI/180);
  }
}

void cross_product(GLfloat A[4][4], GLfloat B[4][4], GLfloat AxB[4][4]) {
  int i,j;
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      AxB[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + A[i][2]*B[2][j] + A[i][3]*B[3][j];
    }
  }
}

void rotate_vector(GLfloat vector[3], GLfloat x, GLfloat y, GLfloat z, GLfloat result[3]) {
  GLfloat temporal1[4] = {vector[0], vector[1], vector[2], 1};
  GLfloat temporal2[4] = {0,0,0,0};
  GLfloat X[4][4] = { { 1,      0,       0, 0},
		      { 0, COS(x),  SIN(x), 0},
		      { 0, -SIN(x),  COS(x), 0},
		      { 0,      0,       0, 1}};

  GLfloat Y[4][4] = { {  COS(y), 0, -SIN(y), 0},
		      {       0, 1,      0, 0},
		      { SIN(y), 0, COS(y), 0},
		      {       0, 0,      0, 1}};
  
  GLfloat Z[4][4] = { { COS(z), SIN(z), 0, 0},
		      { -SIN(z),  COS(z), 0, 0},
		      {      0,       0, 1, 0},
		      {      0,       0, 0, 1}};

  multiply_vector(temporal1, X, temporal2); 
  multiply_vector(temporal2, Y, temporal1);
  multiply_vector(temporal1, Z, temporal2);

  result[0] = temporal2[0];
  result[1] = temporal2[1];
  result[2] = temporal2[2];
}

void multiply_vector(GLfloat *vector, GLfloat mat[4][4], GLfloat *result) {
  int i = 0;
  int j = 0;

  for (i = 0; i != 4; i++) {
    *(result+i) = 0.0f;
    for (j = 0; j != 4; j++) {
      *(result+i) += *(vector+j) * mat[j][i];
    }
  }
  
}

void vector_cross_matrix(GLfloat V[4], GLfloat A[4][4], GLfloat VxA[4]) {
  int i;
  for (i=0;i<4;i++) {
    VxA[i] = V[0] * A[0][i]+
             V[1] * A[1][i]+
             V[2] * A[2][i]+
             V[3] * A[3][i];
  }
}

void vector3d_cross_product(GLfloat A[3], GLfloat B[3], GLfloat AxB[3]) {
  AxB[0] = A[1]*B[2] - A[2]*B[1];
  AxB[1] = A[2]*B[0] - A[0]*B[2];
  AxB[2] = A[0]*B[1] - A[1]*B[0];
}

GLfloat vector_product(GLfloat A[4], GLfloat B[4]) {
  GLfloat product = 0;
  int i;
  for (i=0;i<4;i++) {
    product += A[i]*B[i];
  }
  return product;
}

void normalize_vector3d(GLfloat V[3]) {
  GLfloat modulo = sqrtf(V[0]*V[0] +
			 V[1]*V[1] +
			 V[2]*V[2]);
  V[0] = V[0] / modulo;
  V[1] = V[1] / modulo;
  V[2] = V[2] / modulo;
}

POINT3D calculateSpline(SURFACE *surface,float u, float v) {
  POINT3D punto;
  int i;
  int centeru, centerv;
  GLfloat bsplinex[4][4],
          bspliney[4][4],
          bsplinez[4][4],
          temp[4][4],
          vectoru[4],
          vectorv[4],
          tempvector[4],
          uchica,
          vchica;

  // en cada coordenada hay 8 splines. tengo que ver a cual
  // escogo para dibujar
  if (u < 0) {
    centeru = 0;
    uchica = 0;
  } else if (u > 1) {
    centeru = 7;
    uchica = 1;
  } else {
    centeru = (int)(10*(u*0.8));
    uchica = ((u*0.8 + 0.1) - (centeru+1)*0.1)/0.1;
  }

  if (v < 0) {
    centerv = 0;
    vchica = 0;
  } else if (v > 1) {
    centerv = 7;
    vchica = 1;
  } else {
    centerv = (int)(10*(v*0.8));
    vchica = ((v*0.8 + 0.1) - (centerv+1)*0.1)/0.1;
  }

  // creo los vectores de u y v
  for (i=0;i<4;i++) {
    vectoru[i] = powf(uchica,3-i);
    vectorv[i] = powf(vchica,3-i);    
  }
  // creo las matrices de los puntos
  bsplinepoints(surface,centeru,centerv,bsplinex,bspliney,bsplinez);
  // multiplico las matrices y puntos para cada coordenada
  // puntos en X
  cross_product(m_bspline,bsplinex,temp);
  cross_product(temp,m_bsplineT,bsplinex);
  // puntos en Y
  cross_product(m_bspline,bspliney,temp);
  cross_product(temp,m_bsplineT,bspliney);
  // puntos en Z
  cross_product(m_bspline,bsplinez,temp);
  cross_product(temp,m_bsplineT,bsplinez);
  // multiplico el vector u y el v por las matrices
  // coordenada X
  vector_cross_matrix(vectoru,bsplinex,tempvector);
  punto.x = vector_product(tempvector,vectorv);
  // coordenada Y
  vector_cross_matrix(vectoru,bspliney,tempvector);
  punto.y = vector_product(tempvector,vectorv);
  // coordenada Z
  vector_cross_matrix(vectoru,bsplinez,tempvector);
  punto.z = vector_product(tempvector,vectorv);
  
  return punto;
}

void calculateNormal(SURFACE *surface, float u, float v, POINT3D *punto, POINT3D *normal) {
  POINT3D p  = calculateSpline(surface,u,v),
          p1 = calculateSpline(surface,u+EPSILON,v),
          p2 = calculateSpline(surface,u,        v-EPSILON),
          p3 = calculateSpline(surface,u-EPSILON,v-EPSILON);
  GLfloat vector1[3], vector2[3], vectorn[3], vectorrot[3];
  vector1[0] = p1.x - p2.x;   vector2[0] = p3.x - p2.x;
  vector1[1] = p1.y - p2.y;   vector2[1] = p3.y - p2.y;
  vector1[2] = p1.z - p2.z;   vector2[2] = p3.z - p2.z;
  normalize_vector3d(vector1);
  normalize_vector3d(vector2);
  vector3d_cross_product(vector1,vector2,vectorn);
  normalize_vector3d(vectorn);

  rotate_vector(vectorn, surface->alphax,0.0f, surface->alphaz, vectorrot);

  punto->x = p.x;
  punto->y = p.y;
  punto->z = p.z;

  normal->x = vectorrot[0];
  normal->y = vectorrot[1];
  normal->z = vectorrot[2];
}

void calculateNormalNotRotated(SURFACE *surface, float u, float v, POINT3D *punto, POINT3D *normal) {
  POINT3D p  = calculateSpline(surface,u,v),
          p1 = calculateSpline(surface,u+EPSILON,v),
          p2 = calculateSpline(surface,u,        v-EPSILON),
          p3 = calculateSpline(surface,u-EPSILON,v-EPSILON);
  GLfloat vector1[3], vector2[3], vectorn[3];
  vector1[0] = p1.x - p2.x;   vector2[0] = p3.x - p2.x;
  vector1[1] = p1.y - p2.y;   vector2[1] = p3.y - p2.y;
  vector1[2] = p1.z - p2.z;   vector2[2] = p3.z - p2.z;
  normalize_vector3d(vector1);
  normalize_vector3d(vector2);
  vector3d_cross_product(vector1,vector2,vectorn);
  normalize_vector3d(vectorn);

  punto->x = p.x;
  punto->y = p.y;
  punto->z = p.z;

  normal->x = vectorn[0];
  normal->y = vectorn[1];
  normal->z = vectorn[2];
}

void bspline2bezier(SURFACE *surface, int u, int v, GLfloat puntos[4][4][3]) {
  GLfloat bsplinex[4][4],
          bspliney[4][4],
          bsplinez[4][4],
          bezierx[4][4],
          beziery[4][4],
          bezierz[4][4],
          temp[4][4];
  int i,j;
  // obtener las 3 matrices de X,Y,Z
  bsplinepoints(surface,u,v,bsplinex,bspliney,bsplinez); 
  // puntos en X
  cross_product(m_bspline_bezier,bsplinex,temp);
  cross_product(temp,m_bspline_bezierT,bezierx);
  // puntos en Y
  cross_product(m_bspline_bezier,bspliney,temp);
  cross_product(temp,m_bspline_bezierT,beziery);
  // puntos en Z
  cross_product(m_bspline_bezier,bsplinez,temp);
  cross_product(temp,m_bspline_bezierT,bezierz);

  // obtener los puntos de las 3 matrices X,Y,Z
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      puntos[j][i][0] = bezierx[i][j];
      puntos[j][i][1] = beziery[i][j];
      puntos[j][i][2] = bezierz[i][j];
    }
  }

}

void bsplinepoints(SURFACE *surface, int u, int v, GLfloat puntosx[4][4], GLfloat puntosy[4][4], GLfloat puntosz[4][4]) {
  int i,j;
  int pos = 11*u + v;
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      POINT3D *punto = (surface->ptscontrol+pos);
      puntosx[i][j] = punto->x;
      puntosy[i][j] = punto->y;
      puntosz[i][j] = punto->z;	
      pos += 1;
    }
    pos = pos + 11 - 4;
  }
}

