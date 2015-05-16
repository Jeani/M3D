#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "lib_surface.h"
#include "lib_2d.h"
#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_scene.h"
#include "lib_mat.h"

void pause();
void infoPoint(FILE *, t_point3d *);
void infoTriangle(FILE *, t_triangle3d *);
void infoMaillon (FILE * pf, t_maillon * pt_maillon);
void infoScene (FILE * pf, t_scene * pt_scene);

int main(int argc,char** argv)
{
    t_surface *surface=NULL;
    FILE * pfile = fopen("test.txt","w");
    int cpt = 0;
    int timestart;
    char buf[50];
    double PHI = (1+sqrt(5))/2;
    X = RX/2;
    Y = RY/2;

   t_point3d *p10 = definirPoint3d(200,200,300), *p20 = definirPoint3d(375,200,25), *p30 = definirPoint3d(-50,0,50), *p = definirPoint3d(300,200,0), *p2 = definirPoint3d(1,2,1);
    t_point3d *p11 = definirPoint3d(200,200,0), *p21 = definirPoint3d(375,200,-25), *p31 = definirPoint3d(0,0,-50), *pi = definirPoint3d(100,100,100), *pj = definirPoint3d(75,75,115);
   t_triangle3d *t10 = definirTriangle3d(p, p20, p30), *t11 = definirTriangle3d(p,p21,p31);
    t_point3d *centre10, *centre11, *vect, *vect2, *v1, *v2;

   t_point3d *origine = definirPoint3d(0,0,0), *vecteur;
     t_objet3d *o10 = objet_vide(), *o11 = objet_vide(), *o12 = objet_vide(), *o13 = objet_vide(), *o14 = objet_vide(), *o15 = objet_vide();
     t_maillon * pt_maillon;
     t_scene * scene = scene_vide();

//    rotationObjet3d(o10, origine, 90, 0,0);

    double ps = 50;
    int i=0;
    int lx = 50;
    int ly = 80;
    int lz = 150;
    Uint32 c[6];

    SDL_Init(SDL_INIT_VIDEO);
    surface=creerFenetre(RX,RY);
    timestart = SDL_GetTicks();

    c[0] = RC_BLANC;
    c[4] = RC_ROUGE;
    //c[8] = echelle_de_couleur(200);
    c[1] = RC_BLEU;
    c[5] = RC_VERT;
   // c[6] = echelle_de_couleur(500);
    c[2] = RC_JAUNE;
    c[3] = RC_ORANGE;
    //c[7] = echelle_de_couleur(800);

    //o10 = (300,3);
    //o10 = sapin(200,100);
    //libererObjet3d(o10);
    v1 = definirPoint3d(2,50,-3);
    vect = definirPoint3d(0,0,-1);
    //vect2 = definirPoint3d(0,0,2);
    o10 = sphere(50,1);
    o11 = carre(50,RC_ORANGE);
    o12 = parallelepipede(50,70,1000,c);
    o13 = sphere(70,2);
    o14 = rubiks(40);
    o15 = tetraedre(60);

    translationObjet3d(o10,pj);
    insererScene(scene,o10);

    translationObjet3d(o11,pi);
    insererScene(scene,o11);

    translationObjet3d(o12,p10);
    insererScene(scene,o12);

    translationObjet3d(o13,p20);
    insererScene(scene,o13);

    translationObjet3d(o14,p30);
    insererScene(scene,o14);

    translationObjet3d(o15,p31);
    insererScene(scene,o15);
    infoScene(pfile,scene);
   // dessinerObjet3d(surface, o10);
    // dessinerObjet3d(surface, o11);
    //translationObjet3d(o10,pi);
    //translationObjet3d(o11,pi);
   // fprintf(pfile, "%lf", prod_scal(p2,vect));
    dessinerScene(surface, scene);
    majEcran(surface);
    pause();
    /*effacerFenetre(surface, 0);
    rotationScene(&scene,origine,0,0,180);
    infoScene(pfile,scene);
    dessinerScene(surface, scene);
    majEcran(surface);
    pause();*/

    while (i<1000) {
        effacerFenetre(surface, 0);
        rotationScene(&scene,pi,2,-1.5,-1);
        //rotationObjet3dScene(scene,o13,pi,-2,-1.5,-1);
        //translationScene(&scene,vect);
        //infoScene(pfile,scene);
       // translationObjet3dScene(scene,o10,vect2);
       // translationObjet3d(o10,pi);
        //rotationObjet3d(o11,pi,0,0,0.1);
        //infoTriangle(pfile,t11);
        dessinerScene(surface, scene);
        majEcran(surface);
        //pause();
        SDL_Delay(1000/60);
        i++;
    }




//#define T2D
//#define T3D
//#define O3D

#ifdef T2D
/*
 * etape 1 : ce ne sont que des exemples
 */
        remplirTriangle2d(surface, t1, echelle_de_couleur(30*i));
        SDL_Delay(1000);

#endif
#ifdef T3D
/*
 * etape 2 : ce ne sont que des exemples
 */

    remplirTriangle3d(surface, t10, echelle_de_couleur(30));
    majEcran(surface);
    pause();

    while (i<9) {
        effacerFenetre(surface, 0);
        rotationTriangle3d(t10,p10,0,0,10);
        remplirTriangle3d(surface, t10, echelle_de_couleur(40));
        majEcran(surface);
        pause();
        i++;
    }

#endif
#ifdef O3D
/*
 * etape 3 : ce ne sont que des exemples
 */

    o10 = parallelepipede(lx, ly, lz);
    o11 = parallelepipede(lz, lx, ly);
    dessinerObjet3d(surface, o10);
    majEcran(surface);
    pause();
    effacerFenetre(surface, 0);
    translationObjet3d(o10,p);
    dessinerObjet3d(surface, o10);
    majEcran(surface);
    pause();
    dessinerObjet3d(surface, o11);
    majEcran(surface);
    pause();
    effacerFenetre(surface, 0);
    translationObjet3d(o11,p2);
    dessinerObjet3d(surface, o10);
    dessinerObjet3d(surface, o11);
    majEcran(surface);
    pause();
    effacerFenetre(surface, 0);
    composerObjet3d(o10,o11);
    rotationObjet3d(o10,p10,30,-30,0);
    dessinerObjet3d(surface, o10);
    majEcran(surface);
    pause();*/
#endif

/*


        cpt++;
        sprintf(buf,"%d FPS",(int)(cpt*1000.0/(SDL_GetTicks()-timestart)));
        SDL_WM_SetCaption(buf,NULL);

    }*/
    SDL_Quit();
    fclose(pfile);
    return 0;
}

