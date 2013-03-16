#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <qmath.h>

#include "voutenidabeille3d.h"
#include "domenidabeille3d.h"
#include "objet3d.h"
#include "mathema.h"
#include "rectoid.h"
#include "parametres.h"


void Rectoid::translate(const QVector3D &t)
{
    for (int i = 0; i < parts.count(); ++i)
        parts[i]->translate(t);
}
void Rectoid::rotate(qreal deg, QVector3D axis)
{
    for (int i = 0; i < parts.count(); ++i)
        parts[i]->rotate(deg, axis);
}
void Rectoid::rotate(QQuaternion quater)
{
    for (int i = 0; i < parts.count(); ++i)
        parts[i]->rotate(quater);
}

void Rectoid::setcolor(QColor c)
{
    for(int i = 0; i < parts.count(); ++i)
        qSetColor(parts[i]->faceColor, c);
}


// le g c'est le pointeur vers la geometrie (grosse structure qui contient tous les points et tout)
// apres ici c'est une fonction pour creer des rectangles en volume
RectPrism::RectPrism(Geometry *g, qreal width, qreal height, qreal depth)
{
    //pas bete l'enumeration bl:bas gauche, br:bas droite, tr: haut droit, tl : haut gauche
    enum { bl, br, tr, tl };
    //on cree un nouveau patch
    Patch *fb = new Patch(g);
    //on lui dit qu'il sera de type faceted
    fb->setSmoothing(Patch::Faceted);

    // on cree la face de devant
    //on cree donc 4 points : le points en haut a droite va par exemple se retrouver decale de width en x et y en hauteur
    QVector<QVector3D> r(4);
    r[br].setX(width);
    r[tr].setX(width);
    r[tr].setY(height);
    r[tl].setY(height);
    //puis on decale tous les points pour se retrouver centre mais decale en z de l'epaisseur/2

    //on ajoute cette face (attention definition dans le sens contraire des aiguilles d'une montre
    fb->addQuad(r[bl], r[br], r[tr], r[tl],true,true,true,true);

    // on cree la face arriere facile
    QVector<QVector3D> s = extrude(r, depth);
    fb->addQuad(s[tl], s[tr], s[br], s[bl],true,true,true,true);

    // on cree ensuite les faces de cotes
    Patch *sides = new Patch(g);
    sides->setSmoothing(Patch::Faceted);
    sides->addQuad(s[bl], s[br], r[br], r[bl],false,true,false,false);
    sides->addQuad(s[br], s[tr], r[tr], r[br],false,true,false,false);
    sides->addQuad(s[tr], s[tl], r[tl], r[tr],false,true,false,false);
    sides->addQuad(s[tl], s[bl], r[bl], r[tl],false,true,false,false);

    //et apres on additionne les parties de l'objet
    parts << fb << sides;
}

RectLosangePlan::RectLosangePlan(Geometry *g, QVector3D a, QVector3D b, QVector3D c, QVector3D d)
{
    //on cree un nouveau patch
    Patch *fb = new Patch(g);
    //on lui dit qu'il sera de type faceted
    fb->setSmoothing(Patch::Faceted);
    fb->addQuad(a, b, c, d,true,true,true,true);
    parts << fb;
}

RectLosangeMethode1::RectLosangeMethode1(Geometry *g, double diagVert, double diagHoriz, double retombee, double diagVertInt, double diagHorizInt)
{
    //on cree un nouveau patch
    Patch *fb = new Patch(g);
    fb->setSmoothing(Patch::Faceted);
    QVector<QVector3D> interieurBas, exterieurBas, interieurHaut, exterieurHaut, pointBordHaut, pointBordBas;
    interieurBas.resize(4);
    exterieurBas.resize(4);
    pointBordBas.resize(4);

    exterieurBas[0] = QVector3D(0.0,diagVert/2.0,0.0);
    exterieurBas[1] = QVector3D(-diagHoriz/2.0,0.0,0.0);
    exterieurBas[2] = QVector3D(0.0,-diagVert/2.0,0.0);
    exterieurBas[3] = QVector3D(diagHoriz/2.0,0.0,0.0);

    interieurBas[0] = QVector3D(0.0,diagVertInt/2.0,0.0);
    interieurBas[1] = QVector3D(-diagHorizInt/2.0,0.0,0.0);
    interieurBas[2] = QVector3D(0.0,-diagVertInt/2.0,0.0);
    interieurBas[3] = QVector3D(diagHorizInt/2.0,0.0,0.0);

    pointBordBas[0] = intersection2DroitesPlanOxy(interieurBas[0],interieurBas[3],exterieurBas[0],exterieurBas[1]);
    pointBordBas[1] = intersection2DroitesPlanOxy(interieurBas[0],interieurBas[1],exterieurBas[2],exterieurBas[1]);
    pointBordBas[2] = intersection2DroitesPlanOxy(interieurBas[1],interieurBas[2],exterieurBas[3],exterieurBas[2]);
    pointBordBas[3] = intersection2DroitesPlanOxy(interieurBas[2],interieurBas[3],exterieurBas[0],exterieurBas[3]);

    exterieurHaut = extrude(exterieurBas,-retombee);
    interieurHaut = extrude(interieurBas,-retombee);
    pointBordHaut = extrude(pointBordBas,-retombee);

    Patch *dessus = new Patch(g);
    dessus->addQuad(pointBordHaut[0],exterieurHaut[1],pointBordHaut[1],interieurHaut[0],true,true,true,true);
    dessus->addQuad(pointBordHaut[1],exterieurHaut[2],pointBordHaut[2],interieurHaut[1],true,true,true,true);
    dessus->addQuad(pointBordHaut[2],exterieurHaut[3],pointBordHaut[3],interieurHaut[2],true,true,true,true);
    dessus->addQuad(pointBordHaut[3],exterieurHaut[0],pointBordHaut[0],interieurHaut[3],true,true,true,true);

    Patch *dessous = new Patch(g);
    dessous->addQuad(pointBordBas[1],exterieurBas[1],pointBordBas[0],interieurBas[0],true,true,true,true);
    dessous->addQuad(pointBordBas[2],exterieurBas[2],pointBordBas[1],interieurBas[1],true,true,true,true);
    dessous->addQuad(pointBordBas[3],exterieurBas[3],pointBordBas[2],interieurBas[2],true,true,true,true);
    dessous->addQuad(pointBordBas[0],exterieurBas[0],pointBordBas[3],interieurBas[3],true,true,true,true);

    Patch *faceExt = new Patch(g);
    faceExt->addQuad(pointBordHaut[0],pointBordBas[0],exterieurBas[1],exterieurHaut[1],true,true,true,true);
    faceExt->addQuad(pointBordHaut[1],pointBordBas[1],exterieurBas[2],exterieurHaut[2],true,true,true,true);
    faceExt->addQuad(pointBordHaut[2],pointBordBas[2],exterieurBas[3],exterieurHaut[3],true,true,true,true);
    faceExt->addQuad(pointBordHaut[3],pointBordBas[3],exterieurBas[0],exterieurHaut[0],true,true,true,true);

    faceExt->addQuad(exterieurHaut[1],exterieurBas[1],pointBordBas[1],pointBordHaut[1],true,true,true,true);
    faceExt->addQuad(exterieurHaut[2],exterieurBas[2],pointBordBas[2],pointBordHaut[2],true,true,true,true);
    faceExt->addQuad(exterieurHaut[3],exterieurBas[3],pointBordBas[3],pointBordHaut[3],true,true,true,true);
    faceExt->addQuad(exterieurHaut[0],exterieurBas[0],pointBordBas[0],pointBordHaut[0],true,true,true,true);

    Patch *faceInt = new Patch(g);
    faceInt->addQuad(pointBordHaut[1],pointBordBas[1],interieurBas[0],interieurHaut[0],true,true,true,true);
    faceInt->addQuad(pointBordHaut[2],pointBordBas[2],interieurBas[1],interieurHaut[1],true,true,true,true);
    faceInt->addQuad(pointBordHaut[3],pointBordBas[3],interieurBas[2],interieurHaut[2],true,true,true,true);
    faceInt->addQuad(pointBordHaut[0],pointBordBas[0],interieurBas[3],interieurHaut[3],true,true,true,true);

    faceInt->addQuad(interieurHaut[0],interieurBas[0],pointBordBas[0],pointBordHaut[0],true,true,true,true);
    faceInt->addQuad(interieurHaut[1],interieurBas[1],pointBordBas[1],pointBordHaut[1],true,true,true,true);
    faceInt->addQuad(interieurHaut[2],interieurBas[2],pointBordBas[2],pointBordHaut[2],true,true,true,true);
    faceInt->addQuad(interieurHaut[3],interieurBas[3],pointBordBas[3],pointBordHaut[3],true,true,true,true);

    parts << dessus << dessous << faceExt << faceInt;
}

