#ifndef HELIYOURTE_H
#define HELIYOURTE_H

#include <QVector2D>
#include <QVector3D>
#include <QQuaternion>

struct trou
{
    double bas, haut, hauteur, largeur, angleBiais, distanceRapportBordInt;
};

class heliyourte
{
public:
    heliyourte();
    void setParametres(double h_diametreYourte, double h_hauteurMurs, int h_nbrePans, double h_penteToit, double h_epaisseurPerche, double h_retombeePerche, double h_depassementHautPerche, double h_retombeeTonoo, double h_retombeeTraverse, double h_epaisseurCroix, double h_retombeeCroix);
    double surfaceAuSol();
    double hauteurTonoo();
    double volume();
    double surfaceMur();
    double diametreTonoo();
    double surfaceToit();
    double pointPlusEloigne();
    QString tableauTraverse();
    QString generalTraverse();
    QString tableauCroix();
    QString generalCroix();
    QString generalPoteaux();
    QString PoteauMontantA();
    QString PoteauMontantATable();
    QString PoteauMontantB();
    QString PoteauMontantC();
    QString PoteauMontantCTable();
    QString PercheGeneral();
    QString PercheTable();
    QString debit();
    QString aideGenerale();

    double hel_diametreYourte, hel_hauteurMurs;
    int hel_nbrePans;
    double hel_penteToit, hel_epaisseurPerche, hel_retombeePerche, hel_depassementHautPerche, hel_depassementBasPerche;
    double hel_retombeeTonoo, hel_retombeeTraverse, hel_epaisseurCroix, hel_retombeeCroix;
    double hel_rayonYourte, hel_angleDivision, hel_dIntMur;
    double hel_hauteurTotale;
    double hel_rayonTonoo, hel_diametreTonoo, hel_dIntTonoo;
    double hel_decalageTigeRapportBord;
    double hel_angleInclinaisonPerches, hel_retombeeMiniPerches;
    double hel_AB, hel_longueurMiniPlanchesPerches;
    double hel_angleBeta3, hel_retombeePlanchesMontant, hel_epaisseurPlancheAetC, hel_epaisseurPlancheB, hel_epaisseurTraverse;
    double hel_largeurTrouTraverseMontantCdroit, hel_largeurTrouTraverseMontantAdroit, hel_largeurTrouTraverseMontantAbiais, hel_angleTrouTraverseMontantA;
    double hel_hauteurMiBoisTraverse, hel_tetonTraverse;
    double hel_longueurMiniPlanchesTraverse;
    double hel_angleCoupeTraverseGauche, hel_angleCoupeTraverseDroite;
    double hel_margeHautBas, hel_OmegaCroix, hel_longueurMiniPlanchesCroix;
    QVector<QVector2D> hel_cr, hel_cr_2; //de 0 a 12 -> A a M cote montant C ... de 13 a 25 -> A a M cote montant A ... 0primeprime au 26
    QVector<QVector2D> hel_perche_face;
    QVector<QVector2D> hel_perche_derriere;
    trou hel_DdansA, hel_BdansC, hel_AdansC, hel_CdansA, hel_EdansC, hel_FdansA, hel_montantB_1, hel_montantB_2, hel_montantB_3;
    double hel_margeTrous;
    double hel_H14_H5, hel_H12_H14, hel_H9_H13;
    double phel_BetaTraverse;
    double hel_perche_decx, hel_perche_decy;
    double phel_cr_distanceEntre2B, hel_angleVrilleCroix, hel_angleVrilleCroix2;

private:
    void calcul();


};

#endif // HELIYOURTE_H
