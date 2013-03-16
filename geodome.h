#ifndef GEODOME_H
#define GEODOME_H

#include <QVector2D>
#include <QVector3D>
#include <QQuaternion>
#include <QPaintDevice>

struct sommet
{
    QVector3D coord;
    int faceDeBase;
    int notX, notY;
    int nbVoisins;
    bool tronque;
    int v[6];
    int typ;
};

struct type_sommet
{
    QVector<int> somet;//quels sommets sont de ce type, permet de les compter
    QVector<int> fa;//un type de sommet se decrit par ses types de faces
    QVector<int> fa2; // une sorte d'orientation
    QVector<int> vect; //type de vecteurs
    bool complet; //certain sommet ne feront pas le tour
    QVector<double> ang; //les angles pas mis a plat
    QVector<double> angPl; // les angles mis a plat
};

struct triangle
{
    int s1,s2,s3;
    int ar1,ar2,ar3; // face voisines pour chaque arete
};

struct type_arete
{
    double longueur;
    QVector<int> ar;
    double angleCoupeBout;
};

struct arete
{
    int p1,p2;
    int typ;
};

struct face
{
    int s1,s2,s3;
    int ar1,ar2,ar3;//aretes dans le sens inverse aiguille
    int typ;
    int orient;
};

struct type_face
{
    int typar1,typar2,typar3;//longueur des aretes
    QVector<int> fa;
    double surface;
    double angle1,angle2,angle3;
    double h1;
};

class geodome
{
public:
    geodome();
    void setParametres(int g_solideBase, int g_frequenceA, int g_frequenceB, double g_diametre, double g_hauteur, int g_angleRotation, int g_sabliereHorizontale,  int g_affichage);
    int chercheFace(int ss1, int ss2, int num);
    bool chercheSommet(sommet sss);
    int cherchSommet(int ii, int jj);
    int chercheSommmetBoucle(int xx, int yy, int face);
    int faisTournerX(int face11, int face22, int xxx, int yyy);
    int faisTournerY(int face11, int face22, int xxx, int yyy);
    int faisTournerVect(int face11, int face22, int vect2);
    int TransfertX(int face11, int face22, int xxx, int yyy);
    int TransfertY(int face11, int face22, int xxx, int yyy);
    int TransfertVect(int face11, int face22, int vect2);
    bool ajoutSommet(int x, int y, int face, int vect, int i, int j);
    double hauteurMax();
    int cherchePointCommun(int p1, int p2);
    int cherchePointCommun2(int p1, int p2, int resu1);
    int cherchePointCommun3(int p1, int p2, int resu1, int resu2);
    void ajouteFace(int s1,int s2,int s3);
    int chercheArete(int s1,int s2);
    int chercheTypeFace(int a1, int a2, int a3);
    int chercheFac(int s1, int s2, int s3);
    void ajouteTypeSommet(int fa[6], int nbfa, bool tronq, int num);
    double pointPlusEloigne();
    void geoDessinSommet(QPaintDevice * des, int num);
    void geoDessinFace(QPaintDevice * des, int num);

    // resultats
    double surfaceSol();
    double surfaceToit();
    double volume();
    double diametreSol();
    double hauteurReelle();
    QString tableauFaces(int i=0);
    QString resultatFaces();
    QString tableauMontants();
    QString resultatMontants();
    QString tableauSommets(int i=0);
    QString resultatSommets();
    QString aideConstruction();

    int geo_affichage;
    int geo_solideBase;
    double geo_frequenceA;
    double geo_frequenceB;
    double geo_diametre;
    double geo_hauteur;
    double geo_rayon;
    double rayon;
    double geoHauteurReelle;
    double geo_angleRotation;
    double geo_ratioElliptique;
    int geo_sabliereHorizontale;
    QVector3D centreGeo;
    QVector<sommet> sommets;
    QVector<triangle> triangleBase;
    QVector<sommet> TsLesSomm;
    QVector<sommet> TsLesSomm0;
    QVector<type_arete> typeArete;
    QVector<arete> TsLesAretes;
    QVector<type_face> typeFace;
    QVector<face> TsLesFaces;
    QVector<type_sommet> typeSommet;


private:
    void calcul();
    int transfFace;
};

#endif // GEODOME_H
