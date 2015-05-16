#include <assert.h>
#include <math.h>
#include <time.h>
#include "lib_objet3d.h"

const double PHI = (1+sqrt(5))/2;

t_maillon* __cree_maillon(t_triangle3d *face, Uint32 couleur)
{
	t_maillon* pt_maillon = NULL;
	pt_maillon = (t_maillon*) malloc(sizeof(t_maillon));
	pt_maillon->face = face;
	pt_maillon->couleur = couleur;
	pt_maillon->pt_suiv = NULL;

	return pt_maillon;
}

void __insere_tete(t_objet3d *pt_objet, t_maillon *pt_maillon)
{
	if (pt_objet->tete == NULL) {
        pt_objet->tete = pt_maillon;
        pt_objet->est_trie = true;
	} else {
        pt_objet->est_trie = pt_objet->est_trie && zmoyen(pt_maillon->face)>=zmoyen((pt_objet->tete)->face);
        pt_maillon->pt_suiv = pt_objet->tete;
        pt_objet->tete = pt_maillon;
	}
}

t_objet3d *objet_vide()
{
	t_objet3d *pt_objet = NULL;

	pt_objet = (t_objet3d *) malloc(sizeof(t_objet3d));
	pt_objet->est_trie = true;
	pt_objet->tete = NULL;

	return pt_objet;
}

t_objet3d *camera()
{
	return objet_vide();
}

t_objet3d* parallelepipede(double lx, double ly, double lz, Uint32 C[6])
{
	t_objet3d *pt_objet = NULL;
	t_point3d *tab_points[8];
	t_triangle3d *t = NULL;
	int i;

	pt_objet = objet_vide();

    // On place tous les sommets du parallélépipède dans le tableau
	tab_points[0] = definirPoint3d(0,0,0);
	tab_points[1] = definirPoint3d(lx,0,0);
	tab_points[2] = definirPoint3d(lx,ly,0);
	tab_points[3] = definirPoint3d(0,ly,0);
	tab_points[4] = definirPoint3d(0,ly,lz);
	tab_points[5] = definirPoint3d(lx,ly,lz);
	tab_points[6] = definirPoint3d(lx,0,lz);
	tab_points[7] = definirPoint3d(0,0,lz);

    //face avant
    t = definirTriangle3d(tab_points[0],tab_points[1],tab_points[2]);
    __insere_tete(pt_objet,__cree_maillon(t,C[0]));
    t = definirTriangle3d(tab_points[0],tab_points[2],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,C[0]));
    //triangles ayant 2pts commun avec la face avant
    t= definirTriangle3d(tab_points[0],tab_points[1],tab_points[6]);
    __insere_tete(pt_objet,__cree_maillon(t,C[2]));
    t= definirTriangle3d(tab_points[0],tab_points[3],tab_points[4]);
    __insere_tete(pt_objet,__cree_maillon(t,C[4]));
    t= definirTriangle3d(tab_points[5],tab_points[1],tab_points[2]);
    __insere_tete(pt_objet,__cree_maillon(t,C[5]));
    t= definirTriangle3d(tab_points[5],tab_points[2],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,C[3]));
    //triangles ayant 2pts commun avec la face arrière
    t= definirTriangle3d(tab_points[0],tab_points[6],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,C[2]));
    t= definirTriangle3d(tab_points[0],tab_points[4],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,C[4]));
    t= definirTriangle3d(tab_points[5],tab_points[1],tab_points[6]);
    __insere_tete(pt_objet,__cree_maillon(t,C[5]));
    t= definirTriangle3d(tab_points[5],tab_points[3],tab_points[4]);
    __insere_tete(pt_objet,__cree_maillon(t,C[3]));
    //face arrière
    t= definirTriangle3d(tab_points[5],tab_points[6],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,C[1]));
    t= definirTriangle3d(tab_points[5],tab_points[4],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,C[1]));

    // On libère les points sommets du parallélépipède
    for (i=0;i<8;i++) {
        free(tab_points[i]);
    }

	return pt_objet;
}

