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
                    free((*pt_scene)->centre); // On libère le centre du noeud à supprimer
                    scenetmp = (*pt_scene)->objet_suiv_d; // On garde un pointeur sur son fils droit
                    free(*pt_scene); // On libère le noeud à supprimer
                    *pt_scene = scenetmp; // On lui affecte la valeur de son fils droit
                } else {
                    if ((*pt_scene)->objet_suiv_d == NULL) {
                        free((*pt_scene)->centre); // On libère le centre du noeud à supprimer
                        scenetmp = (*pt_scene)->objet_suiv_g; // On garde un pointeur sur son fils gauche
                        free(*pt_scene); // On libère le noeud à supprimer
                        *pt_scene = scenetmp; // On lui affecte la valeur de son fils gauche
                    } else {
                        scenetmp = plusProchePredecesseur((*pt_scene)->objet_suiv_g); // Recherche du plus proche prédecesseur du noeud à supprimer
                        free((*pt_scene)->centre); // On libère le centre du noeud à supprimer
                        __copierPoint3d(scenetmp->centre,(*pt_scene)->centre); // On remplace le centre du noeud à supprimer par celui du plus proche prédecesseur
                        (*pt_scene)->pt_objet = scenetmp->pt_objet; // Idem pour l'objet du noeud
                        supprimerSceneAux(&((*pt_scene)->objet_suiv_g),n,centre_camera,scenetmp->centre,__distance(n,centre_camera,scenetmp->centre)); // On supprime le plus proche prédecesseur du fils gauche
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

void translationObjet3dScene(t_scene* pt_scene, t_objet3d* pt_objet, t_point3d *vecteur)
{
    supprimerScene(pt_scene,pt_objet);
    translationObjet3d(pt_objet,vecteur);
    insererScene(pt_scene,pt_objet);
}

void __coupure(t_scene* pt_scene, t_scene** pt_sg, t_scene** pt_sd, t_point3d * n, t_point3d *centre_camera)
{
    t_scene *pt_sgtmp, *pt_sdtmp;
    double distance;


    if (pt_scene == NULL){
        *pt_sg = NULL;
        *pt_sd = NULL;
    } else {
        distance = __distance(n,centre_camera,pt_scene->centre);
        if (distance > 0){
            __coupure(pt_scene->objet_suiv_g, &pt_sgtmp, &pt_sdtmp, n, centre_camera);
            *pt_sg = pt_sgtmp;
            *pt_sd = pt_scene;
            (*pt_sd)->objet_suiv_g = pt_sdtmp;
        } else {
            __coupure(pt_scene->objet_suiv_d, &pt_sgtmp, &pt_sdtmp, n, centre_camera);
            *pt_sg = pt_scene;
            (*pt_sg)->objet_suiv_d = pt_sgtmp;
            *pt_sd = pt_sdtmp;
        }
    }
}

void translationScene(t_scene **pt_scene, t_point3d* vecteur)
{
    t_scene *pt_sg, *pt_sd; // Sous scènes pour la coupure
    t_point3d* centre_camera = definirPoint3d(0,0,0); // Nouveau centre camera
    t_objet3d* camera = (*pt_scene)->pt_objet; // Nouveau direction (inchangé pour une translation)
    t_point3d* n = camera->tete->face->abc[0]; // Vecteur de direction camera
    t_scene *scenetmp = NULL; // Plus proche prédécesseur + nouvelle camera
    int i;
    double p = prod_scal(n,vecteur);
    __copierPoint3d((*pt_scene)->centre,centre_camera);

    if (p == 0){ // si 0 pas de réorganisation car translation dans le plan de projection
        // On met à jour le centre de camera
        for (i=0;i<3;i++){
            (*pt_scene)->centre->xyzt[i] += vecteur->xyzt[i];
        }
    } else {
        // On supprime l'ancienne camera dans la scène
        if ((*pt_scene)->objet_suiv_g == NULL) {
            free((*pt_scene)->centre);
            *pt_scene = (*pt_scene)->objet_suiv_d;
        } else {
            if ((*pt_scene)->objet_suiv_d == NULL) {
                free((*pt_scene)->centre);
                *pt_scene = (*pt_scene)->objet_suiv_g;
            } else {
                scenetmp = plusProchePredecesseur((*pt_scene)->objet_suiv_g);

            __copierPoint3d(scenetmp->centre,(*pt_scene)->centre); // On remplace le centre du noeud à supprimer par celui du plus proche prédecesseur
            (*pt_scene)->pt_objet = scenetmp->pt_objet; // Idem pour l'objet du noeud
            supprimerSceneAux(&((*pt_scene)->objet_suiv_g),n,centre_camera,scenetmp->centre,__distance(n,centre_camera,scenetmp->centre)); // On supprime le plus proche prédecesseur dans le fils gauche
            }
        }
        // On met à jour les éléments de la camera sauvegardés précédement
        for (i=0;i<3;i++){
            centre_camera->xyzt[i] += vecteur->xyzt[i];
        }

        // On coupe la scène par rapport à la nouvelle caméra
        __coupure(*pt_scene, &pt_sg, &pt_sd, n, centre_camera);

        // On crée la nouvelle scène en ajoutant la caméra (à partir de scenetmp)
        scenetmp = (t_scene *) malloc(sizeof(t_scene));
        scenetmp->centre = centre_camera;
        scenetmp->pt_objet = camera;
        scenetmp->objet_suiv_g = pt_sg;
        scenetmp->objet_suiv_d = pt_sd;

        *pt_scene = scenetmp;
    }
}

void rotationSceneAux(t_scene *pt_scene, double m[4][4], t_scene *scene)
{
    if (pt_scene != NULL){
        rotationSceneAux(pt_scene->objet_suiv_g, m, scene);

        transformationObjet3d(pt_scene->pt_objet,m);
        pt_scene->pt_objet->est_trie = false;
        insererScene(scene,pt_scene->pt_objet);

        rotationSceneAux(pt_scene->objet_suiv_d, m, scene);
    }
}

void rotationScene(t_scene **pt_scene, t_point3d *centre_rotation, float degreX, float degreY, float degreZ)
{
    t_scene *scene = scene_vide();
    double m[4][4];
    double mres[4][4];
    double mtmp[4][4];
    int i,j;
    float theta;
    __init_m(m);
    m[3][3] = 1;

   // 1ere translation
	__init_m(mres);
    for(i=0;i<4;i++){
        mres[i][3] = centre_rotation->xyzt[i];
        mres[i][i] = 1;
    }

	// initialisation de la matrice de rotation selon x
	theta = __conversion_deg_rad(degreX);
    m[0][0] = 1;
    m[1][1] = cos(theta);
    m[1][2] = -1*sin(theta);
    m[2][1] = sin(theta);
    m[2][2] = cos(theta);
    multiplicationMatrice3d(mtmp,mres,m);

    // initialisation de la matrice de rotation selon y
    theta = __conversion_deg_rad(degreY);
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            m[i][j] = 0;
        }
    }
    m[0][0] = cos(theta);
    m[0][2] = -1*sin(theta);
    m[1][1] = 1;
    m[2][0] = sin(theta);
    m[2][2] = cos(theta);
    multiplicationMatrice3d(mres,mtmp,m);

    // initialisation de la matrice de rotation selon z
    theta = __conversion_deg_rad(degreZ);
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            m[i][j] = 0;
        }
    }
    m[0][0] = cos(theta);
    m[0][1] = -1*sin(theta);
    m[1][0] = sin(theta);
    m[1][1] = cos(theta);
    m[2][2] = 1;
    multiplicationMatrice3d(mtmp,mres,m);


    // Matrice de remise en place de l'objet
    __inversion_3(centre_rotation);
	__init_m(m);
    for(i=0;i<4;i++){
        m[i][3] = centre_rotation->xyzt[i];
        m[i][i] = 1;
    }
    multiplicationMatrice3d(mres,mtmp,m);
    __inversion_3(centre_rotation);

    // On met la camera dans la nouvelle scene
    __copierPoint3d((*pt_scene)->centre,scene->centre);
    scene->pt_objet = (*pt_scene)->pt_objet;

    // On fait la rotation de chaque objet et on l'insère dans la nouvelle scène
    rotationSceneAux((*pt_scene)->objet_suiv_g, mres, scene);
    rotationSceneAux((*pt_scene)->objet_suiv_d, mres, scene);

    libererScene(*pt_scene);
    *pt_scene = scene;
}

