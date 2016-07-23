/**
 * main.h
 *
 * Pelota es un demo/juego que muestra las capacidades de una computadora para
 * simular la física aplicada a una pelota rodando por una superficie.
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
#ifndef MAIN_H
#define MAIN_H

void init_window(int argc, char *argv[]);
void func_keyboard(unsigned char key, int x, int y);
void func_arrows(int key, int x, int y);
void func_mouse(int button, int state, int x, int y);
void func_motion(int x, int y);
void func_passive_motion(int x, int y);
void update(int value);
void randomizePoints(void);
void resetPoints(void);
#endif
