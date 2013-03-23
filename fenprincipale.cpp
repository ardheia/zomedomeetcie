#include "fenprincipale.h"
#include "ui_fenprincipale.h"
#include "glwidget.h"

#include <QtGui>
#include <QtXml>
#include <QPixmap>
#include <QLabel>
#include <QGridLayout>
#include <QApplication>

#include <QVector3D>
#include "mathema.h"
#include "parametres.h"

fenPrincipale::fenPrincipale(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fenPrincipale)
{
    ui->setupUi(this);

    m_couleursDistinctes = false;
    nomDeFichier = "";

    //on declare notre objet de calcul pour la voute nid abeille
    vouteNA = new vouteNidAbeille();
    vouteNA->setParametres(fenPrincipale::ui->vnaChoixTypeVoute->currentIndex(),fenPrincipale::ui->vnaAngleBeta->value(), fenPrincipale::ui->vnaProfondeurVoute->value(),fenPrincipale::ui->vnaHauteurVoute->value(),fenPrincipale::ui->vnaPortee->value(),fenPrincipale::ui->vnaRayonCourbureVoute->value(),fenPrincipale::ui->vnaRangeesVerticales->value(),fenPrincipale::ui->vnaEpaisseurChevron->value(),fenPrincipale::ui->vnaRetombeeChevron->value(),fenPrincipale::ui->vnaEpaisseurFaitiere->value(),fenPrincipale::ui->vnaRetombeeFaitiere->value(),fenPrincipale::ui->vnaEpaisseurRive->value(),fenPrincipale::ui->vnaEpaisseurSabliere->value(),fenPrincipale::ui->vnaDecalageNoeud->value(),fenPrincipale::ui->vnaDistanceNoeudFaitiere->value(),fenPrincipale::ui->vnaDistanceNoeudSabliere->value(),fenPrincipale::ui->vnaDistanceNoeudRiveGauche->value());

    // on declare notre objet de calcul pour le dome nid abeille
    domeNA = new domenidabeille();
    domeNA->setParametres(fenPrincipale::ui->dnaChoixTypeDome->currentIndex(),fenPrincipale::ui->dnaHauteurDome->value(),fenPrincipale::ui->dnaDiametreSol->value(),fenPrincipale::ui->dnaDiametreTonoo->value(),fenPrincipale::ui->dnaRayonCourbureDome->value(),fenPrincipale::ui->dnaNbreLosangesHorizontal->value(),fenPrincipale::ui->dnaDiagonaleDernierLosange->value(),fenPrincipale::ui->dnaEpaisseurChevron->value(),fenPrincipale::ui->dnaRetombeeChevron->value(),fenPrincipale::ui->dnaEpaisseurTonoo->value(),fenPrincipale::ui->dnaRetombeeTonoo->value(),fenPrincipale::ui->dnaEpaisseurSabliere->value(),fenPrincipale::ui->dnaDecalageNoeud->value(),fenPrincipale::ui->dnaDistanceNoeudTonoo->value());

    //on declare notre objet de calcul pour le zome
    zomito = new zome();
    zomito->setParametres(fenPrincipale::ui->zomeChoixMethodeConstructive->currentIndex(),fenPrincipale::ui->zomeNombreOrdre->value(),fenPrincipale::ui->zomeNombreForme->value(),fenPrincipale::ui->zomeEchelle->value(),fenPrincipale::ui->zomeRangees->value(),fenPrincipale::ui->zomeEpaisseurChevronMethode1->value(),fenPrincipale::ui->zomeRetombeeChevronMethode1->value());

    //on declare notre objet de calcul pour l heliyourte
    heliY = new heliyourte();
    heliY->setParametres(fenPrincipale::ui->hlyDiametreYourte->value(),fenPrincipale::ui->hlyHauteurMur->value(),fenPrincipale::ui->hlyNombrePans->value(),fenPrincipale::ui->hlyPenteToit->value(),fenPrincipale::ui->hlyEpaisseurPerche->value(),fenPrincipale::ui->hlyRetombeePerches->value(),fenPrincipale::ui->hlyPercheDepassementHaut->value(),fenPrincipale::ui->hlyRetombeeTonoo->value(),fenPrincipale::ui->hlyRetombeeTraverse->value(),fenPrincipale::ui->hlyEpaisseurCroix->value(),fenPrincipale::ui->hlyRetombeeCroix->value());

    //on declare notre objet de calcul pour le geodome
    geode = new geodome();
    geode->setParametres(fenPrincipale::ui->geoSolideBase->currentIndex(),fenPrincipale::ui->geoFrequenceA->value(),fenPrincipale::ui->geoFrequenceB->value(),fenPrincipale::ui->geoDiametre->value(),fenPrincipale::ui->geoHauteur->value(),fenPrincipale::ui->geoAngleRotation->value(),fenPrincipale::ui->geoSabliereHorizontale->currentIndex(),fenPrincipale::ui->geoAffichage->currentIndex());

    //la on declare nos fenetres 3d pour la voute nid abeille
    glWidgetVouteNidAbeille = new GLWidget(vouteNA,0);
    glWidgetVouteNidAbeilleChevronGauche = new GLWidget(vouteNA,1);
    glWidgetVouteNidAbeilleChevronDroite = new GLWidget(vouteNA,2);
    glWidgetVouteNidAbeilleChevronTronque = new GLWidget(vouteNA,3);
    glWidgetVouteNidAbeilleChevronGauche->setFixedSize(280,320);
    glWidgetVouteNidAbeilleChevronDroite->setFixedSize(280,320);
    glWidgetVouteNidAbeilleChevronTronque->setFixedSize(280,320);

    ui->layout3dvna->addWidget(glWidgetVouteNidAbeille);
    ui->layout3dvnaChevronGauche->addWidget(glWidgetVouteNidAbeilleChevronGauche);
    ui->layout3dvnaChevronDroite->addWidget(glWidgetVouteNidAbeilleChevronDroite);
    ui->layout3dvnaChevronTronque->addWidget(glWidgetVouteNidAbeilleChevronTronque);

    rafraichirResultatsVouteNidAbeille();

    //la on declare nos fenetres 3d pour le dome nid abeille
    glWidgetDomeNidAbeille = new GLWidget(domeNA,0);
    glWidgetDomeNidAbeilleChevron = new GLWidget(domeNA,1);
    glWidgetDomeNidAbeilleChevron->setFixedSize(280,320);

    ui->layout3ddna->addWidget(glWidgetDomeNidAbeille);
    ui->layout3ddnaChevron->addWidget(glWidgetDomeNidAbeilleChevron);

    rafraichirResultatsDomeNidAbeille();

    //la on declare nos fenetres 3d pour le zome
    glWidgetZome = new GLWidget(zomito,0);
    glWidgetZomeLosange = new GLWidget(zomito,1);
    glWidgetZomeLosange->setFixedSize(280,320);

    ui->layout3dzome->addWidget(glWidgetZome);
    ui->layout3dzomelosange->addWidget(glWidgetZomeLosange);

    rafraichirResultatsZome();

    //la on declare nos fenetres 3d pour l heliyourte
    glWidgetHeliyourte = new GLWidget(heliY,0);
    glWidgetHeliyourteTraverse = new GLWidget(heliY,1);
    glWidgetHeliyourteCroix = new GLWidget(heliY,2);
    glWidgetHeliyourteMontant = new GLWidget(heliY,3);
    glWidgetHeliyourteMontantA = new GLWidget(heliY,4);
    glWidgetHeliyourteMontantB = new GLWidget(heliY,5);
    glWidgetHeliyourteMontantC = new GLWidget(heliY,6);
    glWidgetHeliyourtePerche = new GLWidget(heliY,7);
    glWidgetHeliyourteTraverse->setFixedSize(280,320);
    glWidgetHeliyourteCroix->setFixedSize(280,320);
    glWidgetHeliyourteMontant->setFixedSize(280,320);
    glWidgetHeliyourteMontantA->setFixedSize(280,320);
    glWidgetHeliyourteMontantB->setFixedSize(280,320);
    glWidgetHeliyourteMontantC->setFixedSize(280,320);
    glWidgetHeliyourtePerche->setFixedSize(280,320);

    ui->layout3dhly->addWidget(glWidgetHeliyourte);
    ui->layout3dhlyTraverse->addWidget(glWidgetHeliyourteTraverse);
    ui->layout3dhlyCroix->addWidget(glWidgetHeliyourteCroix);
    ui->layout3dhlyPoteauGeneral->addWidget(glWidgetHeliyourteMontant);
    ui->layout3dhlyPoteauMontantA->addWidget(glWidgetHeliyourteMontantA);
    ui->layout3dhlyPoteauMontantB->addWidget(glWidgetHeliyourteMontantB);
    ui->layout3dhlyPoteauMontantC->addWidget(glWidgetHeliyourteMontantC);
    ui->layout3dhlyPerche->addWidget(glWidgetHeliyourtePerche);

    rafraichirResultatsHeliyourte();

    //la on declare nos fenetres 3d pour le geodome
    glWidgetGeodome = new GLWidget(geode,0);
    geonbTypeSommet = -1;
    geonbTypeFace = -1;

    ui->layout3dgeo->addWidget(glWidgetGeodome);

    rafraichirResultatsGeodome();

    designFenetreAPropos();

}


void fenPrincipale::designFenetreAPropos()
{
    //creation de la fenetre a propos
    fenetreAPropos = new QWidget();
    fenetreAPropos->setWindowTitle(QObject::tr("A propos de ce programme"));
    QDesktopWidget bureau;
    QRect surface_bureau = bureau.screenGeometry();
    int largeurFenetreAPropos = 650;
    int hauteurFenetreAPropos = 600;
    int xcentrageFenetreAPropos = surface_bureau.width()/2 - largeurFenetreAPropos/2;
    int ycentrageFenetreAPropos = surface_bureau.height()/2 - hauteurFenetreAPropos/2;
    fenetreAPropos->setGeometry(xcentrageFenetreAPropos,ycentrageFenetreAPropos,largeurFenetreAPropos,hauteurFenetreAPropos);
    //le texte
    QTextBrowser *texteApropos = new QTextBrowser;
    texteApropos->setOpenExternalLinks(true);
    texteApropos->setAcceptRichText(true);
    QString texteHTMLApropos;
    texteHTMLApropos = "<html><p align=left>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Ce programme open-source")+" <a href=\"http://creativecommons.org/licenses/by-nc-sa/3.0/fr/\"><img height=\"31\" width=\"88\" src=\":/images/licenseCC\" /></a> "+QObject::tr("est développé par")+"<strong> "+QObject::tr("l\'association")+" <a href=\"http://ardheia.free.fr\">"+QObject::tr("ARDHEIA")+"</a></strong>.</p>";
    texteHTMLApropos += "<p align=center><img src=\":/images/logoArdheia\" /></p>";
    texteHTMLApropos += "<p align=justify>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Il est toujours en cours de développement, l'idée étant de réunir toutes nos anciens tableurs (dome,zome ...) en un programme plus convivial et fiable. Pour l'instant, vous pouvez visualiser les voutes,les dômes en nid d'abeille, les zomes, les héliyourtes, les géodomes")+". <em><strong>"+QObject::tr("A venir")+" :</strong></em> "+QObject::tr("Dôme Pomme de pin ...")+"</p>";
    texteHTMLApropos += "<p align=justify>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Il est désormais possible d\'exporter n\'importe quelle vue 3D au format Wavefront Obj, pour l\'ouvrir avec un autre logiciel de 3D (Blender, Sketchup ...). Ceci peut être intéressant pour situer les structures dans des environnements ou pour prendre des côtes facilement")+".</p>";
    texteHTMLApropos += "<p align=left>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Il est développé en C++ avec")+" <strong><a href=\"http://qt.nokia.com/title-fr/\">Qt</a>. </strong>"+QObject::tr("Les sources peuvent bien entendu être copiées, modifiées, diffusées à condition de citer la source")+" ...</p>";
    texteHTMLApropos += "<p align=justify>&nbsp;&nbsp;"+QObject::tr("Ce programme sert uniquement à visualiser en 3D les structures imaginées et à vous donner des infos concernant la construction. Il ne garantit en aucun cas la résistance mécanique des ouvrages et l'association ARDHEIA ne pourra donc pas être tenue responsable d'un mauvais dimensionnement d'une structure")+".</p>";
    texteHTMLApropos += "<p align=left>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Pour toutes questions et remarques (il y a encore pas mal de bugs), vous pouvez nous contacter a l'adresse")+" <strong><a href=\"mailto:ardheia@gmail.com\">ardheia@gmail.com</a></strong></p>";
    texteHTMLApropos += "<p align=center><em>"+QObject::tr("Vous avez actuellement la version")+" "+version()+" "+QObject::tr("(année.mois.jour)")+"</em></p></html>";
    texteApropos->setText(texteHTMLApropos);
    //le bouton ok
    QPushButton *okapropos = new QPushButton(QObject::tr("Et Hop !"));
    //le calque vertical
    QVBoxLayout *calqueAproposVert = new QVBoxLayout;
    //que l on remplit avec le texte et le bouton ok
    calqueAproposVert->addWidget(texteApropos);
    calqueAproposVert->addWidget(okapropos);
    //et on remplit le widget
    fenetreAPropos->setLayout(calqueAproposVert);
    QObject::connect(okapropos,SIGNAL(clicked()),fenetreAPropos,SLOT(hide()));
}

//Voute Nid Abeille

