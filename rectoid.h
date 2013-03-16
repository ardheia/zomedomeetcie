#ifndef RECTOID_H
#define RECTOID_H

#include "domenidabeille.h"
#include "voutenidabeille.h"
#include "heliyourte.h"
#include <QVector3D>

/*
Alors la c est ici qu on va definir nos sous objets 3D
genre une rive courbe un chevron courbe un parallepipede rectangle
on peut en rajouter comme on veut
*/

class Rectoid
{
public://ici on fait que redescendre les fonctions translate et rotation
    void translate(const QVector3D &t);
    void rotate(qreal deg, QVector3D axis);
    void rotate(QQuaternion quater);
    void setcolor(QColor c);

    // No special Rectoid destructor - the parts are fetched out of this member
    // variable  and destroyed by the new owner
    QList<Patch*> parts;
};

// ca y est ici on dessine enfin ce qui nous interesse  a la limite jusque ici on s en foutait des fonctions
class RectPrism : public Rectoid
{
public:
    RectPrism(Geometry *g, qreal width, qreal height, qreal depth);
};

class RectLosangePlan : public Rectoid
{
public:
    RectLosangePlan(Geometry *g, QVector3D a, QVector3D b, QVector3D c, QVector3D d);
};

class RectLosangeMethode1 : public Rectoid
{
public:
    RectLosangeMethode1(Geometry *g, double diagVert, double diagHoriz, double retombee, double diagVertInt, double diagHorizInt);
};

class RectTriangleBasMethode1 : public Rectoid
{
public:
    RectTriangleBasMethode1(Geometry *g, double diagVert, double diagHoriz, double retombee, double diagVertInt, double diagHorizInt, double epaisseur);
};

class RectTorus : public Rectoid
{
public:
    RectTorus(Geometry *g, qreal iRad, qreal oRad, qreal depth);
};

class RectChevron : public Rectoid
{
public:
    RectChevron(Geometry *g, vouteNidAbeille *p_voute, int numero);
    RectChevron(Geometry *g, chevronDNA che);
private:
};

class RectChevronTronque : public Rectoid
{
public:
    RectChevronTronque(Geometry *g, vouteNidAbeille *p_voute, int numero);
private:
};


class RectRive : public Rectoid
{
public:
    RectRive(Geometry *g, vouteNidAbeille *p_voute);
};

// pour l heliyourte

class RectPerche : public Rectoid
{
public:
    RectPerche(Geometry *g, heliyourte *p_heliy);
};

class RectCroix : public Rectoid
{
public:
    RectCroix(Geometry *g, heliyourte *p_heliy);
};

class RectMontantA : public Rectoid
{
public:
    RectMontantA(Geometry *g, heliyourte *p_heliy);
};

class RectMontantB : public Rectoid
{
public:
    RectMontantB(Geometry *g, heliyourte *p_heliy);
};

class RectMontantC : public Rectoid
{
public:
    RectMontantC(Geometry *g, heliyourte *p_heliy);
};

class RectTraverse : public Rectoid
{
public:
    RectTraverse(Geometry *g, heliyourte *p_heliy);
};

//pour le geodome
class RectTrianglePlan : public Rectoid
{
public:
    RectTrianglePlan(Geometry *g, QVector3D a, QVector3D b, QVector3D c);
};

class RectArete : public Rectoid
{
public:
    RectArete(Geometry *g, QVector3D a, QVector3D b);
};

class RectSommet : public Rectoid
{
public:
    RectSommet(Geometry *g, QVector3D a);
};

#endif // RECTOID_H
