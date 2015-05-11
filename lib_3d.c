#include "lib_surface.h"
#include "lib_3d.h"
#include "lib_2d.h"
#include "lib_mat.h"
#define M_PI 3.14159265358979323846;
#include <math.h>

const double mproj[2][4] = {1,0,0,RX/2,0,1,0,RY/2};

typedef struct
{
	double m[4][4];
} t_matrice3d;

t_point3d *definirPoint3d(double x, double y, double z)	// attention malloc
{
	t_point3d *p = NULL;

	p = (t_point3d *) malloc(sizeof(t_point3d));

	p->xyzt[0] = x;
	p->xyzt[1] = y;
	p->xyzt[2] = z;
	p->xyzt[3] = 1;

	return p;
}

t_point3d *__init_p3d(){
    t_point3d *p = NULL;
    int i;

    p = (t_point3d *) malloc(sizeof(t_point3d));

    for(i=0;i<4;i++){
        p->xyzt[i] = 0;
    }

    return p;
}

t_point3d *definirVecteur3d(double x, double y, double z)	// attention malloc
{
	t_point3d *p = NULL;

	p = (t_point3d *) malloc(sizeof(t_point3d));

	p->xyzt[0] = x;
	p->xyzt[1] = y;
	p->xyzt[2] = z;
	p->xyzt[3] = 0;

	return p;
}

void __copierPoint3d(t_point3d * src, t_point3d * dest)
{
	int i;
	for(i=0;i<4;i++){
        dest->xyzt[i] = src->xyzt[i];
    }
}

t_triangle3d *definirTriangle3d(t_point3d * a, t_point3d * b, t_point3d * c)	// attention malloc
{
	t_triangle3d *t = NULL;
    int i;

	t = (t_triangle3d *) malloc(sizeof(t_triangle3d));

	for(i=0;i<3;i++){
        t->abc[i] = (t_point3d *) malloc(sizeof(t_point3d));
	}
	__copierPoint3d(a, t->abc[0]);
	__copierPoint3d(b, t->abc[1]);
	__copierPoint3d(c, t->abc[2]);

	return t;
}

t_triangle3d *copierTriangle3d(t_triangle3d *t)
{
	t_triangle3d *n = NULL;
	int i;

	n = (t_triangle3d *) malloc(sizeof(t_triangle3d));
    for(i=0;i<3;i++){
        n->abc[i] = (t_point3d *) malloc(sizeof(t_point3d));
	}
	__copierPoint3d(t->abc[0], n->abc[0]);
	__copierPoint3d(t->abc[1], n->abc[1]);
	__copierPoint3d(t->abc[2], n->abc[2]);

	return n;

}

void libererTriangle3d(t_triangle3d *t)
{
    int i;
    for(i=0;i<3;i++){
        free(t->abc[i]);
    }

	free(t);
}

double __zvaleur(t_point3d *t)
{
	return t->xyzt[2];
}
double zmoyen(t_triangle3d *t)
{
	return ((__zvaleur(t->abc[0])+__zvaleur(t->abc[1])+__zvaleur(t->abc[2]))/3);
}

void __inversion_3(t_point3d *v1) // v1 = -v1 ! attention seulement sur 3 coord
{
	int i;

	for(i=0; i<3; i++)
	{
		v1->xyzt[i] = -v1->xyzt[i];
	}
}

void __addition_3(t_point3d *v1, t_point3d *v2) // v1 = v1+v2 ! attention seulement sur 3 coord
{
	int i;

	for(i=0; i<3; i++)
	{
		v1->xyzt[i] = v1->xyzt[i] + v2->xyzt[i];
	}
}

void __normalisation_vecteur(t_point3d *p3dtmp)
{
	int i;

	for(i=0; i<3; i++)
	{
		p3dtmp->xyzt[0] /= p3dtmp->xyzt[3];
		p3dtmp->xyzt[1] /= p3dtmp->xyzt[3];
		p3dtmp->xyzt[2] /= p3dtmp->xyzt[3];
	}
	p3dtmp->xyzt[3] = 1;
}

