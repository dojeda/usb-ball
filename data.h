/**
 * data.h
 *
 * Este header define las estructuras de datos que utiliza el
 * renderer para mostrar objetos en pantalla.
 *
 * Un ANGLE es un numero real que representa un angulo medido
 * en radianes.
 *
 * Un COORDINATE es un numero real que representa una coordenada
 * en un espacio cualquiera.
 *
 * Un RADIUS es un numero real que representa el radio de una
 * esfera.
 *
 * Un POINT3D es un punto en el espacio X, Y, Z, descrita por
 * 3 COORDINATEs.
 *
 * Un POINTPARAM es un punto en el espacio U, V, descrita por
 * 2 COORDINATEs.
 *
 * Un PLANE es una superficie plana con unas ciertas
 * dimensiones dadas en coordenadas X y Z, se asumen que
 * todos tienen el mismo Y. Tiene como variables los 4 POINT3Ds
 * que definen el plano, y dos ANGLES que representan el
 * angulo de rotacion en X y Z con respecto al eje Y.
 *
 * Un SURFACE es una superficie descrita con varios puntos de
 * control, los cuales pueden interpolarse como un patch de 
 * B-Spline o como un patch de Bezier. Contiene un apuntador
 * a un arreglo de POINT3Ds, que son los puntos de control, y
 * dos ANGLEs, que representan el angulo de rotacion en X y Z
 * con respecto al eje Y.
 * 
 * Un BALL es la pelota que estara rodando sobre una superficie,
 * ya sea PLANE o SURFACE. Contiene un POINTPARAM que define su
 * posicion en la superficie; por la definicion de superficie
 * que utilizamos en este programa, podemos decir que si las
 * coordenadas U, V de la posicion se encuentran entre 0 y 1,
 * inclusive, la pelota esta dentro de la superficie. Tambien
 * contiene un POINTPARAM, que indica la velocidad actual de
 * la pelota, y un RADIUS que indica el radio de la pelota.
 *
 * Un CAMERA contiene los datos para posicionar la camara en
 * el ambiente de la superficie y la pelota. Tiene un POINT3D
 * que representa su posicion con respecto al origen de las
 * coordenadas del mundo, un POINT3D que representa el punto
 * hacia donde esta mirando (el cual para este proyecto sera
 * siempre el origen del sistema de coordenadas), y un POINT3D
 * que es un vector up, el que indica donde es el "arriba" de
 * la camara, por ultimo posee un angulo de apertura, el cual
 * determina las propiedades de la perspectiva de la vista
 *
 * LIGHT_POINT define un punto de luz en el espacio tridimensional.
 * Sus variables son: la posicion del punto de luz, la cantidad
 * de luz ambiental del punto (luz reflejada uniformemente en
 * todo el espacio), la cantidad de luz de difusion (luz reflejada
 * en un triangulo en funcion del angulo de incidencia entre el
 * rayo de luz y la normal del triangulo) y la cantidad de luz
 * especular (luz que llega a la camara en funcion del angulo
 * del rayo de luz contra la superficie y la linea que conecta
 * la camara con la normal del triangulo). Cada una de estas
 * cantidades se define con 4 componentes: luz roja, verde y
 * azul mas el componente alpha.
 *
 * MATERIAL define las propiedades de material para
 * un objeto. Estas propiedades modifican la luz que incide sobre
 * el objeto. Sus variables son: cantidad de luz ambiental,
 * difusion y especular (con el mismo comportamiento definido
 * en LIGHT_POINT) y la cantidad de luz que emite el objeto mismo.
 * Programacion: #00-32749 Ciro Duran
 *               #00-33151 David Ojeda
 *
 * Fecha: 2004-11-11
 *
 * Universidad Simon Bolivar
 * Departamento de Computacion y Tecnologia de la Informacion
 * CI-4321 - Computacion Grafica I
 */

#ifndef DATA_H
#define DATA_H

#include <GL/glut.h>

#define SUBDIVISION_STEPS 20

