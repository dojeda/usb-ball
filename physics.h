/**
 * physics.h
 *
 * Este header especifica las funciones necesarias para hacer un modelo
 * en tiempo real del movimiento de una pelota por una superficie en funcion
 * de la fuerza de gravedad.
 * 
 * Esta libreria asume muchas cosas que no son ciertas en la vida real, de
 * manera de simplificar y acelerar los calculos que haga la computadora,
 * pues el objetivo es hacerlo interactivo mas que fiel a la realidad.
 *
 * Para mas informacion acerca del modelo que se utiliza en este proyecto,
 * leer el informe anexo a este, o visitar la pagina:
 *
 * http://www.ldc.usb.ve/~ciro/files/pelota.pdf
 *
 * Entre otras suposiciones, mencionaremos:
 *
 * - Mientras la posicion de la pelota este dentro de la posicion en u, v
 *   entre 0 y 1, la pelota estara siempre unida al plano por un punto de
 *   contacto.
 *
 * - El roce de la pelota con el plano es suficiente para que el punto
 *   de contacto no tenga velocidad. Es decir, el punto de contacto y el
 *   centro de masa forman una palanca, que permite que la pelota se
 *   autoimpulse.
 *
 * - La pelota es un cuerpo totalmente rígido, por lo tanto, no hay
 *   deformacion de la pelota, y el area de contacto con el piso es un
 *   punto infinitesimal. Luego, se asume que no hay roce por la rotacion
 *   en el eje normal de la pelota.
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
#ifndef PHYSICS_H
#define PHYSICS_H

#include "data.h"

/* VALOR CONSTANTE DE LA GRAVEDAD (en m/s^2) */
#define GRAVITY 10.0f

/* VALOR DE UNA UNIDAD DE TIEMPO (en milisegundos) */
#define TICK 0.01f

void init_physics();

/* 
 * Esta funcion calcula la distancia recorrida por la pelota en un tick de tiempo.
 * Devuelve un arreglo de floats creados dinamicamente, en la primera posicion
 * esta la distancia en el parametro u, y en la segunda posicion la distancia en
 * el parametro v.
 */
void calculate_distance(BALL *ball, SURFACE *surface, float resp[2]);

/*
 * Esta funcion devuelve la distancia recorrida por la pelota sobre un plano
 * determinado por el contexto donde se utilice la funcion. Del plano se utiliza
 * la posicion donde esta la pelota, la normal de la superficie sobre el punto
 * de contacto, y la velocidad.
 * Esta funcion tiene como efecto lateral asignar la nueva velocidad angular de
 * la pelota.
 */
float calculate_distance_plane(BALL *ball, POINT3D *normal, int plane);

/*
 * Calcula la normal a un plano en el sistema de coordenadas del mundo. Debido
 * a que es un plano la normal es la misma en cualquier punto.
 */
void calculate_plane_normal(PLANE *plane, POINT3D *resp);

void check_on_surface(BALL *ball, SURFACE *surface, float distance[2]);

void convert_distance(GLfloat distance[2], BALL *ball, SURFACE *surface);

void calculate_falling(BALL *ball, float distance[1]);

#endif