t_objet3d* carre(double l, Uint32 c)
{
    t_objet3d *pt_objet = NULL;
    t_point3d *tab_points[4];
    t_triangle3d *t = NULL;
    int i;

    pt_objet = objet_vide();

    tab_points[0] = definirPoint3d(0,0,0);
	tab_points[1] = definirPoint3d(l,0,0);
	tab_points[2] = definirPoint3d(l,l,0);
	tab_points[3] = definirPoint3d(0,l,0);

	t = definirTriangle3d(tab_points[0],tab_points[1],tab_points[2]);
    __insere_tete(pt_objet,__cree_maillon(t,c));
    t = definirTriangle3d(tab_points[0],tab_points[2],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,c));

    // On libère les points sommets du carré
    for (i=0;i<4;i++) {
        free(tab_points[i]);
    }

    return pt_objet;
}

t_objet3d* face(double l, Uint32 c[9])
{
    t_objet3d *pt_objet = objet_vide();
    t_objet3d *otmp = objet_vide();
    int i = 0, lin = 0, col = 0;

    for (i=0;i<9;i++) {
        if (col >= 3) {
            col = 0;
            lin++;
        }
        // On crée un carré de la couleur voulue
        otmp = carre(l/3,c[i]);
        // On le place
        translationObjet3d(otmp,definirPoint3d(col*l/3,lin*l/3,0));
        // On l'ajoute à la face
        composerObjet3d(pt_objet,otmp);
        col++;
    }

    return pt_objet;
}



t_objet3d* rubiks(double l)
{
    t_objet3d *pt_objet = NULL;
    t_objet3d *otmp = NULL;
    t_point3d *origine = definirPoint3d(0,0,0);
    Uint32 c[9];

    pt_objet = objet_vide();
    otmp = objet_vide();

    //face avant
    c[0] = RC_ROUGE;
    c[1] = RC_ORANGE;
    c[2] = RC_ROUGE;
    c[3] = RC_ORANGE;
    c[4] = RC_ROUGE;
    c[5] = RC_ORANGE;
    c[6] = RC_ROUGE;
    c[7] = RC_ORANGE;
    c[8] = RC_ROUGE;
    otmp = face(l,c);
    composerObjet3d(pt_objet,otmp);

    //faces latérales
    c[0] = RC_BLANC;
    c[1] = RC_JAUNE;
    c[2] = RC_BLANC;
    c[3] = RC_JAUNE;
    c[4] = RC_BLANC;
    c[5] = RC_JAUNE;
    c[6] = RC_BLANC;
    c[7] = RC_JAUNE;
    c[8] = RC_BLANC;
    otmp = face(l,c);
    rotationObjet3d(otmp,origine,90,0,0);
    composerObjet3d(pt_objet,otmp);

    c[0] = RC_BLEU;
    c[1] = RC_VERT;
    c[2] = RC_BLEU;
    c[3] = RC_VERT;
    c[4] = RC_BLEU;
    c[5] = RC_VERT;
    c[6] = RC_BLEU;
    c[7] = RC_VERT;
    c[8] = RC_BLEU;
    otmp = face(l,c);
    rotationObjet3d(otmp,origine,0,90,0);
    translationObjet3d(otmp,definirPoint3d(l,0,0));
    composerObjet3d(pt_objet,otmp);

    c[0] = RC_JAUNE;
    c[1] = RC_BLANC;
    c[2] = RC_JAUNE;
    c[3] = RC_BLANC;
    c[4] = RC_JAUNE;
    c[5] = RC_BLANC;
    c[6] = RC_JAUNE;
    c[7] = RC_BLANC;
    c[8] = RC_JAUNE;
    otmp = face(l,c);
    rotationObjet3d(otmp,origine,90,0,0);
    translationObjet3d(otmp,definirPoint3d(0,l,0));
    composerObjet3d(pt_objet,otmp);

    c[0] = RC_VERT;
    c[1] = RC_BLEU;
    c[2] = RC_VERT;
    c[3] = RC_BLEU;
    c[4] = RC_VERT;
    c[5] = RC_BLEU;
    c[6] = RC_VERT;
    c[7] = RC_BLEU;
    c[8] = RC_VERT;
    otmp = face(l,c);
    rotationObjet3d(otmp,origine,0,90,0);
    composerObjet3d(pt_objet,otmp);

    //face arrière
    c[0] = RC_ORANGE;
    c[1] = RC_ROUGE;
    c[2] = RC_ORANGE;
    c[3] = RC_ROUGE;
    c[4] = RC_ORANGE;
    c[5] = RC_ROUGE;
    c[6] = RC_ORANGE;
    c[7] = RC_ROUGE;
    c[8] = RC_ORANGE;
    otmp = face(l,c);
    translationObjet3d(otmp,definirPoint3d(0,0,l));
    composerObjet3d(pt_objet,otmp);

    free(origine);

    return pt_objet;
}