#define BALL_ON_SURFACE 0
#define BALL_FALLING 1

typedef float ANGLE;
typedef float COORDINATE;
typedef float RADIUS;

typedef struct {
  COORDINATE x;
  COORDINATE y;
  COORDINATE z;
} POINT3D;

typedef struct {
  COORDINATE u;
  COORDINATE v;
} POINTPARAM;

typedef struct {
	GLfloat position[4];
	GLfloat ambient[4];  //Cantidad de luz 'ambiental'
	GLfloat diffuse[4];  //Cantidad de luz que se refleja, no dep. de la camara
	GLfloat specular[4]; //Cantidad de luz que se refleja y llega a la camara
} LIGHT_POINT;

typedef struct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[1];
} MATERIAL;

typedef struct {
  POINT3D point[4]; //Los puntos que definen el plano, la coordenada Y
                    //es ignorada.
  ANGLE alphax;     //El eje de rotacion con respecto al eje X, con el
                    //cero coincidente con el eje Y.
  ANGLE alphaz;     //El eje de rotacion con respecto al eje Z, con el
                    //cero coincidente con el eje Y.
  MATERIAL *material; //El material del que esta hecho el plano
  POINT3D inter_points[SUBDIVISION_STEPS+1][SUBDIVISION_STEPS+1]; //Puntos interpolados
} PLANE;

typedef struct {
  POINT3D *ptscontrol; //Los puntos que definen el plano.
  ANGLE alphax;        //El eje de rotacion con respecto al eje X, con el
                       //cero coincidente con el eje Y.
  ANGLE alphaz;        //El eje de rotacion con respecto al eje Z, con el
                       //cero coincidente con el eje Y.
} SURFACE;

typedef struct {
  POINTPARAM position; //Posicion de la pelota en la superficie
  POINTPARAM velocity; //Velocidad actual instantanea de la pelota
  RADIUS radius;       //Radio de la pelota
  MATERIAL *material;  //Material del que esta hecha la pelota
  
  int movement_type;   //Tipo de movimiento, si esta en la superficie, o cayendo
  POINT3D outside_position; //Posicion de la pelota fuera del plano
  COORDINATE vertical_velocity; //Velocidad en Y de la pelota fuera del plano
} BALL;

typedef struct {
  POINT3D position;
  POINT3D center;
  POINT3D up;
  COORDINATE aperture;
} CAMERA;

POINT3D *create_point3d(COORDINATE x, COORDINATE y, COORDINATE z);
void destroy_point3d(POINT3D *point);

POINTPARAM *create_pointparam(COORDINATE u, COORDINATE v);
void destroy_pointparam(POINTPARAM *point);

PLANE *create_plane(COORDINATE x1, COORDINATE z1,
		    COORDINATE x2, COORDINATE z2,
		    COORDINATE x3, COORDINATE z3,
		    COORDINATE x4, COORDINATE z4,
		    MATERIAL *material);
void destroy_plane(PLANE *plane);
void calculate_plane_point3d(PLANE *plane, POINTPARAM param, POINT3D *point);

SURFACE *create_surface(POINT3D *ptscontrol);
void destroy_surface(SURFACE *surface);

POINT3D *create_control_points();
void destroy_control_points(POINT3D *puntos);

BALL *create_ball(POINTPARAM startPos, RADIUS radius, MATERIAL *material);
void destroy_ball(BALL *ball);

CAMERA *create_camera(POINT3D position, POINT3D center, POINT3D up, COORDINATE aperture);
void destroy_camera(CAMERA *camera);

LIGHT_POINT *create_light_point(GLfloat position[], 
                                GLfloat ambient[],
                                GLfloat diffuse[],
                                GLfloat specular[]);
void destroy_light_point(LIGHT_POINT *light_point);

MATERIAL *create_material(GLfloat ambient[],
                          GLfloat diffuse[],
                          GLfloat specular[],
                          GLfloat shininess[]);
void destroy_material(MATERIAL *material);
#endif
