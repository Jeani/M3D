#include "lib_scene.h"

t_scene* scene_vide()
{
    t_scene *pt_scene = NULL;
    t_point3d *p3d = definirPoint3d(0,0,1);
    t_triangle3d *t = definirTriangle3d(p3d,p3d,p3d);

	pt_scene = (t_scene *) malloc(sizeof(t_scene));

    pt_scene->centre = definirPoint3d(0,0,0);
    pt_scene->pt_objet = objet_vide();
    __insere_tete(pt_scene->pt_objet,__cree_maillon(t,BLANC));
    pt_scene->objet_suiv_g = NULL;
    pt_scene->objet_suiv_d = NULL;

	return pt_scene;
}

bool estVide (t_scene* pt_scene)
{
    if (pt_scene != NULL && (pt_scene->objet_suiv_g != NULL || pt_scene->objet_suiv_d != NULL)) {
        return false;
    } else {
        return true;
    }
}

double __distance (t_point3d * n, t_point3d *centre_camera, t_point3d *centre_objet)
{
    t_point3d *vect = NULL;
    double norme = 0;
    double distance = 0;
    int i;

    vect = definirVecteur3d(0,0,0);
    for (i=0;i<3;i++){
        vect->xyzt[i] = centre_objet->xyzt[i] - centre_camera->xyzt[i];
        distance += n->xyzt[i] * vect->xyzt[i];
        norme += n->xyzt[i] * n->xyzt[i];
    }

    distance = distance/sqrt(norme);

    free(vect);
    return distance;
}

void insererSceneAux (t_scene** pt_scene, t_objet3d * pt_objet, t_point3d * n, t_point3d *centre_camera, t_point3d *centre_objet, double distance)
{
    if (*pt_scene == NULL) {

        *pt_scene = (t_scene *) malloc(sizeof(t_scene));

        (*pt_scene)->centre = centre_objet;
        (*pt_scene)->pt_objet = pt_objet;
        (*pt_scene)->objet_suiv_g = NULL;
        (*pt_scene)->objet_suiv_d = NULL;
    } else {
        if (distance > __distance(n,centre_camera,(*pt_scene)->centre)) {
            insererSceneAux(&((*pt_scene)->objet_suiv_d),pt_objet,n,centre_camera,centre_objet,distance);
        } else {
            insererSceneAux(&((*pt_scene)->objet_suiv_g),pt_objet,n,centre_camera,centre_objet,distance);
        }
    }

}

void insererScene (t_scene* pt_scene, t_objet3d * pt_objet)
{
    t_maillon * mtmp = pt_scene->pt_objet->tete;
    t_point3d * centre_objet = centreObjet3d(pt_objet);
    double distance = __distance(mtmp->face->abc[0],pt_scene->centre,centre_objet);
    if (distance > 0){
        insererSceneAux(&(pt_scene->objet_suiv_d),pt_objet,mtmp->face->abc[0],pt_scene->centre,centre_objet,distance);
    } else {
        insererSceneAux(&(pt_scene->objet_suiv_g),pt_objet,mtmp->face->abc[0],pt_scene->centre,centre_objet,distance);
    }

}

t_scene* plusProchePredecesseur (t_scene* pt_scene)
{
    if (pt_scene == NULL || pt_scene->objet_suiv_d == NULL) {
            return pt_scene;
    } else {
        return plusProchePredecesseur(pt_scene->objet_suiv_d);
    }
}

void supprimerSceneAux (t_scene** pt_scene, t_point3d * n, t_point3d *centre_camera, t_point3d *centre_objet, double distance)
{
    double dtmp;
    t_scene *scenetmp = NULL;
    if (*pt_scene != NULL) {
        dtmp = __distance(n,centre_camera,(*pt_scene)->centre);

        if (distance > dtmp) {
            supprimerSceneAux(&((*pt_scene)->objet_suiv_d),n,centre_camera,centre_objet,distance);
        } else {
            if (distance == dtmp && egaux(centre_objet,(*pt_scene)->centre)) {
                if ((*pt_scene)->objet_suiv_g == NULL) {
                    free((*pt_scene)->centre); // On lib�re le centre du noeud � supprimer
                    scenetmp = (*pt_scene)->objet_suiv_d; // On garde un pointeur sur son fils droit
                    free(*pt_scene); // On lib�re le noeud � supprimer
                    *pt_scene = scenetmp; // On lui affecte la valeur de son fils droit
                } else {
                    if ((*pt_scene)->objet_suiv_d == NULL) {
                        free((*pt_scene)->centre); // On lib�re le centre du noeud � supprimer
                        scenetmp = (*pt_scene)->objet_suiv_g; // On garde un pointeur sur son fils gauche
                        free(*pt_scene); // On lib�re le noeud � supprimer
                        *pt_scene = scenetmp; // On lui affecte la valeur de son fils gauche
                    } else {
                        scenetmp = plusProchePredecesseur((*pt_scene)->objet_suiv_g); // Recherche du plus proche pr�decesseur du noeud � supprimer
                        free((*pt_scene)->centre); // On lib�re le centre du noeud � supprimer
                        __copierPoint3d(scenetmp->centre,(*pt_scene)->centre); // On remplace le centre du noeud � supprimer par celui du plus proche pr�decesseur
                        (*pt_scene)->pt_objet = scenetmp->pt_objet; // Idem pour l'objet du noeud
                        supprimerSceneAux(&((*pt_scene)->objet_suiv_g),n,centre_camera,scenetmp->centre,__distance(n,centre_camera,scenetmp->centre)); // On supprime le plus proche pr�decesseur du fils gauche
                    }
                }
            } else {
                supprimerSceneAux(&((*pt_scene)->objet_suiv_g),n,centre_camera,centre_objet,distance);
            }
        }
    }

}

void supprimerScene (t_scene* pt_scene, t_objet3d * pt_objet)
{
    t_maillon * mtmp = pt_scene->pt_objet->tete;
    t_point3d * centre_objet = centreObjet3d(pt_objet);
    double distance = __distance(mtmp->face->abc[0],pt_scene->centre,centre_objet);
     if (distance > 0){
        supprimerSceneAux(&(pt_scene->objet_suiv_d),mtmp->face->abc[0],pt_scene->centre,centre_objet,distance);
    } else {
        supprimerSceneAux(&(pt_scene->objet_suiv_g),mtmp->face->abc[0],pt_scene->centre,centre_objet,distance);
    }
}


void rotationObjet3dScene(t_scene* pt_scene, t_objet3d* pt_objet, t_point3d *centre_rotation, float degreX, float degreY, float degreZ)
{
    supprimerScene(pt_scene,pt_objet);
    rotationObjet3d(pt_objet,centre_rotation,degreX,degreY,degreZ);
    insererScene(pt_scene,pt_objet);
}


void dessinerSceneAux (t_surface *surface, t_scene* pt_scene)
{
    // On affiche uniquement les objets du ss arbre d qui contiennent les objets les plus loin devant
    // (plus un objet est � d dans l'arbre, plus il est loin devant dans la sc�ne)
    if (pt_scene != NULL) {
        dessinerSceneAux(surface, pt_scene->objet_suiv_d);
        dessinerObjet3d(surface, pt_scene->pt_objet); // Faux, changer la vue qui pour l'instant correspond � une cam�ra fixe !!!
        dessinerSceneAux(surface, pt_scene->objet_suiv_g);
    }
}

void dessinerScene (t_surface *surface, t_scene* pt_scene)
{
    dessinerSceneAux(surface,pt_scene->objet_suiv_d);
}


