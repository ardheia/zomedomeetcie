#ifndef VOUTENIDABEILLE_H
#define VOUTENIDABEILLE_H

#include <QVector2D>
#include <QVector3D>

/*
  là c'est les calculs de notre structure en fonction des paramètres de la fenetre principale
*/

struct chevronCoupe
{
    QVector3D VA3D, VB3D, VC3D, VD3D, VE3D, VF3D, VG3D, VH3D;
    QVector3D VA, VB, VC, VD, WA, WB, WC, WD, A, B, C, D;
    int ihor,jhor,iver,jver;
    int nombre;
    int orientation; //0 droite 1 gauche
};

struct comptageChevronCoupe
{
    QVector<int> pointeurs;
    short nombre;
    QVector<int> position;
};

class vouteNidAbeille
{
public:
    vouteNidAbeille();
    void setParametres(int pvna_choixType, double pvna_angleBeta, double pvna_profondeurVoute, double pvna_hauteurVoute, double pvna_portee, double pvna_rayonCourbureVoute, int pvna_rangeesVerticales, double pvna_epaisseurChevron, double pvna_retombeeChevron, double pvna_epaisseurFaitiere, double pvna_retombeeFaitiere, double pvna_epaisseurRive, double pvna_epaisseurSabliere, double pvna_decalageNoeud, double pvna_distanceNoeudFaitiere, double pvna_distanceNoeudSabliere, double pvna_distanceNoeudRiveGauche);
    void valeursChevron(QVector<QVector3D>& passageValeur);
    void valeursVoute(QVector<QVector3D>& passageValeur);
    void valeursVoute2(QVector<QVector3D>& passageValeur);
    void valeursRive(QVector<QVector3D>& passageValeur);
    void valeursChevronTronque(QVector<QVector3D>& passageValeur,int numero);

    // la c'est des fonctions qui renvoient une qstring Html de ce qui doit s'afficher
    QString explicationGenerale();
    QString explicationChevrons();
    QString explicationChevronTronque(int num);
    QString tableauChevronDroite();
    QString tableauChevronGauche();
    QString tableauChevronTronque(int num);
    QString debit();
    double volume();
    double surfaceDeToit();
    int nbreChevronsGauche();
    int nbreChevronsDroite();
    int nbreChevronsTronques();

    QString resultats();

    double pointPlusEloigne();
    QStringList listeChevronsTronques();
    int orientationChevronTronque(int num);
    double retombeeChevron();

    double vna_hauteurVoute;

private:
    //fonctions privees ne pouvant etre appelees de l'exterieur
    void calcul();

    //declaration des variables
    int vna_choixType;
    double vna_angleBeta, vna_profondeurVoute, vna_portee, vna_rayonCourbureVoute, vna_angleAlpha;
    int vna_rangeesVerticales;
    double vna_epaisseurChevron, vna_retombeeChevron, vna_epaisseurFaitiere, vna_retombeeFaitiere, vna_epaisseurRive, vna_epaisseurSabliere;
    double vna_decalageNoeud, vna_distanceNoeudFaitiere, vna_distanceNoeudSabliere, vna_distanceNoeudRiveGauche;
    int vna_nombreChevronGauche, vna_nombreChevronDroite;
    int vna_debutChevronVerticalGauche, vna_debutChevronVerticalDroite, vna_debutChevronHorizontalGauche, vna_debutChevronHorizontalDroite;
    int vna_nombreChevronGaucheVertical, vna_nombreChevronGaucheHorizontal, vna_nombreChevronDroiteVertical, vna_nombreChevronDroiteHorizontal;
    double vna_angleChevronBordFaitiere, vna_angleChevronBordSabliere, vna_angleChevronBordRive;

    QVector2D vna_centre;
    double vna_distanceDroiteCoinSabliereCoinFaitiere, vna_largeurSabliere, vna_retombeeRive;
    double vna_angleHorizontalCentreCoinSabliere, vna_angleHorizontalCentreCoinFaitiere, vna_angleCoinSabliereCentreCoinFaitiere;
    double vna_longueurCourbeCoinFaitiereCoinSabliere, vna_longueurCourbeNoeudDepartNoeudArrivee;
    double vna_diagonaleVerticaleLosange;
    double vna_angleDiagonaleVerticaleLosange, vna_demiAngleDiagonaleVerticaleLosange;
    double vna_decalageNoeudHorizontal, vna_demiDecalageNoeudHorizontal, vna_angleDecalageNoeudVertical, vna_demiAngleDecalageNoeudVertical;
    double vna_longueurChevronFilaire, vna_diagonaleHorizontaleLosange, vna_demiDiagonaleHorizontaleLosange;
    double vna_angleNoeudDepartCentreNoeudArrivee, vna_angleHorizontalCentreNoeudDepart, vna_angleHorizontalCentreNoeudArrivee;
    double vna_angleCoinSabliereCentreNoeudDepart, vna_angleNoeudArriveeCentreCoinFaitiere;
    int vna_rangeesHorizontales;
    double vna_demiEpaisseurChevron;
    QVector3D chev_A, chev_B, chev_C, chev_D, chev_E, chev_F, chev_G, chev_H, chev_O1 ,chev_O2, chev_O3, chev_O1prime, chev_O2prime, chev_O3prime;
    QVector3D chev_VA, chev_VB, chev_VC, chev_VD, chev_VE, chev_VF, chev_VG, chev_VH;
    QVector3D chev_n0, chev_n1, chev_n2, chev_n3, chev_m0, chev_m1, chev_m2, chev_m3, chev_s0, chev_s1, chev_s2, chev_s3, chev_q0, chev_q1, chev_q2, chev_q3;
    QVector3D chev_centreChevron, chev_centreChevronPrime;
    QVector3D centreChevron1, centreChevron0, centreChevron2, centreChevron0ass, centreChevron2ass;
    double rayonChevron1, rayonChevron0, rayonChevron2, rayonChevron0ass, rayonChevron2ass;
    QVector3D vecteurNormalChevron1, vecteurNormalChevron0, vecteurNormalChevron2, vecteurNormalChevron0ass, vecteurNormalChevron2ass;
    QVector3D V1haut, W1haut, V2, W2, V1bas, W1bas, V0, W0, V1ass2, W1ass2, V2ass, W2ass, V1ass0, W1ass0, V0ass, W0ass;
    QVector3D O1prime, O1seconde, centreChevron1prime, centreChevron1seconde, O2prime, O0prime, centreChevron2prime, centreChevron0prime;
    QVector3D O1,U1ass,T1ass,O2,T2,U1,T3,O2ass,T3ass,T1,O0,U0,Umoins1,Umoins1ass,O0ass,centreTracageChevron1,centreTracageChevron0et2ass,centreTracageChevron0asset2;
    QVector3D chev_VA3D, chev_VB3D, chev_VC3D, chev_VD3D, chev_VE3D, chev_VF3D, chev_VG3D, chev_VH3D, OintradosPrime, OintradosSeconde;
    QVector<chevronCoupe> chevCoupe;
    QVector<comptageChevronCoupe> ccc;
    double chev_rayonIntrado, chev_rayonExtrado;
    int nombreTronqueaDroite;
    double chev_longueurChevron;
    double chev_angleTracage, chev_angleCoupe;

};

#endif // VOUTENIDABEILLE_H