void pause()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}

void infoPoint (FILE * pf, t_point3d * p) {
    int i = 0;
    fprintf(pf, "(");
    for (i=0;i<3;i++) {
        fprintf(pf, "%lf, ", p->xyzt[i]);
    }
    fprintf(pf, "%lf)\n", p->xyzt[3]);
}

void infoTriangle (FILE * pf, t_triangle3d * t) {
    int i = 0;
    for (i=0;i<3;i++) {
        fprintf(pf, "p%d: (%lf, %lf, %lf, %lf)\n", i, t->abc[i]->xyzt[0], t->abc[i]->xyzt[1], t->abc[i]->xyzt[2], t->abc[i]->xyzt[3]);
    }
}

void infoMaillon (FILE * pf, t_maillon * pt_maillon) {
    infoTriangle(pf, pt_maillon->face);
    fprintf(pf, "couleur : %d\n",pt_maillon->couleur);
    if (pt_maillon->pt_suiv == NULL) {
        fprintf(pf, "pt_suiv : NULL\n");
    } else {
        fprintf(pf, "pt_suiv : %p\n",pt_maillon->pt_suiv);
    }
}

void infoScene (FILE * pf, t_scene * pt_scene) {
    if (pt_scene == NULL) {
        fprintf(pf, "NULL");
    } else {
        fprintf(pf, "<");
        infoPoint(pf,pt_scene->centre);
        fprintf(pf, ",");
        infoScene(pf,pt_scene->objet_suiv_g);
        fprintf(pf, ",");
        infoScene(pf,pt_scene->objet_suiv_d);
        fprintf(pf, ">");
    }
}
