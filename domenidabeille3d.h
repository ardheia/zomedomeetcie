#ifndef DOMENIDABEILLE3D_H
#define DOMENIDABEILLE3D_H

#include <QObject>
#include <QColor>

#include "domenidabeille.h"

class Patch;
struct Geometry;

class domenidabeille3d : public QObject
{
public:
    domenidabeille3d(QObject *parent, domenidabeille *pp_dome, bool p_couleursDistinctes, int p_choix, int p_choix2 = 0, qreal scale = 1.0);
    ~domenidabeille3d();
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
    domenidabeille *m_domena;
    QList<Patch *> parts;
    QList<Patch *> partssim;
    Geometry *geom;
    Geometry *geomsim;
    QVector<int> objets,objetssim;
    QVector<int> couleurs,couleurssim;
};

#endif // DOMENIDABEILLE3D_H
