#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <qmath.h>

#include "objet3d.h"
#include "mathema.h"
#include "rectoid.h"
#include "parametres.h"
#include "zome3d.h"

//ca c'est notre constructeur de classe avec creation d'une variable structuree
zome3D::zome3D(QObject *parent, zome *pp_zome, bool p_couleursDistinctes, int p_choix, int p_choix2, qreal scale)
    : QObject(parent)
    , geom(new Geometry())
    , geomsim(new Geometry())
{
    m_zome = pp_zome;
    m_choix = p_choix;
    m_choix2 = p_choix2;
    couleurDistinct = p_couleursDistinctes;

    coulGeneral = couleur(0);
    coulLosange.resize(16);
    for(int i=0;i<16;i++)
        coulLosange[i] = couleur(i+3);

    buildGeometry(scale);

}

// et le destructeur
zome3D::~zome3D()
{
    qDeleteAll(parts);
    qDeleteAll(partssim);
    delete geom;
    delete geomsim;
}

// la on applique la couleur a toutes les parties de l'objet
void zome3D::setColor(QColor c)
{
    for (int i = 0; i < parts.count(); ++i)
        qSetColor(parts[i]->faceColor, c);
    for (int i = 0; i < partssim.count(); ++i)
        qSetColor(partssim[i]->faceColor, c);
}

//cherche le point le plus eloigne du centre pour definir taille affichage
double zome3D::tailleAffichage()
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
        result = m_zome->pointPlusEloigne()*1.1;
    }
    return result;
}