t_objet3d* tetraedre(double a)
{
	t_objet3d *pt_objet = NULL;
    t_point3d *tab_points[4];
    t_triangle3d *t = NULL;
    int i;

    pt_objet = objet_vide();

    tab_points[0] = definirPoint3d(0,0,0);
	tab_points[1] = definirPoint3d(a,0,0);
	tab_points[2] = definirPoint3d(a/2,a*sqrt(3)/2,0);
	tab_points[3] = definirPoint3d(a/2,a*sqrt(3)/6,a*sqrt(6)/3);

    // Base
    t = definirTriangle3d(tab_points[0],tab_points[1],tab_points[2]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_BLEU));
    // Faces latérales
    t = definirTriangle3d(tab_points[0],tab_points[1],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_VERT));
    t = definirTriangle3d(tab_points[1],tab_points[2],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_JAUNE));
    t = definirTriangle3d(tab_points[0],tab_points[2],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,ROUGEF));

    // On libère les points somments du tetraèdre
    for (i=0;i<4;i++) {
        free(tab_points[i]);
    }

	return pt_objet;
}

t_objet3d* icosaedre(double a)
{
	t_objet3d *pt_objet = NULL;
    t_point3d *tab_points[12];
    t_triangle3d *t = NULL;
    double d1 = a/2;
    double d2 = a* PHI /2;
    int i;

    pt_objet = objet_vide();

    tab_points[0] = definirPoint3d(d1,d2,0);
	tab_points[1] = definirPoint3d(0,d1,d2);
	tab_points[2] = definirPoint3d(d2,0,d1);
	tab_points[3] = definirPoint3d(-d1,d2,0);
	tab_points[4] = definirPoint3d(0,-d1,d2);
	tab_points[5] = definirPoint3d(d2,0,-d1);
	tab_points[6] = definirPoint3d(d1,-d2,0);
	tab_points[7] = definirPoint3d(0,d1,-d2);
	tab_points[8] = definirPoint3d(-d2,0,d1);
	tab_points[9] = definirPoint3d(-d1,-d2,0);
	tab_points[10] = definirPoint3d(0,-d1,-d2);
	tab_points[11] = definirPoint3d(-d2,0,-d1);

	// Avant
    t = definirTriangle3d(tab_points[7],tab_points[10],tab_points[11]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_BLEU));
    t = definirTriangle3d(tab_points[7],tab_points[10],tab_points[5]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_VERT));
    // Faces couronne avant
    t = definirTriangle3d(tab_points[0],tab_points[5],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_BLANC));
    t = definirTriangle3d(tab_points[5],tab_points[6],tab_points[10]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_JAUNE));
    t = definirTriangle3d(tab_points[3],tab_points[7],tab_points[11]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_VERT));
    t = definirTriangle3d(tab_points[9],tab_points[10],tab_points[11]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_JAUNE));
    t = definirTriangle3d(tab_points[0],tab_points[3],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,ROUGEF));
    t = definirTriangle3d(tab_points[6],tab_points[9],tab_points[10]);
    __insere_tete(pt_objet,__cree_maillon(t,ROUGEF));
    // Faces latérales
    t = definirTriangle3d(tab_points[3],tab_points[8],tab_points[11]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_VERT));
    t = definirTriangle3d(tab_points[11],tab_points[8],tab_points[9]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_JAUNE));
    t = definirTriangle3d(tab_points[0],tab_points[2],tab_points[5]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_ORANGE));
    t = definirTriangle3d(tab_points[2],tab_points[5],tab_points[6]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_ORANGE));
    // Faces couronne arrière
    t = definirTriangle3d(tab_points[0],tab_points[1],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_VERT));
    t = definirTriangle3d(tab_points[4],tab_points[6],tab_points[9]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_BLANC));
    t = definirTriangle3d(tab_points[1],tab_points[2],tab_points[0]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_ORANGE));
    t = definirTriangle3d(tab_points[1],tab_points[8],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_JAUNE));
    t = definirTriangle3d(tab_points[4],tab_points[2],tab_points[6]);
    __insere_tete(pt_objet,__cree_maillon(t,ROUGEF));
    t = definirTriangle3d(tab_points[4],tab_points[8],tab_points[9]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_BLEU));
    // Font
    t = definirTriangle3d(tab_points[1],tab_points[4],tab_points[2]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_BLEU));
    t = definirTriangle3d(tab_points[1],tab_points[4],tab_points[8]);
    __insere_tete(pt_objet,__cree_maillon(t,RC_BLANC));

    // On libère les 12 points sommets de l'icosaèdre
    for (i=0;i<12;i++) {
        free(tab_points[i]);
    }

	return pt_objet;
}

