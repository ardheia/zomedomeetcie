#ifndef VOUTENIDABEILLE3D_H
#define VOUTENIDABEILLE3D_H

#include <QObject>
#include <QColor>

#include "voutenidabeille.h"

/*
    là c'est le dessin 3d de notre structure
*/

class Patch;
struct Geometry;

//! [0]
class vouteNidAbeille3D : public QObject
{
public:
    vouteNidAbeille3D(QObject *parent, vouteNidAbeille *pp_voute, bool p_couleursDistinctes, int p_choix, int p_choix2 = 0, qreal scale = 1.0);
    ~vouteNidAbeille3D();
    void setColor(QColor c);
    void draw(bool p_filaire) const;
    double tailleAffichage();
    QString exportObj();

private:
    void buildGeometry(qreal s);

    bool couleurDistinct;
    int m_choix, m_choix2;
    QColor coulGeneral, coulTour, coulChevronDroit, coulChevronGauche;
    QVector<QColor> coulChevronCoupe;
    vouteNidAbeille *m_voute;
    QList<Patch *> parts;
    QList<Patch *> partssim;
    Geometry *geom;
    Geometry *geomsim;
    QVector<int> objets,objetssim;
    QVector<int> couleurs,couleurssim;
};
//! [0]



#endif // QTLOGO_H