// la fonction qui cree l'objet proprement dit
void zome3D::buildGeometry(qreal scale)
{

    //vue d ensemble
    if(m_choix==0)
    {

        QQuaternion quatSelonZ;
        QVector<rangee> rang;
        QVector<QVector3D> valeurG;

        m_zome->valeursSommets(rang);
        m_zome->valeursZomeGenerales(valeurG);

        //pas de methode, affichage zome classique
        if(valeurG[0].z()==1)
        {
            QVector3D m_a,m_b,m_c,m_d;
            double alpha,dec;

            dec = pi/valeurG[0].x();
            for(int i=0;i<valeurG[0].y();i++)
            {

                if(pair(i))
                    alpha = 0;
                else
                    alpha = dec;
                for(int j=0;j<valeurG[0].x();j++)
                {
                    m_a = QVector3D(rang[i].rayonExt*qCos(alpha+2.0*j*dec),rang[i].rayonExt*qSin(alpha+2.0*j*dec),rang[i].z-rang[0].z/2.0);
                    m_b = QVector3D(rang[i+1].rayonExt*qCos(alpha-dec+2.0*j*dec),rang[i+1].rayonExt*qSin(alpha-dec+2.0*j*dec),rang[i+1].z-rang[0].z/2.0);
                    if(i==(valeurG[0].y()-1))
                        m_c = QVector3D(rang[i+1].rayonInt*qCos(alpha+2.0*j*dec),rang[i+1].rayonInt*qSin(alpha+2.0*j*dec),rang[i+1].z-rang[0].z/2.0);
                    else
                        m_c = QVector3D(rang[i+2].rayonExt*qCos(alpha+2.0*j*dec),rang[i+2].rayonExt*qSin(alpha+2.0*j*dec),rang[i+2].z-rang[0].z/2.0);
                    m_d = QVector3D(rang[i+1].rayonExt*qCos(alpha+dec+2.0*j*dec),rang[i+1].rayonExt*qSin(alpha+dec+2.0*j*dec),rang[i+1].z-rang[0].z/2.0);

                    RectLosangePlan losange(geom,m_a,m_b,m_c,m_d);
                    RectLosangePlan losangeInt(geomsim,m_a,m_d,m_c,m_b);
                    objets << 1 << 1;
                    if(couleurDistinct)
                    {
                        losange.setcolor(coulLosange[i]);
                        losangeInt.setcolor(coulLosange[i]);
                        couleurs << i+3;
                        couleurssim << i+3;
                    }
                    else
                    {
                        losange.setcolor(coulGeneral);
                        losangeInt.setcolor(coulGeneral);
                        couleurs << 0;
                        couleurssim << 0;
                    }

                    parts << losange.parts;
                    partssim << losangeInt.parts;
                }
            }
        }
        //methode 1
        if(valeurG[0].z()==0)
        {
            QVector<losange_m1> los;
            m_zome->valeursMethode1(los);

            for(int i=0;i<los.size();i++)
            {
                for(int j=0;j<valeurG[0].x();j++)
                {
                    quatSelonZ.setVector(0.0,0.0,1.0*qSin((j*2.0*pi/valeurG[0].x())/2.0));quatSelonZ.setScalar(qCos((j*2.0*pi/valeurG[0].x())/2.0));
                    quatSelonZ.normalize();

                    if((i==(los.size()-1))&&(i!=(valeurG[0].x()-1)))
                    {
                        RectTriangleBasMethode1 losm1(geom,los[i].diagVert,los[i].diagHoriz,valeurG[1].x(),los[i].DiagVertInt,los[i].DiagHorizInt,valeurG[1].y());
                        losm1.rotate(quatSelonZ);
                        losm1.translate(los[i].barycentre);
                        losm1.rotate(los[i].quatRot);

                        objets << 4;

                        if(couleurDistinct)
                        {
                            losm1.setcolor(coulLosange[i]);
                            couleurs << i+3;
                        }
                        else
                        {
                            losm1.setcolor(coulGeneral);
                            couleurs << 0;
                        }

                        parts << losm1.parts;

                    }
                    else if(i!=(valeurG[0].x()-1))
                    {
                        if(pair(i))
                        {
                            RectLosangeMethode1 losm1(geom,los[i].diagVert,los[i].diagHoriz,valeurG[1].x(),los[i].DiagVertInt,los[i].DiagHorizInt);
                            losm1.rotate(quatSelonZ);
                            losm1.translate(los[i].barycentre);
                            losm1.rotate(los[i].quatRot);
                            objets << 4;

                            if(couleurDistinct)
                            {
                                losm1.setcolor(coulLosange[i]);
                                couleurs << i+3;
                            }
                            else
                            {
                                losm1.setcolor(coulGeneral);
                                couleurs << 0;
                            }
                            parts << losm1.parts;
                        }
                        else
                        {
                            RectLosangeMethode1 losm1(geomsim,los[i].diagVert,los[i].diagHoriz,valeurG[1].x(),los[i].DiagVertInt,los[i].DiagHorizInt);
                            losm1.rotate(quatSelonZ);
                            losm1.translate(los[i].barycentre);
                            losm1.rotate(los[i].quatRot);
                            objetssim << 4;

                            if(couleurDistinct)
                            {
                                losm1.setcolor(coulLosange[i]);
                                couleurssim << i+3;
                            }
                            else
                            {
                                losm1.setcolor(coulGeneral);
                                couleurssim << 0;
                            }
                            partssim << losm1.parts;
                        }
                    }

                }
            }
        }

    }
    //vue particuliere
    if(m_choix==1)
    {

        QQuaternion quatSelonZ;
        QVector<rangee> rang;
        QVector<QVector3D> valeurG;

        m_zome->valeursSommets(rang);
        m_zome->valeursZomeGenerales(valeurG);

           //methode 1
        if(valeurG[0].z()==0)
        {
            QVector<losange_m1> los;
            m_zome->valeursMethode1(los);

            int i = m_choix2;

            //        quatSelonZ.setVector(0.0,0.0,1.0*qSin((j*2.0*pi/valeurG[0].x())/2.0));quatSelonZ.setScalar(qCos((j*2.0*pi/valeurG[0].x())/2.0));
              //      quatSelonZ.normalize();
            if((i==(los.size()-1))&&(i!=(valeurG[0].x()-1)))
            {
                RectTriangleBasMethode1 losm1(geom,los[i].diagVert,los[i].diagHoriz,valeurG[1].x(),los[i].DiagVertInt,los[i].DiagHorizInt,valeurG[1].y());
                //losm1.rotate(quatSelonZ);
                //losm1.translate(los[i].barycentre);
                losm1.rotate(los[i].quatRot);
                objets << 4;

                if(couleurDistinct)
                {
                    losm1.setcolor(coulLosange[i]);
                    couleurs << i+3;
                }
                else
                {
                    losm1.setcolor(coulGeneral);
                    couleurs << 0;
                }

                parts << losm1.parts;

            }
            else if(i!=(valeurG[0].x()-1))
            {
                RectLosangeMethode1 losm1(geom,los[i].diagVert,los[i].diagHoriz,valeurG[1].x(),los[i].DiagVertInt,los[i].DiagHorizInt);
               // losm1.rotate(quatSelonZ);
               // losm1.translate(los[i].barycentre);
                losm1.rotate(los[i].quatRot);
                objets << 4;

                if(couleurDistinct)
                {
                    losm1.setcolor(coulLosange[i]);
                    couleurs << i+3;
                }
                else
                {
                    losm1.setcolor(coulGeneral);
                    couleurs << 0;
                }
                parts << losm1.parts;
            }


        }

    }


    //on normalise les vecteurs
    geom->finalize();
    geomsim->finalize();

}


void zome3D::draw(bool p_filaire) const
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

QString zome3D::exportObj()
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
