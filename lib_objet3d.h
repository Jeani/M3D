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

/*
  Crée un parallélépipède de dimensions lx,ly,lz et de couleur c.
  Le carré a un coin à l'origine et est dans cadran positif.
  Les couleurs sont dans l'ordre du tableau c : avant,arrière,haut,bas,gauche,droite
*/
t_objet3d* parallelepipede(double lx, double ly, double lz, Uint32 C[6]);

/*
  Crée un carré de côté l et de couleur c.
  Le carré a un coin à l'origine et est dans le plan xOy, cadran positif
*/
t_objet3d* carre(double l, Uint32 c);

/*
  Crée une face de côté l faite de 9 carrés identiques de couleur c[0] à c[8].
  La face a un coin à l'origine et est dans le plan xOy, cadran positif
*/
t_objet3d* face(double l, Uint32 c[9]);

/*
  Crée un rubik's cube d'arrête de longueur l.
  Le cube a un coin à l'origine et se situe dans le cadran positif
*/
t_objet3d* rubiks(double l);

/*
  Crée un tétraèdre de côté de longueur a
  ayant un coin à l'origine et une arrête sur l'axe Ox.
  Il se situe dans le cadran positif
*/
t_objet3d* tetraedre(double a);

// Crée un icosaèdre de côté de longueur a centré à l'origine
t_objet3d* icosaedre(double a);

/*
  Crée une sphère de rayon r pour une précision n allant de 0 (icosaèdre) à 5
  La sphère est centrée à l'origine.
*/
t_objet3d* sphere(double r, int n);
/*
  Crée une sphère amige de rayon r possèdant nlat triangles en latitude et nlong triangles en longitude
  La sphère est centrée à l'origine.
*/
t_objet3d* sphere_amiga(double r, double nlat, double nlong);
t_objet3d* sapin(double h, double l);
t_objet3d* arbre(double lx, double ly, double lz);
t_objet3d* damier(double lx, double lz, double nx, double nz);

void dessinerObjet3d(t_surface *surface, t_objet3d* pt_objet);

t_point3d *centreObjet3d(t_objet3d *o);
t_objet3d *copierObjet3d(t_objet3d *o); // attention, effectue une copie
void composerObjet3d(t_objet3d* o, t_objet3d* o2); // o = o + o2
void libererObjet3d(t_objet3d *o);

void translationObjet3d(t_objet3d* pt_objet, t_point3d *vecteur);
void rotationObjet3d(t_objet3d* pt_objet, t_point3d *centre, float degreX, float degreY, float degreZ);
void transformationObjet3d(t_objet3d* pt_objet, double mat[4][4]);

#endif
