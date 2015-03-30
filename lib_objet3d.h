#ifndef _LIB_OBJET3D_H_
#define _LIB_OBJET3D_H_

#include "lib_surface.h"
#include "lib_3d.h"

typedef enum {
	false=0, true
} t_bool;

typedef struct __liste3d t_maillon;

struct __liste3d
{
    t_triangle3d *face;
    Uint32 couleur;
    t_maillon *pt_suiv;
};

typedef struct __objet3d t_objet3d;

struct __objet3d
{
	t_bool est_trie;
    t_maillon *tete;
};

void __insere_tete(t_objet3d *pt_objet, t_maillon *pt_maillon);
t_maillon* __cree_maillon(t_triangle3d *face, Uint32 couleur);
t_objet3d* __transfo_face(t_maillon *pt_maillon,double r);

t_objet3d* objet_vide();
t_objet3d* camera(); // == objet vide

t_objet3d* parallelepipede(double lx, double ly, double lz);
t_objet3d* carre(double l, Uint32 c);
t_objet3d* face(double l, Uint32 c[9]);
t_objet3d* rubiks(double l);
t_objet3d* tetraedre(double a);
t_objet3d* icosaedre(double a);
t_objet3d* sphere(double r);
t_objet3d* sphere_amiga(double r, double nlat, double nlong);
t_objet3d* arbre(double lx, double ly, double lz);
t_objet3d* damier(double lx, double lz, double nx, double nz);

void dessinerObjet3d(t_surface *surface, t_objet3d* pt_objet);

t_objet3d *copierObjet3d(t_objet3d *o); // attention, effectue une copie
void composerObjet3d(t_objet3d* o, t_objet3d* o2); // o = o + o2
void libererObjet3d(t_objet3d *o);

void translationObjet3d(t_objet3d* pt_objet, t_point3d *vecteur);
void rotationObjet3d(t_objet3d* pt_objet, t_point3d *centre, float degreX, float degreY, float degreZ);
void transformationObjet3d(t_objet3d* pt_objet, double mat[4][4]);

#endif
