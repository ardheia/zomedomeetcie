#ifndef HELIYOURTE3D_H
#define HELIYOURTE3D_H

#include <QObject>
#include <QColor>

#include "heliyourte.h"

class Patch;
struct Geometry;

class heliyourte3d : public QObject
{
public:
    heliyourte3d(QObject *parent, heliyourte *pp_heliyourte, bool p_couleursDistinctes, int p_choix, int p_choix2 = 0, qreal scale = 1.0);
    ~heliyourte3d();
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
    heliyourte *m_heliyourte;
    QList<Patch *> parts;
    QList<Patch *> partssim;
    Geometry *geom;
    Geometry *geomsim;
    QVector<int> objets;
    QVector<int> couleurs;

};

#endif // HELIYOURTE3D_H