RectTriangleBasMethode1::RectTriangleBasMethode1(Geometry *g, double diagVert, double diagHoriz, double retombee, double diagVertInt, double diagHorizInt, double epaisseur)
{
    //on cree un nouveau patch
    Patch *fb = new Patch(g);
    fb->setSmoothing(Patch::Faceted);
    QVector<QVector3D> interieurBas, exterieurBas, interieurHaut, exterieurHaut, pointBordHaut, pointBordBas;
    interieurBas.resize(3);
    exterieurBas.resize(3);
    pointBordBas.resize(3);

    exterieurBas[0] = QVector3D(0.0,diagVert,0.0);
    exterieurBas[1] = QVector3D(-diagHoriz/2.0,0.0,0.0);
    exterieurBas[2] = QVector3D(diagHoriz/2.0,0.0,0.0);

    pointBordBas[1] = QVector3D(-diagHorizInt/2.0,0.0,0.0);
    pointBordBas[2] = QVector3D(diagHorizInt/2.0,0.0,0.0);

    interieurBas[0] = QVector3D(0.0,diagVertInt,0.0);
    interieurBas[1] = intersectionDroiteAvecDroiteHorizontale(pointBordBas[1],interieurBas[0],epaisseur);
    interieurBas[2] = intersectionDroiteAvecDroiteHorizontale(pointBordBas[2],interieurBas[0],epaisseur);

    pointBordBas[0] = intersection2DroitesPlanOxy(interieurBas[0],interieurBas[2],exterieurBas[0],exterieurBas[1]);

    exterieurHaut = extrude(exterieurBas,-retombee);
    interieurHaut = extrude(interieurBas,-retombee);
    pointBordHaut = extrude(pointBordBas,-retombee);

    Patch *dessus = new Patch(g);
    dessus->addQuad(pointBordHaut[0],exterieurHaut[1],pointBordHaut[1],interieurHaut[0],true,true,true,true);
    dessus->addQuad(pointBordHaut[1],pointBordHaut[2],interieurHaut[2],interieurHaut[1],true,true,true,true);
    dessus->addQuad(pointBordHaut[2],exterieurHaut[2],exterieurHaut[0],pointBordHaut[0],true,true,true,true);

    Patch *dessous = new Patch(g);
    dessous->addQuad(pointBordBas[1],exterieurBas[1],pointBordBas[0],interieurBas[0],true,true,true,true);
    dessous->addQuad(pointBordBas[1],interieurBas[1],interieurBas[2],pointBordBas[2],true,true,true,true);
    dessous->addQuad(pointBordBas[2],pointBordBas[0],exterieurBas[0],exterieurBas[2],true,true,true,true);

    Patch *faceExt = new Patch(g);
    faceExt->addQuad(pointBordHaut[0],pointBordBas[0],exterieurBas[1],exterieurHaut[1],true,true,true,true);
    faceExt->addQuad(pointBordHaut[1],pointBordBas[1],pointBordBas[2],pointBordHaut[2],true,true,true,true);
    faceExt->addQuad(exterieurHaut[2],exterieurBas[2],exterieurBas[0],exterieurHaut[0],true,true,true,true);

    faceExt->addQuad(exterieurHaut[1],exterieurBas[1],pointBordBas[1],pointBordHaut[1],true,true,true,true);
    faceExt->addQuad(pointBordHaut[2],pointBordBas[2],exterieurBas[2],exterieurHaut[2],true,true,true,true);
    faceExt->addQuad(exterieurHaut[0],exterieurBas[0],pointBordBas[0],pointBordHaut[0],true,true,true,true);

    Patch *faceInt = new Patch(g);
    faceInt->addQuad(interieurHaut[1],interieurBas[1],pointBordBas[1],pointBordHaut[1],true,true,true,true);
    faceInt->addQuad(pointBordHaut[2],pointBordBas[2],interieurBas[2],interieurHaut[2],true,true,true,true);
    faceInt->addQuad(pointBordHaut[0],pointBordBas[0],interieurBas[0],interieurHaut[0],true,true,true,true);

    faceInt->addQuad(pointBordHaut[0],pointBordBas[0],pointBordBas[2],pointBordHaut[2],true,true,true,true);
    faceInt->addQuad(pointBordHaut[1],pointBordBas[1],interieurBas[0],interieurHaut[0],true,true,true,true);
    faceInt->addQuad(interieurHaut[2],interieurBas[2],interieurBas[1],interieurHaut[1],true,true,true,true);

    parts << dessus << dessous << faceExt << faceInt;
}
//et la on cree notre cercle
RectTorus::RectTorus(Geometry *g, qreal iRad, qreal oRad, qreal depth)
{
    QVector<QVector3D> inside;
    QVector<QVector3D> outside;

    int k = finesse()*4.0;
    // on cree les deux cercles interieur et exterieur
    for (int i = 0; i < k; ++i) {
        qreal angle = (i * 2 * M_PI) / k;
        inside << QVector3D(iRad * qSin(angle), iRad * qCos(angle), depth);
        outside << QVector3D(oRad * qSin(angle), oRad * qCos(angle), depth);
    }
    //on ferme les cercles en rajoutant les points finaux
    inside << QVector3D(0.0, iRad, 0.0);
    outside << QVector3D(0.0, oRad, 0.0);
    QVector<QVector3D> in_back = extrude(inside, depth);
    QVector<QVector3D> out_back = extrude(outside, depth);

    // et la on cree les faces
    // Create front, back and sides as separate patches so that smooth normals
    // are generated for the curving sides, but a faceted edge is created between
    // sides and front/back

    Patch *front = new Patch(g);
    for (int i = 0; i < k; ++i)
        front->addQuad(outside[i], inside[i],
                       inside[(i + 1) % k], outside[(i + 1) % k],false,true,false,true);
    Patch *back = new Patch(g);
    for (int i = 0; i < k; ++i)
        back->addQuad(in_back[i], out_back[i],
                      out_back[(i + 1) % k], in_back[(i + 1) % k],false,true,false,true);
    Patch *is = new Patch(g);
    for (int i = 0; i < k; ++i)
        is->addQuad(in_back[i], in_back[(i + 1) % k],
                    inside[(i + 1) % k], inside[i],false,false,false,false);
    Patch *os = new Patch(g);
    for (int i = 0; i < k; ++i)
        os->addQuad(out_back[(i + 1) % k], out_back[i],
                    outside[i], outside[(i + 1) % k],false,false,false,false);
    parts << front << back << is << os;
}



//et la on cree un chevron
//numero : 0 penche a  droite
//         1 penche a  gauche
RectChevron::RectChevron(Geometry *g, vouteNidAbeille *p_voute, int numero)
{
    QVector<QVector3D> intradosFace;
    QVector<QVector3D> extradosFace;
    QVector<QVector3D> intradosDerriere;
    QVector<QVector3D> extradosDerriere;
    QVector<QVector3D> passage;
    QVector3D vecteurHorizontal;

    vecteurHorizontal.setX(1);vecteurHorizontal.setY(0);vecteurHorizontal.setZ(0);
    //nombre de divisions
    int k = finesse();
    double angleInitialExtradosFace, angleFinalExtradosFace;
    double angleInitialIntradosFace, angleFinalIntradosFace;
    double angleInitialExtradosDerriere, angleFinalExtradosDerriere;
    double angleInitialIntradosDerriere, angleFinalIntradosDerriere;
    double iRad,oRad;

    qreal depth;

    //la on va chercher les valeurs du chevron dans voutenidabeille.cpp
    p_voute->valeursChevron(passage);

    depth = passage[8].z();
    oRad = (passage[8]-passage[0]).length();
    iRad = (passage[8]-passage[1]).length();
    if(numero==0)
    {
        angleInitialExtradosFace = angleEntre2Vecteurs3D(passage[3]-passage[8],vecteurHorizontal);
        angleFinalExtradosFace = angleEntre2Vecteurs3D(passage[0]-passage[8],vecteurHorizontal);
        angleInitialIntradosFace = angleEntre2Vecteurs3D(passage[2]-passage[8],vecteurHorizontal);
        angleFinalIntradosFace = angleEntre2Vecteurs3D(passage[1]-passage[8],vecteurHorizontal);
        angleInitialExtradosDerriere = angleEntre2Vecteurs3D(passage[7]-passage[9],vecteurHorizontal);
        angleFinalExtradosDerriere = angleEntre2Vecteurs3D(passage[4]-passage[9],vecteurHorizontal);
        angleInitialIntradosDerriere = angleEntre2Vecteurs3D(passage[6]-passage[9],vecteurHorizontal);
        angleFinalIntradosDerriere = angleEntre2Vecteurs3D(passage[5]-passage[9],vecteurHorizontal);
    }
    else if(numero==1)
    {
        angleInitialExtradosDerriere = angleEntre2Vecteurs3D(passage[3]-passage[8],vecteurHorizontal);
        angleFinalExtradosDerriere = angleEntre2Vecteurs3D(passage[0]-passage[8],vecteurHorizontal);
        angleInitialIntradosDerriere = angleEntre2Vecteurs3D(passage[2]-passage[8],vecteurHorizontal);
        angleFinalIntradosDerriere = angleEntre2Vecteurs3D(passage[1]-passage[8],vecteurHorizontal);
        angleInitialExtradosFace = angleEntre2Vecteurs3D(passage[7]-passage[9],vecteurHorizontal);
        angleFinalExtradosFace = angleEntre2Vecteurs3D(passage[4]-passage[9],vecteurHorizontal);
        angleInitialIntradosFace = angleEntre2Vecteurs3D(passage[6]-passage[9],vecteurHorizontal);
        angleFinalIntradosFace = angleEntre2Vecteurs3D(passage[5]-passage[9],vecteurHorizontal);
    }

    // on cree les 4 morceaux de courbes cercles interieur et exterieur
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalExtradosFace-angleInitialExtradosFace)) / k;
        extradosFace << QVector3D(passage[8].x()+oRad * qCos(angleInitialExtradosFace+angle), passage[8].y()+oRad * qSin(angleInitialExtradosFace+angle), depth);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalIntradosFace-angleInitialIntradosFace)) / k;
        intradosFace << QVector3D(passage[8].x()+iRad*qCos(angleInitialIntradosFace+angle), passage[8].y()+iRad * qSin(angleInitialIntradosFace+angle), depth);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalExtradosDerriere-angleInitialExtradosDerriere)) / k;
        extradosDerriere << QVector3D(passage[9].x()+oRad * qCos(angleInitialExtradosDerriere+angle), passage[9].y()+oRad * qSin(angleInitialExtradosDerriere+angle), -depth);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalIntradosDerriere-angleInitialIntradosDerriere)) / k;
        intradosDerriere << QVector3D(passage[9].x()+iRad*qCos(angleInitialIntradosDerriere+angle), passage[9].y()+iRad * qSin(angleInitialIntradosDerriere+angle), -depth);
    }

    Patch *face = new Patch(g);
    for (int i = 0; i < k; ++i)
        face->addQuad(extradosFace[i], extradosFace[(i + 1)],
                       intradosFace[(i + 1)], intradosFace[i],true,false,true,false);
    Patch *derriere = new Patch(g);
    for (int i = 0; i < k; ++i)
        derriere->addQuad(intradosDerriere[i],intradosDerriere[(i + 1)],
                      extradosDerriere[(i + 1)],  extradosDerriere[i],true,false,true,false);
    Patch *intrados = new Patch(g);
    for (int i = 0; i < k; ++i)
        intrados->addQuad(intradosDerriere[i],intradosFace[i],
                    intradosFace[(i + 1)], intradosDerriere[(i + 1)],false,false,false,false) ;
    Patch *extrados = new Patch(g);
    for (int i = 0; i < k; ++i)
        extrados->addQuad(extradosDerriere[(i + 1)],extradosFace[(i + 1)],
                    extradosFace[i],  extradosDerriere[i],false,false,false,false);
    Patch *boutGauche = new Patch(g);
        boutGauche->addQuad(intradosFace[0],intradosDerriere[0],extradosDerriere[0],extradosFace[0],true,true,true,true);
    Patch *boutDroit = new Patch(g);
        boutDroit->addQuad(extradosFace[k],extradosDerriere[k],intradosDerriere[k],intradosFace[k],true,true,true,true);

    parts << face << derriere << intrados << extrados << boutGauche << boutDroit;

}

//et la on cree un chevron pour le dome nid abeille