t_objet3d* __transfo_face(t_maillon *pt_maillon,double r)
{
    /*
      Le but est de prendre le milieu de chaque côté du triangle
      et de projeter ces points sur la sphère circonscrite de l'objet
      ensuite on crée les 4 nouveaux triangles contenus à l'intérieur de l'ancien.
    */
	t_objet3d *pt_objet = NULL;
	t_triangle3d *t = NULL;
	t_point3d *tab[6];
	double norme;
	int i;

	pt_objet = objet_vide();
	tab[0] = pt_maillon->face->abc[0];
	tab[2] = pt_maillon->face->abc[1];
	tab[4] = pt_maillon->face->abc[2];

	norme = sqrt(pow((tab[0]->xyzt[0]+tab[2]->xyzt[0])/2,2)+pow((tab[0]->xyzt[1]+tab[2]->xyzt[1])/2,2)+pow((tab[0]->xyzt[2]+tab[2]->xyzt[2])/2,2));
    tab[1] = definirPoint3d(r*(tab[0]->xyzt[0]+tab[2]->xyzt[0])/2/norme,r*(tab[0]->xyzt[1]+tab[2]->xyzt[1])/2/norme,r*(tab[0]->xyzt[2]+tab[2]->xyzt[2])/2/norme);
    tab[3] = definirPoint3d(r*(tab[2]->xyzt[0]+tab[4]->xyzt[0])/2/norme,r*(tab[2]->xyzt[1]+tab[4]->xyzt[1])/2/norme,r*(tab[2]->xyzt[2]+tab[4]->xyzt[2])/2/norme);
    tab[5] = definirPoint3d(r*(tab[4]->xyzt[0]+tab[0]->xyzt[0])/2/norme,r*(tab[4]->xyzt[1]+tab[0]->xyzt[1])/2/norme,r*(tab[4]->xyzt[2]+tab[0]->xyzt[2])/2/norme);

	t = definirTriangle3d(tab[0],tab[1],tab[5]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(rand()%1275)));
    t = definirTriangle3d(tab[1],tab[2],tab[3]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(rand()%1275)));
    t = definirTriangle3d(tab[3],tab[4],tab[5]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(rand()%1275)));
    t = definirTriangle3d(tab[1],tab[3],tab[5]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(rand()%1275)));

    // On libère l'ancien triangle
    libererTriangle3d(pt_maillon->face);
    // Et les points du tableau (non déjà libérés)
    free(tab[1]);
    free(tab[3]);
    free(tab[5]);

	return pt_objet;
}

t_objet3d* sphere(double r, int n)
{
	t_objet3d *pt_objet = NULL;
    t_maillon *pt_maillon;
    t_maillon *pt_maillon_free;
    int i;

	pt_objet = objet_vide();
	// On part d'un icosaèdre...
    pt_objet = icosaedre(r*2/sqrt(2+PHI));
    // ...puis on divise les triangles en 4 (à chaque passage de boucle)
    if (n>5) {n=5;}; // /!\ pb de temps de calcul à partir de i<6
    for (i=0;i<n;i++) {
        pt_maillon = pt_objet->tete;
        pt_objet = objet_vide();
        while (pt_maillon != NULL) {
            pt_maillon_free = pt_maillon;
            composerObjet3d(pt_objet,__transfo_face(pt_maillon,r));
            libererTriangle3d(pt_maillon->face);
            pt_maillon = pt_maillon->pt_suiv;
            // On libère l'ancien maillon
            free(pt_maillon_free);
        }
    }

	return pt_objet;
}

