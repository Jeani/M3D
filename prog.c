#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "lib_surface.h"
#include "lib_2d.h"
#include "lib_3d.h"
#include "lib_objet3d.h"

void pause();
void infoPoint(FILE *, t_point3d *);
void infoTriangle(FILE *, t_triangle3d *);
void infoMaillon (FILE * pf, t_maillon * pt_maillon);

int main(int argc,char** argv)
{
    t_surface *surface=NULL;
    FILE * pfile = fopen("test.txt","w");
    int cpt = 0;
    int timestart;
    char buf[50];


//    t_point2d *p1 = definirPoint2d(10,50), *p2 = definirPoint2d(100,240), *p3 = definirPoint2d(50,300);
//    t_triangle2d *t1 = definirTriangle2d(p1, p2, p3);

   t_point3d *p10 = definirPoint3d(200,200,100), *p20 = definirPoint3d(375,200,25), *p30 = definirPoint3d(450,300,50), *p = definirPoint3d(300,200,0), *p2 = definirPoint3d(400,300,100);
    t_point3d *p11 = definirPoint3d(200,200,0), *p21 = definirPoint3d(375,200,-25), *p31 = definirPoint3d(450,300,-50);
   t_triangle3d *t10 = definirTriangle3d(p, p20, p30), *t11 = definirTriangle3d(p,p21,p31);

//    t_point3d *origine = definirPoint3d(0,0,0), *vecteur;
     t_objet3d *o10 = objet_vide(), *o11 = objet_vide();
     t_maillon * pt_maillon;

//    rotationObjet3d(o10, origine, 90, 0,0);

    int i=0;
    int lx = 50;
    int ly = 80;
    int lz = 150;
    Uint32 c[9];

    SDL_Init(SDL_INIT_VIDEO);
    surface=creerFenetre(RX,RY);
    timestart = SDL_GetTicks();

    c[0] = echelle_de_couleur(0);
    c[4] = echelle_de_couleur(100);
    c[8] = echelle_de_couleur(200);
    c[1] = echelle_de_couleur(300);
    c[5] = echelle_de_couleur(400);
    c[6] = echelle_de_couleur(500);
    c[2] = echelle_de_couleur(600);
    c[3] = echelle_de_couleur(700);
    c[7] = echelle_de_couleur(800);

    //__insere_tete(o10,__cree_maillon(t10,echelle_de_couleur(210)));
    //__insere_tete(o10,__cree_maillon(t11,echelle_de_couleur(30)));

    o10 = rubiks(200);
    translationObjet3d(o10,p);
    dessinerObjet3d(surface, o10);
    //remplirTriangle3d(surface,t10,VERTF);
    majEcran(surface);
    pause();
    //infoTriangle(pfile,t11);

    while (1) {
        effacerFenetre(surface, 0);
        rotationObjet3d(o10,p2,1.5,1,2);
        //infoTriangle(pfile,t11);
        dessinerObjet3d(surface, o10);
        majEcran(surface);
        SDL_Delay(1000/50);
        //i++;
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
