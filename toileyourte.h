#ifndef TOILEYOURTE_H
#define TOILEYOURTE_H

#include <QPaintDevice>
#include <QVector2D>
#include <QVector>

struct PointToile
{
    double x,y;
    int type;
};

struct LignePointToile
{
    QVector<PointToile> pts;
    QString ch;
};

struct MorceauToile
{
    QVector<PointToile> pts;
    QVector<PointToile> pts0;
    QVector2D centre0;
    QVector<int> trait;
    double longueur;
};

class ToileYourte
{
public:
    ToileYourte();
    void setParametres(double toil_hauteurMur, int toil_penteYourte, double toil_diametreYourte, double toil_diametreTonoo, double toil_largeurLaize, int toil_orientation, double toil_decalage);
    void calcul();
    void toiDessinToileGeneral(QPaintDevice * des, int taille);
    void toiDessinToileMorceau(QPaintDevice * des,int ch);
    int toiNombMorceaux();
    QString toiCoordMorceau(int ch);
    QString toiExplicationToile();
    PointToile cherchePointGrandCercle(int num, double y);
    PointToile cherchePointPetitCercle(int num, double y);
    PointToile cherchePointSegmentAB(double y);
    PointToile cherchePointSegmentCD(double y);
    PointToile barycentreM(int ij);
    PointToile barycentreM2(int ij);
    int cherchePlusAGauche();
    double angleHor(double x, double y);

    double toi_hauteurMur;
    double toi_penteYourte;
    double toi_diametreYourte;
    double toi_diametreTonoo;
    double toi_largeurLaize;
    double toi_orientation;
    double toi_decalage;
    double toi_perteCouture;
    double toi_perteCoutureOurlet;
    double toi_rayonYourteFictif;
    double toi_rayonTonooFictif;
    double toi_largeurLaizeUtile;
    double toi_perimetreBas;
    double toi_perimetreBasFictif;
    double toi_perimetreHaut;
    double toi_perimetreHautFictif;
    QVector2D toi_A2,toi_B2;
    QVector2D toi_C2,toi_D2;
    double toi_angleDebutTonoo, toi_angleDebutYourte;
    double toi_angleTonoo, toi_angleYourte;
    QVector<MorceauToile> TsLesMorc, TsLesMorcDetail;
    LignePointToile temp;
    QVector<LignePointToile> tablLigne;

    QStringList listeMorceaux();

};

#endif // TOILEYOURTE_H
