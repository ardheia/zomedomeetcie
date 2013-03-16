#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <qmath.h>

#include "voutenidabeille3d.h"
#include "objet3d.h"
#include "mathema.h"
#include "rectoid.h"
#include "parametres.h"

//ca c'est notre constructeur de classe avec creation d'une variable structuree
vouteNidAbeille3D::vouteNidAbeille3D(QObject *parent, vouteNidAbeille *pp_voute, bool p_couleursDistinctes, int p_choix, int p_choix2, qreal scale)
    : QObject(parent)
    , geom(new Geometry())
    , geomsim(new Geometry())
{
    m_voute = pp_voute;
    m_choix = p_choix;
    m_choix2 = p_choix2;
    couleurDistinct = p_couleursDistinctes;

    coulGeneral = couleur(0);
    coulTour = couleur(0);
    coulChevronDroit = couleur(1);
    coulChevronGauche = couleur(2);
    coulChevronCoupe.resize(16);
    for(int i=0;i<16;i++)
        coulChevronCoupe[i] = couleur(i+3);

    buildGeometry(scale);

}

// et le destructeur
vouteNidAbeille3D::~vouteNidAbeille3D()
{
    qDeleteAll(parts);
    qDeleteAll(partssim);
    //delete m_voute;
    delete geom;
    delete geomsim;
}

// la on applique la couleur a toutes les parties de l'objet
// on remarquera comme la fonction setColor est a tous les niveaux
void vouteNidAbeille3D::setColor(QColor c)
{
    for (int i = 0; i < parts.count(); ++i)
        qSetColor(parts[i]->faceColor, c);
    for (int i = 0; i < partssim.count(); ++i)
        qSetColor(partssim[i]->faceColor, c);

}

//cherche le point le plus eloigne du centre pour definir taille affichage
double vouteNidAbeille3D::tailleAffichage()
{
    double result(0.0);
    for (int i = 0; i < geom->vertices.size(); ++i)
    {
        if(geom->vertices[i].length()>result)
        {
            result = geom->vertices[i].length();
        }
    }
    result = result * 1.1;
    if(m_choix==0)
    {
        result = m_voute->pointPlusEloigne()*1.1;
    }
    return result;
}