t_objet3d* sphere_amiga(double r, double nlat, double nlong)
{
	t_objet3d *pt_objet = NULL;
	t_triangle3d *t = NULL;
	t_point3d *tab[4];
	int i,j;
	Uint32 c;
	double theta_i1,theta_i2,phi_j1,phi_j2;

	pt_objet = objet_vide();

	for (i=0;i<nlat;i++) {
        for (j=0;j<nlong;j++) {

            theta_i1 = M_PI*(i/nlat)-M_PI/2;
            phi_j1 = 2*M_PI*(j/nlong)-M_PI;
            theta_i2 = M_PI*((i+1)/nlat)-M_PI/2;
            phi_j2 = 2*M_PI*((j+1)/nlong)-M_PI;

            tab[0] = definirPoint3d(r*cos(theta_i1)*cos(phi_j1),r*cos(theta_i1)*sin(phi_j1),r*sin(theta_i1));
            tab[1] = definirPoint3d(r*cos(theta_i1)*cos(phi_j2),r*cos(theta_i1)*sin(phi_j2),r*sin(theta_i1));
            tab[2] = definirPoint3d(r*cos(theta_i2)*cos(phi_j1),r*cos(theta_i2)*sin(phi_j1),r*sin(theta_i2));
            tab[3] = definirPoint3d(r*cos(theta_i2)*cos(phi_j2),r*cos(theta_i2)*sin(phi_j2),r*sin(theta_i2));

            if (i%2 == j%2) {
                c = ROUGEC;
            } else {
                c = BLANC;
            }
            if (i!=0) {
                t = definirTriangle3d(tab[0],tab[1],tab[2]);
                __insere_tete(pt_objet,__cree_maillon(t,c));
            }
            if (i!=nlat-1) {
                t = definirTriangle3d(tab[1],tab[2],tab[3]);
                __insere_tete(pt_objet,__cree_maillon(t,c));
            }

        }
	}

	return pt_objet;
}

t_objet3d* sapin(double h, double l)
{
	t_objet3d *pt_objet = NULL;
	t_objet3d *otmp = NULL;
	t_objet3d *boule = sphere(h/32,3);
	t_point3d *tab[9];
	t_point3d *pt_point_tmp;
    t_triangle3d *t = NULL;
    Uint32 c[6];
    int i;

	pt_objet = objet_vide();
	otmp = objet_vide();

    // Sommet + Branches
    tab[0] = definirPoint3d(0,0,h);
    tab[1] = definirPoint3d(-l/4,0,3*h/4);
    tab[2] = definirPoint3d(l/4,0,3*h/4);

    tab[3] = definirPoint3d(0,0,7*h/8);
    tab[4] = definirPoint3d(-3*l/8,0,h/2);
    tab[5] = definirPoint3d(3*l/8,0,h/2);

    tab[6] = definirPoint3d(0,0,5*h/8);
    tab[7] = definirPoint3d(-l/2,0,h/4);
    tab[8] = definirPoint3d(l/2,0,h/4);

    // Ajout branches
	 t = definirTriangle3d(tab[0],tab[1],tab[2]);
    __insere_tete(pt_objet,__cree_maillon(t,VERT_SAPIN));
    t = definirTriangle3d(tab[3],tab[4],tab[5]);
    __insere_tete(pt_objet,__cree_maillon(t,VERT_SAPIN));
    t = definirTriangle3d(tab[6],tab[7],tab[8]);
    __insere_tete(pt_objet,__cree_maillon(t,VERT_SAPIN));

    // Ajout boules
    otmp = copierObjet3d(boule);
    pt_point_tmp = definirPoint3d(-l/4,0,3*h/4-h/32);
    translationObjet3d(otmp,pt_point_tmp);
    free(pt_point_tmp);
    composerObjet3d(pt_objet,otmp);
    otmp = copierObjet3d(boule);
    pt_point_tmp = definirPoint3d(l/4,0,3*h/4-h/32);
    translationObjet3d(otmp,pt_point_tmp);
    free(pt_point_tmp);
    composerObjet3d(pt_objet,otmp);
    otmp = copierObjet3d(boule);
    pt_point_tmp = definirPoint3d(-3*l/8,0,h/2-h/32);
    translationObjet3d(otmp,pt_point_tmp);
    free(pt_point_tmp);
    composerObjet3d(pt_objet,otmp);
    otmp = copierObjet3d(boule);
    pt_point_tmp = definirPoint3d(3*l/8,0,h/2-h/32);
    translationObjet3d(otmp,pt_point_tmp);
    free(pt_point_tmp);
    composerObjet3d(pt_objet,otmp);
    otmp = copierObjet3d(boule);
    pt_point_tmp = definirPoint3d(-l/2,0,h/4-h/32);
    translationObjet3d(otmp,pt_point_tmp);
    free(pt_point_tmp);
    composerObjet3d(pt_objet,otmp);
    otmp = copierObjet3d(boule);
    pt_point_tmp = definirPoint3d(l/2,0,h/4-h/32);
    translationObjet3d(otmp,pt_point_tmp);
    free(pt_point_tmp);
    composerObjet3d(pt_objet,otmp);

    // Ajout de la 3eme dimension
    otmp = copierObjet3d(pt_objet);
    pt_point_tmp = definirPoint3d(0,0,0);
    rotationObjet3d(otmp,pt_point_tmp,0,0,-90);
    free(pt_point_tmp);
    composerObjet3d(pt_objet,otmp);

    // Ajout du tronc
    for (i=0;i<6;i++) {
        c[i] = TRONC;
    }
    otmp = parallelepipede(h/16,h/16,h/4,c);
    pt_point_tmp = definirPoint3d(-h/32,-h/32,0);
    translationObjet3d(otmp,pt_point_tmp);
    free(pt_point_tmp);
    composerObjet3d(pt_objet,otmp);

    // Réorienttion de l'arbre
    pt_point_tmp = definirPoint3d(0,0,0);
    rotationObjet3d(pt_objet,pt_point_tmp,90,0,0);
    free(pt_point_tmp);

    // On libère les points du tableau
    for (i=0;i<9;i++) {
        free(tab[i]);
    }

	return pt_objet;

}

