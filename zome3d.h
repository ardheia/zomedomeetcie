#ifndef ZOME3D_H
#define ZOME3D_H

#include <QObject>
#include <QColor>

#include "zome.h"

class Patch;
struct Geometry;

class zome3D : public QObject
{
public:
    zome3D(QObject *parent, zome *pp_zome, bool p_couleursDistinctes, int p_choix, int p_choix2 = 0, qreal scale = 1.0);
    ~zome3D();
    void setColor(QColor c);
    void draw(bool p_filaire) const;
    double tailleAffichage();
    QString exportObj();

private:
    void buildGeometry(qreal s);

    bool couleurDistinct;
    int m_choix, m_choix2;
    QColor coulGeneral;
    QVector<QColor> coulLosange;
    zome *m_zome;
    QList<Patch *> parts;
    QList<Patch *> partssim;
    Geometry *geom;
    Geometry *geomsim;
    QVector<int> objets,objetssim;
    QVector<int> couleurs,couleurssim;

};

#endif // ZOME3D_H
