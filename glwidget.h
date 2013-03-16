#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "voutenidabeille3d.h"
#include "voutenidabeille.h"
#include "domenidabeille.h"
#include "domenidabeille3d.h"
#include "zome.h"
#include "zome3d.h"
#include "heliyourte.h"
#include "heliyourte3d.h"
#include "geodome.h"
#include "geodome3d.h"

/*
  la on a tout pour gérer nos fenêtres 3D : le zoom, les boutons filaire/pas filaire, les rotations
*/

//! [0]
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    //dans les 2 cas le p_choix correspond a questcequ on affiche 0 : vue generale,  1 ...
    GLWidget(vouteNidAbeille *p_voute, int p_choix, QWidget *parent = 0);
    GLWidget(domenidabeille *p_dome, int p_choix, QWidget *parent = 0);
    GLWidget(zome *p_zome, int p_choix, QWidget *parent = 0);
    GLWidget(heliyourte *p_heli, int p_choix, QWidget *parent = 0);
    GLWidget(geodome *p_geod, int p_choix, QWidget *parent = 0);

    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rafraichirWidget(bool p_couleursDistinctes, int p_choix2 = 1000,int remizeAZeroRotation = 0);
    QImage captureEcran();
    void dessinerRepere();
    QString exportObj();
//! [0]

//! [1]
public slots:
    void setXRotation(double angle);
    void setYRotation(double angle);
    void zoom(int valeurZoom);
    void passerEnFilaire();
    void passerEnPlein();
    void initialisationCommune();

//! [1]

//! [2]
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* event);

//! [2]

//! [3]
private:
    vouteNidAbeille *voute;
    vouteNidAbeille3D *voute3d;
    domenidabeille *domena;
    domenidabeille3d *domena3d;
    zome *zomit;
    zome3D *zomit3d;
    heliyourte *heliyo;
    heliyourte3d *heliyo3d;
    geodome *geod;
    geodome3D *geod3d;

    QVector3D positionCamera, positionCible, vecteurCamera;
    double c_anglePhi, c_angleAlpha, c_rayon;
    int choix;
    int quelleStructure; //0 vna , 1 dna
    bool premiereFois;
    double xRot;
    double yRot;
    double zRot;
    QVector3D translationeur;
    int choix2;
    // filaire = true si filaire, false si plein
    bool filaire;
    bool coulDistinctes;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;
    double zoomm1;

};
//! [3]

#endif