t_objet3d* arbre(double lx, double ly, double lz)
{
	t_objet3d *pt_objet = NULL;

	pt_objet = objet_vide();
	// TODO

	return pt_objet;

}

t_objet3d* damier(double Lx, double Ly, double nx, double ny)
{
	t_objet3d *pt_objet = NULL;
	t_triangle3d *t = NULL;
	t_point3d *tab[4];
	int i,j;
	Uint32 c;
	double lx = Lx/nx;
	double ly = Ly/ny;

	pt_objet = objet_vide();

	for (i=0;i<nx;i++) {
        for (j=0;j<ny;j++) {

            tab[0] = definirPoint3d(i*lx,j*ly,0);
            tab[1] = definirPoint3d(i*lx,(j+1)*ly,0);
            tab[2] = definirPoint3d((i+1)*lx,(j+1)*ly,0);
            tab[3] = definirPoint3d((i+1)*lx,j*ly,0);

            if (i%2 == j%2) {
                c = NOIR;
            } else {
                c = BLANC;
            }

            t = definirTriangle3d(tab[0],tab[1],tab[2]);
            __insere_tete(pt_objet,__cree_maillon(t,c));
            t = definirTriangle3d(tab[0],tab[2],tab[3]);
            __insere_tete(pt_objet,__cree_maillon(t,c));

        }
	}

	return pt_objet;

}

double __max(double a, double b, double c)
{
    if (a>b) {
        b = a;
    }
    if (b>c) {
        c = b;
    }
    return c;
}

double __min(double a, double b, double c)
{
    if (a<b) {
        b = a;
    }
    if (b<c) {
        c = b;
    }
    return c;
}