RectChevron::RectChevron(Geometry *g, chevronDNA che)
{
    QVector<QVector3D> intradosFace;
    QVector<QVector3D> extradosFace;
    QVector<QVector3D> intradosDerriere;
    QVector<QVector3D> extradosDerriere;
    QVector<QVector3D> passage;
    QVector3D vecteurHorizontal;

    vecteurHorizontal.setX(1);vecteurHorizontal.setY(0);vecteurHorizontal.setZ(0);
    //nombre de divisions
    int k = finesse();
    double angleInitialExtradosFace, angleFinalExtradosFace;
    double angleInitialIntradosFace, angleFinalIntradosFace;
    double angleInitialExtradosDerriere, angleFinalExtradosDerriere;
    double angleInitialIntradosDerriere, angleFinalIntradosDerriere;
    double iRad,oRad;

    qreal depth;

    depth = che.epaisseurChevron;
    oRad = che.rayonExtrados;
    iRad = che.rayonIntrados;

    angleInitialExtradosFace = angleEntre2Vecteurs3D(che.VD-che.centre2D,vecteurHorizontal);
    angleFinalExtradosFace = angleEntre2Vecteurs3D(che.VA-che.centre2D,vecteurHorizontal);
    angleInitialIntradosFace = angleEntre2Vecteurs3D(che.VC-che.centre2D,vecteurHorizontal);
    angleFinalIntradosFace = angleEntre2Vecteurs3D(che.VB-che.centre2D,vecteurHorizontal);
    angleInitialExtradosDerriere = angleEntre2Vecteurs3D(che.WD-che.centre2D,vecteurHorizontal);
    angleFinalExtradosDerriere = angleEntre2Vecteurs3D(che.WA-che.centre2D,vecteurHorizontal);
    angleInitialIntradosDerriere = angleEntre2Vecteurs3D(che.WC-che.centre2D,vecteurHorizontal);
    angleFinalIntradosDerriere = angleEntre2Vecteurs3D(che.WB-che.centre2D,vecteurHorizontal);

    // on cree les 4 morceaux de courbes cercles interieur et exterieur
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalExtradosFace-angleInitialExtradosFace)) / k;
        extradosFace << QVector3D(che.centre2D.x()+oRad * qCos(angleInitialExtradosFace+angle), che.centre2D.y()+oRad * qSin(angleInitialExtradosFace+angle), 0.0);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalIntradosFace-angleInitialIntradosFace)) / k;
        intradosFace << QVector3D(che.centre2D.x()+iRad*qCos(angleInitialIntradosFace+angle), che.centre2D.y()+iRad * qSin(angleInitialIntradosFace+angle), 0.0);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalExtradosDerriere-angleInitialExtradosDerriere)) / k;
        extradosDerriere << QVector3D(che.centre2D.x()+oRad * qCos(angleInitialExtradosDerriere+angle), che.centre2D.y()+oRad * qSin(angleInitialExtradosDerriere+angle), -depth);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalIntradosDerriere-angleInitialIntradosDerriere)) / k;
        intradosDerriere << QVector3D(che.centre2D.x()+iRad*qCos(angleInitialIntradosDerriere+angle), che.centre2D.y()+iRad * qSin(angleInitialIntradosDerriere+angle), -depth);
    }

    Patch *face = new Patch(g);
    for (int i = 0; i < k; ++i)
        face->addQuad(extradosFace[i], extradosFace[(i + 1)],
                       intradosFace[(i + 1)], intradosFace[i],true,false,true,false);
    Patch *derriere = new Patch(g);
    for (int i = 0; i < k; ++i)
        derriere->addQuad(intradosDerriere[i],intradosDerriere[(i + 1)],
                      extradosDerriere[(i + 1)],  extradosDerriere[i],true,false,true,false);
    Patch *intrados = new Patch(g);
    for (int i = 0; i < k; ++i)
        intrados->addQuad(intradosDerriere[i],intradosFace[i],
                    intradosFace[(i + 1)], intradosDerriere[(i + 1)],false,false,false,false) ;
    Patch *extrados = new Patch(g);
    for (int i = 0; i < k; ++i)
        extrados->addQuad(extradosDerriere[(i + 1)],extradosFace[(i + 1)],
                    extradosFace[i],  extradosDerriere[i],false,false,false,false);
    Patch *boutGauche = new Patch(g);
        boutGauche->addQuad(intradosFace[0],intradosDerriere[0],extradosDerriere[0],extradosFace[0],true,true,true,true);
    Patch *boutDroit = new Patch(g);
        boutDroit->addQuad(extradosFace[k],extradosDerriere[k],intradosDerriere[k],intradosFace[k],true,true,true,true);

    parts << face << derriere << intrados << extrados << boutGauche << boutDroit;

}

//et la on cree un chevron
//numero :
RectChevronTronque::RectChevronTronque(Geometry *g, vouteNidAbeille *p_voute, int numero)
{
    QVector<QVector3D> intradosFace;
    QVector<QVector3D> extradosFace;
    QVector<QVector3D> intradosDerriere;
    QVector<QVector3D> extradosDerriere;
    QVector<QVector3D> passage;
    QVector3D vecteurHorizontal;
    vecteurHorizontal.setX(1);vecteurHorizontal.setY(0);vecteurHorizontal.setZ(0);
    //nombre de divisions
    int k = finesse();
    double angleInitialExtradosFace, angleFinalExtradosFace;
    double angleInitialIntradosFace, angleFinalIntradosFace;
    double angleInitialExtradosDerriere, angleFinalExtradosDerriere;
    double angleInitialIntradosDerriere, angleFinalIntradosDerriere;
    double angleInitialExtradosFace3D, angleFinalExtradosFace3D;
    double angleInitialIntradosFace3D, angleFinalIntradosFace3D;
    double angleInitialExtradosDerriere3D, angleFinalExtradosDerriere3D;
    double angleInitialIntradosDerriere3D, angleFinalIntradosDerriere3D;
    double iRad,oRad;

    qreal depth;

    //la on va chercher les valeurs du chevron dans voutenidabeille.cpp
    p_voute->valeursChevronTronque(passage,numero);


    depth = passage[13].z()/2.0;

    oRad = passage[14].x();
    iRad = passage[14].y();
    double l1,l2,l3,l4;
    l1 = (passage[0]-passage[1]).length();
    l2 = (passage[4]-passage[5]).length();
    l3 = (passage[0]-passage[4]).length();
    l4 = (passage[1]-passage[5]).length();

    angleInitialExtradosFace3D = angleEntre2Vecteurs3D(passage[3]-passage[17],passage[10]-passage[17]);
    angleFinalExtradosFace3D = angleEntre2Vecteurs3D(passage[0]-passage[17],passage[10]-passage[17]);
    angleInitialIntradosFace3D = angleEntre2Vecteurs3D(passage[2]-passage[17],passage[10]-passage[17]);
    angleFinalIntradosFace3D = angleEntre2Vecteurs3D(passage[1]-passage[17],passage[10]-passage[17]);
    angleInitialExtradosDerriere3D = angleEntre2Vecteurs3D(passage[7]-passage[18],passage[11]-passage[18]);
    angleFinalExtradosDerriere3D = angleEntre2Vecteurs3D(passage[4]-passage[18],passage[11]-passage[18]);
    angleInitialIntradosDerriere3D = angleEntre2Vecteurs3D(passage[6]-passage[18],passage[11]-passage[18]);
    angleFinalIntradosDerriere3D = angleEntre2Vecteurs3D(passage[5]-passage[18],passage[11]-passage[18]);

    if(((passage[3]-passage[0]).length()>(passage[3]-passage[10]).length())&&((passage[3]-passage[0]).length()>(passage[0]-passage[10]).length()))
    {angleInitialExtradosFace=pi/2.0-angleInitialExtradosFace3D;angleFinalExtradosFace=pi/2.0+angleFinalExtradosFace3D;}
    if(((passage[3]-passage[10]).length()>(passage[3]-passage[0]).length())&&((passage[3]-passage[10]).length()>(passage[0]-passage[10]).length()))
    {angleInitialExtradosFace=pi/2.0-angleInitialExtradosFace3D;angleFinalExtradosFace=pi/2.0-angleFinalExtradosFace3D;}
    if(((passage[0]-passage[10]).length()>(passage[3]-passage[10]).length())&&((passage[0]-passage[10]).length()>(passage[3]-passage[0]).length()))
    {angleInitialExtradosFace=pi/2.0+angleInitialExtradosFace3D;angleFinalExtradosFace=pi/2.0+angleFinalExtradosFace3D;}

    if(((passage[2]-passage[1]).length()>(passage[2]-passage[15]).length())&&((passage[2]-passage[1]).length()>(passage[1]-passage[15]).length()))
    {angleInitialIntradosFace=pi/2.0-angleInitialIntradosFace3D;angleFinalIntradosFace=pi/2.0+angleFinalIntradosFace3D;}
    if(((passage[2]-passage[15]).length()>(passage[2]-passage[1]).length())&&((passage[2]-passage[15]).length()>(passage[1]-passage[15]).length()))
    {angleInitialIntradosFace=pi/2.0-angleInitialIntradosFace3D;angleFinalIntradosFace=pi/2.0-angleFinalIntradosFace3D;}
    if(((passage[1]-passage[15]).length()>(passage[2]-passage[1]).length())&&((passage[1]-passage[15]).length()>(passage[2]-passage[15]).length()))
    {angleInitialIntradosFace=pi/2.0+angleInitialIntradosFace3D;angleFinalIntradosFace=pi/2.0+angleFinalIntradosFace3D;}

    if(((passage[7]-passage[4]).length()>(passage[4]-passage[11]).length())&&((passage[7]-passage[4]).length()>(passage[7]-passage[11]).length()))
    {angleInitialExtradosDerriere=pi/2.0-angleInitialExtradosDerriere3D;angleFinalExtradosDerriere=pi/2.0+angleFinalExtradosDerriere3D;}
    if(((passage[7]-passage[11]).length()>(passage[7]-passage[4]).length())&&((passage[7]-passage[11]).length()>(passage[4]-passage[11]).length()))
    {angleInitialExtradosDerriere=pi/2.0-angleInitialExtradosDerriere3D;angleFinalExtradosDerriere=pi/2.0-angleFinalExtradosDerriere3D;}
    if(((passage[4]-passage[11]).length()>(passage[7]-passage[4]).length())&&((passage[4]-passage[11]).length()>(passage[7]-passage[11]).length()))
    {angleInitialExtradosDerriere=pi/2.0+angleInitialExtradosDerriere3D;angleFinalExtradosDerriere=pi/2.0+angleFinalExtradosDerriere3D;}

    if(((passage[6]-passage[5]).length()>(passage[6]-passage[16]).length())&&((passage[6]-passage[5]).length()>(passage[5]-passage[16]).length()))
    {angleInitialIntradosDerriere=pi/2.0-angleInitialIntradosDerriere3D;angleFinalIntradosDerriere=pi/2.0+angleFinalIntradosDerriere3D;}
    if(((passage[6]-passage[16]).length()>(passage[6]-passage[5]).length())&&((passage[6]-passage[16]).length()>(passage[5]-passage[16]).length()))
    {angleInitialIntradosDerriere=pi/2.0-angleInitialIntradosDerriere3D;angleFinalIntradosDerriere=pi/2.0-angleFinalIntradosDerriere3D;}
    if(((passage[5]-passage[16]).length()>(passage[6]-passage[5]).length())&&((passage[5]-passage[16]).length()>(passage[6]-passage[16]).length()))
    {angleInitialIntradosDerriere=pi/2.0+angleInitialIntradosDerriere3D;angleFinalIntradosDerriere=pi/2.0+angleFinalIntradosDerriere3D;}

    //ca c'est si c'est un chevron gauche
    if(numero>=passage[14].z())
    {
        double sauv;
        sauv = angleInitialExtradosFace;angleInitialExtradosFace = angleInitialExtradosDerriere;angleInitialExtradosDerriere = sauv;
        sauv = angleFinalExtradosFace;angleFinalExtradosFace = angleFinalExtradosDerriere;angleFinalExtradosDerriere = sauv;
        sauv = angleInitialIntradosFace;angleInitialIntradosFace = angleInitialIntradosDerriere;angleInitialIntradosDerriere = sauv;
        sauv = angleFinalIntradosFace;angleFinalIntradosFace = angleFinalIntradosDerriere; angleFinalIntradosDerriere = sauv;
    }

    // on cree les 4 morceaux de courbes cercles interieur et exterieur
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalExtradosFace-angleInitialExtradosFace)) / k;
        extradosFace << QVector3D(passage[8].x()+oRad * qCos(angleInitialExtradosFace+angle), passage[8].y()+oRad * qSin(angleInitialExtradosFace+angle), depth);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalIntradosFace-angleInitialIntradosFace)) / k;
        intradosFace << QVector3D(passage[8].x()+iRad*qCos(angleInitialIntradosFace+angle), passage[8].y()+iRad * qSin(angleInitialIntradosFace+angle), depth);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalExtradosDerriere-angleInitialExtradosDerriere)) / k;
        extradosDerriere << QVector3D(passage[9].x()+oRad * qCos(angleInitialExtradosDerriere+angle), passage[9].y()+oRad * qSin(angleInitialExtradosDerriere+angle), -depth);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalIntradosDerriere-angleInitialIntradosDerriere)) / k;
        intradosDerriere << QVector3D(passage[9].x()+iRad*qCos(angleInitialIntradosDerriere+angle), passage[9].y()+iRad * qSin(angleInitialIntradosDerriere+angle), -depth);
    }

    Patch *face = new Patch(g);
    for (int i = 0; i < k; ++i)
        face->addQuad(extradosFace[i], extradosFace[(i + 1)],
                       intradosFace[(i + 1)], intradosFace[i],true,false,true,false);
    Patch *derriere = new Patch(g);
    for (int i = 0; i < k; ++i)
        derriere->addQuad(intradosDerriere[i],intradosDerriere[(i + 1)],
                      extradosDerriere[(i + 1)],  extradosDerriere[i],true,false,true,false);
    Patch *intrados = new Patch(g);
    for (int i = 0; i < k; ++i)
        intrados->addQuad(intradosDerriere[i],intradosFace[i],
                    intradosFace[(i + 1)], intradosDerriere[(i + 1)],false,false,false,false) ;
    Patch *extrados = new Patch(g);
    for (int i = 0; i < k; ++i)
        extrados->addQuad(extradosDerriere[(i + 1)],extradosFace[(i + 1)],
                    extradosFace[i],  extradosDerriere[i],false,false,false,false);
    Patch *boutGauche = new Patch(g);
        boutGauche->addQuad(intradosFace[0],intradosDerriere[0],extradosDerriere[0],extradosFace[0],true,true,true,true);
    Patch *boutDroit = new Patch(g);
        boutDroit->addQuad(extradosFace[k],extradosDerriere[k],intradosDerriere[k],intradosFace[k],true,true,true,true);

    parts << face << derriere << intrados << extrados << boutGauche << boutDroit;

}




