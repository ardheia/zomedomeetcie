#include <QtGui>
#include <QtOpenGL>
#include <QPushButton>
#include <GL/glu.h>
#include <math.h>
#include "mathema.h"

#include "glwidget.h"
#include "voutenidabeille3d.h"

//ca c'est un vague truc de multi-echantillonage ou multi sampling, j'ai l'impression que c'est une
// meilleure optimisation memoire-affichage
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


//constructeur du qwidget pour voute nid abeille
//! [0]
GLWidget::GLWidget(vouteNidAbeille *p_voute, int p_choix, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    initialisationCommune();
    choix = p_choix;
    voute3d = 0;
    voute = p_voute;
    quelleStructure = 0;
    if(choix==0)
    {
        translationeur = QVector3D(0.0, p_voute->vna_hauteurVoute/2.0, 0.0);
        positionCible = QVector3D(0.0, 1.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 10.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
    }
    else
    {
        translationeur = QVector3D(0.0, 0.0, 0.0);
        positionCible = QVector3D(0.0, 0.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 2.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
    }
}

//constructeur du qwidget pour dome nid abeille
GLWidget::GLWidget(domenidabeille *p_dome, int p_choix, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    initialisationCommune();
    choix = p_choix;
    domena3d = 0;
    domena = p_dome;
    quelleStructure = 1;
    if(choix==0)
    {
        translationeur = QVector3D(0.0, p_dome->dna_hauteur/2.0, 0.0);
        positionCible = QVector3D(0.0, 1.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 10.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
        xRot = -90.0*16.0;
    }
    else
    {
        translationeur = QVector3D(0.0, 0.0, 0.0);
        positionCible = QVector3D(0.0, 0.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 2.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
    }
}

//constructeur pour le zome
GLWidget::GLWidget(zome *p_zome, int p_choix, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    initialisationCommune();
    choix = p_choix;
    zomit3d = 0;
    zomit = p_zome;
    quelleStructure = 2;
    if(choix==0)
    {
        translationeur = QVector3D(0.0, p_zome->zom_hauteurTotale/2.0, 0.0);
        positionCible = QVector3D(0.0, 1.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 10.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
        xRot = -90.0*16.0;
    }
    else
    {
        translationeur = QVector3D(0.0, 0.0, 0.0);
        positionCible = QVector3D(0.0, 0.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 2.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
        xRot = -90.0*16.0;
        yRot = -90.0*16.0;
        zRot = -90.0*16.0;
    }
}

//constructeur pour l heliyourte
GLWidget::GLWidget(heliyourte *p_heli, int p_choix, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    initialisationCommune();
    choix = p_choix;
    heliyo3d = 0;
    heliyo = p_heli;
    quelleStructure = 3;
    translationeur = QVector3D(0.0, 0.0, 0.0);
    if(choix==0)
    {
        positionCible = QVector3D(0.0, 1.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 10.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
    }
    else
    {
        positionCible = QVector3D(0.0, 0.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 2.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
    }
}

//constructeur pour le geodome
GLWidget::GLWidget(geodome *p_geod, int p_choix, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    initialisationCommune();
    choix = p_choix;
    geod3d = 0;
    geod = p_geod;
    quelleStructure = 4;
    translationeur = QVector3D(0.0, 0.0, 0.0);
    if(choix==0)
    {
        positionCible = QVector3D(0.0, 1.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 10.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
        xRot = -90.0*16.0;
    }
    else
    {
        positionCible = QVector3D(0.0, 0.0, 0.0);
        vecteurCamera = QVector3D(0.0, 1.0, 0.0);
        c_rayon = 2.0;
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
    }
}

//! [0]

//! [1]
GLWidget::~GLWidget()
{
}
//! [1]

QImage GLWidget::captureEcran()
{
    QImage myImage = grabFrameBuffer();
    return myImage;
}

void GLWidget::initialisationCommune()
{
    setFocusPolicy(Qt::StrongFocus);
    filaire = false;
    coulDistinctes = false;
    premiereFois = true;

    choix2 = 0;

    // mise a 0 des angles de rotation
    xRot = 0;
    yRot = 0;
    zRot = 0;

    //definition des deux couleurs utilisees le violet du fond et le vert du symbole
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    QPushButton* filair = new QPushButton();
    QPushButton* plein = new QPushButton();

    filair->setFixedSize(30,30);
    QIcon icon(":/images/filaire");
    filair->setIcon(icon);
    filair->setIconSize(QSize(25,25));
    filair->setToolTip(tr("Afficher en filaire"));
    plein->setFixedSize(30,30);
    QIcon icon2(":/images/plein");
    plein->setIcon(icon2);
    plein->setIconSize(QSize(28,28));
    plein->setToolTip(tr("Afficher en plein"));
    QHBoxLayout* layout = new QHBoxLayout();
    QVBoxLayout* premierlayout = new QVBoxLayout();
    layout->addWidget(filair);
    layout->addWidget(plein);
    layout->addSpacing(2000);

    this->setLayout(premierlayout);

    premierlayout->addSpacing(2000);
    premierlayout->addLayout(layout);

    QObject::connect(filair,SIGNAL(clicked()),this,SLOT(passerEnFilaire()));
    QObject::connect(plein,SIGNAL(clicked()),this,SLOT(passerEnPlein()));
}

void GLWidget::passerEnFilaire()
{
    filaire = true;
    updateGL();
}

void GLWidget::passerEnPlein()
{
    filaire = false;
    updateGL();
}

//la c'est pour interdire de reduire la fenetre trop petite
//! [2]
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}
//! [2]

//la c'est la taille de depart
//! [3]
QSize GLWidget::sizeHint() const
//! [3] //! [4]
{
    return QSize(400, 400);
}
//! [4]

void GLWidget::rafraichirWidget(bool p_couleursDistinctes, int p_choix2, int remizeAZeroRotation)
{
    coulDistinctes = p_couleursDistinctes;
    if(p_choix2!=1000)
    {
        choix2 = p_choix2;
    }
    if(remizeAZeroRotation==1)
    {
        if((quelleStructure==2)&&(choix==1)&&(choix2==1))
        {
            xRot = -90.0*16.0;
            yRot = -90.0*16.0;
            zRot = -90.0*16.0;
        }
        c_anglePhi = pi/2.0;
        c_angleAlpha = 0.0;
    }
    initializeGL();
    paintGL();
    updateGL();
}

//ca c'est une fonction qui normalise l'angle pour jamais qu'il soit superieur a 360*16 ou inferieur a 0
static void qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 2.0*pi;
    while (angle > 2.0*pi)
        angle -= 2.0*pi;
}
static void qNormalizeAngle2(double &angle)
{
    while (angle < -pi)
        angle += 2.0*pi;
    while (angle > pi)
        angle -= 2.0*pi;
}

//les fonctions de rotation appelees par la souris ou la barre de defilement
//! [5]
void GLWidget::setXRotation(double angle)
{
    //on normalise l'angle
    qNormalizeAngle(angle);
    //si l'angle est different de l'angle actuel de rotation on le change
    if (angle != c_anglePhi) {
        c_anglePhi = angle;
        // on appelle la fonction comme quoi la rotation a changee en emettant un signal
        //on remet a jour l'affichage
        updateGL();
    }
}
//! [5]

void GLWidget::setYRotation(double angle)
{
    qNormalizeAngle2(angle);
    if (angle != c_angleAlpha) {

        if((angle<(pi/2.0))&&(angle>(-pi/2.0)))
        {
              c_angleAlpha = angle;
              updateGL();
        }

    }
}


//essayons de zoomer
void GLWidget::zoom(int valeurZoom)
{
    c_rayon -= valeurZoom/500.0;
    if(c_rayon<0.1)
        c_rayon = 0.1;
    updateGL();
}

// fonction initialisation du widget3d
//! [6]
void GLWidget::initializeGL()
{
    //definit la couleur de fond : etonnant, le darker retourne la couleur en plus sombre, il existe lighter pour faire le contraire
    qglClearColor(qtPurple.darker());

    // cree le logo en faisant passer comme argument l'endroit ou il doit etre cree et la precision du lissage
    // avant ca, si c'est pas la premiere fois, il detruit le pointeur pour liberer de la memoire
    if(premiereFois==false)
    {
        switch(quelleStructure)
        {
        case 0:
            delete voute3d;
                break;
        case 1:
            delete domena3d;
            break;
        case 2:
            delete zomit3d;
            break;
        case 3:
            delete heliyo3d;
            break;
        case 4:
            delete geod3d;
            break;
        }
    }
    switch(quelleStructure)
    {
    case 0:
        voute3d = new vouteNidAbeille3D(this,voute,coulDistinctes,choix,choix2);
        break;
    case 1:
        domena3d = new domenidabeille3d(this,domena,coulDistinctes,choix,choix2);
        break;
    case 2:
        zomit3d = new zome3D(this,zomit,coulDistinctes,choix,choix2);
        break;
    case 3:
        heliyo3d = new heliyourte3d(this,heliyo,coulDistinctes,choix,choix2);
        break;
    case 4:
        geod3d = new geodome3D(this,geod,coulDistinctes,choix,choix2);
        break;
    }


    // activation de differents parametres d'opengl, pour les desactiver il faudrait faire glDisable()

    // activation de l'elimination des parties cachees (test de profondeur)
    glEnable(GL_DEPTH_TEST);
    //alors ca ca cache tous les triangles qui sont dessines a l'ecran dans le sens des aiguilles d'une montre
    //ca veut dire qu'il faut definir les points d'un triangle en regardant sa face exterieur dans le sens inverse des aiguilles d'une montre
    glEnable(GL_CULL_FACE);
    //ca smooth comme ca on voit pas les aretes
    glShadeModel(GL_SMOOTH);
    //activation de la gestion des lumieres et des materiaux

    glEnable(GL_LIGHTING);
    //allume la premiere lumiere
    glEnable(GL_LIGHT0);

    glEnable(GL_MULTISAMPLE);

    // definition des 3 composantes de la lumiere
    float Light1Dif[4] = {0.7, 0.7, 0.7, 1.0f};
    float Light1Spec[4] = {1.0f, 0.2f, 0.2f, 1.0f};
    float Light1Amb[4] = {0.7f, 0.7f, 0.7f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light1Dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Light1Spec);
    glLightfv(GL_LIGHT0, GL_AMBIENT, Light1Amb);

}
//! [6]

//! [7]
// fonction dessin de la fenetre 3d
void GLWidget::paintGL()
{
    //on efface l'ecran avant de dessiner ... logique
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // remet la matrice identite dans la matrice (comprend pas trop)

    glMatrixMode( GL_MODELVIEW );

    glLoadIdentity();
    GLfloat lightPosition[4] = { 5.0, -5.0, 10.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    positionCamera.setX(positionCible.x()+c_rayon*qCos(c_angleAlpha)*qCos(c_anglePhi));
    positionCamera.setY(positionCible.y()+c_rayon*qSin(c_angleAlpha));
    positionCamera.setZ(positionCible.z()+c_rayon*qCos(c_angleAlpha)*qSin(c_anglePhi));

    gluLookAt(positionCamera.x(), positionCamera.y(), positionCamera.z(), positionCible.x(), positionCible.y(), positionCible.z(), vecteurCamera.x(), vecteurCamera.y(), vecteurCamera.z());

  //  dessinerRepere();
    // dessine le dessin

    //deplace la matrice
    glTranslatef(translationeur.x(),translationeur.y(),translationeur.z());
    //fait tourner la matrice d'un angle en degres selon un axe determine par un vecteur x y z
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);


    switch(quelleStructure)
    {
    case 0:
        voute3d->draw(filaire);
            break;
    case 1:
        domena3d->draw(filaire);
        break;
    case 2:
        zomit3d->draw(filaire);
        break;
    case 3:
        heliyo3d->draw(filaire);
        break;
    case 4:
        geod3d->draw(filaire);
        break;
    }
}
//! [7]

//! [8]
void GLWidget::resizeGL(int width, int height)
{
    //la on redimensionne l'affichage de la 3D, qui est compris dans le widget qu'on vient de changer de taille
    //en gros grace a ca le logo sera toujours au milieu du widget (grace aux deux premier parametres qui determine
    //la position x y du coin haut gauche de l'affichage) et ne sortira jamais de l'ecran grace aux deux derniers parametres
    // qui sont la taille du carre d'affichage 3d

    glViewport(0.0, 0.0, width, height);
    // choisit sur quelle matrice vont avoir lieu les operations suivantes : ici sur le modele
    glMatrixMode(GL_PROJECTION);
    // remet la matrice identite dans la matrice (comprend pas trop)
    glLoadIdentity();
    gluPerspective(70.0,(double) width/height,0.1,200.0);

    //choisit sur quelle matrice vont avoir lieu les operations suivantes : ici sur les cameras ?

    glMatrixMode(GL_MODELVIEW);
}
//! [8]

//! [9]
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    //si on clique definit la ou est la souris dans un qpoint qui est la position x y du pointeur
    lastPos = event->pos();
}
//! [9]

//! [10]
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // fait tourner en fonction du deplacement de la souris en comparant avec la position anterieur
    double red = 0.005;
    double red2 = -0.005;
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    // si bouton gauche alors on tourne
    if (event->buttons() & Qt::LeftButton) {
        setXRotation(c_anglePhi + red * dx);
        setYRotation(c_angleAlpha + red * dy);
    //si bouton droit alors on se deplace
    } else if (event->buttons() & Qt::RightButton) {
        QVector3D A,B,C;
        if(positionCible.z()>positionCamera.z())
            red2 = - red2;
        A = (positionCible-positionCamera).normalized();
        C = QVector3D(1.0,0.0,-A.x()/A.z()).normalized();
        B = QVector3D::crossProduct(A,C).normalized();
        positionCible += red2 * dx * C;
        positionCible += red2 * dy * B;
        updateGL();
    }
    lastPos = event->pos();
}
//! [10]

void GLWidget::wheelEvent(QWheelEvent *event)
{
    zoom(event->delta());
}

void GLWidget::dessinerRepere()
{
    glDisable ( GL_TEXTURE_2D );
    glEnable(GL_COLOR_MATERIAL);
    glLineWidth(8);
    glPushMatrix();

    glBegin(GL_LINES);

    glColor3ub(0,0,255); //axe X en Bleu
    glVertex3d(0,0,0);
    glVertex3d(2,0,0);
    glVertex3f(1.5 , 0 , 0.5); //les 2 trait du X
    glVertex3f(2 , 0 , 1.3);
    glVertex3f(1.5 , 0 , 1.3);
    glVertex3f(2 , 0 , 0.5);


    glColor3ub(0,255,0); //axe Y en Vert
    glVertex3d(0,0,0);
    glVertex3d(0,2,0);
    glVertex3f(0 , 1.5 , 1); //les 2 trait du Y
    glVertex3f(0 , 1.2 , 2);
    glVertex3f(0 , 1.6 , 2);
    glVertex3f(0 , 1.4 , 1.5);


    glColor3ub(255,0,0); //axe Z en Rouge;
    glVertex3d(0,0,0);
    glVertex3d(0,0,2);
    glVertex3f(0.2 , 0 , 2); //les 3 trait du Z
    glVertex3f(0.7 , 0 , 2);
    glVertex3f(0.7 , 0 , 2);
    glVertex3f(0.2 , 0 , 1.5);
    glVertex3f(0.2 , 0 , 1.5);
    glVertex3f(0.7 , 0 , 1.5);

    glColor3ub(255,255,255);
    glEnd();
    glPopMatrix();

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
}


void GLWidget::keyPressEvent(QKeyEvent *event)
{
    double val= 50;
    if(event->key() == Qt::Key_Up)
        zoom(val);
    if(event->key() == Qt::Key_Down)
        zoom(-val);
}

QString GLWidget::exportObj()
{
    QString resu;
    switch(quelleStructure)
    {
    case 0:
        resu = voute3d->exportObj();
            break;
    case 1:
        resu = domena3d->exportObj();
        break;
    case 2:
        resu = zomit3d->exportObj();
        break;
    case 3:
        resu = heliyo3d->exportObj();
        break;
    case 4:
        resu = geod3d->exportObj();
        break;
    }
    return resu;
}
