#ifndef _LIB_3D_H_
#define _LIB_3D_H_

#include "lib_2d.h"

#define D 3
#define ZOOM 500

double X, Y;

typedef enum {
	false=0, true
} t_bool;

typedef struct
{
    double xyzt[4]; // vu en TD, coordonnées homogènes
} t_point3d;

typedef struct
{
    t_point3d* abc[3];
} t_triangle3d;

t_point3d *definirPoint3d(double x, double y, double z);
t_point3d *definirVecteur3d(double x, double y, double z);
t_triangle3d *definirTriangle3d(t_point3d * a, t_point3d * b, t_point3d * c);
t_triangle3d *copierTriangle3d(t_triangle3d *t);
t_bool egaux(t_point3d * p1, t_point3d * p2);
void libererTriangle3d(t_triangle3d *t);

void __init_m(double m[4][4]);
float __conversion_deg_rad(float theta);
void __inversion_3(t_point3d *v1);
void __copierPoint3d(t_point3d * src, t_point3d * dest);

double zmoyen(t_triangle3d *t);
void remplirTriangle3d(t_surface * surface, t_triangle3d * triangle, Uint32 c);

void translationTriangle3d(t_triangle3d *t, t_point3d *vecteur);
void homothetieTriangle3d(t_triangle3d *t, double coef); // A modifier si coef différents par coordonnée
void rotationTriangle3d(t_triangle3d *t, t_point3d *centre, float degreX, float degreY, float degreZ);

void transformationTriangle3d(t_triangle3d *t, double mat[4][4]);

#endif
