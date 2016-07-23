/**
 * mathutil.h
 *
 * Este header especifica algunas funciones matematicas de utilidad para
 * los calculos que se hacen dentro del programa.
 *
 * Las funciones contenidas aqui cumplen con diversas tareas:
 *
 * - Funciones seno y coseno tabuladas, para mayor rapidez (las macros
 *   SIN() y COS()).
 * - Rotacion de vectores en 3 dimensiones (rotate_vector())
 * - Multiplicacion de vectores con una matriz (multiply_vector())
 * - Producto cruz (cross_product())
 * - Producto punto (vector_product())
 * - Normalizacion de vectores (normalize_vector3d())
 * - Evaluación de splines (calculateSpline())
 * - Evaluación de normales en un punto de un spline (calculateNormal())
 * - Conversion de B-Splines a Splines de Bézier (bspline2bezier())
 *
 * Es de importancia notar que para utilizar esta libreria, es necesario
 * inicializar algunos valores que utiliza llamando a init_math() antes
 * de usar cualquier otra función.
 *
 * Programacion: #00-32749 Ciro Duran
 *               #00-33151 David Ojeda
 *
 * Fecha: 2004-11-17
 *
 * Universidad Simon Bolivar
 * Departamento de Computacion y Tecnologia de la Informacion
 * CI-4321 - Computacion Grafica I
 */

#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <GL/glut.h>
#include <math.h>
#include "data.h"

//Distancia necesaria para el calculo de la normal en un spline
#define EPSILON 0.00001f
//Distancia en coordenadas parametricas para la conversion de distancias
#define SIGMA   0.1f

#define SIN(x) (x >= 0? sin_values[((int)rintf(x))%360]: -sin_values[((int)rintf(-x))%360])
#define COS(x) (x >= 0? cos_values[((int)rintf(x))%360]:  cos_values[((int)rintf(-x))%360])

void init_math(void);
void cross_product(GLfloat A[4][4], GLfloat B[4][4], GLfloat AxB[4][4]);
void rotate_vector(GLfloat vector[3], GLfloat x, GLfloat y, GLfloat z, GLfloat result[3]);
void multiply_vector(GLfloat vector[4], GLfloat mat[4][4], GLfloat result[4]);
void vector_cross_matrix(GLfloat V[4], GLfloat A[4][4], GLfloat VxA[4]);
void vector3d_cross_product(GLfloat A[3], GLfloat B[3], GLfloat AxB[3]);
GLfloat vector_product(GLfloat A[4], GLfloat B[4]);
void normalize_vector3d(GLfloat V[3]);
POINT3D calculateSpline(SURFACE *surface,float u, float v);
void calculateNormal(SURFACE *surface, float u, float v, POINT3D *punto, POINT3D *normal);
void calculateNormalNotRotated(SURFACE *surface, float u, float v, POINT3D *punto, POINT3D *normal);
void bspline2bezier(SURFACE *surface,int u, int v, GLfloat puntos[4][4][3]);
void bsplinepoints(SURFACE *surface, int u, int v, GLfloat puntosx[4][4], GLfloat puntosy[4][4], GLfloat puntosz[4][4]);

GLfloat m_bspline_bezier[4][4], 
  m_bspline_bezierT[4][4],
  m_bspline[4][4],
  m_bsplineT[4][4];

#endif
