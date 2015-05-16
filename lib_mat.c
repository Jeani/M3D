#include "lib_mat.h"

void multiplicationVecteur3d(t_point3d *v1, double m[4][4], t_point3d *v2) // v1 = m*v2
{
	int i, j;
    for(i=0;i<4;i++){
        v1->xyzt[i] = 0;
        for(j=0;j<4;j++){
            v1->xyzt[i] += m[i][j] * v2->xyzt[j];
        }
    }
}

void multiplicationMatrice3d(double m1[4][4], double m2[4][4], double m3[4][4]) // m1 = m2*m3
{
	int i, j, k;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            m1[i][j] = 0;
            for(k=0;k<4;k++){
                m1[i][j] += m2[i][k] * m3[k][j];
            }
        }
    }
}

double prod_scal (t_point3d *v1,t_point3d *v2)
{
    double p = 0;
    int i;

    for (i=0;i<3;i++){
        p += v1->xyzt[i] * v2->xyzt[i];
    }

    return p;
}

/*void projection(t_point2d *p2d, double m[2][4], t_point3d *p3d) // p2d = m*p3d
{
	int i, j;
	p2d->x = 0;
	p2d->y = 0;
    for(i=0;i<4;i++){
            p2d->x += m[0][i] * p3d->xyzt[i];
            p2d->y += m[1][i] * p3d->xyzt[i];
    }
}*/

void copierMatrice3d(double m1[4][4], double m2[4][4]) // m1 = m2
{
	int i, j;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            m1[i][j] = m2[i][j];
        }
    }
}


