#ifndef ZOME_H
#define ZOME_H

#include <QVector2D>
#include <QVector3D>
#include <QQuaternion>


struct rangee
{
    double rayonExt, rayonInt, z;
};

struct losange_m1
{
    double diagVert, diagHoriz, alpha, beta, angleReglageLame, longueur, DiagVertInt, DiagHorizInt, longueurPlancheBrut;
    QVector3D barycentre,Vn;
    QQuaternion quatRot;
    double angleAvecRangeePrecedente, angleAvecRangeeSuivante,angleAvecHorizontale;
};

class zome
{
public:
    zome();
    void setParametres(int z_methodeConstruction, int z_nombreOrdre, double z_nombreForme, double z_echelle, int z_rangees, double z_epaisseurChevronsMethode1, double z_retombeeChevronsMethode1);
    double surfaceAuSol();
    double surfaceDeToit();
    double hauteur();
    double volume();
    double diametreSol();
    double pointPlusEloigne();
    bool triangle(int num);
    void valeursZomeGenerales(QVector<QVector3D>& passageValeur);
    void valeursSommets(QVector<rangee>& passageRangee);
    void valeursMethode1(QVector<losange_m1>& passageMethode1);

    QStringList listeLosanges();

    // la c'est des fonctions qui renvoient une qstring Html de ce qui doit s'afficher
    QString explicationGenerale();
    QString explicationLosange(int num);
    QString debit();

    double zom_hauteurTotale;

private:
    void calcul();

    int zom_methodeConstruction;
    int zom_nombreOrdre;
    double zom_nombreForme;
    double zom_petiteDiagonaleRangee1;
    int zom_rangees;
    double zom_epaisseurChevronsMethode1;
    double zom_retombeeChevronsMethode1;
    double zom_diametreSolInt, zom_diametreSolExt;
    double zom_longeurCoteLosange,zom_demiHauteurLosangeRangee1;
    double zom_longeurCoteLosange_m1,zom_demiHauteurLosangeRangee1_m1,zom_hauteurTotale_m1,zom_petiteDiagonaleRangee1_m1;
    QVector<rangee> rang;
    QVector<rangee> rang_m1;
    QVector<losange_m1> los_m1,los;

};

#endif // ZOME_H