//! [3]
// la fonction qui cree l'objet proprement dit
void vouteNidAbeille3D::buildGeometry(qreal scale)
{
    QVector<QVector3D> passage2, passage3;
    QMatrix4x4 initial, final, rotationChevronDroite, rotationChevronGauche;
    QQuaternion quatChevronGauche, quatChevronDroite, quatSelonZ, quatResultat, quatSelonY, quatdemiSelonY;
    QVector3D vecteurRotation, vecteurNormalChevron, vecteurLongueur1, vecteurLongueur2, vecteurTranslation, vecteurX;
    //double angleRotation;
    //int nombreCases;

    //calcul des quaternions
    m_voute->valeursVoute(passage2);
    vecteurNormalChevron.setX(0);vecteurNormalChevron.setY(0);vecteurNormalChevron.setZ(1);

    //on definit le quaternion de symetrie centrale
    quatSelonY.setVector(0.0,1.0*qSin(pi/2.0),0.0);quatSelonY.setScalar(qCos(pi/2.0));
    quatSelonY.normalize();
    quatdemiSelonY.setVector(0.0,1.0*qSin(-pi/4.0),0.0);quatdemiSelonY.setScalar(qCos(-pi/4.0));
    quatdemiSelonY.normalize();

    // d'abord on cherche quelle est la matrice de rotation de base pour les chevrons penche a droite
    final(0,0) = passage2[1].normalized().x();
    final(1,0) = passage2[1].normalized().y();
    final(2,0) = passage2[1].normalized().z();
    final(3,0) = 0;
    final(0,1) = passage2[0].normalized().x();
    final(1,1) = passage2[0].normalized().y();
    final(2,1) = passage2[0].normalized().z();
    final(3,1) = 0;
    final(0,2) = QVector3D::crossProduct(passage2[1],passage2[0]).normalized().x();
    final(1,2) = QVector3D::crossProduct(passage2[1],passage2[0]).normalized().y();
    final(2,2) = QVector3D::crossProduct(passage2[1],passage2[0]).normalized().z();
    final(3,2) = 0;
    final(0,3) = 0;
    final(1,3) = 0;
    final(2,3) = 0;
    final(3,3) = 1;
    vecteurX = (passage2[11]-passage2[12]).normalized();
    QVector3D vecteurNormalInitial(0,0,1);
    initial(0,0) = vecteurX.x();
    initial(1,0) = vecteurX.y();
    initial(2,0) = vecteurX.z();
    initial(3,0) = 0;
    initial(0,1) = vecteurNormalInitial.normalized().x();
    initial(1,1) = vecteurNormalInitial.normalized().y();
    initial(2,1) = vecteurNormalInitial.normalized().z();
    initial(3,1) = 0;
    initial(0,2) = QVector3D::crossProduct(vecteurX,vecteurNormalInitial).normalized().x();
    initial(1,2) = QVector3D::crossProduct(vecteurX,vecteurNormalInitial).normalized().y();
    initial(2,2) = QVector3D::crossProduct(vecteurX,vecteurNormalInitial).normalized().z();
    initial(3,2) = 0;
    initial(0,3) = 0;
    initial(1,3) = 0;
    initial(2,3) = 0;
    initial(3,3) = 1;
    rotationChevronDroite = (final*(initial.inverted()));
    quatChevronDroite = convertirMatriceRotation(rotationChevronDroite);
    quatChevronDroite.normalize();
    //et pour les chevrons penches a gauche
    final(2,0) = -final(2,0);final(2,1) = -final(2,1);
    final(0,2) = -final(0,2);final(1,2) = -final(1,2);
    //initial(0,0) = -initial(0,0);
    //vecteurX.setX(-vecteurX.x());
    initial(2,1) = -initial(2,1);
    vecteurNormalInitial.setZ(-vecteurNormalInitial.z());
    initial(0,2) = QVector3D::crossProduct(vecteurX,vecteurNormalInitial).normalized().x();
    initial(1,2) = QVector3D::crossProduct(vecteurX,vecteurNormalInitial).normalized().y();
    initial(2,2) = QVector3D::crossProduct(vecteurX,vecteurNormalInitial).normalized().z();
    rotationChevronGauche = (final*(initial.inverted()));
    quatChevronGauche = convertirMatriceRotation(rotationChevronGauche);
    quatChevronGauche.normalize();

    //vue d'ensemble
    if(m_choix==0)
    {

            QVector<RectChevron *> chevron;
            QVector<RectChevron *> chevronsim;
            QVector<RectChevronTronque *> chevronTronq;
            QVector<RectChevronTronque *> chevronTronqsim;

            // on cree les sablieres
            RectPrism sabliere(geom,passage2[4].z(),passage2[4].y(),passage2[4].x());
            vecteurTranslation.setZ(passage2[4].x()/2.0);vecteurTranslation.setY(0);vecteurTranslation.setX(passage2[5].x()/2.0-passage2[4].z());
            sabliere.translate(vecteurTranslation);
            sabliere.translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));
            RectPrism sabliere2(geomsim,passage2[4].z(),passage2[4].y(),passage2[4].x());
            vecteurTranslation.setZ(passage2[4].x()/2.0);vecteurTranslation.setY(0);vecteurTranslation.setX(-passage2[5].x()/2.0);
            sabliere2.translate(vecteurTranslation);
            sabliere2.translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));

            // on cree la demi-faitiere
            RectPrism faitiere(geom,passage2[6].x()*2.0,passage2[6].y(),passage2[4].x());
            vecteurTranslation.setZ(passage2[4].x()/2.0);vecteurTranslation.setX(-passage2[6].x());vecteurTranslation.setY(passage2[6].z()-passage2[6].y());
            faitiere.translate(vecteurTranslation);
            faitiere.translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));

            //on cree les 2 rives
            RectRive rive1(geom,m_voute);
            vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(passage2[4].x()/2.0);
            rive1.translate(vecteurTranslation);
            rive1.translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));
            RectRive rive2(geom,m_voute);
            vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(-passage2[4].x()/2.0+passage2[5].y());
            rive2.translate(vecteurTranslation);
            rive2.translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));
            RectRive rive3(geomsim,m_voute);
            vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(passage2[4].x()/2.0);
            rive3.rotate(quatSelonY);
            rive3.translate(vecteurTranslation);
            rive3.translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));
            RectRive rive4(geomsim,m_voute);
            vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(-passage2[4].x()/2.0+passage2[5].y());
            rive4.rotate(quatSelonY);
            rive4.translate(vecteurTranslation);
            rive4.translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));


            if(couleurDistinct)
            {
                sabliere.setcolor(coulTour);
                sabliere2.setcolor(coulTour);
                faitiere.setcolor(coulTour);
                rive1.setcolor(coulTour);
                rive2.setcolor(coulTour);
                rive3.setcolor(coulTour);
                rive4.setcolor(coulTour);
            }
            else
            {
                sabliere.setcolor(coulGeneral);
                sabliere2.setcolor(coulGeneral);
                faitiere.setcolor(coulGeneral);
                rive1.setcolor(coulGeneral);
                rive2.setcolor(coulGeneral);
                rive3.setcolor(coulGeneral);
                rive4.setcolor(coulGeneral);
            }

            //on met ou pas la faitiere
            if((passage2[8].x()==0)||(passage2[8].x()==2))
            {
                parts << sabliere.parts << faitiere.parts << rive1.parts << rive2.parts;
                partssim << rive3.parts << rive4.parts << sabliere2.parts;
                objets << 2 << 2 << 6 << 6;
                objetssim << 6 << 6 << 2;
                couleurs << 0 << 0 << 0 << 0;
                couleurssim << 0 << 0 << 0;
            }
            else
            {
                parts << sabliere.parts << rive1.parts << rive2.parts;
                partssim << rive3.parts << rive4.parts << sabliere2.parts;
                objets << 2 << 6 << 6;
                objetssim << 6 << 6 << 2;
                couleurs << 0 << 0 << 0;
                couleurssim << 0 << 0 << 0;
            }

            //et les chevrons
            m_voute->valeursVoute2(passage3);

            //on place les chevrons penches a droite
            for (int f = 0; f < passage3[2].y(); ++f)
            {
                for (int i = 0; i < passage3[0].y(); ++i)
                {
                    RectChevron *chev;
                    chev = new RectChevron(geom,m_voute,0);
                    chevron << chev;

                    vecteurTranslation.setX(passage2[9].x()+passage2[10].x()*qCos(passage2[10].z()+(f+passage3[2].x())*passage2[10].y()));vecteurTranslation.setY(passage2[9].y()+passage2[10].x()*qSin(passage2[10].z()+(f+passage3[2].x())*passage2[10].y()));vecteurTranslation.setZ(passage2[7].z());
                    chevron[i+f*passage3[0].y()]->translate(vecteurTranslation);
                    vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(-passage2[3].z());
                    chevron[i+f*passage3[0].y()]->translate((i+passage3[0].x())*vecteurTranslation);
                    chevron[i+f*passage3[0].y()]->translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));
                    quatSelonZ.setVector(0.0,0.0,1.0*qSin((f+passage3[2].x())*passage2[10].y()/2.0));quatSelonZ.setScalar(qCos((f+passage3[2].x())*passage2[10].y()/2.0));
                    quatSelonZ.normalize();

                    quatResultat = quatSelonZ*quatChevronDroite;
                    chevron[i+f*passage3[0].y()]->rotate(quatResultat);
                    objets << 6;

                    if(couleurDistinct)
                    {
                        chevron[i+f*passage3[0].y()]->setcolor(coulChevronDroit);
                        couleurs << 1;
                    }
                    else
                    {
                        chevron[i+f*passage3[0].y()]->setcolor(coulGeneral);
                        couleurs << 0;
                    }
                    parts << chevron[i+f*passage3[0].y()]->parts;

                    if((passage2[8].x()==0)||(passage2[8].x()==2))
                    {
                        RectChevron *chevsim;
                        chevsim = new RectChevron(geomsim,m_voute,0);
                        chevronsim << chevsim;

                        vecteurTranslation.setX(-(passage2[9].x()+passage2[10].x()*qCos(passage2[10].z()+(f+passage3[2].x())*passage2[10].y())));vecteurTranslation.setY(passage2[9].y()+passage2[10].x()*qSin(passage2[10].z()+(f+passage3[2].x())*passage2[10].y()));vecteurTranslation.setZ(-passage2[7].z());
                        chevronsim[i+f*passage3[0].y()]->translate(vecteurTranslation);
                        vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(passage2[3].z());
                        chevronsim[i+f*passage3[0].y()]->translate((i+passage3[0].x())*vecteurTranslation);
                        chevronsim[i+f*passage3[0].y()]->translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));

                        quatSelonZ.setVector(0.0,0.0,1.0*qSin((f+passage3[2].x())*passage2[10].y()/2.0));quatSelonZ.setScalar(qCos((f+passage3[2].x())*passage2[10].y()/2.0));
                        quatSelonZ.normalize();

                        quatResultat = quatSelonY*quatSelonZ*quatChevronDroite;
                        chevronsim[i+f*passage3[0].y()]->rotate(quatResultat);
                        objetssim << 6;

                        if(couleurDistinct)
                        {
                            chevronsim[i+f*passage3[0].y()]->setcolor(coulChevronDroit);
                            couleurssim << 1;
                        }
                        else
                        {
                            chevronsim[i+f*passage3[0].y()]->setcolor(coulGeneral);
                            couleurssim << 0;
                        }

                        partssim << chevronsim[i+f*passage3[0].y()]->parts;
                    }
                }
            }
            double compteur;
            compteur = chevron.count();
            //puis les chevrons penches a gauche
            for (int f = 0; f < passage3[3].y(); ++f)
            {
                for (int i = 0; i < passage3[1].y(); ++i)
                {
                    RectChevron *chev;
                    chev = new RectChevron(geom,m_voute,1);
                    chevron << chev;

                    vecteurTranslation.setX(passage2[9].x()+passage2[10].x()*qCos(passage2[10].z()+passage2[8].y()+(f+passage3[3].x())*passage2[10].y()));vecteurTranslation.setY(passage2[9].y()+passage2[10].x()*qSin(passage2[10].z()+passage2[8].y()+(f+passage3[3].x())*passage2[10].y()));vecteurTranslation.setZ(passage2[7].z()-passage2[9].z());
                    chevron[compteur+i+f*passage3[1].y()]->translate(vecteurTranslation);
                    vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(-passage2[3].z());
                    chevron[compteur+i+f*passage3[1].y()]->translate((i+passage3[1].x())*vecteurTranslation);
                    chevron[compteur+i+f*passage3[1].y()]->translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));

                    quatSelonZ.setVector(0.0,0.0,1.0*qSin((passage2[8].y()+(f+passage3[3].x())*passage2[10].y())/2.0));quatSelonZ.setScalar(qCos((passage2[8].y()+(f+passage3[3].x())*passage2[10].y())/2.0));
                    quatSelonZ.normalize();

                    quatResultat = quatSelonZ*quatChevronGauche;
                    chevron[compteur+i+f*passage3[1].y()]->rotate(quatResultat);
                    objets << 6;

                    if(couleurDistinct)
                    {
                        chevron[compteur+i+f*passage3[1].y()]->setcolor(coulChevronGauche);
                        couleurs << 2;
                    }
                    else
                    {
                        chevron[compteur+i+f*passage3[1].y()]->setcolor(coulGeneral);
                        couleurs << 0;
                    }

                    parts << chevron[compteur+i+f*passage3[1].y()]->parts;

                    if((passage2[8].x()==0)||(passage2[8].x()==2))
                    {
                        RectChevron *chevsim;
                        chevsim = new RectChevron(geomsim,m_voute,1);
                        chevronsim << chevsim;

                        vecteurTranslation.setX(-(passage2[9].x()+passage2[10].x()*qCos(passage2[10].z()+passage2[8].y()+(f+passage3[3].x())*passage2[10].y())));vecteurTranslation.setY(passage2[9].y()+passage2[10].x()*qSin(passage2[10].z()+passage2[8].y()+(f+passage3[3].x())*passage2[10].y()));vecteurTranslation.setZ(-(passage2[7].z()-passage2[9].z()));
                        chevronsim[compteur+i+f*passage3[1].y()]->translate(vecteurTranslation);
                        vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(passage2[3].z());
                        chevronsim[compteur+i+f*passage3[1].y()]->translate((i+passage3[1].x())*vecteurTranslation);
                        chevronsim[compteur+i+f*passage3[1].y()]->translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));

                        quatSelonZ.setVector(0.0,0.0,1.0*qSin((passage2[8].y()+(f+passage3[3].x())*passage2[10].y())/2.0));quatSelonZ.setScalar(qCos((passage2[8].y()+(f+passage3[3].x())*passage2[10].y())/2.0));
                        quatSelonZ.normalize();

                        quatResultat = quatSelonY*quatSelonZ*quatChevronGauche;
                        chevronsim[compteur+i+f*passage3[1].y()]->rotate(quatResultat);
                        objetssim << 6;

                        if(couleurDistinct)
                        {
                            chevronsim[compteur+i+f*passage3[1].y()]->setcolor(coulChevronGauche);
                            couleurssim << 2;
                        }
                        else
                        {
                            chevronsim[compteur+i+f*passage3[1].y()]->setcolor(coulGeneral);
                            couleurssim << 0;
                        }
                        partssim << chevronsim[compteur+i+f*passage3[1].y()]->parts;
                    }
                }
            }
            // et les chevrons tronques penche a droite
            for(int num = 0;num<(int)passage3[4].y();num++)
            {
                QVector<QVector3D> passage4;
                m_voute->valeursChevronTronque(passage4,num);
                for (int f = (int)passage4[13].x(); f <= (int)passage4[13].y(); ++f)
                {
                    for (int i = (int)passage4[12].x(); i <= (int)passage4[12].y(); ++i)
                    {
                        RectChevronTronque *chev;
                        chev = new RectChevronTronque(geom,m_voute,num);
                        chevronTronq << chev;
                        compteur = chevronTronq.count()-1;

                        vecteurTranslation.setX(passage2[9].x()+passage2[10].x()*qCos(passage2[10].z()+(f)*passage2[10].y()));vecteurTranslation.setY(passage2[9].y()+passage2[10].x()*qSin(passage2[10].z()+(f)*passage2[10].y()));vecteurTranslation.setZ(passage2[7].z());
                        chevronTronq[compteur]->translate(vecteurTranslation);
                        vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(-passage2[3].z());
                        chevronTronq[compteur]->translate((i)*vecteurTranslation);
                        chevronTronq[compteur]->translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));

                        quatSelonZ.setVector(0.0,0.0,1.0*qSin((f)*passage2[10].y()/2.0));quatSelonZ.setScalar(qCos((f)*passage2[10].y()/2.0));
                        quatSelonZ.normalize();

                        quatResultat = quatSelonZ*quatChevronDroite;
                        chevronTronq[compteur]->rotate(quatResultat);
                        objets << 6;

                        if(couleurDistinct)
                        {
                            chevronTronq[compteur]->setcolor(coulChevronCoupe[num]);
                            couleurs << num+3;
                        }
                        else
                        {
                            chevronTronq[compteur]->setcolor(coulGeneral);
                            couleurs << 0;
                        }

                        parts << chevronTronq[compteur]->parts;

                        if((passage2[8].x()==0)||(passage2[8].x()==2))
                        {
                            RectChevronTronque *chevsim;
                            chevsim = new RectChevronTronque(geomsim,m_voute,num);
                            chevronTronqsim << chevsim;
                            compteur = chevronTronqsim.count()-1;

                            vecteurTranslation.setX(-(passage2[9].x()+passage2[10].x()*qCos(passage2[10].z()+(f)*passage2[10].y())));vecteurTranslation.setY(passage2[9].y()+passage2[10].x()*qSin(passage2[10].z()+(f)*passage2[10].y()));vecteurTranslation.setZ(-passage2[7].z());
                            chevronTronqsim[compteur]->translate(vecteurTranslation);
                            vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(passage2[3].z());
                            chevronTronqsim[compteur]->translate((i)*vecteurTranslation);
                            chevronTronqsim[compteur]->translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));

                            quatSelonZ.setVector(0.0,0.0,1.0*qSin((f)*passage2[10].y()/2.0));quatSelonZ.setScalar(qCos((f)*passage2[10].y()/2.0));
                            quatSelonZ.normalize();

                            quatResultat = quatSelonY*quatSelonZ*quatChevronDroite;
                            chevronTronqsim[compteur]->rotate(quatResultat);
                            objetssim << 6;

                            if(couleurDistinct)
                            {
                                chevronTronqsim[compteur]->setcolor(coulChevronCoupe[num]);
                                couleurssim << num+3;
                            }
                            else
                            {
                                chevronTronqsim[compteur]->setcolor(coulGeneral);
                                couleurssim << 0;
                            }

                            partssim << chevronTronqsim[compteur]->parts;
                        }
                    }
                }
            }
            // et les chevrons tronques penche a gauche
            for(int num = (int)passage3[4].y();num<(int)passage3[4].x();num++)
            {
                QVector<QVector3D> passage4;
                m_voute->valeursChevronTronque(passage4,num);
                for (int f = (int)passage4[13].x(); f <= (int)passage4[13].y(); ++f)
                {
                    for (int i = (int)passage4[12].x(); i <= (int)passage4[12].y(); ++i)
                    {
                        RectChevronTronque *chev;
                        chev = new RectChevronTronque(geom,m_voute,num);
                        chevronTronq << chev;
                        compteur = chevronTronq.count()-1;

                        vecteurTranslation.setX(passage2[9].x()+passage2[10].x()*qCos(passage2[10].z()+passage2[8].y()+(f)*passage2[10].y()));vecteurTranslation.setY(passage2[9].y()+passage2[10].x()*qSin(passage2[10].z()+passage2[8].y()+(f)*passage2[10].y()));vecteurTranslation.setZ(passage2[7].z()-passage2[9].z());
                        chevronTronq[compteur]->translate(vecteurTranslation);
                        vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(-passage2[3].z());
                        chevronTronq[compteur]->translate((i+passage3[1].x())*vecteurTranslation);
                        chevronTronq[compteur]->translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));

                        quatSelonZ.setVector(0.0,0.0,1.0*qSin((passage2[8].y()+(f)*passage2[10].y())/2.0));quatSelonZ.setScalar(qCos((passage2[8].y()+(f)*passage2[10].y())/2.0));
                        quatSelonZ.normalize();

                        quatResultat = quatSelonZ*quatChevronGauche;
                        chevronTronq[compteur]->rotate(quatResultat);
                        objets << 6;

                        if(couleurDistinct)
                        {
                            chevronTronq[compteur]->setcolor(coulChevronCoupe[num]);
                            couleurs << num+3;
                        }
                        else
                        {
                            chevronTronq[compteur]->setcolor(coulGeneral);
                            couleurs << 0;
                        }

                        parts << chevronTronq[compteur]->parts;

                        if((passage2[8].x()==0)||(passage2[8].x()==2))
                        {
                            RectChevronTronque *chevsim;
                            chevsim = new RectChevronTronque(geomsim,m_voute,num);
                            chevronTronqsim << chevsim;
                            compteur = chevronTronqsim.count()-1;

                            vecteurTranslation.setX(-(passage2[9].x()+passage2[10].x()*qCos(passage2[10].z()+passage2[8].y()+(f)*passage2[10].y())));vecteurTranslation.setY(passage2[9].y()+passage2[10].x()*qSin(passage2[10].z()+passage2[8].y()+(f)*passage2[10].y()));vecteurTranslation.setZ(-(passage2[7].z()-passage2[9].z()));
                            chevronTronqsim[compteur]->translate(vecteurTranslation);
                            vecteurTranslation.setX(0);vecteurTranslation.setY(0);vecteurTranslation.setZ(passage2[3].z());
                            chevronTronqsim[compteur]->translate((i+passage3[1].x())*vecteurTranslation);
                            chevronTronqsim[compteur]->translate(QVector3D(0.0,-passage2[6].z()/2.0,0.0));

                            quatSelonZ.setVector(0.0,0.0,1.0*qSin((passage2[8].y()+(f)*passage2[10].y())/2.0));quatSelonZ.setScalar(qCos((passage2[8].y()+(f)*passage2[10].y())/2.0));
                            quatSelonZ.normalize();

                            quatResultat = quatSelonY*quatSelonZ*quatChevronGauche;
                            chevronTronqsim[compteur]->rotate(quatResultat);
                            objetssim << 6;

                            if(couleurDistinct)
                            {
                                chevronTronqsim[compteur]->setcolor(coulChevronCoupe[num]);
                                couleurssim << num+3;
                            }
                            else
                            {
                                chevronTronqsim[compteur]->setcolor(coulGeneral);
                                couleurssim << 0;
                            }

                            partssim << chevronTronqsim[compteur]->parts;
                        }
                    }
                }
            }


    }
    //vue chevron gauche
    else if(m_choix==1)
    {
        RectChevron chevron(geom,m_voute,1);
        quatResultat = quatdemiSelonY*quatChevronGauche;
        chevron.rotate(quatResultat);
        objets << 6;
        if(couleurDistinct)
        {
            chevron.setcolor(coulChevronGauche);
            couleurs << 2;
        }
        else
        {
            chevron.setcolor(coulGeneral);
            couleurs << 0;
        }
        //on additionne les parties
        parts << chevron.parts;
    }
    //vue chevron droit
    else if(m_choix==2)
    {
        RectChevron chevron(geom,m_voute,0);
        quatResultat = quatdemiSelonY*quatChevronDroite;
        chevron.rotate(quatResultat);
        objets << 6;
        if(couleurDistinct)
        {
            chevron.setcolor(coulChevronDroit);
            couleurs << 1;
        }
        else
        {
            chevron.setcolor(coulGeneral);
            couleurs << 0;
        }
        //on additionne les parties
        parts << chevron.parts;
    }
    else if(m_choix==3)
    {
        RectChevronTronque chevron(geom,m_voute,m_choix2);
        if(m_voute->orientationChevronTronque(m_choix2)<8)
        {
            quatResultat = quatdemiSelonY*quatChevronDroite;
        }
        else
        {
            quatResultat = quatdemiSelonY*quatChevronGauche;
        }
        chevron.rotate(quatResultat);
        objets << 6;
        if(couleurDistinct)
        {
            chevron.setcolor(coulChevronCoupe[m_choix2]);
            couleurs << m_choix2+3;
        }
        else
        {
            chevron.setcolor(coulGeneral);
            couleurs << 0;
        }
        //on additionne les parties
        parts << chevron.parts;
    }

    //on normalise les vecteurs
    geom->finalize();
    geomsim->finalize();

}
//! [3]

