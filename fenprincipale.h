#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QMainWindow>
#include <QWidget>
#include "glwidget.h"
#include "voutenidabeille.h"
#include "domenidabeille.h"
#include "zome.h"
#include "heliyourte.h"
#include "geodome.h"
#include <QPushButton>
#include <QPaintDevice>

namespace Ui {
    class fenPrincipale;
}

class fenPrincipale : public QMainWindow
{
    Q_OBJECT

public:
    explicit fenPrincipale(QWidget *parent = 0);
    ~fenPrincipale();
    void rafraichirVouteNidAbeille();
    void rafraichirDomeNidAbeille();
    void rafraichirZome();
    void rafraichirHeliyourte();
    void rafraichirGeodome();


private slots:
    // la c'est pour la voute nid abeille

    // modification des zones accessibles de la fenetre en fonction du choix de la configuration de calcul choisi
    void on_vnaChoixTypeVoute_currentIndexChanged(int choix);
    //si on modifie un des parametres de la voute nid abeille, ca part direct
    void on_vnaAngleBeta_valueChanged();
    void on_vnaProfondeurVoute_valueChanged();
    void on_vnaHauteurVoute_valueChanged();
    void on_vnaPortee_valueChanged();
    void on_vnaRayonCourbureVoute_valueChanged();
    void on_vnaRangeesVerticales_valueChanged();
    void on_vnaEpaisseurChevron_valueChanged();
    void on_vnaRetombeeChevron_valueChanged();
    void on_vnaEpaisseurFaitiere_valueChanged();
    void on_vnaRetombeeFaitiere_valueChanged();
    void on_vnaEpaisseurRive_valueChanged();
    void on_vnaEpaisseurSabliere_valueChanged();
    void on_vnaDecalageNoeud_valueChanged();
    void on_vnaDistanceNoeudFaitiere_valueChanged();
    void on_vnaDistanceNoeudSabliere_valueChanged();
    void on_vnaDistanceNoeudRiveGauche_valueChanged();
    //modification du chevron tronque affiche
    void on_vnaSelectionChevronTronque_currentIndexChanged(int choix);

    // la c'est pour le dome nid abeille
    void on_dnaChoixTypeDome_currentIndexChanged(int choix);
    void on_dnaHauteurDome_valueChanged();
    void on_dnaDiametreSol_valueChanged();
    void on_dnaDiametreTonoo_valueChanged();
    void on_dnaRayonCourbureDome_valueChanged();
    void on_dnaNbreLosangesHorizontal_valueChanged();
    void on_dnaDiagonaleDernierLosange_valueChanged();
    void on_dnaEpaisseurChevron_valueChanged();
    void on_dnaRetombeeChevron_valueChanged();
    void on_dnaEpaisseurTonoo_valueChanged();
    void on_dnaRetombeeTonoo_valueChanged();
    void on_dnaEpaisseurSabliere_valueChanged();
    void on_dnaDecalageNoeud_valueChanged();
    void on_dnaDistanceNoeudTonoo_valueChanged();
    //modification du chevron affiche
    void on_dnaSelectionChevron_currentIndexChanged(int choix);

    // la c est pour le zome
    void on_zomeChoixMethodeConstructive_currentIndexChanged(int choix);
    void on_zomeNombreOrdre_valueChanged();
    void on_zomeNombreForme_valueChanged();
    void on_zomeEchelle_valueChanged();
    void on_zomeRangees_valueChanged();
    void on_zomeEpaisseurChevronMethode1_valueChanged();
    void on_zomeRetombeeChevronMethode1_valueChanged();
    void on_zomeselectionLosange_currentIndexChanged(int choix);

