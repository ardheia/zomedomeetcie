#ifndef MATHEMA_H
#define MATHEMA_H

#include "complexes.h"
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix3x3>
/*
  la on a toutes nos fonctions mathematiques, ce n est pas une classe mais plein de fonctions
  appelables de partout
*/


//fonctions angles
double radians(double angledeg);
double degres(double anglerad);

double absol(double nombre);
int absol(int nombre2);
int compte(QVector<short> tab,int i);
bool pair(int h);
int signe(double h);

double determinantMatrice3par3(QMatrix3x3 matr);

//un peu de methode de cardan pour le 3eme degre ay3+by2+cy+d=0
QVector<complexes> resolutionDegre3Cardan(double a,double b,double c,double d);

//resolution equation du 4 eme degre az4+bz3+cz2+dz+e=0 d'apres ferrari ou lagrange
QVector<double> resolutionDegre4Ferrari(double a,double b,double c,double d,double e);
QVector<complexes> resolutionDegre4Lagrange(double a,double b,double c,double d,double e);


//fonctions geometriques


//conversion de matrice de rotation en quaternion
QQuaternion convertirMatriceRotation(QMatrix4x4 matr);

//rotation3D d'un point grace a un quatrenion
QVector3D rotationSelonQuaternion(QVector3D Vinit, QQuaternion quat);

//fonctions 3D
QVector3D milieu2Points3D(QVector3D a, QVector3D b);
QVector3D centreCercle3Points3D(QVector3D b, QVector3D a, QVector3D c);
double rayonCercle3Points3D(QVector3D a, QVector3D b, QVector3D c);
QVector3D vecteurNormalNorme3Points3D(QVector3D a, QVector3D b, QVector3D c);
QVector3D intersection2Plans1Sphere3D(QVector3D pointPlan1, QVector3D VecteurNormalPlan1, QVector3D pointPlan2, QVector3D VecteurNormalPlan2, QVector3D centreSphere, double rayonSphere);
QVector3D intersection1Plan1CercleDansPlanOxz3D(QVector3D pointPlan, QVector3D VecteurNormalPlan, QVector3D centreCercle, double rayonCercle);
double angleEntre2Vecteurs3D(QVector3D a, QVector3D b);
double valeurLaPlusAGauche(QVector3D a, QVector3D b, QVector3D c, QVector3D d);
double valeurLaPlusADroite(QVector3D a, QVector3D b, QVector3D c, QVector3D d);
double valeurLaPlusEnBas(QVector3D a, QVector3D b, QVector3D c, QVector3D d);
double angleHorizontalCentrePoint3D(QVector3D centre, QVector3D point);
double anglePhiHorizontalEntre2PointsEt1Centre3D(QVector3D centre, QVector3D point1, QVector3D point2);
QVector3D rotationAutourDeZ(QVector3D a, double phi);
QVector3D rotationAutourDeZetCentre(QVector3D point, double ang, QVector3D centre);
QVector3D intersection1CylindreSelonOzEtUnCercleDansUnPlan(double rayonCylindre, double rayonCercle, QVector3D centreCercle, QVector3D pointPlan, QVector3D VNplan);


//fonctions 2D
QVector3D intersection2DroitesPlanOxy(QVector3D point1droite1, QVector3D point2droite1, QVector3D point1droite2, QVector3D point2droite2);
QVector3D intersectionDroite1Cercle2D(QVector3D premierPointDroite, QVector3D deuxiemePointDroite, QVector3D centreCercle, double rayonCercle);
QVector3D intersection2Cercles2D(QVector3D centrePremierCercle, double rayonPremierCercle, QVector3D centreDeuxiemeCercle, double rayonDeuxiemeCercle, int positionGaucheouDroiteRapportCentrePremierCercle);
QVector2D milieu2Points2D(QVector2D a, QVector2D b);
QVector2D centreCercle2Points2DavecRayon1(QVector2D a, QVector2D b, double rayon);
QVector2D centreCercle2Points2DavecRayon2(QVector2D a, QVector2D b, double rayon);
QVector2D centreCercle3Points2D(QVector2D a, QVector2D b, QVector2D c);
double rayonCercle3Points2D(QVector2D a, QVector2D b, QVector2D c);
double angleHorizontalCentrePoint2D(QVector2D centre, QVector2D point);
double angleHorizontalCentrePoint2D_2(QVector2D centre, QVector2D point);
QVector3D intersectionDroiteAvecDroiteHorizontale(QVector3D premierPointDroite, QVector3D deuxiemePointDroite, double YdeLaDroiteHorizontale);
QVector2D rotation2DautourOrigine(QVector2D a,double angle);
double angleDeDifferenceDuPremierVersLeDeuxieme(double premierAngle, double secondAngle, bool horaire);

//constantes
double const pi=3.14159265;

#endif // MATHEMA_H