void fenPrincipale::rafraichirVouteNidAbeille()
{

    vouteNA->setParametres(fenPrincipale::ui->vnaChoixTypeVoute->currentIndex(),fenPrincipale::ui->vnaAngleBeta->value(), fenPrincipale::ui->vnaProfondeurVoute->value(),fenPrincipale::ui->vnaHauteurVoute->value(),fenPrincipale::ui->vnaPortee->value(),fenPrincipale::ui->vnaRayonCourbureVoute->value(),fenPrincipale::ui->vnaRangeesVerticales->value(),fenPrincipale::ui->vnaEpaisseurChevron->value(),fenPrincipale::ui->vnaRetombeeChevron->value(),fenPrincipale::ui->vnaEpaisseurFaitiere->value(),fenPrincipale::ui->vnaRetombeeFaitiere->value(),fenPrincipale::ui->vnaEpaisseurRive->value(),fenPrincipale::ui->vnaEpaisseurSabliere->value(),fenPrincipale::ui->vnaDecalageNoeud->value(),fenPrincipale::ui->vnaDistanceNoeudFaitiere->value(),fenPrincipale::ui->vnaDistanceNoeudSabliere->value(),fenPrincipale::ui->vnaDistanceNoeudRiveGauche->value());
    if(vnaTestBugRetombeeChevron())
    {
        vouteNA->setParametres(fenPrincipale::ui->vnaChoixTypeVoute->currentIndex(),fenPrincipale::ui->vnaAngleBeta->value(), fenPrincipale::ui->vnaProfondeurVoute->value(),fenPrincipale::ui->vnaHauteurVoute->value(),fenPrincipale::ui->vnaPortee->value(),fenPrincipale::ui->vnaRayonCourbureVoute->value(),fenPrincipale::ui->vnaRangeesVerticales->value(),fenPrincipale::ui->vnaEpaisseurChevron->value(),fenPrincipale::ui->vnaRetombeeChevron->value(),fenPrincipale::ui->vnaEpaisseurFaitiere->value(),fenPrincipale::ui->vnaRetombeeFaitiere->value(),fenPrincipale::ui->vnaEpaisseurRive->value(),fenPrincipale::ui->vnaEpaisseurSabliere->value(),fenPrincipale::ui->vnaDecalageNoeud->value(),fenPrincipale::ui->vnaDistanceNoeudFaitiere->value(),fenPrincipale::ui->vnaDistanceNoeudSabliere->value(),fenPrincipale::ui->vnaDistanceNoeudRiveGauche->value());
    }

    glWidgetVouteNidAbeille->rafraichirWidget(m_couleursDistinctes);
    glWidgetVouteNidAbeilleChevronGauche->rafraichirWidget(m_couleursDistinctes);
    glWidgetVouteNidAbeilleChevronDroite->rafraichirWidget(m_couleursDistinctes);
    glWidgetVouteNidAbeilleChevronTronque->rafraichirWidget(m_couleursDistinctes,0);

    rafraichirResultatsVouteNidAbeille();
    rafraichirResultatsVnaChevronTronque(0);
}

void fenPrincipale::rafraichirResultatsVouteNidAbeille()
{
    QString str;

    //la on met a jour les champs de resultats de la premiere page
    fenPrincipale::ui->vnaVolume->setText(str.setNum(vouteNA->volume(),'f',1)+" m3 ");
    fenPrincipale::ui->vnaSurfaceDeToit->setText(str.setNum(vouteNA->surfaceDeToit(),'f',1)+" m2 ");
    fenPrincipale::ui->vnaSurfaceAuSol->setText(str.setNum(fenPrincipale::ui->vnaPortee->value()*fenPrincipale::ui->vnaProfondeurVoute->value(),'f',1)+" m2 ");

    //page chevron
    fenPrincipale::ui->vnaDonneesGeneralesATousChevronsTexte->setText(vouteNA->explicationChevrons());
    fenPrincipale::ui->vna_nombreChevronsGaucheText->setTitle(str.setNum((double)vouteNA->nbreChevronsGauche(),'f',0)+" "+QObject::tr("chevrons penchés à gauche"));
    fenPrincipale::ui->vna_nombreChevronsDroiteText->setTitle(str.setNum((double)vouteNA->nbreChevronsGauche(),'f',0)+" "+QObject::tr("chevrons penchés à droite"));
    fenPrincipale::ui->vnatableChevronDroiteText->setText(vouteNA->tableauChevronDroite());
    fenPrincipale::ui->vnatableChevronGaucheText->setText(vouteNA->tableauChevronGauche());

    //page chevron tronque
    fenPrincipale::ui->vnaSelectionChevronTronque->clear();
    fenPrincipale::ui->vnaSelectionChevronTronque->addItems(vouteNA->listeChevronsTronques());
    fenPrincipale::ui->vnaSelectionChevronTronque->setCurrentIndex(0);

    //page debit
    fenPrincipale::ui->vnadebittexte->setHtml(vouteNA->debit());

    //page explications
    fenPrincipale::ui->vnaAideConstruction->setHtml(vouteNA->explicationGenerale());
 }

void fenPrincipale::rafraichirResultatsDomeNidAbeille()
{
    QString str;

    //la on met a jour les champs de resultats de la premiere page
    fenPrincipale::ui->dnaVolume->setText(str.setNum(domeNA->volume(),'f',1)+" m3 ");
    fenPrincipale::ui->dnaSurfaceDeToit->setText(str.setNum(domeNA->surfaceDeToit(),'f',1)+" m2 ");
    fenPrincipale::ui->dnaSurfaceAuSol->setText(str.setNum(domeNA->surfaceAuSol(),'f',1)+" m2 ");

    //page chevrons
    fenPrincipale::ui->dnaSelectionChevron->clear();
    fenPrincipale::ui->dnaSelectionChevron->addItems(domeNA->listeChevrons());
    fenPrincipale::ui->dnaSelectionChevron->setCurrentIndex(0);
    fenPrincipale::ui->dnatableChevronNumeroText->setText(domeNA->tableauChevronNumero(0));
    fenPrincipale::ui->dnaChevronNumeroTexte->setText(domeNA->explicationChevronNumero(0));

    //page aide construction
    fenPrincipale::ui->dnaAideConstruction->setHtml(domeNA->explicationGenerale());

    //page debit
    fenPrincipale::ui->dnadebittexte->setHtml(domeNA->debit());
}

void fenPrincipale::rafraichirResultatsDnaChevron(int ch)
{
    glWidgetDomeNidAbeilleChevron->rafraichirWidget(m_couleursDistinctes,ch,1);

    fenPrincipale::ui->dnatableChevronNumeroText->setText(domeNA->tableauChevronNumero(ch));
    fenPrincipale::ui->dnaChevronNumeroTexte->setText(domeNA->explicationChevronNumero(ch));

    if(domeNA->orientation(ch)==0)
        fenPrincipale::ui->dnaimageChevron->setPixmap(QPixmap(":/images/dna/dnaDroitdeFace"));
    else
        fenPrincipale::ui->dnaimageChevron->setPixmap(QPixmap(":/images/dna/dnaGauchedeFace"));
}

void fenPrincipale::rafraichirResultatsVnaChevronTronque(int ch)
{
    glWidgetVouteNidAbeilleChevronTronque->rafraichirWidget(m_couleursDistinctes,ch,1);

    switch(vouteNA->orientationChevronTronque(ch))
    {
    case 0:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaDroitCoupeBas"));
    break;
    case 1:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaDroitCoupeBas"));
    break;
    case 2:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaDroitCoupeHaut"));
    break;
    case 3:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaDroitCoupeHaut"));
    break;
    case 5:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaCoupeDeuxBouts"));
    break;
    case 6:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaCoupeDeuxBouts"));
    break;
    case 8:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaGaucheCoupeHaut"));
    break;
    case 9:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaGaucheCoupeBas"));
    break;
    case 10:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaGaucheCoupeBas"));
    break;
    case 11:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaGaucheCoupeHaut"));
    break;
    case 12:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaCoupeDeuxBouts"));
    break;
    case 15:
    fenPrincipale::ui->vnaimageChevronTronque->setPixmap(QPixmap(":/images/vna/vnaCoupeDeuxBouts"));
    break;
    }

    fenPrincipale::ui->vnaChevronTronqueTexte->setText(vouteNA->explicationChevronTronque(ch));
    fenPrincipale::ui->vnatableChevronTronqueText->setText(vouteNA->tableauChevronTronque(ch));

}

void fenPrincipale::on_vnaSelectionChevronTronque_currentIndexChanged(int choix)
{
    if(choix<0){choix=0;}
    rafraichirResultatsVnaChevronTronque(choix);
}

//modification du chevron affiche
void fenPrincipale::on_dnaSelectionChevron_currentIndexChanged(int choix)
{
    if(choix<0){choix=0;}
    rafraichirResultatsDnaChevron(choix);
}

void fenPrincipale::on_vnaChoixTypeVoute_currentIndexChanged(int choix)
{
    switch (choix)
    {
    //cylindre regulier avec faitiere
    case 0:
    fenPrincipale::ui->vnaRayonCourbureVoute->setEnabled(false);
    fenPrincipale::ui->vnaRayonCourbureVouteTexte->setEnabled(false);
    fenPrincipale::ui->vnaFaitiereTexte->setEnabled(true);
    fenPrincipale::ui->vnaDistanceNoeudFaitiere->setEnabled(true);
    fenPrincipale::ui->vnaDistanceNoeudFaitiereTexte->setEnabled(true);
        break;

    //cylindre regulier sans faitiere
    case 1:
    fenPrincipale::ui->vnaRayonCourbureVoute->setEnabled(false);
    fenPrincipale::ui->vnaRayonCourbureVouteTexte->setEnabled(false);
    fenPrincipale::ui->vnaFaitiereTexte->setEnabled(false);
    fenPrincipale::ui->vnaDistanceNoeudFaitiere->setEnabled(false);
    fenPrincipale::ui->vnaDistanceNoeudFaitiereTexte->setEnabled(false);
        break;

    //cylindre irregulier avec faitiere
    case 2:
    fenPrincipale::ui->vnaRayonCourbureVoute->setEnabled(true);
    fenPrincipale::ui->vnaRayonCourbureVouteTexte->setEnabled(true);
    fenPrincipale::ui->vnaFaitiereTexte->setEnabled(true);
    fenPrincipale::ui->vnaDistanceNoeudFaitiere->setEnabled(true);
    fenPrincipale::ui->vnaDistanceNoeudFaitiereTexte->setEnabled(true);
        break;
    }

    rafraichirVouteNidAbeille();
 }

