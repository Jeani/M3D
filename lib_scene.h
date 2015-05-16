#ifndef _LIB_SCENE_H_
#define _LIB_SCENE_H_

#include "lib_objet3d.h"
#include "lib_mat.h"
#include <stdbool.h>
#include <math.h>

typedef struct __scene t_scene;

struct __scene
{
    t_point3d* centre;
    t_objet3d* pt_objet;
    t_scene *objet_suiv_g;
    t_scene *objet_suiv_d;
};

t_scene* scene_vide();

bool estVide (t_scene* pt_scene);

double __distance (t_point3d * n, t_point3d *centre_camera, t_point3d *centre_objet);

void insererScene (t_scene* pt_scene, t_objet3d * pt_objet);
void supprimerScene (t_scene* pt_scene, t_objet3d * pt_objet);

// Modifier un objet d'une scène
void rotationObjet3dScene(t_scene* pt_scene, t_objet3d* pt_objet, t_point3d *centre_rotation, float degreX, float degreY, float degreZ);
void translationObjet3dScene(t_scene* pt_scene, t_objet3d* pt_objet, t_point3d *vecteur);

// Modifier la caméra d'une scène
void translationScene(t_scene **pt_scene, t_point3d* vecteur);
void rotationScene(t_scene **pt_scene, t_point3d *centre_rotation, float degreX, float degreY, float degreZ);

void libererSceneEtObjet(t_scene *pt_scene);
//lirère la scène mais sans les objets
void libererScene(t_scene *pt_scene);

void dessinerScene (t_surface *surface, t_scene* pt_scene);


#endif // _LIB_SCENE_H_