//et la on cree une rive
RectRive::RectRive(Geometry *g, vouteNidAbeille *p_voute)
{
    QVector<QVector3D> intradosFace;
    QVector<QVector3D> extradosFace;
    QVector<QVector3D> passage;

    //nombre de divisions
    int k = finesse();
    double angleInitialExtrados, angleFinalExtrados;
    double angleInitialIntrados, angleFinalIntrados;
    double intradosRayon,extradosRayon;

    qreal epaisseurRive;

    //la on va chercher les valeurs du chevron dans voutenidabeille.cpp
    p_voute->valeursRive(passage);

    epaisseurRive = passage[1].x();
    extradosRayon = passage[0].z();
    intradosRayon = extradosRayon-passage[1].y();
    angleInitialExtrados = passage[1].z();
    angleFinalExtrados = passage[2].x();
    QVector3D centre;
    centre.setX(passage[0].x());centre.setY(passage[0].y());
    angleInitialIntrados = qAsin(extradosRayon*qSin(angleInitialExtrados)/intradosRayon);
    if(passage[2].y()==1)
    {
        angleFinalIntrados = pi-angleInitialIntrados;
    }
    else
    {
        angleFinalIntrados = qAcos(extradosRayon*qCos(angleFinalExtrados)/intradosRayon);
    }
    // on cree les 4 morceaux de courbes cercles interieur et exterieur
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalExtrados-angleInitialExtrados)) / k;
        extradosFace << QVector3D(centre.x()+extradosRayon*qCos(angleInitialExtrados+angle), centre.y()+extradosRayon*qSin(angleInitialExtrados+angle), 0);
    }
    for (int i = 0; i <= k; ++i) {
        qreal angle = (i * (angleFinalIntrados-angleInitialIntrados)) / k;
        intradosFace << QVector3D(centre.x()+intradosRayon*qCos(angleInitialIntrados+angle), centre.y()+intradosRayon*qSin(angleInitialIntrados+angle), 0);
    }

    QVector<QVector3D> intradosDerriere = extrude(intradosFace,epaisseurRive);
    QVector<QVector3D> extradosDerriere = extrude(extradosFace,epaisseurRive);

    Patch *face = new Patch(g);
    for (int i = 0; i < k; ++i)
        face->addQuad(extradosFace[i], extradosFace[(i + 1)],
                       intradosFace[(i + 1)], intradosFace[i],true,false,true,false);
    Patch *derriere = new Patch(g);
    for (int i = 0; i < k; ++i)
        derriere->addQuad(intradosDerriere[i],intradosDerriere[(i + 1)],
                      extradosDerriere[(i + 1)],  extradosDerriere[i],true,false,true,false);
    Patch *intrados = new Patch(g);
    for (int i = 0; i < k; ++i)
        intrados->addQuad(intradosDerriere[i],intradosFace[i],
                    intradosFace[(i + 1)], intradosDerriere[(i + 1)],false,false,false,false) ;
    Patch *extrados = new Patch(g);
    for (int i = 0; i < k; ++i)
        extrados->addQuad(extradosDerriere[(i + 1)],extradosFace[(i + 1)],
                    extradosFace[i],  extradosDerriere[i],false,false,false,false);

    Patch *boutGauche = new Patch(g);
        boutGauche->addQuad(intradosFace[0],intradosDerriere[0],extradosDerriere[0],extradosFace[0],true,true,true,true);
    Patch *boutDroit = new Patch(g);
        boutDroit->addQuad(extradosFace[k],extradosDerriere[k],intradosDerriere[k],intradosFace[k],true,true,true,true);

    parts << face << derriere << intrados << extrados << boutGauche << boutDroit;

}

// pour l heliyourte

// une traverse

RectTraverse::RectTraverse(Geometry *g, heliyourte *p_heliy)
{
    QVector<QVector3D> face, derriere;

    face << QVector3D(0.0,0.0,0.0);
    face << QVector3D(p_heliy->hel_tetonTraverse,0.0,0.0);
    face << QVector3D(p_heliy->hel_tetonTraverse+p_heliy->hel_H12_H14,0.0,0.0);
    face << QVector3D(2.0*p_heliy->hel_tetonTraverse+p_heliy->hel_H12_H14,0.0,0.0);
    face << QVector3D(2.0*p_heliy->hel_tetonTraverse+p_heliy->hel_H12_H14,p_heliy->hel_hauteurMiBoisTraverse,0.0);
    face << QVector3D(p_heliy->hel_tetonTraverse+p_heliy->hel_H12_H14,p_heliy->hel_hauteurMiBoisTraverse,0.0);
    face << QVector3D(p_heliy->hel_tetonTraverse+p_heliy->hel_H12_H14,p_heliy->hel_retombeeTraverse,0.0);
    face << QVector3D(p_heliy->hel_tetonTraverse,p_heliy->hel_retombeeTraverse,0.0);
    face << QVector3D(p_heliy->hel_tetonTraverse,p_heliy->hel_hauteurMiBoisTraverse,0.0);
    face << QVector3D(0.0,p_heliy->hel_hauteurMiBoisTraverse,0.0);

    derriere << QVector3D(0.0,0.0,-p_heliy->hel_epaisseurTraverse);
    derriere << QVector3D(p_heliy->hel_tetonTraverse,0.0,-p_heliy->hel_epaisseurTraverse);
    derriere << QVector3D(p_heliy->hel_tetonTraverse+p_heliy->hel_H9_H13,0.0,-p_heliy->hel_epaisseurTraverse);
    derriere << QVector3D(2.0*p_heliy->hel_tetonTraverse+p_heliy->hel_H9_H13,0.0,-p_heliy->hel_epaisseurTraverse);
    derriere << QVector3D(2.0*p_heliy->hel_tetonTraverse+p_heliy->hel_H9_H13,p_heliy->hel_hauteurMiBoisTraverse,-p_heliy->hel_epaisseurTraverse);
    derriere << QVector3D(p_heliy->hel_tetonTraverse+p_heliy->hel_H9_H13,p_heliy->hel_hauteurMiBoisTraverse,-p_heliy->hel_epaisseurTraverse);
    derriere << QVector3D(p_heliy->hel_tetonTraverse+p_heliy->hel_H9_H13,p_heliy->hel_retombeeTraverse,-p_heliy->hel_epaisseurTraverse);
    derriere << QVector3D(p_heliy->hel_tetonTraverse,p_heliy->hel_retombeeTraverse,-p_heliy->hel_epaisseurTraverse);
    derriere << QVector3D(p_heliy->hel_tetonTraverse,p_heliy->hel_hauteurMiBoisTraverse,-p_heliy->hel_epaisseurTraverse);
    derriere << QVector3D(0.0,p_heliy->hel_hauteurMiBoisTraverse,-p_heliy->hel_epaisseurTraverse);

    for(int i=0;i<10;i++)
    {
        face[i].setX(face[i].x()-(p_heliy->hel_H12_H14/2.0+p_heliy->hel_tetonTraverse));
        face[i].setY(face[i].y()-p_heliy->hel_hauteurMiBoisTraverse);
        face[i].setZ(face[i].z()+p_heliy->hel_epaisseurTraverse/2.0);
        derriere[i].setX(derriere[i].x()-(p_heliy->hel_H12_H14/2.0+p_heliy->hel_tetonTraverse));
        derriere[i].setY(derriere[i].y()-p_heliy->hel_hauteurMiBoisTraverse);
        derriere[i].setZ(derriere[i].z()+p_heliy->hel_epaisseurTraverse/2.0);
    }

    Patch *facepart = new Patch(g);
    facepart->addQuad(face[0], face[1], face[8], face[9], true,false,true,true);
    facepart->addQuad(face[8], face[5], face[6], face[7], false,true,true,true);
    facepart->addQuad(face[1], face[2], face[5], face[8], true,false,false,false);
    facepart->addQuad(face[2], face[3], face[4], face[5], true,true,true,false);

    Patch *derrierepart = new Patch(g);
    derrierepart->addQuad(derriere[3], derriere[2], derriere[5], derriere[4], true,false,true,true);
    derrierepart->addQuad(derriere[2], derriere[1], derriere[8], derriere[5], true,false,false,false);
    derrierepart->addQuad(derriere[5], derriere[8], derriere[7], derriere[6], false,true,true,true);
    derrierepart->addQuad(derriere[1], derriere[0], derriere[9], derriere[8], true,true,true,false);

    Patch *tour = new Patch(g);
    tour->setSmoothing(Patch::Faceted);
    tour->addQuad(derriere[0],face[0],face[9],derriere[9],true,true,true,true);
    tour->addQuad(derriere[9],face[9],face[8],derriere[8],true,true,true,true);
    tour->addQuad(derriere[8],face[8],face[7],derriere[7],true,true,true,true);
    tour->addQuad(derriere[7],face[7],face[6],derriere[6],true,true,true,true);
    tour->addQuad(derriere[6],face[6],face[5],derriere[5],true,true,true,true);
    tour->addQuad(derriere[5],face[5],face[4],derriere[4],true,true,true,true);
    tour->addQuad(derriere[4],face[4],face[3],derriere[3],true,true,true,true);
    tour->addQuad(derriere[3],face[3],face[0],derriere[0],true,true,true,true);

    parts << facepart << derrierepart << tour;

}

