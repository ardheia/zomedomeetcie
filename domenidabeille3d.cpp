#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <qmath.h>

#include "objet3d.h"
#include "mathema.h"
#include "rectoid.h"
#include "parametres.h"
#include "domenidabeille3d.h"

//ca c'est notre constructeur de classe avec creation d'une variable structure
domenidabeille3d::domenidabeille3d(QObject *parent, domenidabeille *pp_dome, bool p_couleursDistinctes, int p_choix, int p_choix2, qreal scale)
    : QObject(parent)
    , geom(new Geometry())
    , geomsim(new Geometry())
{
    m_domena = pp_dome;
    m_choix = p_choix;
    m_choix2 = p_choix2;
    couleurDistinct = p_couleursDistinctes;

    coulGeneral = couleur(0);
    coulTour = couleur(0);
    coulChevronDroit = couleur(1);
    coulChevronGauche = couleur(2);
    coulChevronCoupe.resize(2);
    for(int i=0;i<2;i++)
        coulChevronCoupe[i] = couleur(i+3);

    buildGeometry(scale);

}

// et le destructeur
domenidabeille3d::~domenidabeille3d()
{
    qDeleteAll(parts);
    qDeleteAll(partssim);
    delete geom;
    delete geomsim;
}

// la on applique la couleur a toutes les parties de l'objet
// on remarquera comme la fonction setColor est a tous les niveaux
void domenidabeille3d::setColor(QColor c)
{
    for (int i = 0; i < parts.count(); ++i)
        qSetColor(parts[i]->faceColor, c);
    for (int i = 0; i < partssim.count(); ++i)
        qSetColor(partssim[i]->faceColor, c);
}

//cherche le point le plus eloigne du centre pour definir taille affichage
double domenidabeille3d::tailleAffichage()
{
    double result(0.0);
    for (int i = 0; i < geom->vertices.size(); ++i)
    {
        if(geom->vertices[i].length()>result)
        {
            result = geom->vertices[i].length();
        }
    }
    result = result * 0.55;
    if(m_choix==0)
    {
        result = m_domena->pointPlusEloigne()*1.1;
    }
    return result;
}

//! [3]
// la fonction qui cree l objet proprement dit
void domenidabeille3d::buildGeometry(qreal scale)
{
    QVector<QVector3D> passage2;
    QQuaternion quatSelonZ, quatdemiSelonX, quatdemiSelonY;
    QVector3D vecteurTranslation;
    QVector<chevronDNA> tabChevron;

    m_domena->valeursDomeGenerales(passage2);

    //et les chevrons
    m_domena->valeursChevrons(tabChevron);

    quatdemiSelonY.setVector(0.0,1.0*qSin(-pi/4.0),0.0);quatdemiSelonY.setScalar(qCos(-pi/4.0));
    quatdemiSelonY.normalize();

    //vue d'ensemble
    if(m_choix==0)
    {

            //sabliere
            RectTorus sabliere(geom,passage2[0].z()/2.0,passage2[0].x()/2.0,passage2[0].y());
            sabliere.translate(-QVector3D(0.0,0.0,passage2[2].y()/2.0));

            //tonoo
            RectTorus tonoo(geom,passage2[1].x()/2.0-passage2[1].y(),passage2[1].x()/2.0,passage2[1].z());
            vecteurTranslation = QVector3D(0.0,0.0,passage2[2].y()-passage2[1].z());
            tonoo.translate(-QVector3D(0.0,0.0,passage2[2].y()/2.0));
            tonoo.translate(vecteurTranslation);
            objets << 4 << 4;
            couleurs << 0 << 0;

            if(couleurDistinct)
            {
                sabliere.setcolor(coulTour);
                tonoo.setcolor(coulTour);
            }
            else
            {
                sabliere.setcolor(coulGeneral);
                tonoo.setcolor(coulGeneral);
            }

            parts << sabliere.parts << tonoo.parts;

            //on place les chevrons penches a droite
            for (int f = 0; f < tabChevron.size(); ++f)
           // for (int f = 0; f < 2; ++f)
            {
                for (int i = 0; i < passage2[2].x(); ++i)
                {
                    RectChevron *chevr;
                    if(tabChevron[f].orientation==0)
                    {
                        chevr = new RectChevron(geom,tabChevron[f]);
                        objets << 6;
                    }
                    else
                    {
                        chevr = new RectChevron(geomsim,tabChevron[f]);
                        objetssim << 6;
                    }


                    quatSelonZ.setVector(0.0,0.0,1.0*qSin((i*2.0*pi/passage2[2].x())/2.0));quatSelonZ.setScalar(qCos((i*2.0*pi/passage2[2].x())/2.0));
                    quatSelonZ.normalize();
                    chevr->translate(-QVector3D(0.0,0.0,passage2[2].y()/2.0));
                    chevr->rotate(quatSelonZ);
                    chevr->rotate(tabChevron[f].quatRot);
                    chevr->translate(tabChevron[f].translation);

                    if(tabChevron[f].orientation==0)
                    {
                        if(couleurDistinct)
                        {
                            chevr->setcolor(couleur(f));
                            couleurs << f;
                        }
                        else
                        {
                            chevr->setcolor(coulGeneral);
                            couleurs << 0;
                        }
                        parts << chevr->parts;
                    }
                    else
                    {
                        if(couleurDistinct)
                        {
                            chevr->setcolor(couleur(f));
                            couleurssim << f;
                        }
                        else
                        {
                            chevr->setcolor(coulGeneral);
                            couleurssim << 0;
                        }
                        partssim << chevr->parts;
                    }


                }
            }

    }
    //vue chevron en particulier
    else if(m_choix==1)
    {
        RectChevron chev(geom,tabChevron[m_choix2]);
        vecteurTranslation = QVector3D(-tabChevron[m_choix2].C.x()/2.0,-tabChevron[m_choix2].A.y()/2.0,tabChevron[m_choix2].epaisseurChevron/2.0);

        chev.rotate(quatdemiSelonY);
        quatdemiSelonX.setVector(1.0*qSin(-pi/4.0),0.0,0.0);quatdemiSelonX.setScalar(qCos(-pi/4.0));
        quatdemiSelonX.normalize();
        chev.rotate(quatdemiSelonX);
        chev.rotate(tabChevron[m_choix2].quatRot);
        chev.translate(vecteurTranslation);
        objets << 6;

        if(couleurDistinct)
        {
            chev.setcolor(couleur(m_choix2));
            couleurs << m_choix2;
        }
        else
        {
            chev.setcolor(coulGeneral);
            couleurs << 0;
        }
        //on additionne les parties
        parts << chev.parts;
    }

    //on normalise les vecteurs
    geom->finalize();
    geomsim->finalize();

}
//! [3]

//! [4]
void domenidabeille3d::draw(bool p_filaire) const
{
    //ca c est la fonction pour dessiner
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

QString domenidabeille3d::exportObj()
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