t_point3d *centreObjet3d(t_objet3d *o)
{
    // Contiendra les coordonnées max de l'objet
    t_point3d *p3d_max = NULL;
    // Contiendra les coordonnées min de l'objet
    t_point3d *p3d_min = NULL;
    // Contiendra les coordonnées moyennes de l'objet
    t_point3d *p3d = NULL;
    t_maillon *mtmp;
    t_triangle3d *ttmp;
    int i,j;
    // On initialise à l'origine du repère (ie si un objet est vide son centre correspond à l'origine du repère)
    p3d = definirPoint3d(0, 0, 0);

    if (o != NULL && o->tete != NULL) {

            mtmp = o->tete;

            // init p3d_max et p3d_min
            ttmp = mtmp->face;
            p3d_max = definirPoint3d(0, 0, 0);
            p3d_min = definirPoint3d(0, 0, 0);

            for (i=0;i<3;i++){
                p3d_max->xyzt[i] = __max( ttmp->abc[0]->xyzt[i], ttmp->abc[1]->xyzt[i], ttmp->abc[2]->xyzt[i]);
                p3d_min->xyzt[i] = __min( ttmp->abc[0]->xyzt[i], ttmp->abc[1]->xyzt[i], ttmp->abc[2]->xyzt[i]);
            }

            mtmp = mtmp->pt_suiv;

            // On met dans p3d_min (resp. p3d_max) le min (resp. max) de chaque coordonnées de l'ensemble des sommets de l'objet
            while(mtmp != NULL) {
                ttmp = mtmp->face;
                // On compare chaque coordonnée des 3 pts du triangle en cours aux coordonnées min/max
                for (i=0;i<3;i++){
                    for (j=0;j<3;j++){
                        if (p3d_max->xyzt[i] < ttmp->abc[j]->xyzt[i]){
                            p3d_max->xyzt[i] = ttmp->abc[j]->xyzt[i];
                        }
                        if (p3d_min->xyzt[i] > ttmp->abc[j]->xyzt[i]){
                            p3d_min->xyzt[i] = ttmp->abc[j]->xyzt[i];
                        }

                    }
                }
                mtmp = mtmp->pt_suiv;
            }

            // On insère dans p3d les coordonnées moyennes de l'objet
            for (i=0;i<3;i++){
                p3d->xyzt[i] = (p3d_max->xyzt[i] + p3d_min->xyzt[i])/2;
            }

            // On libère les points temp
            free(p3d_max);
            free(p3d_min);
    }

    return p3d;
}

t_objet3d *copierObjet3d(t_objet3d *o) // attention, effectue une copie mirroir
{
	t_objet3d *n = objet_vide();
	t_maillon *pt_maillon; // Parcourt la liste de maillons

	if (o != NULL) {
        pt_maillon = o->tete;
        while (pt_maillon != NULL) { // /!\ 2
            __insere_tete(n,__cree_maillon(copierTriangle3d(pt_maillon->face),pt_maillon->couleur)); // On insère en tête de n un nouveau maillon identique à celui pointé par pt_maillon
            pt_maillon = pt_maillon->pt_suiv;
        }
	}

	return n;
}

void composerObjet3d(t_objet3d* o, t_objet3d* o2) /* o = o+o2, o2 ne signifiera plus rien */
{
    t_maillon *pt_maillon;
    if (o != NULL && o2 != NULL) {
        if (o->tete == NULL) {
            o->est_trie = o2->est_trie;
            o->tete = o2->tete;
        } else if (o2->tete != NULL) {
            pt_maillon = o->tete;
            while (pt_maillon->pt_suiv != NULL) {
                pt_maillon = pt_maillon->pt_suiv;
            }
            o->est_trie = o->est_trie && o2->est_trie && zmoyen(pt_maillon->face)>=zmoyen((o2->tete)->face);
            pt_maillon->pt_suiv = o2->tete;
        }
        free(o2);
    }
}

void libererObjet3d(t_objet3d *o)
{
    t_maillon *pt_maillon;
    t_maillon *pt_maillon_to_free;
	if (o != NULL) {
        pt_maillon = o->tete;
        while (pt_maillon != NULL) {
            pt_maillon_to_free = pt_maillon;
            libererTriangle3d(pt_maillon->face);
            pt_maillon = pt_maillon->pt_suiv;
            free(pt_maillon_to_free);
        }
        free(o);
	}

}

t_maillon *__separer(t_maillon *pt_maillon)
{
    /* La moitié des maillons reste pointée par le pointeur donné en paramètre,
       l'autre moitié est pointée par la pointeur que retourne la fonction */
    t_maillon * pt_maillon_tmp;

    if (pt_maillon == NULL || pt_maillon->pt_suiv == NULL) {
        return NULL;
    } else {
        // On sépare les maillons pairs/impairs
        pt_maillon_tmp = pt_maillon->pt_suiv;
        pt_maillon->pt_suiv = pt_maillon_tmp->pt_suiv;
        pt_maillon_tmp->pt_suiv = __separer(pt_maillon_tmp->pt_suiv);
        return pt_maillon_tmp;
    }
}