    // la c est pour l heliyourte
    void on_hlyDiametreYourte_valueChanged();
    void on_hlyEpaisseurCroix_valueChanged();
    void on_hlyEpaisseurLiteaux_valueChanged();
    void on_hlyEpaisseurPerche_valueChanged();
    void on_hlyHauteurMur_valueChanged();
    void on_hlyNombrePans_valueChanged();
    void on_hlyPenteToit_valueChanged();
    void on_hlyPercheDepassementHaut_valueChanged();
    void on_hlyRetombeeCroix_valueChanged();
    void on_hlyRetombeePerches_valueChanged();
    void on_hlyRetombeeTonoo_valueChanged();
    void on_hlyRetombeeTraverse_valueChanged();

    // la c est pour le geodome
    void on_geoSolideBase_currentIndexChanged(int choix);
    void on_geoFrequenceA_valueChanged();
    void on_geoFrequenceB_valueChanged();
    void on_geoDiametre_valueChanged();
    void on_geoHauteur_valueChanged();
    void on_geoAngleRotation_valueChanged();
    void on_geoSabliereHorizontale_currentIndexChanged(int choix);
    void on_geoAffichage_currentIndexChanged(int choix);
    void on_ongletsGeodome_currentChanged(int choix);

    // la c'est les actions des menus
    void on_actionA_propos_activated();
    void on_actionCouleurs_distinctes_activated();
    void on_actionExporter_resultats_PDF_activated();
    void on_actionExporter_capture_3D_activated();
    void on_actionExporter_en_format_Obj_activated();
    void on_actionOuvrir_activated();
    void on_actionEnregistrer_activated();
    void on_actionEnregistrer_sous_activated();
    void on_actionQuitter_activated();

    void ouvrir(QString nom);

private:
    //voute nid abeille
    void rafraichirResultatsVouteNidAbeille();
    void rafraichirResultatsVnaChevronTronque(int ch);
    void vnaTestBugRayonCourbure();
    bool vnaTestBugRetombeeChevron();

    //dome nid abeille
    void rafraichirResultatsDomeNidAbeille();
    void rafraichirResultatsDnaChevron(int ch);
    void dnaTestBugRayonCourbure();

    //zome
    void rafraichirResultatsZome();
    void rafraichirResultatZomeLosange(int ch);
    void zomeTestBugNbRangees();
    void zomeTestBugNombreForme();

    //heliyourte
    void rafraichirResultatsHeliyourte();
    void heliTestBugBoutPerche();

    //geodome
    void rafraichirResultatsGeodome();

    // general
    void designFenetreAPropos();
    void rafraichirTousLesWidgets();
    void exporterEnPDF();
    void enregistrer();

    Ui::fenPrincipale *ui;

    GLWidget *glWidgetVouteNidAbeille;
    GLWidget *glWidgetVouteNidAbeilleChevronGauche;
    GLWidget *glWidgetVouteNidAbeilleChevronDroite;
    GLWidget *glWidgetVouteNidAbeilleChevronTronque;
    GLWidget *glWidgetDomeNidAbeille;
    GLWidget *glWidgetDomeNidAbeilleChevron;
    GLWidget *glWidgetZome;
    GLWidget *glWidgetZomeLosange;
    GLWidget *glWidgetHeliyourte;
    GLWidget *glWidgetHeliyourteTraverse;
    GLWidget *glWidgetHeliyourteCroix;
    GLWidget *glWidgetHeliyourteMontant;
    GLWidget *glWidgetHeliyourteMontantA;
    GLWidget *glWidgetHeliyourteMontantB;
    GLWidget *glWidgetHeliyourteMontantC;
    GLWidget *glWidgetHeliyourtePerche;
    GLWidget *glWidgetGeodome;

    vouteNidAbeille *vouteNA;
    domenidabeille *domeNA;
    zome *zomito;
    heliyourte *heliY;
    geodome *geode;

    QWidget *fenetreAPropos;
    bool m_couleursDistinctes;
    QString nomDeFichier;
    int geonbTypeSommet;
    int geonbTypeFace;

};

#endif // FENPRINCIPALE_H
