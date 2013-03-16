#ifndef GEODOME3D_H
#define GEODOME3D_H

#include <QObject>
#include <QColor>

#include "geodome.h"

class Patch;
struct Geometry;

class geodome3D : public QObject
{
public:
    geodome3D(QObject *parent, geodome *pp_geodome, bool p_couleursDistinctes, int p_choix, int p_choix2 = 0, qreal scale = 1.0);
    ~geodome3D();
    void setColor(QColor c);
    void draw(bool p_filaire) const;
    QString exportObj();
    double tailleAffichage();

private:
    void buildGeometry(qreal s);

    bool couleurDistinct;
    int m_choix, m_choix2;
    QColor coulGeneral;
    QVector<QColor> coulLosange;
    geodome *m_geodome;
    QList<Patch *> parts;
    QList<Patch *> partssim2;
    Geometry *geom;
    Geometry *geomsim2;
    QVector<int> objets;//, objetssim2;
    QVector<int> couleurs;//, couleurssim2;

};

#endif // GEODOME3D_H