t_maillon *__fusion (t_maillon * pt_maillon1, t_maillon * pt_maillon2)
{
     // On fusionne en comparant les zmoyen des têtes de listes
        if (pt_maillon1 == NULL) {
            return pt_maillon2;
        } else if (pt_maillon2 == NULL) {
            return pt_maillon1;
        } else if (zmoyen(pt_maillon1->face)>=zmoyen(pt_maillon2->face)) {
            pt_maillon1->pt_suiv = __fusion(pt_maillon1->pt_suiv,pt_maillon2);
            return pt_maillon1;
        } else {
            pt_maillon2->pt_suiv = __fusion(pt_maillon1,pt_maillon2->pt_suiv);
            return pt_maillon2;
        }
}

void __trier_maillons (t_maillon ** ppt_maillon)
{
    // Principe du tri fusion sur une liste chainée
    t_maillon * pt_maillon_tmp;

    if (*ppt_maillon != NULL && (*ppt_maillon)->pt_suiv != NULL) {
        // On sépare la liste en deux
        pt_maillon_tmp = __separer(*ppt_maillon);
        // On trie les deux sous listes
        __trier_maillons(ppt_maillon);
        __trier_maillons(&pt_maillon_tmp);
        // On fusionne les deux sous listes triées
        *ppt_maillon = __fusion(*ppt_maillon,pt_maillon_tmp);
    }

}

void __trier_objet(t_objet3d *pt_objet)
{
    if (pt_objet != NULL && pt_objet->est_trie == false) {
        // Les triangles sont triés suivant les zmoyen décroissants
        __trier_maillons(&pt_objet->tete);
        pt_objet->est_trie = true;
    }
}

void dessinerObjet3dAux(t_surface *surface,t_maillon *pt_maillon)
{
    if (pt_maillon != NULL) {
        // On dessine les triangles dans l'ordre de la liste
        remplirTriangle3d(surface,pt_maillon->face,pt_maillon->couleur);
        dessinerObjet3dAux(surface,pt_maillon->pt_suiv);
    }
}

void dessinerObjet3d(t_surface *surface, t_objet3d* pt_objet)
{
	if (pt_objet != NULL ) {
        if (!pt_objet->est_trie) {
            // On trie l'objet si il n'est pas déjà trié
            __trier_objet(pt_objet);
        }
        // Ensuite on le dessine
        dessinerObjet3dAux(surface,pt_objet->tete);
    }
}

void translationObjet3d(t_objet3d* pt_objet, t_point3d *vecteur)
{
    double mt[4][4];
    int i;

	if (pt_objet != NULL && vecteur != NULL) {

        // initialisation de la matrice de translation
        __init_m(mt);
        for(i=0;i<4;i++){
            mt[i][3] = vecteur->xyzt[i];
            mt[i][i] = 1;
        }

        transformationObjet3d(pt_objet,mt);
	}
}

void rotationObjet3d(t_objet3d* pt_objet, t_point3d *centre, float degreX, float degreY, float degreZ)
{
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
        mres[i][3] = centre->xyzt[i];
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
    __inversion_3(centre);
	__init_m(m);
    for(i=0;i<4;i++){
        m[i][3] = centre->xyzt[i];
        m[i][i] = 1;
    }
    multiplicationMatrice3d(mres,mtmp,m);
    __inversion_3(centre);

    // On applique une tranformation à l'aide de la matrice de rotation mres
     transformationObjet3d(pt_objet,mres);

	// Seule la rotation modifie le zmoyen d'un triangle
	pt_objet->est_trie = false;

}

void transformationObjet3d(t_objet3d* pt_objet, double mat[4][4])
{
    t_maillon *pt_maillon;

    pt_maillon = pt_objet->tete;
	while (pt_maillon != NULL) {
            transformationTriangle3d(pt_maillon->face,mat);
            pt_maillon = pt_maillon->pt_suiv;
    }
}