//! [4]
void vouteNidAbeille3D::draw(bool p_filaire) const
{
    //ca c'est la fonction pour dessiner
    geom->loadArrays();

    // dit a opengl qu'on va bosser avec des tableaux de sommets (ou vertex arrays)
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    for (int i = 0; i < parts.count(); ++i)
        parts[i]->draw(p_filaire);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    geomsim->loadArrays();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    for (int i = 0; i < partssim.count(); ++i)
        partssim[i]->draw(p_filaire);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
//! [4]


QString vouteNidAbeille3D::exportObj()
{
    QString str,str2,str3;
    QString resu = "";
    Geometry *geomtamp;
    geomtamp = new Geometry(*geom);
    Geometry *geomsimtamp;
    geomsimtamp = new Geometry(*geomsim);
    int nbSommets, nbObj;

    for(int i=0;i<parts.size();i++)
    {
        for(int debsom = parts[i]->initv;debsom<parts[i]->initv+parts[i]->countv;debsom++)
        {
            geomtamp->vertices[debsom] = parts[i]->mat*geom->vertices[debsom];
        }
    }
    for(int i=0;i<geom->vertices.size();i++)
    {
        resu += "v "+str.setNum(geomtamp->vertices[i].x(),'f',6)+" "+str2.setNum(geomtamp->vertices[i].y(),'f',6)+" "+str3.setNum(geomtamp->vertices[i].z(),'f',6)+"\n";
    }
    for(int i=0;i<geom->vertices.size();i++)
    {
        resu += "vn "+str.setNum(geomtamp->normals[i].x(),'f',6)+" "+str2.setNum(geomtamp->normals[i].y(),'f',6)+" "+str3.setNum(geomtamp->normals[i].z(),'f',6)+"\n";
    }
    int comptparts = 0;
    for(int j=0;j<objets.size();j++)
    {
        resu += "o objet"+str.setNum(j)+"\n";
        resu += "usemtl couleur"+str.setNum(couleurs[j])+"\n";
        for(int i=comptparts;i<comptparts+objets[j];i++)
        {
            int hh = i+1;
            int hh2 = i+1-comptparts;
            resu += "g part"+str.setNum(j)+"_"+str2.setNum(hh2)+"\n";
            if(parts[i]->sm == 1)
            {
                resu += "s "+str.setNum(hh)+"\n";
            }
            for(int debface = parts[i]->start;debface<parts[i]->start+parts[i]->count;debface = debface+3)
            {
                resu += "f "+str.setNum(geom->faces[debface]+1)+"//"+str2.setNum(geom->faces[debface]+1);
                resu += " "+str.setNum(geom->faces[debface+1]+1)+"//"+str2.setNum(geom->faces[debface+1]+1);
                resu += " "+str.setNum(geom->faces[debface+2]+1)+"//"+str2.setNum(geom->faces[debface+2]+1);
                resu += "\n";
            }
        }
        comptparts += objets[j];
    }

    nbSommets = geom->vertices.size();
    nbObj = objets.size();
// et pour la partie sim
    for(int i=0;i<partssim.size();i++)
    {
        for(int debsom = partssim[i]->initv;debsom<partssim[i]->initv+partssim[i]->countv;debsom++)
        {
            geomsimtamp->vertices[debsom] = partssim[i]->mat*geomsim->vertices[debsom];
        }
    }
    for(int i=0;i<geomsim->vertices.size();i++)
    {
        resu += "v "+str.setNum(geomsimtamp->vertices[i].x(),'f',6)+" "+str2.setNum(geomsimtamp->vertices[i].y(),'f',6)+" "+str3.setNum(geomsimtamp->vertices[i].z(),'f',6)+"\n";
    }
    for(int i=0;i<geomsim->vertices.size();i++)
    {
        resu += "vn "+str.setNum(geomsimtamp->normals[i].x(),'f',6)+" "+str2.setNum(geomsimtamp->normals[i].y(),'f',6)+" "+str3.setNum(geomsimtamp->normals[i].z(),'f',6)+"\n";
    }
    int comptpartssim = 0;
    for(int j=0;j<objetssim.size();j++)
    {
        resu += "o objet"+str.setNum(j+nbObj)+"\n";
        resu += "usemtl couleur"+str.setNum(couleurssim[j])+"\n";
        for(int i=comptpartssim;i<comptpartssim+objetssim[j];i++)
        {
            int hh = i+1;
            int hh2 = i+1-comptparts;
            resu += "g part"+str.setNum(j+nbObj)+"_"+str2.setNum(hh2)+"\n";
            if(partssim[i]->sm == 1)
            {
                resu += "s "+str.setNum(hh)+"\n";
            }
            for(int debface = partssim[i]->start;debface<partssim[i]->start+partssim[i]->count;debface = debface+3)
            {
                resu += "f "+str.setNum(geomsim->faces[debface]+1+nbSommets)+"//"+str2.setNum(geomsim->faces[debface]+1+nbSommets);
                resu += " "+str.setNum(geomsim->faces[debface+1]+1+nbSommets)+"//"+str2.setNum(geomsim->faces[debface+1]+1+nbSommets);
                resu += " "+str.setNum(geomsim->faces[debface+2]+1+nbSommets)+"//"+str2.setNum(geomsim->faces[debface+2]+1+nbSommets);
                resu += "\n";
            }
        }
        comptpartssim += objetssim[j];
    }


    return resu;
}
