#ifndef DOMENIDABEILLE_H
#define DOMENIDABEILLE_H

#include <QVector2D>
#include <QVector3D>
#include <QQuaternion>


struct strucNoeud
{
    double alphaZero, alphaPrime, alphaSeconde;
    double phiZero, phiPrime, phiSeconde;
};

struct chevronDNA
{
    QVector3D pointN, pointNmoins1prime, pointNplus1seconde, pointNcoteV, pointNcoteW;
    double rayonExtrados, rayonIntrados;
    QVector3D centre, VNormal, centreCoteV, centreCoteW, centre2D;
    QVector3D VA3D, VB3D, VC3D, VD3D, WA3D, WB3D, WC3D, WD3D, n1_3D, n2_3D, m1_3D, m2_3D, s1_3D, s2_3D, q1_3D, q2_3D;
    QVector3D VA, VB, VC, VD, WA, WB, WC, WD, A, B, C, D, n0, n1, n2, n3, m0, m1, m2, m3, s0, s1, s2, s3, q0, q1, q2, q3;
    int orientation; //0 droite 1 gauche
    int tronque; //0 non, 1 en haut, 2 en bas
    double retombeePlanche,longueurPlanche,epaisseurChevron;
    QQuaternion quatRot;
    QVector3D translation;
    QVector3D O,O2; //point haut
    double angleTracageCoteAB, angleTracageCoteCD, angleCoupeCoteAB, angleCoupeCoteCD;
};

class domenidabeille
{
public:
    domenidabeille();
    void setParametres(int pdna_choix, double pdna_hauteur, double pdna_diametreSol, double pdna_diametreTonoo, double pdna_rayonCourbureDome, int pdna_NbreLosangesHorizontal, double pdna_diagonaleDernierLosange, double pdna_epaisseurChevron, double pdna_retombeeChevron, double pdna_epaisseurTonoo, double pdna_retombeeTonoo, double pdna_epaisseurSabliere, double pdna_decalageNoeud, double pdna_DistanceNoeudTonoo);
    void calcul();
    double volume();
    double surfaceDeToit();
    double surfaceAuSol();
    double pointPlusEloigne();
    void valeursDomeGenerales(QVector<QVector3D>& passageValeur);
    void valeursChevrons(QVector<chevronDNA>& passageChevron);
    int nbChevrons();

    QStringList listeChevrons();

    QString explicationChevronNumero(int num);
    QString debit();
    QString explicationGenerale();
    QString tableauChevronNumero(int num);
    int orientation(int num);

    double dna_hauteur;

private:
    QVector3D point3D(double alpha, double phi);
    void calculPointPrime(int numeroDuPoint);
    void calculPointSeconde(int numeroDuPoint);
    chevronDNA rotateChevron(chevronDNA chev, double angPhi);
    double distanceAuCentre(QVector3D chev);

    int dna_choix, dna_NbreLosangesHorizontal;
    double dna_diametreSol, dna_diametreTonoo, dna_rayonCourbureDome, dna_angleBeta, dna_epaisseurChevron, dna_retombeeChevron, dna_epaisseurTonoo, dna_retombeeTonoo, dna_epaisseurSabliere, dna_decalageNoeud, dna_DistanceNoeudSabliere;
    double dna_diametreInterieurSabliere, dna_premiereDiagonaleHorizontaleLosangeHaut, dna_DistanceNoeudTonoo;

    QVector2D dna_centreCourbe;
    QVector3D dna_centreCourbe3D;
    double dna_angleHorizontalCentreSabliere, dna_angleHorizontalCentreTonoo, dna_angleSabliereCentreTonoo;
    double dna_longueurArcCourbeSabliereTonoo;
    double dna_angleDemiDiagonaleHorizontal, dna_angleDiagonaleHorizontal;
    QVector<double> dna_angleVerticalNoeuds;
    QVector<strucNoeud> dna_noeud;
    QVector<chevronDNA> chev;

};

#endif // DOMENIDABEILLE_H