void fenPrincipale::on_vnaAngleBeta_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaProfondeurVoute_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaHauteurVoute_valueChanged()
{
    if(fenPrincipale::ui->vnaChoixTypeVoute->currentIndex()==2)
        vnaTestBugRayonCourbure();
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaPortee_valueChanged()
{
    if(fenPrincipale::ui->vnaChoixTypeVoute->currentIndex()==2)
        vnaTestBugRayonCourbure();
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaRayonCourbureVoute_valueChanged()
{
    vnaTestBugRayonCourbure();
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaRangeesVerticales_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaEpaisseurChevron_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaRetombeeChevron_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaEpaisseurFaitiere_valueChanged()
{
    vnaTestBugRayonCourbure();
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaRetombeeFaitiere_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaEpaisseurRive_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaEpaisseurSabliere_valueChanged()
{
    vnaTestBugRayonCourbure();
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaDecalageNoeud_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaDistanceNoeudFaitiere_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaDistanceNoeudSabliere_valueChanged()
{
    rafraichirVouteNidAbeille();
}

void fenPrincipale::on_vnaDistanceNoeudRiveGauche_valueChanged()
{
    rafraichirVouteNidAbeille();
}

fenPrincipale::~fenPrincipale()
{
    delete ui;
}

void fenPrincipale::on_actionA_propos_activated()
{
    fenetreAPropos->show();
}

void fenPrincipale::on_actionCouleurs_distinctes_activated()
{
    if(fenPrincipale::ui->actionCouleurs_distinctes->isChecked())
    {
        m_couleursDistinctes = true;
    }
    else
    {
        m_couleursDistinctes = false;
    }
    rafraichirTousLesWidgets();
}

void fenPrincipale::rafraichirTousLesWidgets()
{
    glWidgetVouteNidAbeille->rafraichirWidget(m_couleursDistinctes);
    glWidgetVouteNidAbeilleChevronGauche->rafraichirWidget(m_couleursDistinctes);
    glWidgetVouteNidAbeilleChevronDroite->rafraichirWidget(m_couleursDistinctes);
    glWidgetVouteNidAbeilleChevronTronque->rafraichirWidget(m_couleursDistinctes);
    glWidgetDomeNidAbeille->rafraichirWidget(m_couleursDistinctes);
    glWidgetDomeNidAbeilleChevron->rafraichirWidget(m_couleursDistinctes);
    glWidgetZome->rafraichirWidget(m_couleursDistinctes);
    glWidgetZomeLosange->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourte->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteTraverse->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteCroix->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteMontant->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteMontantA->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteMontantB->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteMontantC->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourtePerche->rafraichirWidget(m_couleursDistinctes);
    glWidgetGeodome->rafraichirWidget(m_couleursDistinctes);
}

void fenPrincipale::vnaTestBugRayonCourbure()
{
    double dist;
    QString str;
    dist = qSqrt(qPow(fenPrincipale::ui->vnaPortee->value()/2.0-fenPrincipale::ui->vnaEpaisseurFaitiere->value()/200.0,2.0)+qPow(fenPrincipale::ui->vnaEpaisseurSabliere->value()/100.0-fenPrincipale::ui->vnaHauteurVoute->value(),2.0));
    if(fenPrincipale::ui->vnaRayonCourbureVoute->value()*2.0<dist)
    {
        QMessageBox::warning(this,QObject::tr("Rayon de courbure trop faible"),QObject::tr("Le rayon de courbure ne peut être inférieur à ")+str.setNum(dist/2.0,'f',1)+" m.");
        fenPrincipale::ui->vnaRayonCourbureVoute->setValue(dist/2.0+0.05);
    }
}

bool fenPrincipale::vnaTestBugRetombeeChevron()
{
    double ret;
    QString str;
    ret = vouteNA->retombeeChevron()*100.0;
    if(ret>fenPrincipale::ui->vnaRetombeeChevron->value())
    {
        fenPrincipale::ui->vnaRetombeeChevron->setValue(ret+10.0);
        QMessageBox::warning(this,QObject::tr("Modification de la largeur des planches"),QObject::tr("Afin de pouvoir tailler les chevrons voulus, le programme a automatiquement augmenté la valeur <largeur planche> à")+" "+str.setNum(ret+10.0,'f',1)+" cm "+QObject::tr("dans l'onglet de paramètres <bois>")+".");
        return true;
    }
    else
    {
        return false;
    }
}


//Dome Nid Abeille

void fenPrincipale::rafraichirDomeNidAbeille()
{
    QString str;
    domeNA->setParametres(fenPrincipale::ui->dnaChoixTypeDome->currentIndex(),fenPrincipale::ui->dnaHauteurDome->value(),fenPrincipale::ui->dnaDiametreSol->value(),fenPrincipale::ui->dnaDiametreTonoo->value(),fenPrincipale::ui->dnaRayonCourbureDome->value(),fenPrincipale::ui->dnaNbreLosangesHorizontal->value(),fenPrincipale::ui->dnaDiagonaleDernierLosange->value(),fenPrincipale::ui->dnaEpaisseurChevron->value(),fenPrincipale::ui->dnaRetombeeChevron->value(),fenPrincipale::ui->dnaEpaisseurTonoo->value(),fenPrincipale::ui->dnaRetombeeTonoo->value(),fenPrincipale::ui->dnaEpaisseurSabliere->value(),fenPrincipale::ui->dnaDecalageNoeud->value(),fenPrincipale::ui->dnaDistanceNoeudTonoo->value());

    glWidgetDomeNidAbeille->rafraichirWidget(m_couleursDistinctes);
    rafraichirResultatsDomeNidAbeille();
    rafraichirResultatsDnaChevron(0);
}

void fenPrincipale::on_dnaChoixTypeDome_currentIndexChanged(int choix)
{
    switch (choix)
    {
    //spherique
    case 0:
        fenPrincipale::ui->dnaRayonCourbureDome->setEnabled(false);
        fenPrincipale::ui->dnaRayonCourbureDomeTexte->setEnabled(false);
        break;

    //non spherique
    case 1:
        fenPrincipale::ui->dnaRayonCourbureDome->setEnabled(true);
        fenPrincipale::ui->dnaRayonCourbureDomeTexte->setEnabled(true);
        break;
    }
    dnaTestBugRayonCourbure();
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaHauteurDome_valueChanged()
{
    dnaTestBugRayonCourbure();
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaDiametreSol_valueChanged()
{
    if(fenPrincipale::ui->dnaDiametreSol->value()<=fenPrincipale::ui->dnaDiametreTonoo->value())
    {
        QMessageBox::warning(this,QObject::tr("A noter")+" :",QObject::tr("Le diamètre au sol ne peut être inférieur au diamètre du tonoo"));
        fenPrincipale::ui->dnaDiametreSol->setValue(fenPrincipale::ui->dnaDiametreTonoo->value()+0.5);
    }
    dnaTestBugRayonCourbure();
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaDiametreTonoo_valueChanged()
{
    if(fenPrincipale::ui->dnaDiametreSol->value()<=fenPrincipale::ui->dnaDiametreTonoo->value())
    {
        QMessageBox::warning(this,QObject::tr("A noter")+" :",QObject::tr("Le diamètre au sol ne peut être inférieur au diamètre du tonoo"));
        fenPrincipale::ui->dnaDiametreTonoo->setValue(fenPrincipale::ui->dnaDiametreSol->value()-0.5);
    }
    dnaTestBugRayonCourbure();
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaRayonCourbureDome_valueChanged()
{
    dnaTestBugRayonCourbure();
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaNbreLosangesHorizontal_valueChanged()
{
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaDiagonaleDernierLosange_valueChanged()
{
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaEpaisseurChevron_valueChanged()
{
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaRetombeeChevron_valueChanged()
{
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaEpaisseurTonoo_valueChanged()
{
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaRetombeeTonoo_valueChanged()
{
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaEpaisseurSabliere_valueChanged()
{
    dnaTestBugRayonCourbure();
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaDecalageNoeud_valueChanged()
{
    rafraichirDomeNidAbeille();
}

void fenPrincipale::on_dnaDistanceNoeudTonoo_valueChanged()
{
    rafraichirDomeNidAbeille();
}

void fenPrincipale::dnaTestBugRayonCourbure()
{
    double dist;
    QString str;
    dist = qSqrt(qPow(fenPrincipale::ui->dnaDiametreSol->value()/2.0-fenPrincipale::ui->dnaDiametreTonoo->value()/2.0,2.0)+qPow(fenPrincipale::ui->dnaEpaisseurSabliere->value()/100.0-fenPrincipale::ui->dnaHauteurDome->value(),2.0));
    if(fenPrincipale::ui->dnaRayonCourbureDome->value()*2.0<dist)
    {
        QMessageBox::warning(this,QObject::tr("Rayon de courbure trop faible"),QObject::tr("Le rayon de courbure ne peut être inférieur à")+" "+str.setNum(dist/2.0,'f',1)+" m.");
        fenPrincipale::ui->dnaRayonCourbureDome->setValue(dist/2.0+0.05);
    }
}

// actions du zome

void fenPrincipale::rafraichirZome()
{
    zomito->setParametres(fenPrincipale::ui->zomeChoixMethodeConstructive->currentIndex(),fenPrincipale::ui->zomeNombreOrdre->value(),fenPrincipale::ui->zomeNombreForme->value(),fenPrincipale::ui->zomeEchelle->value(),fenPrincipale::ui->zomeRangees->value(),fenPrincipale::ui->zomeEpaisseurChevronMethode1->value(),fenPrincipale::ui->zomeRetombeeChevronMethode1->value());
    glWidgetZome->rafraichirWidget(m_couleursDistinctes);
    glWidgetZomeLosange->rafraichirWidget(m_couleursDistinctes);

    rafraichirResultatsZome();
}

void fenPrincipale::rafraichirResultatsZome()
{
    QString str;
    fenPrincipale::ui->ZomeSurfaceAuSol->setText(str.setNum(zomito->surfaceAuSol(),'f',1)+" m2 ");
    fenPrincipale::ui->zomeSurfaceDeToit->setText(str.setNum(zomito->surfaceDeToit(),'f',1)+" m2 ");
    fenPrincipale::ui->zomeVolume->setText(str.setNum(zomito->volume(),'f',1)+" m3 ");
    fenPrincipale::ui->zomeHauteur->setText(str.setNum(zomito->hauteur(),'f',2)+" m ");
    fenPrincipale::ui->zomeDiametreSol->setText(str.setNum(zomito->diametreSol(),'f',2)+" m ");

    fenPrincipale::ui->zomeselectionLosange->clear();
    fenPrincipale::ui->zomeselectionLosange->addItems(zomito->listeLosanges());
    fenPrincipale::ui->zomeselectionLosange->setCurrentIndex(0);
    fenPrincipale::ui->zomeAideConstruction->setHtml(zomito->explicationGenerale());
    fenPrincipale::ui->zomedebittexte->setHtml(zomito->debit());

    rafraichirResultatZomeLosange(0);
}

void fenPrincipale::rafraichirResultatZomeLosange(int ch)
{
    glWidgetZomeLosange->rafraichirWidget(m_couleursDistinctes,ch,1);
    fenPrincipale::ui->zomeLosangeTexte->setText(zomito->explicationLosange(ch));
    switch(fenPrincipale::ui->zomeChoixMethodeConstructive->currentIndex())
    {
    case 0:
        if(zomito->triangle(ch))
            fenPrincipale::ui->zomimageLosange->setPixmap(QPixmap(":/images/zome/zomTriangle"));
        else
            fenPrincipale::ui->zomimageLosange->setPixmap(QPixmap(":/images/zome/zomLosange"));
        break;
    case 1:
            fenPrincipale::ui->zomimageLosange->setPixmap(QPixmap(":/images/logoArdheia"));
        break;
    }

}

void fenPrincipale::on_zomeselectionLosange_currentIndexChanged(int choix)
{
    if(choix<0)
        choix = 0;
    rafraichirResultatZomeLosange(choix);
}

void fenPrincipale::zomeTestBugNbRangees()
{
    QString str;
    if(fenPrincipale::ui->zomeRangees->value()>fenPrincipale::ui->zomeNombreOrdre->value())
    {
        QMessageBox::warning(this,QObject::tr("Nombre de rangées trop grandes"),QObject::tr("Le nombre de rangées ne peut être supérieur au nombre d\'ordre, c\'est à dire dans notre cas")+" "+str.setNum((double)fenPrincipale::ui->zomeNombreOrdre->value(),'f',0)+".");
        fenPrincipale::ui->zomeRangees->setValue(fenPrincipale::ui->zomeNombreOrdre->value());
    }
}

void fenPrincipale::zomeTestBugNombreForme()
{
    QString str;
    double jj = 1.0/qTan(pi/fenPrincipale::ui->zomeNombreOrdre->value())+0.2;
    if(fenPrincipale::ui->zomeNombreForme->value()<jj)
    {
        QMessageBox::warning(this,QObject::tr("Nombre de forme trop petit"),QObject::tr("Le nombre de forme, dans notre cas, doit être supérieur à")+" "+str.setNum(jj,'f',2)+".");
        fenPrincipale::ui->zomeNombreForme->setValue(jj);
    }
}

void fenPrincipale::on_zomeChoixMethodeConstructive_currentIndexChanged(int choix)
{
    if(choix==0)
    {
        fenPrincipale::ui->zomeGroupeChevronsMethode1->setEnabled(true);
        fenPrincipale::ui->ongletsZome->setTabText(1,QObject::tr("Losanges"));
        fenPrincipale::ui->zomeselectionLosangeTexte->setText(QObject::tr("Choisissez un losange dans la liste ci-dessous")+" :");
    }
    if(choix==1)
    {
        fenPrincipale::ui->zomeGroupeChevronsMethode1->setEnabled(false);
        fenPrincipale::ui->ongletsZome->setTabText(1,QObject::tr("Rien"));
        fenPrincipale::ui->zomeselectionLosangeTexte->setText(QObject::tr("Rien d\'intéressant à cette page pour cette méthode de construction, allez à la suivante"));
    }
    rafraichirZome();
}

void fenPrincipale::on_zomeNombreOrdre_valueChanged()
{
    zomeTestBugNbRangees();
    zomeTestBugNombreForme();
    rafraichirZome();
}

void fenPrincipale::on_zomeNombreForme_valueChanged()
{
    zomeTestBugNombreForme();
    rafraichirZome();
}

void fenPrincipale::on_zomeEchelle_valueChanged()
{
    rafraichirZome();
}

void fenPrincipale::on_zomeRangees_valueChanged()
{
    zomeTestBugNbRangees();
    rafraichirZome();
}

void fenPrincipale::on_zomeEpaisseurChevronMethode1_valueChanged()
{
    rafraichirZome();
}

void fenPrincipale::on_zomeRetombeeChevronMethode1_valueChanged()
{
    rafraichirZome();
}

// pour l heliyourte

void fenPrincipale::rafraichirHeliyourte()
{
    heliY->setParametres(fenPrincipale::ui->hlyDiametreYourte->value(),fenPrincipale::ui->hlyHauteurMur->value(),fenPrincipale::ui->hlyNombrePans->value(),fenPrincipale::ui->hlyPenteToit->value(),fenPrincipale::ui->hlyEpaisseurPerche->value(),fenPrincipale::ui->hlyRetombeePerches->value(),fenPrincipale::ui->hlyPercheDepassementHaut->value(),fenPrincipale::ui->hlyRetombeeTonoo->value(),fenPrincipale::ui->hlyRetombeeTraverse->value(),fenPrincipale::ui->hlyEpaisseurCroix->value(),fenPrincipale::ui->hlyRetombeeCroix->value());
    glWidgetHeliyourte->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteTraverse->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteCroix->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteMontant->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteMontantA->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteMontantB->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourteMontantC->rafraichirWidget(m_couleursDistinctes);
    glWidgetHeliyourtePerche->rafraichirWidget(m_couleursDistinctes);

    rafraichirResultatsHeliyourte();
}

void fenPrincipale::rafraichirResultatsHeliyourte()
{
    QString str;
    fenPrincipale::ui->hlySurfaceAuSol->setText(str.setNum(heliY->surfaceAuSol(),'f',1)+" m2 ");
    fenPrincipale::ui->hlySurfaceDeToit->setText(str.setNum(heliY->surfaceToit(),'f',1)+" m2 ");
    fenPrincipale::ui->hlySurfaceMur->setText(str.setNum(heliY->surfaceMur(),'f',1)+" m2 ");
    fenPrincipale::ui->hlyVolume->setText(str.setNum(heliY->volume(),'f',1)+" m3 ");
    fenPrincipale::ui->hlyHauteurTonoo->setText(str.setNum(heliY->hauteurTonoo(),'f',1)+" m ");
    fenPrincipale::ui->hlyTraverseGeneralTexte->setText(heliY->generalTraverse());
    fenPrincipale::ui->hlytableTraverseText->setText(heliY->tableauTraverse());
    fenPrincipale::ui->hlyCroixGeneralTexte->setText(heliY->generalCroix());
    fenPrincipale::ui->hlyTableCroix->setText(heliY->tableauCroix());
    fenPrincipale::ui->hlyPoteauMontantAText->setText(heliY->PoteauMontantA());
    fenPrincipale::ui->hlyPoteauxGeneralText->setText(heliY->generalPoteaux());
    fenPrincipale::ui->hlyPoteauMontantBtext->setText(heliY->PoteauMontantB());
    fenPrincipale::ui->hlyPoteauMontantCtext->setText(heliY->PoteauMontantC());
    fenPrincipale::ui->hlytablePoteauMontantA->setText(heliY->PoteauMontantATable());
    fenPrincipale::ui->hlytablePoteauMontantC->setText(heliY->PoteauMontantCTable());
    fenPrincipale::ui->hlyPercheGeneralText->setText(heliY->PercheGeneral());
    fenPrincipale::ui->hlyPercheTable->setText(heliY->PercheTable());
    fenPrincipale::ui->hlyDebitTexte->setHtml(heliY->debit());
    fenPrincipale::ui->hlyAideConstruction->setHtml(heliY->aideGenerale());
}

void fenPrincipale::on_hlyDiametreYourte_valueChanged()
{
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyEpaisseurCroix_valueChanged()
{
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyEpaisseurPerche_valueChanged()
{
    heliTestBugBoutPerche();
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyHauteurMur_valueChanged()
{
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyNombrePans_valueChanged()
{
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyPenteToit_valueChanged()
{
    heliTestBugBoutPerche();
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyPercheDepassementHaut_valueChanged()
{
    heliTestBugBoutPerche();
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyRetombeeCroix_valueChanged()
{
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyRetombeePerches_valueChanged()
{
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyRetombeeTonoo_valueChanged()
{
    heliTestBugBoutPerche();
    rafraichirHeliyourte();
}

void fenPrincipale::on_hlyRetombeeTraverse_valueChanged()
{
    rafraichirHeliyourte();
}

void fenPrincipale::heliTestBugBoutPerche()
{
    double thel_rayonYourte, thel_angleDivision, thel_dIntMur, thel_AB, thel_dIntTonoo, thel_angleInclinaisonPerches;
    double thel_rayonTonoo, thel_retombeeMiniPerches,thel_decalageTigeRapportBord;

    thel_rayonYourte = fenPrincipale::ui->hlyDiametreYourte->value()/2.0;
    thel_angleDivision = 2.0*pi/fenPrincipale::ui->hlyNombrePans->value();
    thel_dIntMur = 2.0*thel_rayonYourte*qSin(thel_angleDivision/2.0);

    double tphel_A, tphel_B, tphel_C, tphel_Delta, tphel_X2;
    double tphel_hIntTonoo, tphel_lprimePerche;

    tphel_A = qPow(qCos(thel_angleDivision/2.0),2.0);
    tphel_B = -qPow(thel_rayonYourte,2.0);
    tphel_C = qPow(thel_rayonYourte,4.0)*qPow(qSin(thel_angleDivision/2.0),2.0);
    tphel_Delta = qPow(tphel_B,2.0)-4.0*tphel_A*tphel_C;
    tphel_X2 = (-tphel_B-qSqrt(tphel_Delta))/(2.0*tphel_A);
    thel_rayonTonoo = qSqrt(tphel_X2);

    tphel_hIntTonoo = thel_rayonTonoo*qCos(thel_angleDivision/2.0);
    tphel_lprimePerche = qSqrt(qPow(thel_rayonYourte,2.0)-qPow(tphel_hIntTonoo,2.0));
    thel_dIntTonoo = 2.0*thel_rayonTonoo*qSin(thel_angleDivision/2.0);

    thel_decalageTigeRapportBord = ((fenPrincipale::ui->hlyEpaisseurPerche->value()/100.0)/qTan(thel_angleDivision)+(fenPrincipale::ui->hlyEpaisseurPerche->value()/100.0)/qSin(thel_angleDivision))/2.0;

    thel_AB = qSqrt(qPow((thel_dIntTonoo/2.0-tphel_lprimePerche),2.0)+qPow((thel_rayonYourte-thel_rayonTonoo),2.0)*qPow(qTan(radians(fenPrincipale::ui->hlyPenteToit->value())),2.0));
    thel_angleInclinaisonPerches = qAcos(qAbs(thel_dIntTonoo/2.0-tphel_lprimePerche)/thel_AB);
    thel_retombeeMiniPerches = (2.0*thel_decalageTigeRapportBord+thel_dIntTonoo)*qSin(thel_angleInclinaisonPerches);


    QString str;
    if(qTan(thel_angleInclinaisonPerches)*fenPrincipale::ui->hlyPercheDepassementHaut->value()>fenPrincipale::ui->hlyRetombeeTonoo->value())
    {
        QMessageBox::warning(this,QObject::tr("Dépassement haut de perche trop important"),QObject::tr("Le dépassement horizontal haut de la perche ne peut etre supérieur à")+" "+str.setNum((double)(fenPrincipale::ui->hlyRetombeeTonoo->value()/qTan(thel_angleInclinaisonPerches)),'f',1)+" cm, "+QObject::tr("ou alors il faut augmenter la retombée du tonoo, ou diminuer la pente du toit")+".");
        fenPrincipale::ui->hlyPercheDepassementHaut->setValue((fenPrincipale::ui->hlyRetombeeTonoo->value()/qTan(thel_angleInclinaisonPerches))-0.01);
    }

    double rajout = (fenPrincipale::ui->hlyRetombeeTonoo->value()-thel_retombeeMiniPerches*100.0/qCos(thel_angleInclinaisonPerches))/qTan(thel_angleInclinaisonPerches);
    if(fenPrincipale::ui->hlyPercheDepassementHaut->value()<rajout)
    {
        QMessageBox::warning(this,QObject::tr("Dépassement haut de perche trop faible"),QObject::tr("Le dépassement horizontal haut de la perche ne peut etre inférieur à")+" "+str.setNum((double)(rajout),'f',1)+" cm, "+QObject::tr("ou alors il faut diminuer la retombée du tonoo, ou augmenter la pente du toit")+".");
        fenPrincipale::ui->hlyPercheDepassementHaut->setValue(rajout+0.01);
    }
}

// et le geodome

void fenPrincipale::rafraichirGeodome()
{

    geode->setParametres(fenPrincipale::ui->geoSolideBase->currentIndex(),fenPrincipale::ui->geoFrequenceA->value(),fenPrincipale::ui->geoFrequenceB->value(),fenPrincipale::ui->geoDiametre->value(),fenPrincipale::ui->geoHauteur->value(),fenPrincipale::ui->geoAngleRotation->value(),fenPrincipale::ui->geoSabliereHorizontale->currentIndex(),fenPrincipale::ui->geoAffichage->currentIndex());

    glWidgetGeodome->rafraichirWidget(m_couleursDistinctes);

    rafraichirResultatsGeodome();
}

void fenPrincipale::rafraichirResultatsGeodome()
{
    QString str;
    fenPrincipale::ui->geoSurfaceAuSol->setText(str.setNum(geode->surfaceSol(),'f',1)+" m2 ");
    fenPrincipale::ui->geoSurfaceDeToit->setText(str.setNum(geode->surfaceToit(),'f',1)+" m2 ");
    fenPrincipale::ui->geoVolume->setText(str.setNum(geode->volume(),'f',1)+" m3 ");
    fenPrincipale::ui->geoHauteurReelle->setText(str.setNum(geode->hauteurReelle(),'f',2)+" m ");
    fenPrincipale::ui->geoDiametreSol->setText(str.setNum(geode->diametreSol(),'f',2)+" m ");
    fenPrincipale::ui->geoFacesTable->setText(geode->tableauFaces());
    fenPrincipale::ui->geoMontantTable->setText(geode->tableauMontants());
    fenPrincipale::ui->geoMontantText->setText(geode->resultatMontants());
    fenPrincipale::ui->geoSommetTable->setText(geode->tableauSommets());
    fenPrincipale::ui->geoFacesText->setText(geode->resultatFaces());
    fenPrincipale::ui->geoSommetTexte->setText(geode->resultatSommets());
    fenPrincipale::ui->geoAideConstruction->setText(geode->aideConstruction());

}

void fenPrincipale::on_ongletsGeodome_currentChanged(int choix)
{
    switch(choix)
    {
    case 1:
        if(geonbTypeFace!=-1)
            for(int i=0;i<geonbTypeFace;i++)
            {
                QLabel* image1 = new QLabel();
                QPixmap pix(300,370);
                geode->geoDessinFace(&pix,-1);
                image1->setPixmap(pix);
                fenPrincipale::ui->geoFaceGrille->addWidget(image1,(int)(i/3.0),i%3);
            }
        for(int i=0;i<geode->typeFace.size();i++)
        {
            QLabel* image1 = new QLabel();
            QPixmap pix(300,370);
            geode->geoDessinFace(&pix,i);
            image1->setPixmap(pix);
            fenPrincipale::ui->geoFaceGrille->addWidget(image1,(int)(i/3.0),i%3);
        }
        geonbTypeFace = geode->typeFace.size();
        break;
    case 3:
        if(geonbTypeSommet!=-1)
            for(int i=0;i<geonbTypeSommet;i++)
            {
                QLabel* image1 = new QLabel();
                QPixmap pix(300,370);
                geode->geoDessinSommet(&pix,-1);
                image1->setPixmap(pix);
                fenPrincipale::ui->geoSommetsGrilleImages->addWidget(image1,(int)(i/3.0),i%3);
            }
        for(int i=0;i<geode->typeSommet.size();i++)
        {
            QLabel* image1 = new QLabel();
            QPixmap pix(300,370);
            geode->geoDessinSommet(&pix,i);
            image1->setPixmap(pix);
            fenPrincipale::ui->geoSommetsGrilleImages->addWidget(image1,(int)(i/3.0),i%3);
        }
        geonbTypeSommet = geode->typeSommet.size();
        break;
    default:
        break;
    }
}

void fenPrincipale::on_geoSolideBase_currentIndexChanged(int choix)
{
    rafraichirGeodome();
}

void fenPrincipale::on_geoFrequenceA_valueChanged()
{
    rafraichirGeodome();
}

void fenPrincipale::on_geoFrequenceB_valueChanged()
{
    rafraichirGeodome();
}

void fenPrincipale::on_geoDiametre_valueChanged()
{
    rafraichirGeodome();
}

void fenPrincipale::on_geoHauteur_valueChanged()
{
    rafraichirGeodome();
}

void fenPrincipale::on_geoAngleRotation_valueChanged()
{
    rafraichirGeodome();
}

void fenPrincipale::on_geoSabliereHorizontale_currentIndexChanged(int choix)
{
    rafraichirGeodome();
}

void fenPrincipale::on_geoAffichage_currentIndexChanged(int choix)
{
    rafraichirGeodome();
}

//action des menus

void fenPrincipale::on_actionExporter_en_format_Obj_activated()
{
    QVector<QString> nom;
    QString sss;
    sss = QObject::tr("VouteNidAbeilleChevronBord")+(QString)(char)(fenPrincipale::ui->vnaSelectionChevronTronque->currentIndex()+65);
    nom << QObject::tr("VouteNidAbeille") << QObject::tr("VouteNidAbeilleChevron") << sss;
    nom << QObject::tr("DomeNidAbeille") << QObject::tr("DomeNidAbeilleChevronType")+(QString)(char)(fenPrincipale::ui->dnaSelectionChevron->currentIndex()+65);
    nom << QObject::tr("Zome") << QObject::tr("ZomeLosange")+sss.setNum(fenPrincipale::ui->zomeselectionLosange->currentIndex()+1);
    nom << QObject::tr("Heliyourte") << QObject::tr("HeliyourtePerche") << QObject::tr("HeliyourtePoteau") << QObject::tr("HeliyourteCroix") << QObject::tr("HeliyourteTraverse");
    nom << QObject::tr("Geodome");

    int ouh = 500;
    if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==0)
    {
        switch(fenPrincipale::ui->ongletsVouteNidAbeille->currentIndex())
        {
        case 0:
            ouh = 0;
            break;
        case 1:
            ouh = 1;
            break;
        case 2:
            ouh = 2;
            break;
        default:
            break;
        }
    }
    else if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==1)
    {

        switch(fenPrincipale::ui->ongletsDomeNidAbeille->currentIndex())
        {
        case 0:
            ouh = 3;
            break;
        case 1:
            ouh = 4;
            break;
        default:
            break;
        }
    }
    else if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==2)
    {

        switch(fenPrincipale::ui->ongletsZome->currentIndex())
        {
        case 0:
            ouh = 5;
            break;
        case 1:
            if(fenPrincipale::ui->zomeChoixMethodeConstructive->currentIndex() == 0)
            {
                ouh = 6;
            }
            break;
        default:
            break;
        }
    }
    else if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==3)
    {

        switch(fenPrincipale::ui->ongletsHeliyourte->currentIndex())
        {
        case 0:
            ouh = 7;
            break;
        case 1:
            ouh = 8;
            break;
        case 2:
            ouh = 9;
            break;
        case 3:
            ouh = 10;
            break;
        case 4:
            ouh = 11;
            break;
        default:
            break;
        }
    }
    else if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==4)
    {

        switch(fenPrincipale::ui->ongletsGeodome->currentIndex())
        {
        case 0:
            ouh = 12;
            break;
        default:
            break;
        }
    }

    if(ouh == 500)
    {
        QMessageBox::warning(this,QObject::tr("Pas d'export en format Obj 3D possible sur cet onglet"),QObject::tr("Cette fonction sert juste à exporter les objets 3D en format Wavefront Obj"));
    }
    else
    {
        QString contenu ="";
        QString nomDeFichier2 = QFileDialog::getSaveFileName(this,QObject::tr("Exporter sous")+" ...",nom[ouh]+".obj",QObject::tr("Fichier")+" 3D Wavefront (*.obj)");
        if(nomDeFichier2 !="")
        {
            QFile fichier(nomDeFichier2);
            fichier.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream flux(&fichier);
            nomDeFichier2.remove(".obj");
            contenu += "#"+nom[ouh]+"\n";
            contenu += "mtllib "+nomDeFichier2+".mtl\n";
            switch(ouh)
            {
            case 0:
                contenu += glWidgetVouteNidAbeille->exportObj();
            break;
            case 1:
                contenu += glWidgetVouteNidAbeilleChevronGauche->exportObj();
            break;
            case 2:
                contenu += glWidgetVouteNidAbeilleChevronTronque->exportObj();
            break;
            case 3:
                contenu += glWidgetDomeNidAbeille->exportObj();
            break;
            case 4:
                contenu += glWidgetDomeNidAbeilleChevron->exportObj();
            break;
            case 5:
                contenu += glWidgetZome->exportObj();
            break;
            case 6:
                contenu += glWidgetZomeLosange->exportObj();
            break;
            case 7:
                contenu += glWidgetHeliyourte->exportObj();
            break;
            case 8:
                contenu += glWidgetHeliyourtePerche->exportObj();
            break;
            case 9:
                contenu += glWidgetHeliyourteMontant->exportObj();
            break;
            case 10:
                contenu += glWidgetHeliyourteCroix->exportObj();
            break;
            case 11:
                contenu += glWidgetHeliyourteTraverse->exportObj();
            break;
            case 12:
                contenu += glWidgetGeodome->exportObj();
            break;
            }

            flux << contenu;
            fichier.close();

            QString contenumtl = "";
            QString str4,str5,str6;
            QFile fichiermtl(nomDeFichier2+".mtl");
            fichiermtl.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream fluxmtl(&fichiermtl);
            for(int ddd=0;ddd<38;ddd++)
            {
                double rouge =(double)couleur(ddd).red()/255.0;
                double vert =(double)couleur(ddd).green()/255.0;
                double bleu =(double)couleur(ddd).blue()/255.0;
                contenumtl += "newmtl couleur"+str4.setNum(ddd)+"\n";
                contenumtl += "Ka "+str4.setNum(rouge,'f',4)+" "+str5.setNum(vert,'f',4)+" "+str6.setNum(bleu,'f',4)+"\n";
                contenumtl += "Kd "+str4.setNum(rouge,'f',4)+" "+str5.setNum(vert,'f',4)+" "+str6.setNum(bleu,'f',4)+"\n";
                contenumtl += "Ks "+str4.setNum(rouge,'f',4)+" "+str5.setNum(vert,'f',4)+" "+str6.setNum(bleu,'f',4)+"\n";
                contenumtl += "NS 500\n\n";
            }
            fluxmtl << contenumtl;
            fichiermtl.close();

        }
    }

}


void fenPrincipale::on_actionExporter_resultats_PDF_activated()
{
    exporterEnPDF();
}

void fenPrincipale::exporterEnPDF()
{
    QTextEdit doc;
    QString str;
    QPrinter printer(QPrinter::HighResolution); // et lui s occupera de l impression du rendu
    QString html;
    QFile im1;
    QImage image, il;
    QEventLoop evtLoop;
    int indexOnglet;

    switch(fenPrincipale::ui->ongletsChoixStructure->currentIndex())
    {
        case 0: //voute nid abeille
        //la on recupere les captures d ecrans 3D
        indexOnglet = fenPrincipale::ui->ongletsVouteNidAbeille->currentIndex();
        fenPrincipale::ui->ongletsVouteNidAbeille->setCurrentIndex(0);

        QTimer::singleShot(500, &evtLoop, SLOT(quit()));
        evtLoop.exec();
        image = glWidgetVouteNidAbeille->captureEcran();
        il = image.scaled(600,600,Qt::KeepAspectRatio);
        il.save("im1.bmp");
        fenPrincipale::ui->ongletsVouteNidAbeille->setCurrentIndex(indexOnglet);

        // configuration du printer   format A4  rendu en PDF
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer un fichier"), QObject::tr("VouteNidAbeille")+".pdf", QObject::tr("Document")+" Pdf (*.pdf)"));

        // on donne au QTextEdit le code HTML a interpreter
        html = "<p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("Compte-rendu technique d'une Voute avec la technique Nid d'Abeille")+"</strong></span></p>";
        html += "<p align=right><span style=\"font-size: 12pt; color: #000000;\"><em>"+QObject::tr("Document généré automatiquement par le logiciel ZomeDomeEtCie (version")+" "+version()+") "+QObject::tr("développé par l'association ARDHEIA")+" <a href=\"http://ardheia.free.fr\">http://ardheia.free.fr</a></em></span></p>";
        html += "<p align=right><img height=\"91\" width=\"220\" src=\":/images/logoArdheia\" /></p>";
        html += "<p><br /><br /></p><p align=center><img src=\"im1.bmp\" /></p>";
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("M")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("ETHODE DE CONSTRUCTION")+"</strong></span></p>";
        html += vouteNA->explicationGenerale();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("D")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("EBIT")+"</strong></span></p>";
        html += vouteNA->debit();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("L")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("ES CHEVRONS ENTIERS")+" ...</strong></span></p>";
        html += vouteNA->explicationChevrons();
        html += "<p><br /><br /></p><p><span style=\"font-size: 14pt; color: #000000;\"><strong>"+str.setNum((double)vouteNA->nbreChevronsDroite(),'f',0)+" "+QObject::tr("Chevrons penchés à droite")+"</strong></span></p>";
        html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaDroitdeFace\" /></p>";
        html += vouteNA->tableauChevronDroite();
        html += "<p><br /><br /></p><p><span style=\"font-size: 14pt; color: #000000;\"><strong>"+str.setNum((double)vouteNA->nbreChevronsGauche(),'f',0)+" "+QObject::tr("Chevrons penchés à gauche")+"</strong></span></p>";
        html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaGauchedeFace\" /></p>";
        html += vouteNA->tableauChevronGauche();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 14pt; color: #000000;\"><strong>... </strong></span><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("E")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("T LES CHEVRONS TRONQUES")+"</strong></span></p>";
        for(int i=0;i<vouteNA->nbreChevronsTronques();i++)
        {
            html += "<p><br /><br /></p><p><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("Chevrons tronqués de Type")+" ";
            html+= (char)(i+65);
            html += " :</strong></span></p>";
            switch(vouteNA->orientationChevronTronque(i))
            {
            case 0:
            html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaDroitCoupeBas\" /></p>";
            break;
            case 1:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaDroitCoupeBas\" /></p>";
            break;
            case 2:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaDroitCoupeHaut\" /></p>";
            break;
            case 3:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaDroitCoupeHaut\" /></p>";
            break;
            case 5:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaCoupeDeuxBouts\" /></p>";
            break;
            case 6:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaCoupeDeuxBouts\" /></p>";
            break;
            case 8:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaGaucheCoupeHaut\" /></p>";
            break;
            case 9:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaGaucheCoupeBas\" /></p>";
            break;
            case 10:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaGaucheCoupeBas\" /></p>";
            break;
            case 11:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaGaucheCoupeHaut\" /></p>";
            break;
            case 12:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaCoupeDeuxBouts\" /></p>";
            break;
            case 15:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/vna/vnaCoupeDeuxBouts\" /></p>";
            break;
            }
            html += vouteNA->explicationChevronTronque(i);
            html += vouteNA->tableauChevronTronque(i);
        }
        doc.setHtml(html);
        doc.print(&printer); // impression de la page en utilisant le printer
        im1.setFileName("im1.bmp");
        im1.remove();
        break;

        case 1: //dome nid abeille
        //la on recupere les captures d ecrans 3D
        indexOnglet = fenPrincipale::ui->ongletsDomeNidAbeille->currentIndex();
        fenPrincipale::ui->ongletsDomeNidAbeille->setCurrentIndex(0);
        QTimer::singleShot(500, &evtLoop, SLOT(quit()));
        evtLoop.exec();
        image = glWidgetDomeNidAbeille->captureEcran();
        il = image.scaled(600,600,Qt::KeepAspectRatio);
        il.save("im1.bmp");
        fenPrincipale::ui->ongletsDomeNidAbeille->setCurrentIndex(indexOnglet);

        // configuration du printer   format A4  rendu en PDF
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer un fichier"), QObject::tr("DomeNidAbeille")+".pdf", QObject::tr("Document")+" Pdf (*.pdf)"));

        // on donne au QTextEdit le code HTML a interpreter
        html = "<p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("Compte-rendu technique d'un Dôme avec la technique Nid d'Abeille")+"</strong></span></p>";
        html += "<p align=right><span style=\"font-size: 12pt; color: #000000;\"><em>"+QObject::tr("Document généré automatiquement par le logiciel ZomeDomeEtCie (version")+" "+version()+") "+QObject::tr("développé par l'association ARDHEIA")+" <a href=\"http://ardheia.free.fr\">http://ardheia.free.fr</a></em></span></p>";
        html += "<p align=right><img height=\"91\" width=\"220\" src=\":/images/logoArdheia\" /></p>";
        html += "<p><br /><br /></p><p align=center><img src=\"im1.bmp\" /></p>";
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("M")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("ETHODE DE CONSTRUCTION")+"</strong></span></p>";
        html += domeNA->explicationGenerale();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("D")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("EBIT")+"</strong></span></p>";
        html += domeNA->debit();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("L")+"</strong></span><span style=\"font-size: 14pt; color: #ff9900;\"><strong>"+QObject::tr("ES CHEVRONS")+"</strong></span></p>";
        for(int i=0;i<domeNA->nbChevrons();i++)
        {
            html += "<p><br /><br /></p><p><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("Chevrons de Type")+" ";
            html+= (char)(i+65);
            html += " :</strong></span></p>";
            html += domeNA->explicationChevronNumero(i);
            switch(domeNA->orientation(i))
            {
            case 0:
            html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/dna/dnaDroitdeFace\" /></p>";
            break;
            case 1:
                html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/dna/dnaGauchedeFace\" /></p>";
            break;
            }
            html += domeNA->tableauChevronNumero(i);
        }
        doc.setHtml(html);
        doc.print(&printer); // impression de la page en utilisant le printer
        im1.setFileName("im1.bmp");
        im1.remove();
        break;
    case 2: //zome
        //la on recupere les captures d ecrans 3D
        indexOnglet = fenPrincipale::ui->ongletsZome->currentIndex();
        fenPrincipale::ui->ongletsZome->setCurrentIndex(0);
        QTimer::singleShot(500, &evtLoop, SLOT(quit()));
        evtLoop.exec();
        image = glWidgetZome->captureEcran();
        il = image.scaled(600,600,Qt::KeepAspectRatio);
        il.save("im1.bmp");
        fenPrincipale::ui->ongletsZome->setCurrentIndex(indexOnglet);

        // configuration du printer   format A4  rendu en PDF
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer un fichier"), QObject::tr("Zome")+".pdf", QObject::tr("Document")+" Pdf (*.pdf)"));

        // on donne au QTextEdit le code HTML a interpreter
        html = "<p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("Compte-rendu technique d'un Zome")+"</strong></span></p>";
        html += "<p align=right><span style=\"font-size: 12pt; color: #000000;\"><em>"+QObject::tr("Document généré automatiquement par le logiciel ZomeDomeEtCie (version")+" "+version()+") "+QObject::tr("développé par l'association ARDHEIA")+" <a href=\"http://ardheia.free.fr\">http://ardheia.free.fr</a></em></span></p>";
        html += "<p align=right><img height=\"91\" width=\"220\" src=\":/images/logoArdheia\" /></p>";
        html += "<p><br /><br /></p><p align=center><img src=\"im1.bmp\" /></p>";
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("M")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("ETHODE DE CONSTRUCTION")+"</strong></span></p>";
        html += zomito->explicationGenerale();
        if(fenPrincipale::ui->zomeChoixMethodeConstructive->currentIndex()!=2)
        {
            html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("D")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("EBIT")+"</strong></span></p>";
            html += zomito->debit();
            html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("L")+"</strong></span><span style=\"font-size: 14pt; color: #ff9900;\"><strong>"+QObject::tr("ES CHEVRONS")+"</strong></span></p>";
            html += "<p align=center><img height=\"317\" width=\"357\" src=\":/images/zome/zomLosange\" /></p>";
            for(int i=0;i<zomito->listeLosanges().size();i++)
            {
                html += "<p><br /><br /></p><p><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("Rangée")+" n° "+str.setNum((double)(i+1),'f',0);
                html += " :</strong></span></p>";
                if(zomito->triangle(i))
                    html += "<p align=center><img height=\"317\" width=\"357\" src=\":/images/zome/zomTriangle\" /></p>";
                html += zomito->explicationLosange(i);
            }
        }
        doc.setHtml(html);
        doc.print(&printer); // impression de la page en utilisant le printer
        im1.setFileName("im1.bmp");
        im1.remove();
    break;
    case 3: //heliyourte
        //la on recupere les captures d ecrans 3D
        indexOnglet = fenPrincipale::ui->ongletsHeliyourte->currentIndex();
        fenPrincipale::ui->ongletsHeliyourte->setCurrentIndex(0);
        QTimer::singleShot(500, &evtLoop, SLOT(quit()));
        evtLoop.exec();
        image = glWidgetHeliyourte->captureEcran();
        il = image.scaled(600,600,Qt::KeepAspectRatio);
        il.save("im1.bmp");
        fenPrincipale::ui->ongletsHeliyourte->setCurrentIndex(indexOnglet);

        // configuration du printer   format A4  rendu en PDF
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer un fichier"), QObject::tr("Heliyourte")+".pdf", QObject::tr("Document")+" Pdf (*.pdf)"));

        // on donne au QTextEdit le code HTML a interpreter
        html = "<p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("Compte-rendu technique d'une Héliyourte")+"</strong></span></p>";
        html += "<p align=right><span style=\"font-size: 12pt; color: #000000;\"><em>"+QObject::tr("Document généré automatiquement par le logiciel ZomeDomeEtCie (version")+" "+version()+") "+QObject::tr("développé par l'association ARDHEIA")+" <a href=\"http://ardheia.free.fr\">http://ardheia.free.fr</a></em></span></p>";
        html += "<p align=right><img height=\"91\" width=\"220\" src=\":/images/logoArdheia\" /></p>";
        html += "<p><br /><br /></p><p align=center><img src=\"im1.bmp\" /></p>";
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("M")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("ETHODE DE CONSTRUCTION")+"</strong></span></p>";
        html += heliY->aideGenerale();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("D")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("EBIT")+"</strong></span></p>";
        html += heliY->debit();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("L")+"</strong></span><span style=\"font-size: 14pt; color: #ff9900;\"><strong>"+QObject::tr("ES PERCHES")+"</strong></span></p>";
        html += heliY->PercheGeneral();
        html += "<p><br /></p><p align=center><img height=\"145\" width=\"526\" src=\":/images/hly/hlyPerche\" /></p>";
        html += "<p><br /></p>";
        html += heliY->PercheTable();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("L")+"</strong></span><span style=\"font-size: 14pt; color: #ff9900;\"><strong>"+QObject::tr("ES POTEAUX")+"</strong></span></p>";
        html += heliY->generalPoteaux();
        html += "<p><br /><br /></p><p><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("Montant A")+"</strong></span></p>";
        html += heliY->PoteauMontantA();
        html += "<p><br /></p>";
        html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/hly/hlyMontantA\" /></p>";
        html += "<p><br /></p>";
        html += heliY->PoteauMontantATable();
        html += "<p><br /><br /></p><p><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("Montant C")+"</strong></span></p>";
        html += heliY->PoteauMontantC();
        html += "<p><br /></p>";
        html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/hly/hlyMontantC\" /></p>";
        html += "<p><br /></p>";
        html += heliY->PoteauMontantCTable();
        html += "<p><br /><br /></p><p><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("Montant B")+"</strong></span></p>";
        html += heliY->PoteauMontantB();
        html += "<p><br /></p><p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("L")+"</strong></span><span style=\"font-size: 14pt; color: #ff9900;\"><strong>"+QObject::tr("ES CROIX")+"</strong></span></p>";
        html += heliY->generalCroix();
        html += "<p><br /></p>";
        html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/hly/hlyCroix\" /></p>";
        html += "<p><br /></p>";
        html += heliY->tableauCroix();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("L")+"</strong></span><span style=\"font-size: 14pt; color: #ff9900;\"><strong>"+QObject::tr("ES TRAVERSES")+"</strong></span></p>";
        html += heliY->generalTraverse();
        html += "<p><br /></p>";
        html += "<p align=center><img height=\"145\" width=\"526\" src=\":/images/hly/hlyTraverse\" /></p>";
        html += "<p><br /></p>";
        html += heliY->tableauTraverse();



        doc.setHtml(html);
        doc.print(&printer); // impression de la page en utilisant le printer
        im1.setFileName("im1.bmp");
        im1.remove();
    break;
    case 4: //geodome
        //la on recupere les captures d ecrans 3D
        indexOnglet = fenPrincipale::ui->ongletsGeodome->currentIndex();
        fenPrincipale::ui->ongletsGeodome->setCurrentIndex(0);
        QTimer::singleShot(500, &evtLoop, SLOT(quit()));
        evtLoop.exec();
        image = glWidgetGeodome->captureEcran();
        il = image.scaled(600,600,Qt::KeepAspectRatio);
        il.save("im1.bmp");
        fenPrincipale::ui->ongletsGeodome->setCurrentIndex(indexOnglet);

        // configuration du printer format A4  rendu en PDF
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer un fichier"), QObject::tr("Geodome")+".pdf", QObject::tr("Document")+" Pdf (*.pdf)"));

        QString cha,str,str2;
        if(fenPrincipale::ui->geoSolideBase->currentIndex()==0)
            cha = "III "+str.setNum(fenPrincipale::ui->geoFrequenceA->value())+" "+str2.setNum(fenPrincipale::ui->geoFrequenceB->value());
        if(fenPrincipale::ui->geoSolideBase->currentIndex()==1)
            cha = "IV "+str.setNum(fenPrincipale::ui->geoFrequenceA->value())+" "+str2.setNum(fenPrincipale::ui->geoFrequenceB->value());
        if(fenPrincipale::ui->geoSolideBase->currentIndex()==2)
            cha = "V "+str.setNum(fenPrincipale::ui->geoFrequenceA->value())+" "+str2.setNum(fenPrincipale::ui->geoFrequenceB->value());
        // on donne au QTextEdit le code HTML a interpreter
        html = "<p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("Compte-rendu technique d'un Geodome")+" "+cha+"</strong></span></p>";
        html += "<p align=right><span style=\"font-size: 12pt; color: #000000;\"><em>"+QObject::tr("Document généré automatiquement par le logiciel ZomeDomeEtCie (version")+" "+version()+") "+QObject::tr("développé par l'association ARDHEIA")+" <a href=\"http://ardheia.free.fr\">http://ardheia.free.fr</a></em></span></p>";
        html += "<p align=right><img height=\"91\" width=\"220\" src=\":/images/logoArdheia\" /></p>";
        html += "<p><br /><br /></p><p align=center><img src=\"im1.bmp\" /></p>";
        html += "<p><br /></p>";
        html += "<p><br /></p>";
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #ff9900;\"><strong>"+QObject::tr("M")+"</strong></span><span style=\"font-size: 14pt; color: #000000;\"><strong>"+QObject::tr("ETHODE DE CONSTRUCTION")+"</strong></span></p>";
        html += geode->aideConstruction();
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("F")+"</strong></span><span style=\"font-size: 14pt; color: #ff9900;\"><strong>"+QObject::tr("ACES")+"</strong></span></p>";
        html += geode->resultatFaces();
        html += "<p><br /></p><p align=center><img height=\"325\" width=\"408\" src=\":/images/geo/geoFace\" /></p>";
        html += "<p><br /></p>";
        html += geode->tableauFaces(1);
        html += "<table cellpadding=\"2\" border=\"0\" align=\"center\"><tbody>";
        for(int i=0;i<geode->typeFace.size()-1;i+=2)
        {
            QString strrr,strrr2;
            QImage im(300,370,QImage::Format_RGB32);
            geode->geoDessinFace(&im,i);
            im.save("im"+strrr.setNum(i+2)+".bmp");
            QImage im2(300,370,QImage::Format_RGB32);
            geode->geoDessinFace(&im2,i+1);
            im2.save("im"+strrr.setNum(i+3)+".bmp");
            html += "<tr><td><p align=\"center\"><img src=\"im"+strrr.setNum(i+2)+".bmp\" /></p></td><td><p align=\"center\"><img src=\"im"+strrr2.setNum(i+3)+".bmp\" /></p></td></tr>";
        }
        if(geode->typeFace.size()%2==1)
        {
            QString strrr;
            QImage im2(300,370,QImage::Format_RGB32);
            geode->geoDessinFace(&im2,geode->typeFace.size()-1);
            im2.save("im"+strrr.setNum(geode->typeFace.size()+1)+".bmp");
            html += "<tr><td><p align=\"center\"><img src=\"im"+strrr.setNum(geode->typeFace.size()-1)+".bmp\" /></p></td><td><p align=\"center\"></p></td></tr>";
        }
        html += "</tbody></table>";
        html += "<p><br /><br /></p><p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("A")+"</strong></span><span style=\"font-size: 14pt; color: #ff9900;\"><strong>"+QObject::tr("RETES")+"</strong></span></p>";
        html += geode->resultatMontants();
        html += "<p><br /></p><p align=center><img height=\"119\" width=\"461\" src=\":/images/geo/geoArete\" /></p>";
        html += "<p><br /></p>";
        html += geode->tableauMontants();
        html += "<p><br /></p><p align=center><span style=\"font-size: 18pt; color: #000000;\"><strong>"+QObject::tr("S")+"</strong></span><span style=\"font-size: 14pt; color: #ff9900;\"><strong>"+QObject::tr("OMMETS")+"</strong></span></p>";
        html += geode->resultatSommets();
        html += "<p><br /></p>";
        html += geode->tableauSommets(1);
        html += "<table cellpadding=\"2\" border=\"0\" align=\"center\"><tbody>";
        for(int i=0;i<geode->typeSommet.size()-1;i+=2)
        {
            QString strrr,strrr2;
            QImage im(300,370,QImage::Format_RGB32);
            geode->geoDessinSommet(&im,i);
            im.save("ims"+strrr.setNum(i+2)+".bmp");
            QImage im2(300,370,QImage::Format_RGB32);
            geode->geoDessinSommet(&im2,i+1);
            im2.save("ims"+strrr.setNum(i+3)+".bmp");
            html += "<tr><td><p align=\"center\"><img src=\"ims"+strrr.setNum(i+2)+".bmp\" /></p></td><td><p align=\"center\"><img src=\"ims"+strrr2.setNum(i+3)+".bmp\" /></p></td></tr>";
        }
        if(geode->typeSommet.size()%2==1)
        {
            QString strrr;
            QImage im2(300,370,QImage::Format_RGB32);
            geode->geoDessinSommet(&im2,geode->typeSommet.size()-1);
            im2.save("im"+strrr.setNum(geode->typeSommet.size()+1)+".bmp");
            html += "<tr><td><p align=\"center\"><img src=\"im"+strrr.setNum(geode->typeSommet.size()-1)+".bmp\" /></p></td><td><p align=\"center\"></p></td></tr>";
        }
        html += "</tbody></table>";

        doc.setHtml(html);
        doc.print(&printer); // impression de la page en utilisant le printer
        im1.setFileName("im1.bmp");
        im1.remove();
        QString kk;
        for(int i=0;i<geode->typeFace.size();i++)
        {
            im1.setFileName("im"+kk.setNum(i+1)+".bmp");
            im1.remove();
        }
        for(int i=0;i<geode->typeSommet.size();i++)
        {
            im1.setFileName("ims"+kk.setNum(i+1)+".bmp");
            im1.remove();
        }
    break;
    }

}

void fenPrincipale::on_actionExporter_capture_3D_activated()
{
    QImage image;
    if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==0)
    {
        switch(fenPrincipale::ui->ongletsVouteNidAbeille->currentIndex())
        {
        case 0:
            image = glWidgetVouteNidAbeille->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D de la voute en nid d'abeille"), QObject::tr("generalVouteNidAbeille")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        case 1:
            image = glWidgetVouteNidAbeilleChevronDroite->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D d'un chevron penché à droite"), QObject::tr("chevronDroiteVouteNidAbeille")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        case 2:
            image = glWidgetVouteNidAbeilleChevronTronque->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D d'un chevron tronqué"), QObject::tr("chevronTronqueVouteNidAbeille")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        default:
            QMessageBox::warning(this,QObject::tr("Pas de visualisation 3D sur cet onglet"),QObject::tr("Cette fonction sert juste à capturer et enregistrer les vues 3D"));
            break;
        }
    }
    else if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==1)
    {

        switch(fenPrincipale::ui->ongletsDomeNidAbeille->currentIndex())
        {
        case 0:
            image = glWidgetDomeNidAbeille->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D du dome en nid d'abeille"), QObject::tr("generalDomeNidAbeille")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        case 1:
            image = glWidgetDomeNidAbeilleChevron->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D d'un chevron du dome en nid d'abeille"), QObject::tr("chevronDomeNidAbeille")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        default:
            QMessageBox::warning(this,QObject::tr("Pas de visualisation 3D sur cet onglet"),QObject::tr("Cette fonction sert juste à capturer et enregistrer les vues 3D"));
            break;
        }
    }
    else if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==2)
    {

        switch(fenPrincipale::ui->ongletsZome->currentIndex())
        {
        case 0:
            image = glWidgetZome->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D du zome"), QObject::tr("generalZome")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        case 1:
            image = glWidgetZomeLosange->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D d'un element de zome"), QObject::tr("elementZome")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        default:
            QMessageBox::warning(this,QObject::tr("Pas de visualisation 3D sur cet onglet"),QObject::tr("Cette fonction sert juste à capturer et enregistrer les vues 3D"));
            break;
        }
    }
    else if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==3)
    {

        switch(fenPrincipale::ui->ongletsHeliyourte->currentIndex())
        {
        case 0:
            image = glWidgetHeliyourte->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D de l'héliyourte"), QObject::tr("generalHeliyourte")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        case 1:
            image = glWidgetHeliyourtePerche->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D d'une perche de l'héliyourte"), QObject::tr("percheHeliyourte")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Image")+"s Bmp (*.bmp)"),0,100);
            break;
        case 2:
            image = glWidgetHeliyourteMontant->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D d'un poteau de l'héliyourte"), QObject::tr("poteauHeliyourte")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        case 3:
            image = glWidgetHeliyourteCroix->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D d'une croix de l'héliyourte"), QObject::tr("croixHeliyourte")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        case 4:
            image = glWidgetHeliyourteTraverse->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D d'une traverse de l'héliyourte"), QObject::tr("traverseHeliyourte")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        default:
            QMessageBox::warning(this,QObject::tr("Pas de visualisation 3D sur cet onglet"),QObject::tr("Cette fonction sert juste à capturer et enregistrer les vues 3D"));
            break;
        }
    }
    else if(fenPrincipale::ui->ongletsChoixStructure->currentIndex()==4)
    {

        switch(fenPrincipale::ui->ongletsGeodome->currentIndex())
        {
        case 0:
            image = glWidgetGeodome->captureEcran();
            image.save(QFileDialog::getSaveFileName(this, QObject::tr("Enregistrer capture 3D du Geodome"), QObject::tr("Geodome")+".jpg", QObject::tr("Images")+" Jpeg (*.jpg);;"+QObject::tr("Images")+" Bmp (*.bmp)"),0,100);
            break;
        default:
            QMessageBox::warning(this,QObject::tr("Pas de visualisation 3D sur cet onglet"),QObject::tr("Cette fonction sert juste à capturer et enregistrer les vues 3D"));
            break;
        }
    }
}

void fenPrincipale::on_actionOuvrir_activated()
{
    //la on recupere l arbre xml dans le fichier a ouvrir
    nomDeFichier = QFileDialog::getOpenFileName(this, QObject::tr("Ouvrir"), "", QObject::tr("Sauvegarde")+" ZomeDomeEtCie (*.zdc)");
    ouvrir(nomDeFichier);
}

void fenPrincipale::ouvrir(QString nom)
{
    QDomDocument doc;
    if(nom=="")
        return;
    QFile fichier(nom);
    if(!fichier.open(QIODevice::ReadOnly))
    {
         QMessageBox::warning(this,QObject::tr("Erreur à l'ouverture de la sauvegarde"),QObject::tr("La sauvegarde n'a pas pu être ouverte")+".");
         return;
    }
    if (!doc.setContent(&fichier))
    {
         fichier.close();
         QMessageBox::warning(this,QObject::tr("Erreur à la lecture du fichier sauvegarde"),QObject::tr("il y a des erreurs dans le fichier")+".");
         return;
     }

    //si tout s est bien passe on va parcourir cette arbre a la recherche d infos
    QDomElement root=doc.documentElement();
    QDomElement child=root.firstChild().toElement();
    while(!child.isNull())
    {
      //la voute nid abeille
      if (child.tagName() == "VouteNidAbeille")
      {
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
        {
            if (grandChild.tagName() == "general")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "type")
                            fenPrincipale::ui->vnaChoixTypeVoute->setCurrentIndex(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "profondeur")
                            fenPrincipale::ui->vnaProfondeurVoute->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "hauteur")
                            fenPrincipale::ui->vnaHauteurVoute->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "portee")
                            fenPrincipale::ui->vnaPortee->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "rayon")
                            fenPrincipale::ui->vnaRayonCourbureVoute->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "rangees")
                            fenPrincipale::ui->vnaRangeesVerticales->setValue(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "angleBeta")
                            fenPrincipale::ui->vnaAngleBeta->setValue(grandChild2.text().toInt());
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            if (grandChild.tagName() == "bois")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "chevrons")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->vnaEpaisseurChevron->setValue(grandChild3.text().toDouble());
                                if (grandChild3.tagName() == "largeur")
                                    fenPrincipale::ui->vnaRetombeeChevron->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        if (grandChild2.tagName() == "faitiere")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->vnaEpaisseurFaitiere->setValue(grandChild3.text().toDouble());
                                if (grandChild3.tagName() == "retombee")
                                    fenPrincipale::ui->vnaRetombeeFaitiere->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        if (grandChild2.tagName() == "sabliere")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->vnaEpaisseurSabliere->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        if (grandChild2.tagName() == "rives")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->vnaEpaisseurRive->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            if (grandChild.tagName() == "distances")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "decalage")
                            fenPrincipale::ui->vnaDecalageNoeud->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "noeudFaitiere")
                            fenPrincipale::ui->vnaDistanceNoeudFaitiere->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "noeudSabliere")
                            fenPrincipale::ui->vnaDistanceNoeudSabliere->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "noeudRiveGauche")
                            fenPrincipale::ui->vnaDistanceNoeudRiveGauche->setValue(grandChild2.text().toDouble());
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            grandChild = grandChild.nextSibling().toElement();
        }
      }
      //la voute nid abeille
      if (child.tagName() == "DomeNidAbeille")
      {
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
        {
            if (grandChild.tagName() == "general")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "type")
                            fenPrincipale::ui->dnaChoixTypeDome->setCurrentIndex(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "hauteur")
                            fenPrincipale::ui->dnaHauteurDome->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "diametreSol")
                            fenPrincipale::ui->dnaDiametreSol->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "diametreTonoo")
                            fenPrincipale::ui->dnaDiametreTonoo->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "rayon")
                            fenPrincipale::ui->dnaRayonCourbureDome->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "nbLosanges")
                            fenPrincipale::ui->dnaNbreLosangesHorizontal->setValue(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "diagonale")
                            fenPrincipale::ui->dnaDiagonaleDernierLosange->setValue(grandChild2.text().toDouble());
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            if (grandChild.tagName() == "bois")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "chevrons")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->dnaEpaisseurChevron->setValue(grandChild3.text().toDouble());
                                if (grandChild3.tagName() == "retombee")
                                    fenPrincipale::ui->dnaRetombeeChevron->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        if (grandChild2.tagName() == "tonoo")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->dnaEpaisseurTonoo->setValue(grandChild3.text().toDouble());
                                if (grandChild3.tagName() == "retombee")
                                    fenPrincipale::ui->dnaRetombeeTonoo->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        if (grandChild2.tagName() == "sabliere")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->dnaEpaisseurSabliere->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            if (grandChild.tagName() == "distances")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "decalage")
                            fenPrincipale::ui->dnaDecalageNoeud->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "noeudTonoo")
                            fenPrincipale::ui->dnaDistanceNoeudTonoo->setValue(grandChild2.text().toDouble());
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            grandChild = grandChild.nextSibling().toElement();
        }
      }

      //le zome
      if (child.tagName() == "Zome")
      {
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
        {
            if (grandChild.tagName() == "general")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "methode")
                            fenPrincipale::ui->zomeChoixMethodeConstructive->setCurrentIndex(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "nombreOrdre")
                            fenPrincipale::ui->zomeNombreOrdre->setValue(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "nombreForme")
                            fenPrincipale::ui->zomeNombreForme->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "echelle")
                            fenPrincipale::ui->zomeEchelle->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "rangees")
                            fenPrincipale::ui->zomeRangees->setValue(grandChild2.text().toInt());
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            if (grandChild.tagName() == "bois")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "methode1")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->zomeEpaisseurChevronMethode1->setValue(grandChild3.text().toDouble());
                                if (grandChild3.tagName() == "retombee")
                                    fenPrincipale::ui->zomeRetombeeChevronMethode1->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            grandChild = grandChild.nextSibling().toElement();
        }
      }     

      //l heliyourte
      if (child.tagName() == "Heliyourte")
      {
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
        {
            if (grandChild.tagName() == "general")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "diametre")
                            fenPrincipale::ui->hlyDiametreYourte->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "hauteurMur")
                            fenPrincipale::ui->hlyHauteurMur->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "nombrePans")
                            fenPrincipale::ui->hlyNombrePans->setValue(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "penteToit")
                            fenPrincipale::ui->hlyPenteToit->setValue(grandChild2.text().toInt());
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            if (grandChild.tagName() == "bois")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "perches")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->hlyEpaisseurPerche->setValue(grandChild3.text().toDouble());
                                if (grandChild3.tagName() == "retombee")
                                    fenPrincipale::ui->hlyRetombeePerches->setValue(grandChild3.text().toDouble());
                                if (grandChild3.tagName() == "surplusHaut")
                                    fenPrincipale::ui->hlyPercheDepassementHaut->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        if (grandChild2.tagName() == "tonoo")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "retombee")
                                    fenPrincipale::ui->hlyRetombeeTonoo->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        if (grandChild2.tagName() == "traverse")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "retombee")
                                    fenPrincipale::ui->hlyRetombeeTraverse->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        if (grandChild2.tagName() == "croix")
                        {
                            QDomElement grandChild3=grandChild2.firstChild().toElement();
                            while(!grandChild3.isNull())
                            {
                                if (grandChild3.tagName() == "epaisseur")
                                    fenPrincipale::ui->hlyEpaisseurCroix->setValue(grandChild3.text().toDouble());
                                if (grandChild3.tagName() == "retombee")
                                    fenPrincipale::ui->hlyRetombeeCroix->setValue(grandChild3.text().toDouble());
                                grandChild3 = grandChild3.nextSibling().toElement();
                            }
                        }
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            grandChild = grandChild.nextSibling().toElement();
        }
      }

      //le geodome
      if (child.tagName() == "Geodome")
      {
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
        {
            if (grandChild.tagName() == "general")
                {
                    QDomElement grandChild2=grandChild.firstChild().toElement();
                    while(!grandChild2.isNull())
                    {
                        if (grandChild2.tagName() == "solideBase")
                            fenPrincipale::ui->geoSolideBase->setCurrentIndex(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "frequenceA")
                            fenPrincipale::ui->geoFrequenceA->setValue(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "frequenceB")
                            fenPrincipale::ui->geoFrequenceB->setValue(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "diametre")
                            fenPrincipale::ui->geoDiametre->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "hauteur")
                            fenPrincipale::ui->geoHauteur->setValue(grandChild2.text().toDouble());
                        if (grandChild2.tagName() == "angle")
                            fenPrincipale::ui->geoAngleRotation->setValue(grandChild2.text().toInt());
                        if (grandChild2.tagName() == "solideBase")
                            fenPrincipale::ui->geoSabliereHorizontale->setCurrentIndex(grandChild2.text().toInt());
                        grandChild2 = grandChild2.nextSibling().toElement();
                    }
                }
            grandChild = grandChild.nextSibling().toElement();
        }
      }

      child = child.nextSibling().toElement();
    }

    rafraichirVouteNidAbeille();
    rafraichirDomeNidAbeille();
    rafraichirZome();
    rafraichirHeliyourte();
    rafraichirGeodome();
}