// le montant A

RectMontantA::RectMontantA(Geometry *g, heliyourte *p_heliy)
{
    QVector<QVector3D> face, derriere;

    face << QVector3D(0.0,0.0,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,0.0,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_hauteurMurs+p_heliy->hel_retombeePlanchesMontant*qTan(p_heliy->hel_penteToit),0.0);
    face << QVector3D(0.0,p_heliy->hel_hauteurMurs,0.0);
    face << QVector3D(0.0,p_heliy->hel_DdansA.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_DdansA.distanceRapportBordInt-p_heliy->hel_DdansA.largeur,p_heliy->hel_DdansA.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_DdansA.distanceRapportBordInt,p_heliy->hel_DdansA.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_DdansA.bas,0.0);
    face << QVector3D(0.0,p_heliy->hel_DdansA.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_DdansA.distanceRapportBordInt-p_heliy->hel_DdansA.largeur,p_heliy->hel_DdansA.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_DdansA.distanceRapportBordInt,p_heliy->hel_DdansA.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_DdansA.haut,0.0);
    face << QVector3D(0.0,p_heliy->hel_CdansA.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_CdansA.distanceRapportBordInt-p_heliy->hel_CdansA.largeur,p_heliy->hel_CdansA.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_CdansA.distanceRapportBordInt,p_heliy->hel_CdansA.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_CdansA.bas,0.0);
    face << QVector3D(0.0,p_heliy->hel_CdansA.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_CdansA.distanceRapportBordInt-p_heliy->hel_CdansA.largeur,p_heliy->hel_CdansA.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_CdansA.distanceRapportBordInt,p_heliy->hel_CdansA.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_CdansA.haut,0.0);
    face << QVector3D(0.0,p_heliy->hel_FdansA.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_FdansA.distanceRapportBordInt-p_heliy->hel_FdansA.largeur,p_heliy->hel_FdansA.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_FdansA.distanceRapportBordInt,p_heliy->hel_FdansA.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_FdansA.bas,0.0);
    face << QVector3D(0.0,p_heliy->hel_FdansA.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_FdansA.distanceRapportBordInt-p_heliy->hel_FdansA.largeur,p_heliy->hel_FdansA.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_FdansA.distanceRapportBordInt,p_heliy->hel_FdansA.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_FdansA.haut,0.0);

    derriere = extrude(face,p_heliy->hel_epaisseurPlancheAetC);
    derriere[5].setX(p_heliy->hel_FdansA.distanceRapportBordInt);
    derriere[6].setX(p_heliy->hel_FdansA.distanceRapportBordInt+p_heliy->hel_FdansA.largeur);
    derriere[9].setX(p_heliy->hel_FdansA.distanceRapportBordInt);
    derriere[10].setX(p_heliy->hel_FdansA.distanceRapportBordInt+p_heliy->hel_FdansA.largeur);
    derriere[13].setX(p_heliy->hel_FdansA.distanceRapportBordInt);
    derriere[14].setX(p_heliy->hel_FdansA.distanceRapportBordInt+p_heliy->hel_FdansA.largeur);
    derriere[17].setX(p_heliy->hel_FdansA.distanceRapportBordInt);
    derriere[18].setX(p_heliy->hel_FdansA.distanceRapportBordInt+p_heliy->hel_FdansA.largeur);
    derriere[21].setX(p_heliy->hel_FdansA.distanceRapportBordInt);
    derriere[22].setX(p_heliy->hel_FdansA.distanceRapportBordInt+p_heliy->hel_FdansA.largeur);
    derriere[25].setX(p_heliy->hel_FdansA.distanceRapportBordInt);
    derriere[26].setX(p_heliy->hel_FdansA.distanceRapportBordInt+p_heliy->hel_FdansA.largeur);

    for(int i=0;i<28;i++)
    {
        face[i].setX(face[i].x()-p_heliy->hel_retombeePlanchesMontant/2.0);
        face[i].setY(face[i].y()-p_heliy->hel_hauteurMurs/2.0);
        face[i].setZ(face[i].z()+p_heliy->hel_epaisseurPlancheAetC/2.0);
        derriere[i].setX(derriere[i].x()-p_heliy->hel_retombeePlanchesMontant/2.0);
        derriere[i].setY(derriere[i].y()-p_heliy->hel_hauteurMurs/2.0);
        derriere[i].setZ(derriere[i].z()+p_heliy->hel_epaisseurPlancheAetC/2.0);
    }

    Patch *facepart = new Patch(g);
    facepart->addQuad(face[0], face[1], face[7], face[4], true,true,false,true);
    facepart->addQuad(face[4], face[5], face[9], face[8], false,true,false,true);
    facepart->addQuad(face[6], face[7], face[11], face[10], false,true,false,true);
    facepart->addQuad(face[8], face[11], face[15], face[12], false,true,false,true);
    facepart->addQuad(face[12], face[13], face[17], face[16], false,true,false,true);
    facepart->addQuad(face[14], face[15], face[19], face[18], false,true,false,true);
    facepart->addQuad(face[16], face[19], face[23], face[20], false,true,false,true);
    facepart->addQuad(face[20], face[21], face[25], face[24], false,true,false,true);
    facepart->addQuad(face[22], face[23], face[27], face[26], false,true,false,true);
    facepart->addQuad(face[24], face[27], face[2], face[3], false,true,false,true);

    Patch *derrierepart = new Patch(g);
    derrierepart->addQuad(derriere[1], derriere[0], derriere[4], derriere[7], true,true,false,true);
    derrierepart->addQuad(derriere[5], derriere[4], derriere[8], derriere[9], false,true,false,true);
    derrierepart->addQuad(derriere[7], derriere[6], derriere[10], derriere[11], false,true,false,true);
    derrierepart->addQuad(derriere[11], derriere[8], derriere[12], derriere[15], false,true,false,true);
    derrierepart->addQuad(derriere[13], derriere[12], derriere[16], derriere[17], false,true,false,true);
    derrierepart->addQuad(derriere[15], derriere[14], derriere[18], derriere[19], false,true,false,true);
    derrierepart->addQuad(derriere[19], derriere[16], derriere[20], derriere[23], false,true,false,true);
    derrierepart->addQuad(derriere[21], derriere[20], derriere[24], derriere[25], false,true,false,true);
    derrierepart->addQuad(derriere[23], derriere[22], derriere[26], derriere[27], false,true,false,true);
    derrierepart->addQuad(derriere[27], derriere[24], derriere[3], derriere[2], false,true,false,true);

    Patch *tour = new Patch(g);
    tour->setSmoothing(Patch::Faceted);
    tour->addQuad(derriere[0],face[0],face[3],derriere[3],true,true,true,true);
    tour->addQuad(derriere[3],face[3],face[2],derriere[2],true,true,true,true);
    tour->addQuad(derriere[2],face[2],face[1],derriere[1],true,true,true,true);
    tour->addQuad(derriere[1],face[1],face[0],derriere[0],true,true,true,true);

    Patch *trouD = new Patch(g);
    trouD->setSmoothing(Patch::Faceted);
    trouD->addQuad(derriere[5],face[5],face[6],derriere[6],true,true,true,true);
    trouD->addQuad(derriere[6],face[6],face[10],derriere[10],true,true,true,true);
    trouD->addQuad(derriere[10],face[10],face[9],derriere[9],true,true,true,true);
    trouD->addQuad(derriere[9],face[9],face[5],derriere[5],true,true,true,true);

    Patch *trouC = new Patch(g);
    trouC->setSmoothing(Patch::Faceted);
    trouC->addQuad(derriere[13],face[13],face[14],derriere[14],true,true,true,true);
    trouC->addQuad(derriere[14],face[14],face[18],derriere[18],true,true,true,true);
    trouC->addQuad(derriere[18],face[18],face[17],derriere[17],true,true,true,true);
    trouC->addQuad(derriere[17],face[17],face[13],derriere[13],true,true,true,true);

    Patch *trouF = new Patch(g);
    trouF->setSmoothing(Patch::Faceted);
    trouF->addQuad(derriere[21],face[21],face[22],derriere[22],true,true,true,true);
    trouF->addQuad(derriere[22],face[22],face[26],derriere[26],true,true,true,true);
    trouF->addQuad(derriere[26],face[26],face[25],derriere[25],true,true,true,true);
    trouF->addQuad(derriere[25],face[25],face[21],derriere[21],true,true,true,true);


    parts << facepart << derrierepart << tour << trouD << trouC << trouF;

}

// le montant C