t_point2d *__conversion_2d_3d(t_point3d *p3d)
{
    t_point2d *p2d;
    //double m[2][4];
    int i, j;

    /* /!\ l'origine en 3D est au centre de l'écran !!! */

    // Définir m[2][4] : (1 0 1/D X/2)
    //                   (0 1 1/D Y/2)
    /*m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0; // 1/D
    m[0][3] = RX/2; // X/2
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0; // 1/D
    m[1][3] = RY/2; // Y/2*/


    p2d = (t_point2d *) malloc(sizeof(t_point2d));


	p2d->x = 0;
	p2d->y = 0;
    for(i=0;i<4;i++){
            p2d->x += (int) mproj[0][i] * p3d->xyzt[i];
            p2d->y += (int) mproj[1][i] * p3d->xyzt[i];
    }

	return p2d;
}

void remplirTriangle3d(t_surface * surface, t_triangle3d * triangle, Uint32 c)
{
	t_triangle2d *t;
	t_point2d *p2dtmp[3];
	int i;

    //On transforme le t3d en t2d
	for(i=0;i<3;i++){
        p2dtmp[i] = __conversion_2d_3d(triangle->abc[i]);
	}
	t = definirTriangle2d(p2dtmp[0],p2dtmp[1],p2dtmp[2]);

    // et on utilise la fct de remplissage d'un t2d
	remplirTriangle2d(surface,t,c);

	//liberation p2dtmp
    for(i=0;i<3;i++){
        free(p2dtmp[i]);
    }
}

void __init_m(double m[4][4]){
    int i,j;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            m[i][j] = 0;
        }
    }
}

void translationTriangle3d(t_triangle3d *t, t_point3d *vecteur)
{
	double mt[4][4];
	int i;
	__init_m(mt);

	for(i=0;i<4;i++){ // initialisation de la matrice de translation (vu en TD)
        mt[i][3] = vecteur->xyzt[i];
        mt[i][i] = 1;
	}
	transformationTriangle3d(t,mt);
}

void homothetieTriangle3d(t_triangle3d *t, double coef)
{
	double mt[4][4];
	__init_m(mt);

	// initialisation de la matrice de translation (vu en TD)
	mt[0][0] = coef;
	mt[1][1] = coef;
	mt[2][2] = coef;
    mt[3][3] = 1;

	transformationTriangle3d(t,mt);
}

float __conversion_deg_rad(float theta)
{
    return  theta*M_PI/180;
}

void rotationTriangle3d(t_triangle3d *t, t_point3d *centre, float degreX, float degreY, float degreZ)
{
    double m[4][4];
    int i,j;
    float theta;
    __init_m(m);
    m[3][3] = 1;

    __inversion_3(centre);
	translationTriangle3d(t,centre);

	// initialisation de la matrice de rotation selon x
	theta = __conversion_deg_rad(degreX);
    m[0][0] = 1;
    m[1][1] = cos(theta);
    m[1][2] = -1*sin(theta);
    m[2][1] = sin(theta);
    m[2][2] = cos(theta);
    transformationTriangle3d(t,m);

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
    transformationTriangle3d(t,m);

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
    transformationTriangle3d(t,m);

    __inversion_3(centre);
	translationTriangle3d(t,centre);
}

void transformationTriangle3d(t_triangle3d *t, double mat[4][4])
{
	t_triangle3d *ttrans = NULL;
	int i;

	ttrans = (t_triangle3d *) malloc(sizeof(t_triangle3d));
	for(i=0;i<3;i++){ // transformation de chaque point du triangle t dans ttrans
        ttrans->abc[i] = (t_point3d *) malloc(sizeof(t_point3d));
        multiplicationVecteur3d(ttrans->abc[i],mat,t->abc[i]); // affecte la valeur après transformation
	}

    // copie dans t de ttrans
    for(i=0;i<3;i++){ // copie des points du triangle ttrans dans t
        free(t->abc[i]); // On libère le point précédent
        t->abc[i] = ttrans->abc[i]; // On affecte le nouveau
    }
	free(ttrans); // on libère juste le triangle ttrans car les points sont réutilisés par t
}