void fenPrincipale::on_actionQuitter_activated()
{
    int reponse = QMessageBox::question(this, QObject::tr("Fin"), QObject::tr("Etes-vous sûr(e) de vouloir quitter ?"), QMessageBox::Yes | QMessageBox::No);
    if (reponse == QMessageBox::Yes)
    {
        QCoreApplication::quit();
    }
}

void fenPrincipale::enregistrer()
{
    QFile fichier(nomDeFichier);
    fichier.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream flux(&fichier);

    QString str;
    QDomImplementation impl = QDomDocument().implementation();
    // document with document type
    QString name = "ZomeDomeetCie";
    QString publicId = "";//-//XADECK//DTD Stone 1.0 //EN";
    QString systemId = "";//http://www-imagis.imag.fr/DTD/stone1.dtd";
    QDomDocument doc(impl.createDocumentType(name,publicId,systemId));

    doc.appendChild(doc.createComment("Ce fichier est fichier de sauvegarde du logiciel ZomeDomeEtCie développé par l'association ARDHEIA"));
    doc.appendChild(doc.createComment("http://ardheia.free.fr"));
    doc.appendChild(doc.createComment("La version du logiciel est la : "+version()));
    doc.appendChild(doc.createTextNode("\n")); // on retourne a la ligne

    //noeud principal vna
    QDomElement structureNoeud = doc.createElement("structure");
    doc.appendChild(structureNoeud);
    QDomElement vnastructureNoeud = doc.createElement("VouteNidAbeille");

    //general vna
    QDomElement vnageneralNoeud = doc.createElement("general");
    QDomElement vnatypeVouteNoeud = doc.createElement("type");
    vnatypeVouteNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->vnaChoixTypeVoute->currentIndex(),'f',0)));
    QDomElement vnaprofondeurNoeud = doc.createElement("profondeur");
    vnaprofondeurNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaProfondeurVoute->value(),'f',2)));
    QDomElement vnahauteurNoeud = doc.createElement("hauteur");
    vnahauteurNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaHauteurVoute->value(),'f',2)));
    QDomElement vnaporteeNoeud = doc.createElement("portee");
    vnaporteeNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaPortee->value(),'f',2)));
    QDomElement vnarayonNoeud = doc.createElement("rayon");
    vnarayonNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaRayonCourbureVoute->value(),'f',2)));
    QDomElement vnarangeesNoeud = doc.createElement("rangees");
    vnarangeesNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->vnaRangeesVerticales->value(),'f',0)));
    QDomElement vnaangleBetaNoeud = doc.createElement("angleBeta");
    vnaangleBetaNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->vnaAngleBeta->value(),'f',0)));

    vnageneralNoeud.appendChild(vnatypeVouteNoeud);
    vnageneralNoeud.appendChild(vnaprofondeurNoeud);
    vnageneralNoeud.appendChild(vnahauteurNoeud);
    vnageneralNoeud.appendChild(vnaporteeNoeud);
    vnageneralNoeud.appendChild(vnarayonNoeud);
    vnageneralNoeud.appendChild(vnarangeesNoeud);
    vnageneralNoeud.appendChild(vnaangleBetaNoeud);
    vnastructureNoeud.appendChild(vnageneralNoeud);

    // bois
    QDomElement vnaboisNoeud = doc.createElement("bois");
    QDomElement vnachevronsNoeud = doc.createElement("chevrons");
    QDomElement vnachevEpaisseur = doc.createElement("epaisseur");
    vnachevEpaisseur.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaEpaisseurChevron->value(),'f',1)));
    QDomElement vnachevLargeur = doc.createElement("largeur");
    vnachevLargeur.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaRetombeeChevron->value(),'f',1)));
    vnachevronsNoeud.appendChild(vnachevEpaisseur);
    vnachevronsNoeud.appendChild(vnachevLargeur);
    vnaboisNoeud.appendChild(vnachevronsNoeud);

    QDomElement vnafaitiereNoeud = doc.createElement("faitiere");
    QDomElement vnafaitEpaisseur = doc.createElement("epaisseur");
    vnafaitEpaisseur.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaEpaisseurFaitiere->value(),'f',1)));
    QDomElement vnafaitRetombee = doc.createElement("retombee");
    vnafaitRetombee.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaRetombeeFaitiere->value(),'f',1)));
    vnafaitiereNoeud.appendChild(vnafaitEpaisseur);
    vnafaitiereNoeud.appendChild(vnafaitRetombee);
    vnaboisNoeud.appendChild(vnafaitiereNoeud);

    QDomElement vnasabliereNoeud = doc.createElement("sabliere");
    QDomElement vnasabEpaisseur = doc.createElement("epaisseur");
    vnasabEpaisseur.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaEpaisseurSabliere->value(),'f',1)));
    vnasabliereNoeud.appendChild(vnasabEpaisseur);
    vnaboisNoeud.appendChild(vnasabliereNoeud);

    QDomElement vnarivesNoeud = doc.createElement("rives");
    QDomElement vnariveEpaisseur = doc.createElement("epaisseur");
    vnariveEpaisseur.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaEpaisseurRive->value(),'f',1)));
    vnarivesNoeud.appendChild(vnariveEpaisseur);
    vnaboisNoeud.appendChild(vnarivesNoeud);

    vnastructureNoeud.appendChild(vnaboisNoeud);

    //Distances et noeuds
    QDomElement vnadistancesNoeud = doc.createElement("distances");
    QDomElement vnadecalageNoeud = doc.createElement("decalage");
    vnadecalageNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaDecalageNoeud->value(),'f',1)));
    QDomElement vnanoeudFaitiere = doc.createElement("noeudFaitiere");
    vnanoeudFaitiere.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaDistanceNoeudFaitiere->value(),'f',1)));
    QDomElement vnanoeudSabliere = doc.createElement("noeudSabliere");
    vnanoeudSabliere.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaDistanceNoeudSabliere->value(),'f',1)));
    QDomElement vnanoeudRiveGauche = doc.createElement("noeudRiveGauche");
    vnanoeudRiveGauche.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->vnaDistanceNoeudRiveGauche->value(),'f',1)));
    vnadistancesNoeud.appendChild(vnadecalageNoeud);
    vnadistancesNoeud.appendChild(vnanoeudFaitiere);
    vnadistancesNoeud.appendChild(vnanoeudSabliere);
    vnadistancesNoeud.appendChild(vnanoeudRiveGauche);
    vnastructureNoeud.appendChild(vnadistancesNoeud);

    structureNoeud.appendChild(vnastructureNoeud);

    //et le dome en nid abeille
    QDomElement dnastructureNoeud = doc.createElement("DomeNidAbeille");

    //general dna
    QDomElement dnageneralNoeud = doc.createElement("general");
    QDomElement dnatypeDomeNoeud = doc.createElement("type");
    dnatypeDomeNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->dnaChoixTypeDome->currentIndex(),'f',0)));
    QDomElement dnahauteurNoeud = doc.createElement("hauteur");
    dnahauteurNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaHauteurDome->value(),'f',2)));
    QDomElement dnadiametreSolNoeud = doc.createElement("diametreSol");
    dnadiametreSolNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaDiametreSol->value(),'f',2)));
    QDomElement dnadiametreTonooNoeud = doc.createElement("diametreTonoo");
    dnadiametreTonooNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaDiametreTonoo->value(),'f',2)));
    QDomElement dnarayonNoeud = doc.createElement("rayon");
    dnarayonNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaRayonCourbureDome->value(),'f',2)));
    QDomElement dnanbreLosangesNoeud = doc.createElement("nbLosanges");
    dnanbreLosangesNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->dnaNbreLosangesHorizontal->value(),'f',0)));
    QDomElement dnadiagonaleNoeud = doc.createElement("diagonale");
    dnadiagonaleNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaDiagonaleDernierLosange->value(),'f',2)));

    dnageneralNoeud.appendChild(dnatypeDomeNoeud);
    dnageneralNoeud.appendChild(dnahauteurNoeud);
    dnageneralNoeud.appendChild(dnadiametreSolNoeud);
    dnageneralNoeud.appendChild(dnadiametreTonooNoeud);
    dnageneralNoeud.appendChild(dnarayonNoeud);
    dnageneralNoeud.appendChild(dnanbreLosangesNoeud);
    dnageneralNoeud.appendChild(dnadiagonaleNoeud);
    dnastructureNoeud.appendChild(dnageneralNoeud);

    // bois
    QDomElement dnaboisNoeud = doc.createElement("bois");
    QDomElement dnachevronsNoeud = doc.createElement("chevrons");
    QDomElement dnachevEpaisseur = doc.createElement("epaisseur");
    dnachevEpaisseur.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaEpaisseurChevron->value(),'f',1)));
    QDomElement dnachevRetombee = doc.createElement("retombee");
    dnachevRetombee.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaRetombeeChevron->value(),'f',1)));
    dnachevronsNoeud.appendChild(dnachevEpaisseur);
    dnachevronsNoeud.appendChild(dnachevRetombee);
    dnaboisNoeud.appendChild(dnachevronsNoeud);

    QDomElement dnatonooNoeud = doc.createElement("tonoo");
    QDomElement dnatonooEpaisseur = doc.createElement("epaisseur");
    dnatonooEpaisseur.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaEpaisseurTonoo->value(),'f',1)));
    QDomElement dnatonooRetombee = doc.createElement("retombee");
    dnatonooRetombee.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaRetombeeTonoo->value(),'f',1)));
    dnatonooNoeud.appendChild(dnatonooEpaisseur);
    dnatonooNoeud.appendChild(dnatonooRetombee);
    dnaboisNoeud.appendChild(dnatonooNoeud);

    QDomElement dnasabliereNoeud = doc.createElement("sabliere");
    QDomElement dnasabEpaisseur = doc.createElement("epaisseur");
    dnasabEpaisseur.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaEpaisseurSabliere->value(),'f',1)));
    dnasabliereNoeud.appendChild(dnasabEpaisseur);
    dnaboisNoeud.appendChild(dnasabliereNoeud);

    dnastructureNoeud.appendChild(dnaboisNoeud);

    //Distances et noeuds
    QDomElement dnadistancesNoeud = doc.createElement("distances");
    QDomElement dnadecalageNoeud = doc.createElement("decalage");
    dnadecalageNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaDecalageNoeud->value(),'f',1)));
    QDomElement dnanoeudTonoo = doc.createElement("noeudTonoo");
    dnanoeudTonoo.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->dnaDistanceNoeudTonoo->value(),'f',1)));
    dnadistancesNoeud.appendChild(dnadecalageNoeud);
    dnadistancesNoeud.appendChild(dnanoeudTonoo);
    dnastructureNoeud.appendChild(dnadistancesNoeud);

    structureNoeud.appendChild(dnastructureNoeud);

    //et le zome
    QDomElement zomstructureNoeud = doc.createElement("Zome");

    //general zome
    QDomElement zomgeneralNoeud = doc.createElement("general");
    QDomElement zommethodeConstructionNoeud = doc.createElement("methode");
    zommethodeConstructionNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->zomeChoixMethodeConstructive->currentIndex(),'f',0)));
    QDomElement zomnombreOrdreNoeud = doc.createElement("nombreOrdre");
    zomnombreOrdreNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->zomeNombreOrdre->value(),'f',0)));
    QDomElement zomnombreFormeNoeud = doc.createElement("nombreForme");
    zomnombreFormeNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->zomeNombreForme->value(),'f',2)));
    QDomElement zomechelleNoeud = doc.createElement("echelle");
    zomechelleNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->zomeEchelle->value(),'f',2)));
    QDomElement zomerangeesNoeud = doc.createElement("rangees");
    zomerangeesNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->zomeRangees->value(),'f',0)));

    zomgeneralNoeud.appendChild(zommethodeConstructionNoeud);
    zomgeneralNoeud.appendChild(zomnombreOrdreNoeud);
    zomgeneralNoeud.appendChild(zomnombreFormeNoeud);
    zomgeneralNoeud.appendChild(zomechelleNoeud);
    zomgeneralNoeud.appendChild(zomerangeesNoeud);
    zomstructureNoeud.appendChild(zomgeneralNoeud);

    // bois
    QDomElement zomboisNoeud = doc.createElement("bois");
    QDomElement zommethode1Noeud = doc.createElement("methode1");
    QDomElement zomepaisseurPlancheMethode1 = doc.createElement("epaisseur");
    zomepaisseurPlancheMethode1.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->zomeEpaisseurChevronMethode1->value(),'f',1)));
    QDomElement zomeretombeePlancheMethode1 = doc.createElement("retombee");
    zomeretombeePlancheMethode1.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->zomeRetombeeChevronMethode1->value(),'f',1)));
    zommethode1Noeud.appendChild(zomepaisseurPlancheMethode1);
    zommethode1Noeud.appendChild(zomeretombeePlancheMethode1);
    zomboisNoeud.appendChild(zommethode1Noeud);

    structureNoeud.appendChild(zomstructureNoeud);

    //et l heliyourte
    QDomElement hlystructureNoeud = doc.createElement("Heliyourte");

    //general heliyourte
    QDomElement hlygeneralNoeud = doc.createElement("general");
    QDomElement hlyDiametreNoeud = doc.createElement("diametre");
    hlyDiametreNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->hlyDiametreYourte->value(),'f',2)));
    QDomElement hlyHauteurMurNoeud = doc.createElement("hauteurMur");
    hlyHauteurMurNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->hlyHauteurMur->value(),'f',2)));
    QDomElement hlyNombrePansNoeud = doc.createElement("nombrePans");
    hlyNombrePansNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->hlyNombrePans->value(),'f',0)));
    QDomElement hlyPenteToitNoeud = doc.createElement("penteToit");
    hlyPenteToitNoeud.appendChild(doc.createTextNode(str.setNum((double)fenPrincipale::ui->hlyPenteToit->value(),'f',0)));

    hlygeneralNoeud.appendChild(hlyDiametreNoeud);
    hlygeneralNoeud.appendChild(hlyHauteurMurNoeud);
    hlygeneralNoeud.appendChild(hlyNombrePansNoeud);
    hlygeneralNoeud.appendChild(hlyPenteToitNoeud);
    hlystructureNoeud.appendChild(hlygeneralNoeud);

    // bois
    QDomElement hlyboisNoeud = doc.createElement("bois");
    QDomElement hlyPerchesNoeud = doc.createElement("perches");
    QDomElement hlyPerchesEpaisseurNoeud = doc.createElement("epaisseur");
    hlyPerchesEpaisseurNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->hlyEpaisseurPerche->value(),'f',1)));
    QDomElement hlyPerchesRetombeeNoeud = doc.createElement("retombee");
    hlyPerchesRetombeeNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->hlyRetombeePerches->value(),'f',1)));
    QDomElement hlyPerchesSurplusNoeud = doc.createElement("surplusHaut");
    hlyPerchesSurplusNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->hlyPercheDepassementHaut->value(),'f',1)));
    hlyPerchesNoeud.appendChild(hlyPerchesEpaisseurNoeud);
    hlyPerchesNoeud.appendChild(hlyPerchesRetombeeNoeud);
    hlyPerchesNoeud.appendChild(hlyPerchesSurplusNoeud);
    hlyboisNoeud.appendChild(hlyPerchesNoeud);

    QDomElement hlyTonooNoeud = doc.createElement("tonoo");
    QDomElement hlyTonooRetombeeNoeud = doc.createElement("retombee");
    hlyTonooRetombeeNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->hlyRetombeeTonoo->value(),'f',1)));
    hlyTonooNoeud.appendChild(hlyTonooRetombeeNoeud);
    hlyboisNoeud.appendChild(hlyTonooNoeud);

    QDomElement hlyTraversesNoeud = doc.createElement("traverse");
    QDomElement hlyTraversesRetombeeNoeud = doc.createElement("retombee");
    hlyTraversesRetombeeNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->hlyRetombeeTraverse->value(),'f',1)));
    hlyTraversesNoeud.appendChild(hlyTraversesRetombeeNoeud);
    hlyboisNoeud.appendChild(hlyTraversesNoeud);

    QDomElement hlyCroixNoeud = doc.createElement("croix");
    QDomElement hlyCroixEpaisseurNoeud = doc.createElement("epaisseur");
    hlyCroixEpaisseurNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->hlyEpaisseurCroix->value(),'f',1)));
    QDomElement hlyCroixRetombeeNoeud = doc.createElement("retombee");
    hlyCroixRetombeeNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->hlyRetombeeCroix->value(),'f',1)));
    hlyCroixNoeud.appendChild(hlyCroixEpaisseurNoeud);
    hlyCroixNoeud.appendChild(hlyCroixRetombeeNoeud);
    hlyboisNoeud.appendChild(hlyCroixNoeud);

    structureNoeud.appendChild(hlystructureNoeud);

    //et le geodome
    QDomElement geostructureNoeud = doc.createElement("Geodome");

    //general geodome
    QDomElement geogeneralNoeud = doc.createElement("general");
    QDomElement geoSolideBaseNoeud = doc.createElement("solideBase");
    geoSolideBaseNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->geoSolideBase->currentIndex())));
    QDomElement geoFrequenceANoeud = doc.createElement("frequenceA");
    geoFrequenceANoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->geoFrequenceA->value())));
    QDomElement geoFrequenceBNoeud = doc.createElement("frequenceB");
    geoFrequenceBNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->geoFrequenceB->value())));
    QDomElement geoDiametreNoeud = doc.createElement("diametre");
    geoDiametreNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->geoDiametre->value(),'f',2)));
    QDomElement geoHauteurNoeud = doc.createElement("hauteur");
    geoHauteurNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->geoHauteur->value(),'f',2)));
    QDomElement geoAngleNoeud = doc.createElement("angle");
    geoAngleNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->geoAngleRotation->value())));
    QDomElement geoMisAPlatNoeud = doc.createElement("solideBase");
    geoMisAPlatNoeud.appendChild(doc.createTextNode(str.setNum(fenPrincipale::ui->geoSabliereHorizontale->currentIndex())));

    geogeneralNoeud.appendChild(geoSolideBaseNoeud);
    geogeneralNoeud.appendChild(geoFrequenceANoeud);
    geogeneralNoeud.appendChild(geoFrequenceBNoeud);
    geogeneralNoeud.appendChild(geoDiametreNoeud);
    geogeneralNoeud.appendChild(geoHauteurNoeud);
    geogeneralNoeud.appendChild(geoAngleNoeud);
    geogeneralNoeud.appendChild(geoMisAPlatNoeud);
    geostructureNoeud.appendChild(geogeneralNoeud);

    structureNoeud.appendChild(geostructureNoeud);

    flux << doc.toString();
    fichier.close();

}

void fenPrincipale::on_actionEnregistrer_activated()
{
    if(nomDeFichier=="")
        nomDeFichier = QFileDialog::getSaveFileName(this,QObject::tr("Enregistrer sous")+" ...","sauv",QObject::tr("Sauvegarde")+" ZomeDomeEtCie (*.zdc)");
    enregistrer();
}

void fenPrincipale::on_actionEnregistrer_sous_activated()
{
    nomDeFichier = QFileDialog::getSaveFileName(this,QObject::tr("Enregistrer sous")+" ...","sauv",QObject::tr("Sauvegarde")+" ZomeDomeEtCie (*.zdc)");
    enregistrer();
}