RectMontantC::RectMontantC(Geometry *g, heliyourte *p_heliy)
{
    QVector<QVector3D> face, derriere;

    face << QVector3D(0.0,0.0,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,0.0,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_hauteurMurs+p_heliy->hel_retombeePlanchesMontant*qTan(p_heliy->hel_penteToit),0.0);
    face << QVector3D(0.0,p_heliy->hel_hauteurMurs,0.0);
    face << QVector3D(0.0,p_heliy->hel_BdansC.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_BdansC.distanceRapportBordInt-p_heliy->hel_BdansC.largeur,p_heliy->hel_BdansC.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_BdansC.distanceRapportBordInt,p_heliy->hel_BdansC.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_BdansC.bas,0.0);
    face << QVector3D(0.0,p_heliy->hel_BdansC.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_BdansC.distanceRapportBordInt-p_heliy->hel_BdansC.largeur,p_heliy->hel_BdansC.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_BdansC.distanceRapportBordInt,p_heliy->hel_BdansC.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_BdansC.haut,0.0);
    face << QVector3D(0.0,p_heliy->hel_AdansC.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_AdansC.largeur,p_heliy->hel_AdansC.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_AdansC.bas,0.0);
    face << QVector3D(0.0,p_heliy->hel_AdansC.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_AdansC.largeur,p_heliy->hel_AdansC.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_AdansC.haut,0.0);
    face << QVector3D(0.0,p_heliy->hel_EdansC.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_EdansC.distanceRapportBordInt-p_heliy->hel_EdansC.largeur,p_heliy->hel_EdansC.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_EdansC.distanceRapportBordInt,p_heliy->hel_EdansC.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_EdansC.bas,0.0);
    face << QVector3D(0.0,p_heliy->hel_EdansC.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_EdansC.distanceRapportBordInt-p_heliy->hel_EdansC.largeur,p_heliy->hel_EdansC.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_EdansC.distanceRapportBordInt,p_heliy->hel_EdansC.haut,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_EdansC.haut,0.0);

    derriere = extrude(face,p_heliy->hel_epaisseurPlancheAetC);

    for(int i=0;i<26;i++)
    {
        face[i].setX(face[i].x()-p_heliy->hel_retombeePlanchesMontant/2.0);
        face[i].setY(face[i].y()-p_heliy->hel_hauteurMurs/2.0);
        face[i].setZ(face[i].z()+p_heliy->hel_epaisseurPlancheAetC/2.0);
        derriere[i].setX(derriere[i].x()-p_heliy->hel_retombeePlanchesMontant/2.0);
        derriere[i].setY(derriere[i].y()-p_heliy->hel_hauteurMurs/2.0);
        derriere[i].setZ(derriere[i].z()+p_heliy->hel_epaisseurPlancheAetC/2.0);
    }

    Patch *facepart = new Patch(g);
    facepart->addQuad(face[0], face[1], face[7], face[4], true,true,false,true);
    facepart->addQuad(face[4], face[5], face[9], face[8], false,true,false,true);
    facepart->addQuad(face[6], face[7], face[11], face[10], false,true,false,true);
    facepart->addQuad(face[8], face[11], face[14], face[12], false,true,false,true);
    facepart->addQuad(face[12], face[13], face[16], face[15], false,true,false,true);
    facepart->addQuad(face[15], face[17], face[21], face[18], false,true,false,true);
    facepart->addQuad(face[18], face[19], face[23], face[22], false,true,false,true);
    facepart->addQuad(face[20], face[21], face[25], face[24], false,true,false,true);
    facepart->addQuad(face[22], face[25], face[2], face[3], false,true,false,true);

    Patch *derrierepart = new Patch(g);
    derrierepart->addQuad(derriere[1], derriere[0], derriere[4], derriere[7], true,true,false,true);
    derrierepart->addQuad(derriere[5], derriere[4], derriere[8], derriere[9], false,true,false,true);
    derrierepart->addQuad(derriere[7], derriere[6], derriere[10], derriere[11], false,true,false,true);
    derrierepart->addQuad(derriere[11], derriere[8], derriere[12], derriere[14], false,true,false,true);
    derrierepart->addQuad(derriere[13], derriere[12], derriere[15], derriere[16], false,true,false,true);
    derrierepart->addQuad(derriere[17], derriere[15], derriere[18], derriere[21], false,true,false,true);
    derrierepart->addQuad(derriere[19], derriere[18], derriere[22], derriere[23], false,true,false,true);
    derrierepart->addQuad(derriere[21], derriere[20], derriere[24], derriere[25], false,true,false,true);
    derrierepart->addQuad(derriere[25], derriere[22], derriere[3], derriere[2], false,true,false,true);

    Patch *tour = new Patch(g);
    tour->setSmoothing(Patch::Faceted);
    tour->addQuad(derriere[0],face[0],face[3],derriere[3],true,true,true,true);
    tour->addQuad(derriere[3],face[3],face[2],derriere[2],true,true,true,true);
    tour->addQuad(derriere[2],face[2],face[17],derriere[17],true,true,true,true);
    tour->addQuad(derriere[14],face[14],face[1],derriere[1],true,true,true,true);
    tour->addQuad(derriere[1],face[1],face[0],derriere[0],true,true,true,true);

    Patch *trouA = new Patch(g);
    trouA->setSmoothing(Patch::Faceted);
    trouA->addQuad(derriere[5],face[5],face[6],derriere[6],true,true,true,true);
    trouA->addQuad(derriere[6],face[6],face[10],derriere[10],true,true,true,true);
    trouA->addQuad(derriere[10],face[10],face[9],derriere[9],true,true,true,true);
    trouA->addQuad(derriere[9],face[9],face[5],derriere[5],true,true,true,true);

    Patch *trouB = new Patch(g);
    trouB->setSmoothing(Patch::Faceted);
    trouB->addQuad(derriere[13],face[13],face[14],derriere[14],true,true,true,true);
    trouB->addQuad(derriere[16],face[16],face[13],derriere[13],true,true,true,true);
    trouB->addQuad(derriere[17],face[17],face[16],derriere[16],true,true,true,true);

    Patch *trouE = new Patch(g);
    trouE->setSmoothing(Patch::Faceted);
    trouE->addQuad(derriere[19],face[19],face[20],derriere[20],true,true,true,true);
    trouE->addQuad(derriere[20],face[20],face[24],derriere[24],true,true,true,true);
    trouE->addQuad(derriere[24],face[24],face[23],derriere[23],true,true,true,true);
    trouE->addQuad(derriere[23],face[23],face[19],derriere[19],true,true,true,true);


    parts << facepart << derrierepart << tour << trouA << trouB << trouE;

}


// le montant B

RectMontantB::RectMontantB(Geometry *g, heliyourte *p_heliy)
{
    QVector<QVector3D> face, derriere;

    face << QVector3D(0.0,p_heliy->hel_montantB_1.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_montantB_1.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_montantB_1.haut,0.0);
    face << QVector3D(0.0,p_heliy->hel_montantB_1.haut,0.0);
    face << QVector3D(0.0,p_heliy->hel_montantB_2.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_montantB_2.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_montantB_2.haut,0.0);
    face << QVector3D(0.0,p_heliy->hel_montantB_2.haut,0.0);
    face << QVector3D(0.0,p_heliy->hel_montantB_3.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_montantB_3.bas,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant,p_heliy->hel_montantB_3.haut,0.0);
    face << QVector3D(0.0,p_heliy->hel_montantB_3.haut,0.0);

    derriere = extrude(face,p_heliy->hel_epaisseurPlancheB);

    for(int i=0;i<12;i++)
    {
        face[i].setX(face[i].x()-p_heliy->hel_retombeePlanchesMontant/2.0);
        face[i].setY(face[i].y()-p_heliy->hel_hauteurMurs/2.0);
        face[i].setZ(face[i].z()+p_heliy->hel_epaisseurPlancheB/2.0);
        derriere[i].setX(derriere[i].x()-p_heliy->hel_retombeePlanchesMontant/2.0);
        derriere[i].setY(derriere[i].y()-p_heliy->hel_hauteurMurs/2.0);
        derriere[i].setZ(derriere[i].z()+p_heliy->hel_epaisseurPlancheB/2.0);
    }

    Patch *facepart = new Patch(g);
    facepart->setSmoothing(Patch::Faceted);
    facepart->addQuad(face[0], face[1], face[2], face[3], true,true,true,true);
    facepart->addQuad(face[4], face[5], face[6], face[7], true,true,true,true);
    facepart->addQuad(face[8], face[9], face[10], face[11], true,true,true,true);

    Patch *derrierepart = new Patch(g);
    derrierepart->setSmoothing(Patch::Faceted);
    derrierepart->addQuad(derriere[1], derriere[0], derriere[3], derriere[2], true,true,true,true);
    derrierepart->addQuad(derriere[5], derriere[4], derriere[7], derriere[6], true,true,true,true);
    derrierepart->addQuad(derriere[9], derriere[8], derriere[11], derriere[10], true,true,true,true);

    Patch *tour = new Patch(g);
    tour->setSmoothing(Patch::Faceted);
    tour->addQuad(derriere[0],face[0],face[3],derriere[3],true,true,true,true);
    tour->addQuad(derriere[3],face[3],face[2],derriere[2],true,true,true,true);
    tour->addQuad(derriere[2],face[2],face[1],derriere[1],true,true,true,true);
    tour->addQuad(derriere[1],face[1],face[0],derriere[0],true,true,true,true);
    tour->addQuad(derriere[4],face[4],face[7],derriere[7],true,true,true,true);
    tour->addQuad(derriere[7],face[7],face[6],derriere[6],true,true,true,true);
    tour->addQuad(derriere[6],face[6],face[5],derriere[5],true,true,true,true);
    tour->addQuad(derriere[5],face[5],face[4],derriere[4],true,true,true,true);
    tour->addQuad(derriere[8],face[8],face[11],derriere[11],true,true,true,true);
    tour->addQuad(derriere[11],face[11],face[10],derriere[10],true,true,true,true);
    tour->addQuad(derriere[10],face[10],face[9],derriere[9],true,true,true,true);
    tour->addQuad(derriere[9],face[9],face[8],derriere[8],true,true,true,true);

    parts << facepart << derrierepart << tour;

}

// une perche

