#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include <QQuaternion>
#include <qmath.h>

#include "objet3d.h"
#include "mathema.h"
#include "rectoid.h"
#include "parametres.h"
#include "geodome3d.h"


//ca c'est notre constructeur de classe avec creation d'une variable structuree
geodome3D::geodome3D(QObject *parent, geodome *pp_geodome, bool p_couleursDistinctes, int p_choix, int p_choix2, qreal scale)
    : QObject(parent)
    , geom(new Geometry())
    , geomsim2(new Geometry())
{
    m_geodome = pp_geodome;
    m_choix = p_choix;
    m_choix2 = p_choix2;
    couleurDistinct = p_couleursDistinctes;

    coulGeneral = couleur(0);

    buildGeometry(scale);
}

// et le destructeur
geodome3D::~geodome3D()
{
    qDeleteAll(parts);
    qDeleteAll(partssim2);
    delete geom;
    delete geomsim2;
}

// la on applique la couleur a toutes les parties de l'objet
void geodome3D::setColor(QColor c)
{
    for (int i = 0; i < parts.count(); ++i)
        qSetColor(parts[i]->faceColor, c);
    for (int i = 0; i < partssim2.count(); ++i)
        qSetColor(partssim2[i]->faceColor, c);
}

//cherche le point le plus eloigne du centre pour definir taille affichage
double geodome3D::tailleAffichage()
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
        result = m_geodome->pointPlusEloigne()*1.1;
    }
    return result;
}

// la fonction qui cree l'objet proprement dit
void geodome3D::buildGeometry(qreal scale)
{

    if((m_geodome->geo_affichage==0)||(m_geodome->geo_affichage==3)||(m_geodome->geo_affichage==4)||(m_geodome->geo_affichage==6))
    {
        for(int i=0;i<m_geodome->TsLesFaces.size();i++)
        {
            RectTrianglePlan tri(geom,m_geodome->TsLesSomm[m_geodome->TsLesFaces[i].s1].coord,m_geodome->TsLesSomm[m_geodome->TsLesFaces[i].s2].coord,m_geodome->TsLesSomm[m_geodome->TsLesFaces[i].s3].coord);
            RectTrianglePlan tri2(geom,m_geodome->TsLesSomm[m_geodome->TsLesFaces[i].s1].coord,m_geodome->TsLesSomm[m_geodome->TsLesFaces[i].s3].coord,m_geodome->TsLesSomm[m_geodome->TsLesFaces[i].s2].coord);
            objets << 1 << 1;
            if(couleurDistinct)
            {
                tri.setcolor(couleur(m_geodome->TsLesFaces[i].typ));
                tri2.setcolor(couleur(m_geodome->TsLesFaces[i].typ));
                couleurs << m_geodome->TsLesFaces[i].typ;
                couleurs << m_geodome->TsLesFaces[i].typ;
            }
            else
            {
                tri.setcolor(coulGeneral);
                tri2.setcolor(coulGeneral);
                couleurs << 0;
                couleurs << 0;
            }
          //  if(i<m_geodome->TsLesFaces.size()/2.0)
            parts << tri.parts << tri2.parts;
            /*else
            {
                partssim2 << tri.parts << tri2.parts;
            }*/
        }
    }
    if((m_geodome->geo_affichage==1)||(m_geodome->geo_affichage==3)||(m_geodome->geo_affichage==5)||(m_geodome->geo_affichage==6))
    {
        for(int i=0;i<m_geodome->TsLesAretes.size();i++)
        {
            if(i==12)
                int ggg=5;
            RectArete are(geom,m_geodome->TsLesSomm[m_geodome->TsLesAretes[i].p1].coord,m_geodome->TsLesSomm[m_geodome->TsLesAretes[i].p2].coord);
            objets << 1;
            if(couleurDistinct)
            {
                are.setcolor(couleur(m_geodome->TsLesAretes[i].typ));
                couleurs << m_geodome->TsLesAretes[i].typ;
            }
            else
            {
                are.setcolor(coulGeneral);
                couleurs << 0;
            }
            parts << are.parts;
        }
    }
    if((m_geodome->geo_affichage==2)||(m_geodome->geo_affichage==4)||(m_geodome->geo_affichage==5)||(m_geodome->geo_affichage==6))
    {
        for(int i=0;i<m_geodome->TsLesSomm.size();i++)
        {
            RectSommet som(geom,m_geodome->TsLesSomm[i].coord);
            objets << 1;
            if(couleurDistinct)
            {
                som.setcolor(couleur(m_geodome->TsLesSomm[i].typ));
                couleurs << m_geodome->TsLesSomm[i].typ;
            }
            else
            {
                som.setcolor(coulGeneral);
                couleurs << 0;
            }
            parts << som.parts;
        }
    }


    //on normalise les vecteurs
    geom->finalize();
   // geomsim2->finalize();
}



void geodome3D::draw(bool p_filaire) const
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

    geomsim2->loadArrays();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    for (int i = 0; i < partssim2.count(); ++i)
        partssim2[i]->draw(p_filaire);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

QString geodome3D::exportObj()
{
    QString str,str2,str3;
    QString resu = "";
    Geometry *geomtamp;
    geomtamp = new Geometry(*geom);

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
    return resu;
}