void libererSceneEtObjet(t_scene *pt_scene)
{
    if (pt_scene != NULL) {
        // On libère les sous scènes
        libererScene(pt_scene->objet_suiv_g);
        libererScene(pt_scene->objet_suiv_d);

        // On libère le centre et l'objet en cours
        free(pt_scene->centre);
        libererObjet3d(pt_scene->pt_objet);
        // On libère la scène
        free(pt_scene);
    }
}


void libererScene(t_scene *pt_scene)
{
    if (pt_scene != NULL) {
        // On libère les sous scènes
        libererScene(pt_scene->objet_suiv_g);
        libererScene(pt_scene->objet_suiv_d);

        // On libère le centre
        free(pt_scene->centre);
        // On libère la scène
        free(pt_scene);
    }
}

void dessinerSceneAux (t_surface *surface, t_scene* pt_scene)
{
    // On affiche uniquement les objets du ss arbre d qui contiennent les objets les plus loin devant
    // (plus un objet est à d dans l'arbre, plus il est loin devant dans la scène)
    if (pt_scene != NULL) {
        dessinerSceneAux(surface, pt_scene->objet_suiv_d);
        dessinerObjet3d(surface, pt_scene->pt_objet);
        dessinerSceneAux(surface, pt_scene->objet_suiv_g);
    }
}

void dessinerScene (t_surface *surface, t_scene* pt_scene)
{
    X = RX/2 - pt_scene->centre->xyzt[0];
    Y = RY/2 - pt_scene->centre->xyzt[1];
    dessinerSceneAux(surface,pt_scene->objet_suiv_d);
}