RectPerche::RectPerche(Geometry *g, heliyourte *p_heliy)
{
    QVector<QVector3D> face, derriere, tige;

    face << QVector3D(0.0,p_heliy->hel_hauteurMurs,0.0);
    face << QVector3D(0.0,p_heliy->hel_montantB_3.haut+p_heliy->hel_hauteurMiBoisTraverse,0.0);
    face << QVector3D(p_heliy->hel_retombeePlanchesMontant-p_heliy->hel_EdansC.distanceRapportBordInt,face[1].y(),0.0);
    face << QVector3D(face[2].x(),p_heliy->hel_montantB_3.haut,0.0);

    double a, b1, b2;
    a = tan(p_heliy->hel_angleInclinaisonPerches);
    b1 = face[0].y()-a*face[0].x();
    b2 = face[0].y()-p_heliy->hel_retombeeMiniPerches*qCos(p_heliy->hel_angleInclinaisonPerches)-a*(face[0].x()+p_heliy->hel_retombeeMiniPerches*qSin(p_heliy->hel_angleInclinaisonPerches));

    face << QVector3D((face[3].y()-b2)/a,face[3].y(),0.0);
    face << QVector3D(face[2].x(),a*face[2].x()+b1,0.0);

    QVector2D B;
    B = QVector2D(p_heliy->hel_AB*qCos(p_heliy->hel_angleInclinaisonPerches),p_heliy->hel_AB*qSin(p_heliy->hel_angleInclinaisonPerches));
    double lh;
    lh = p_heliy->hel_retombeeMiniPerches/qSin(p_heliy->hel_angleInclinaisonPerches);

    face << QVector3D(B.x()-p_heliy->hel_decalageTigeRapportBord,a*(B.x()-p_heliy->hel_decalageTigeRapportBord)+b1,0.0);
    face << QVector3D(face[6].x()+p_heliy->hel_epaisseurPerche/qSin(p_heliy->hel_angleDivision),face[6].y(),0.0);
    face << QVector3D(face[7].x(),a*face[7].x()+b1,0.0);
    //9
    face << QVector3D((face[7].y()+p_heliy->hel_retombeeTonoo-b1)/a,face[7].y()+p_heliy->hel_retombeeTonoo,0.0);
    face << QVector3D(B.x()+p_heliy->hel_dIntTonoo-p_heliy->hel_decalageTigeRapportBord+p_heliy->hel_epaisseurPerche/qTan(p_heliy->hel_angleDivision),face[7].y(),0.0);
    face << QVector3D(face[10].x(),a*face[10].x()+b2,0.0);
    face << QVector3D((face[10].y()-b2)/a,face[10].y(),0.0);
    face << QVector3D(face[12].x()+p_heliy->hel_depassementHautPerche,(face[12].x()+p_heliy->hel_depassementHautPerche)*a+b2,0.0);
    face << QVector3D(face[13].x(),face[9].y(),0.0);

    double rayon_tige;
    rayon_tige = 0.005;

    face << QVector3D(B.x()+p_heliy->hel_dIntTonoo+rayon_tige,face[10].y(),0.0);
    face << QVector3D(B.x()+p_heliy->hel_dIntTonoo-rayon_tige,face[10].y(),0.0);
    face << QVector3D(B.x()+rayon_tige,face[10].y(),0.0);
    face << QVector3D(B.x()-rayon_tige,face[10].y(),0.0);

    derriere = extrude(face,-p_heliy->hel_epaisseurPerche);

    derriere[7].setX(B.x()+p_heliy->hel_decalageTigeRapportBord);
    derriere[8].setX(derriere[7].x());
    derriere[8].setY(a*derriere[8].x()+b1);


    bool percheTropEpaisse = false;
    // si la perche est tres epaisse
    if(derriere[8].x()>derriere[9].x())
    {
        derriere[8].setY(derriere[9].y());
        percheTropEpaisse = true;
        derriere[9].setZ(derriere[9].z()-(p_heliy->hel_epaisseurPerche-p_heliy->hel_epaisseurPerche*(face[8].x()-face[9].x())/(face[8].x()-derriere[8].x())));
    }

    derriere[10].setX(B.x()+p_heliy->hel_dIntTonoo-p_heliy->hel_decalageTigeRapportBord);
    derriere[11].setX(derriere[10].x());
    derriere[11].setY(a*derriere[11].x()+b2);

    double hauteur_tige;

    hauteur_tige = 0.66*p_heliy->hel_retombeeTonoo;

    tige << QVector3D(face[15].x(),face[15].y(),p_heliy->hel_epaisseurPerche/2.0-rayon_tige);
    tige << QVector3D(face[16].x(),face[15].y(),p_heliy->hel_epaisseurPerche/2.0-rayon_tige);
    tige << QVector3D(face[16].x(),face[15].y(),p_heliy->hel_epaisseurPerche/2.0+rayon_tige);
    tige << QVector3D(face[15].x(),face[15].y(),p_heliy->hel_epaisseurPerche/2.0+rayon_tige);

    tige << QVector3D(face[17].x(),face[15].y(),p_heliy->hel_epaisseurPerche/2.0-rayon_tige);
    tige << QVector3D(face[18].x(),face[15].y(),p_heliy->hel_epaisseurPerche/2.0-rayon_tige);
    tige << QVector3D(face[18].x(),face[15].y(),p_heliy->hel_epaisseurPerche/2.0+rayon_tige);
    tige << QVector3D(face[17].x(),face[15].y(),p_heliy->hel_epaisseurPerche/2.0+rayon_tige);

    tige << QVector3D(face[15].x(),face[15].y()+hauteur_tige,p_heliy->hel_epaisseurPerche/2.0-rayon_tige);
    tige << QVector3D(face[16].x(),face[15].y()+hauteur_tige,p_heliy->hel_epaisseurPerche/2.0-rayon_tige);
    tige << QVector3D(face[16].x(),face[15].y()+hauteur_tige,p_heliy->hel_epaisseurPerche/2.0+rayon_tige);
    tige << QVector3D(face[15].x(),face[15].y()+hauteur_tige,p_heliy->hel_epaisseurPerche/2.0+rayon_tige);

    tige << QVector3D(face[17].x(),face[15].y()+hauteur_tige,p_heliy->hel_epaisseurPerche/2.0-rayon_tige);
    tige << QVector3D(face[18].x(),face[15].y()+hauteur_tige,p_heliy->hel_epaisseurPerche/2.0-rayon_tige);
    tige << QVector3D(face[18].x(),face[15].y()+hauteur_tige,p_heliy->hel_epaisseurPerche/2.0+rayon_tige);
    tige << QVector3D(face[17].x(),face[15].y()+hauteur_tige,p_heliy->hel_epaisseurPerche/2.0+rayon_tige);

    double dec_x, dec_y;
    dec_x = (face[14].x()-face[0].x())/2.0;
    dec_y = p_heliy->hel_hauteurMurs+(face[14].y()-face[0].y())/2.0;

    p_heliy->hel_perche_decx = dec_x;
    p_heliy->hel_perche_decy = dec_y;

    for(int i=0;i<19;i++)
    {
        if(i<16)
        {
            tige[i].setX(tige[i].x()-dec_x);
            tige[i].setY(tige[i].y()-dec_y);
            tige[i].setZ(tige[i].z()+p_heliy->hel_epaisseurPerche/2.0);
        }
        face[i].setX(face[i].x()-dec_x);
        face[i].setY(face[i].y()-dec_y);
        face[i].setZ(face[i].z()+p_heliy->hel_epaisseurPerche/2.0);
        derriere[i].setX(derriere[i].x()-dec_x);
        derriere[i].setY(derriere[i].y()-dec_y);
        derriere[i].setZ(derriere[i].z()+p_heliy->hel_epaisseurPerche/2.0);
    }


    Patch *facepart = new Patch(g);
    facepart->addQuad(face[0], face[5], face[2], face[1], true,false,true,true);
    facepart->addTri(face[3], face[5], face[4],QVector3D(0.0,0.0,0.0), false,false,true);
    facepart->addQuad(face[4], face[5], face[6], face[11], false,true,false,true);
    facepart->addTri(face[11], face[6], face[10],QVector3D(0.0,0.0,0.0), false,false,true);
    facepart->addQuad(face[10], face[7], face[8], face[9], false,true,true,false);
    facepart->addTri(face[10], face[9], face[12],QVector3D(0.0,0.0,0.0), false,false,true);
    facepart->addQuad(face[12], face[9], face[14], face[13], false,true,true,true);

    Patch *derrierepart = new Patch(g);
    derrierepart->addQuad(derriere[0], derriere[1], derriere[2], derriere[5], true,true,false,true);
    derrierepart->addTri(derriere[3], derriere[4], derriere[5],QVector3D(0.0,0.0,0.0), true,false,false);
    derrierepart->addQuad(derriere[4], derriere[11], derriere[6], derriere[5], true,false,true,false);
    derrierepart->addTri(derriere[11], derriere[10], derriere[6],QVector3D(0.0,0.0,0.0), true,false,false);
    if(percheTropEpaisse)
    {
        derrierepart->addQuad(derriere[10], derriere[12], derriere[8], derriere[7], true, false,true,false);
        derrierepart->addQuad(derriere[12], derriere[13], derriere[14], derriere[8], true,true,true,false);

    }else
    {
        derrierepart->addQuad(derriere[10], derriere[9], derriere[8], derriere[7], false,true,true,false);
        derrierepart->addTri(derriere[10], derriere[12], derriere[9],QVector3D(0.0,0.0,0.0), true,false,false);
        derrierepart->addQuad(derriere[12], derriere[13], derriere[14], derriere[9], true,true,true,false);
    }

    Patch *tour = new Patch(g);
    tour->setSmoothing(Patch::Faceted);
    tour->addQuad(derriere[4],face[4],face[11],derriere[11],true,true,true,true);
    tour->addQuad(derriere[11],face[11],face[10],derriere[10],true,true,true,true);
    tour->addQuad(derriere[12],face[12],face[13],derriere[13],true,true,true,true);
    tour->addQuad(derriere[13],face[13],face[14],derriere[14],true,true,true,true);
    if(percheTropEpaisse)
    {
        tour->addQuad(derriere[14],face[14],face[9],derriere[8],true,true,false,true);
        tour->addTri(derriere[9],derriere[8],face[9],QVector3D(0.0,0.0,0.0),true,false,true);
        tour->addTri(derriere[9],face[9],face[8],QVector3D(0.0,0.0,0.0),true,true,true);
    }
    else
    {
        tour->addQuad(derriere[14],face[14],face[9],derriere[9],true,true,true,true);
        tour->addQuad(derriere[9],face[9],face[8],derriere[8],true,true,true,true);
    }
    tour->addQuad(derriere[8],face[8],face[7],derriere[7],true,true,true,true);
    tour->addQuad(derriere[6],face[6],face[0],derriere[0],true,true,true,true);
    tour->addQuad(derriere[0],face[0],face[1],derriere[1],true,true,true,true);
    tour->addQuad(derriere[1],face[1],face[2],derriere[2],true,true,true,true);
    tour->addQuad(derriere[2],face[2],face[3],derriere[3],true,true,true,true);
    tour->addQuad(derriere[3],face[3],face[4],derriere[4],true,true,true,true);

    Patch *toura = new Patch(g);
    toura->addQuad(derriere[10],face[10],tige[1],tige[2],true,false,true,false);
    toura->addQuad(tige[1],face[10],face[12],tige[0],false,true,false,true);
    toura->addQuad(derriere[10],tige[2],tige[3],derriere[12],false,true,false,true);
    toura->addQuad(tige[3],tige[0],face[12],derriere[12],true,false,true,false);

    Patch *tourb = new Patch(g);
    tourb->addQuad(derriere[7],face[7],tige[4],tige[7],true,false,true,false);
    tourb->addQuad(tige[4],face[7],face[6],tige[5],false,true,false,true);
    tourb->addQuad(derriere[7],tige[7],tige[6],derriere[6],false,true,false,true);
    tourb->addQuad(tige[6],tige[5],face[6],derriere[6],true,false,true,false);

    Patch *tigepart = new Patch(g);
    tigepart->setSmoothing(Patch::Faceted);
    tigepart->addQuad(tige[3],tige[2],tige[10],tige[11],true,true,true,true);
    tigepart->addQuad(tige[0],tige[3],tige[11],tige[8],true,true,true,true);
    tigepart->addQuad(tige[1],tige[0],tige[8],tige[9],true,true,true,true);
    tigepart->addQuad(tige[2],tige[1],tige[9],tige[10],true,true,true,true);
    tigepart->addQuad(tige[11],tige[10],tige[9],tige[8],true,true,true,true);

    tigepart->addQuad(tige[4],tige[5],tige[13],tige[12],true,true,true,true);
    tigepart->addQuad(tige[5],tige[6],tige[14],tige[13],true,true,true,true);
    tigepart->addQuad(tige[6],tige[7],tige[15],tige[14],true,true,true,true);
    tigepart->addQuad(tige[7],tige[4],tige[12],tige[15],true,true,true,true);
    tigepart->addQuad(tige[12],tige[13],tige[14],tige[15],true,true,true,true);

    parts << facepart << derrierepart << tour << toura << tourb << tigepart;

}

// un morceau de croix

