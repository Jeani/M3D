#ifndef LIB_MAT_H_
#define LIB_MAT_H_

void multiplicationVecteur3d(t_point3d *v1, double m[4][4], t_point3d *v2); // v1 = m*v2
void multiplicationMatrice3d(double m1[4][4], double m2[4][4], double m3[4][4]); // m1 = m2*m3
void copierMatrice3d(double m1[4][4], double m2[4][4]); // m1 = m2

#endif