RectCroix::RectCroix(Geometry *g, heliyourte *p_heliy)
{
    QVector<QVector3D> face, derriere;

    face << QVector3D(p_heliy->hel_cr_2[1],0.0);
    face << QVector3D(p_heliy->hel_cr_2[3],0.0);
    face << QVector3D(p_heliy->hel_cr_2[18],0.0);
    face << QVector3D(p_heliy->hel_cr_2[17],0.0);
    face << QVector3D(p_heliy->hel_cr_2[22],0.0);
    face << QVector3D(p_heliy->hel_cr_2[23],0.0);

    face << QVector3D(p_heliy->hel_cr_2[23]+(p_heliy->hel_cr_2[13]-p_heliy->hel_cr_2[23])/2.0-(p_heliy->hel_cr_2[14]-p_heliy->hel_cr_2[23])/6.0,0.0);
    face << QVector3D(p_heliy->hel_cr_2[13],0.0);
    face << QVector3D(p_heliy->hel_cr_2[14]+(p_heliy->hel_cr_2[13]-p_heliy->hel_cr_2[14])/2.0-(p_heliy->hel_cr_2[23]-p_heliy->hel_cr_2[14])/6.0,0.0);

    face << QVector3D(p_heliy->hel_cr_2[14],0.0);
    face << QVector3D(p_heliy->hel_cr_2[16],0.0);
    face << QVector3D(p_heliy->hel_cr_2[5],0.0);
    face << QVector3D(p_heliy->hel_cr_2[4],0.0);
    face << QVector3D(p_heliy->hel_cr_2[9],0.0);
    face << QVector3D(p_heliy->hel_cr_2[10],0.0);

    face << QVector3D(p_heliy->hel_cr_2[10]+(p_heliy->hel_cr_2[0]-p_heliy->hel_cr_2[10])/2.0-(p_heliy->hel_cr_2[1]-p_heliy->hel_cr_2[10])/6.0,0.0);
    face << QVector3D(p_heliy->hel_cr_2[0],0.0);
    face << QVector3D(p_heliy->hel_cr_2[1]+(p_heliy->hel_cr_2[0]-p_heliy->hel_cr_2[1])/2.0-(p_heliy->hel_cr_2[10]-p_heliy->hel_cr_2[1])/6.0,0.0);

    derriere = extrude(face,p_heliy->hel_epaisseurCroix);

    double dec_x, dec_y;
    dec_x = (p_heliy->phel_cr_distanceEntre2B)/2.0;
    dec_y = (p_heliy->hel_retombeeCroix)/2.0;

    for(int i=0;i<18;i++)
    {
        face[i].setX(face[i].x()-dec_x);
        face[i].setY(face[i].y()-dec_y);
        face[i].setZ(face[i].z()+p_heliy->hel_epaisseurCroix/2.0);
        derriere[i].setX(derriere[i].x()-dec_x);
        derriere[i].setY(derriere[i].y()-dec_y);
        derriere[i].setZ(derriere[i].z()+p_heliy->hel_epaisseurCroix/2.0);
    }


    Patch *derrierepart = new Patch(g);
    derrierepart->addQuad(derriere[10], derriere[11], derriere[1], derriere[2], true,false,true,false);
    derrierepart->addQuad(derriere[11], derriere[12], derriere[0], derriere[1], true,false,true,false);
    derrierepart->addQuad(derriere[12], derriere[13], derriere[14], derriere[0], true,true,false,false);
    derrierepart->addQuad(derriere[14], derriere[15], derriere[16], derriere[0], true,true,false,false);
    derrierepart->addTri(derriere[0], derriere[16], derriere[17],QVector3D(0.0,0.0,0.0), false,true,true);
    derrierepart->addQuad(derriere[9], derriere[10], derriere[2], derriere[3], true,false,true,false);
    derrierepart->addQuad(derriere[3], derriere[4], derriere[5], derriere[9], true,true,false,false);
    derrierepart->addQuad(derriere[5], derriere[6], derriere[7], derriere[9], true,true,false,false);
    derrierepart->addTri(derriere[9], derriere[7], derriere[8],QVector3D(0.0,0.0,0.0), false,true,true);

    Patch *facepart = new Patch(g);
    facepart->addQuad(face[11], face[10], face[2], face[1], true,false,true,false);
    facepart->addQuad(face[12], face[11], face[1], face[0], true,false,true,false);
    facepart->addQuad(face[14], face[13], face[12], face[0], true,true,false,false);
    facepart->addQuad(face[16], face[15], face[14], face[0], true,true,false,false);
    facepart->addTri(face[16], face[0], face[17],QVector3D(0.0,0.0,0.0), false,true,true);
    facepart->addQuad(face[10], face[9], face[3], face[2], true,false,true,false);
    facepart->addQuad(face[5], face[4], face[3], face[9], true,true,false,false);
    facepart->addQuad(face[7], face[6], face[5], face[9], true,true,false,false);
    facepart->addTri(face[7], face[9], face[8],QVector3D(0.0,0.0,0.0), false,true,true);

    Patch *tour = new Patch(g);
    tour->setSmoothing(Patch::Faceted);
    for(int i=0;i<17;i++)
    {
        tour->addQuad(derriere[i],face[i],face[i+1],derriere[i+1],true,true,true,true);
    }
    tour->addQuad(derriere[17],face[17],face[0],derriere[0],true,true,true,true);

    parts << facepart << derrierepart << tour;

}

//pour le geodome
RectTrianglePlan::RectTrianglePlan(Geometry *g, QVector3D a, QVector3D b, QVector3D c)
{
    //on cree un nouveau patch
    Patch *fb = new Patch(g);
    //on lui dit qu'il sera de type faceted
    fb->setSmoothing(Patch::Faceted);
    fb->addTri(a, b, c,QVector3D(0.0,0.0,0.0),true,true,true);
    parts << fb;
}

RectArete::RectArete(Geometry *g, QVector3D a, QVector3D b)
{
    int fin = 3;
    bool test = false;
    double lPointe = 0.15;
    double rayonCercle = 0.05;
    QVector3D coteA[3];
    QVector3D coteB[3];
    for(int i=0;i<fin;i++)
    {
        coteA[i] = QVector3D(lPointe,rayonCercle*qCos(2*pi*i/fin),rayonCercle*qSin(2*pi*i/fin));
        coteB[i] = QVector3D((b-a).length()-lPointe,rayonCercle*qCos(2*pi*i/fin),rayonCercle*qSin(2*pi*i/fin));
    }
    QMatrix4x4 final,initial,rotation;
    QVector3D losX,losY;
    losX = b-a;
    losY = QVector3D(-losX.y()/losX.x(),1.0,0.0);
    if((qAbs(losX.x())<0.001)&&(qAbs(losX.z())>0.001))
        losY = QVector3D(0.0,1.0,-losX.y()/losX.z());
    else if((qAbs(losX.x())<0.001)&&(qAbs(losX.z())<0.001))
        losY = QVector3D(1.0,0.0,0.0);
    if((qAbs(QVector3D::crossProduct(losX,losY).normalized().x())<0.0001)&&(qAbs(QVector3D::crossProduct(losX,losY).normalized().y())<0.0001)&&(qAbs(QVector3D::crossProduct(losX,losY).normalized().z()+1.0)<0.0001))
    {
        if(qAbs(losX.x())>=0.001)
            losY = QVector3D((-losX.y()-losX.z())/losX.x(),1.0,1.0);
        if((qAbs(losX.x())<0.001)&&(qAbs(losX.z())>0.001))
            losY = QVector3D(0.0,-1.0,losX.y()/losX.z());
        else if((qAbs(losX.x())<0.001)&&(qAbs(losX.z())<0.001))
            losY = QVector3D(-1.0,0.0,0.0);
    }
    if((qAbs(losX.normalized().x()+1.0)<0.001)&&(qAbs(losX.normalized().y())<0.0001)&&(qAbs(losX.normalized().z())<0.0001))
    {
        test = true;
        for(int i=0;i<fin;i++)
        {
            double tamp;
            tamp = coteA[i].x();
            coteA[i].setX(-coteA[i].z());
            coteA[i].setZ(tamp);
            tamp = coteB[i].x();
            coteB[i].setX(-coteB[i].z());
            coteB[i].setZ(tamp);
        }
    }

    //le quaternion de rotation
    final(0,0) = losX.normalized().x();
    final(1,0) = losX.normalized().y();
    final(2,0) = losX.normalized().z();
    final(3,0) = 0;
    final(0,1) = losY.normalized().x();
    final(1,1) = losY.normalized().y();
    final(2,1) = losY.normalized().z();
    final(3,1) = 0;
    final(0,2) = QVector3D::crossProduct(losX,losY).normalized().x();
    final(1,2) = QVector3D::crossProduct(losX,losY).normalized().y();
    final(2,2) = QVector3D::crossProduct(losX,losY).normalized().z();
    final(3,2) = 0;
    final(0,3) = 0;
    final(1,3) = 0;
    final(2,3) = 0;
    final(3,3) = 1;
    initial(0,1) = 0;
    initial(1,1) = 1;
    initial(2,1) = 0;
    initial(3,1) = 0;
    if(test)
    {
        initial(0,0) = 0;
        initial(1,0) = 0;
        initial(2,0) = 1;
        initial(3,0) = 0;
        initial(0,2) = -1;
        initial(1,2) = 0;
        initial(2,2) = 0;
        initial(3,2) = 0;
    }
    else
    {
        initial(0,0) = 1;
        initial(1,0) = 0;
        initial(2,0) = 0;
        initial(3,0) = 0;
        initial(0,2) = 0;
        initial(1,2) = 0;
        initial(2,2) = 1;
        initial(3,2) = 0;
    }
    initial(0,3) = 0;
    initial(1,3) = 0;
    initial(2,3) = 0;
    initial(3,3) = 1;
    rotation = (final*(initial.inverted()));
    QQuaternion quatRot = convertirMatriceRotation(rotation);
    for(int i=0;i<fin;i++)
    {
        coteA[i] = rotationSelonQuaternion(coteA[i],quatRot)+a;
        coteB[i] = rotationSelonQuaternion(coteB[i],quatRot)+a;
    }
    //on cree un nouveau patch
    Patch *fb = new Patch(g);
    //on lui dit qu'il sera de type faceted
    fb->setSmoothing(Patch::Faceted);
    for(int i=0;i<fin-1;i++)
    {
        fb->addTri(a, coteA[i+1], coteA[i],QVector3D(0.0,0.0,0.0),true,true,true);
        fb->addTri(b, coteB[i], coteB[i+1],QVector3D(0.0,0.0,0.0),true,true,true);
        fb->addQuad(coteA[i], coteA[i+1], coteB[i+1], coteB[i],true,true,true,true);
    }
    fb->addTri(a, coteA[0], coteA[fin-1],QVector3D(0.0,0.0,0.0),true,true,true);
    fb->addTri(b, coteB[fin-1], coteB[0],QVector3D(0.0,0.0,0.0),true,true,true);
    fb->addQuad(coteA[fin-1], coteA[0], coteB[0], coteB[fin-1],true,true,true,true);
    parts << fb;
}

RectSommet::RectSommet(Geometry *g, QVector3D a)
{
    double l = 0.1;
    QVector3D p[6];
    p[0] = QVector3D(a.x()+l,a.y(),a.z());
    p[1] = QVector3D(a.x()-l,a.y(),a.z());
    p[2] = QVector3D(a.x(),a.y()+l,a.z());
    p[3] = QVector3D(a.x(),a.y(),a.z()+l);
    p[4] = QVector3D(a.x(),a.y()-l,a.z());
    p[5] = QVector3D(a.x(),a.y(),a.z()-l);

    //on cree un nouveau patch
    Patch *fb = new Patch(g);
    //on lui dit qu'il sera de type faceted
    fb->setSmoothing(Patch::Faceted);
    fb->addTri(p[0], p[2], p[3],QVector3D(0.0,0.0,0.0),true,true,true);
    fb->addTri(p[0], p[3], p[4],QVector3D(0.0,0.0,0.0),true,true,true);
    fb->addTri(p[0], p[4], p[5],QVector3D(0.0,0.0,0.0),true,true,true);
    fb->addTri(p[0], p[5], p[2],QVector3D(0.0,0.0,0.0),true,true,true);
    fb->addTri(p[1], p[4], p[3],QVector3D(0.0,0.0,0.0),true,true,true);
    fb->addTri(p[1], p[3], p[2],QVector3D(0.0,0.0,0.0),true,true,true);
    fb->addTri(p[1], p[2], p[5],QVector3D(0.0,0.0,0.0),true,true,true);
    fb->addTri(p[1], p[5], p[4],QVector3D(0.0,0.0,0.0),true,true,true);
    parts << fb;

}
