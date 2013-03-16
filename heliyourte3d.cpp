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
#include "heliyourte3d.h"

//ca c'est notre constructeur de classe avec creation d'une variable structuree
heliyourte3d::heliyourte3d(QObject *parent, heliyourte *pp_heliyourte, bool p_couleursDistinctes, int p_choix, int p_choix2, qreal scale)
    : QObject(parent)
    , geom(new Geometry())
    , geomsim(new Geometry())
{
    m_heliyourte = pp_heliyourte;
    m_choix = p_choix;
    m_choix2 = p_choix2;
    couleurDistinct = p_couleursDistinctes;

    coulGeneral = couleur(0);

    buildGeometry(scale);
}


// et le destructeur
heliyourte3d::~heliyourte3d()
{
    qDeleteAll(parts);
    qDeleteAll(partssim);
    delete geom;
    delete geomsim;
}

// la on applique la couleur a toutes les parties de l'objet
void heliyourte3d::setColor(QColor c)
{
    for (int i = 0; i < parts.count(); ++i)
        qSetColor(parts[i]->faceColor, c);
    for (int i = 0; i < partssim.count(); ++i)
        qSetColor(partssim[i]->faceColor, c);
}

//cherche le point le plus eloigne du centre pour definir taille affichage
double heliyourte3d::tailleAffichage()
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
        result = m_heliyourte->pointPlusEloigne()*1.1;
    }
    return result;
}

// la fonction qui cree l'objet proprement dit
void heliyourte3d::buildGeometry(qreal scale)
{

    QVector<RectTraverse *> traverses;
    QVector<RectMontantA *> montantsA;
    QVector<RectMontantB *> montantsB;
    QVector<RectMontantC *> montantsC;
    QVector<RectPerche *> perches;
    QVector<RectCroix *> croixs;
    QVector<RectCroix *> croixs2;

    QVector3D vecteurTranslation, vecteurTranslation2;
    QQuaternion quatSelonY, quatSelonY_90, quatSelonYTraverse, quatSelonZCroix1, quatSelonZCroix2, quatSelonYCroix, quatSelonXCroix180, quatSelonYCroixBeta;

    quatSelonY_90.setVector(0.0,1.0*qSin((pi/2.0)/2.0),0.0);quatSelonY_90.setScalar(qCos((pi/2.0)/2.0));
    quatSelonY_90.normalize();

    vecteurTranslation2 = QVector3D(-m_heliyourte->hel_rayonYourte*qCos(m_heliyourte->hel_angleDivision/2.0),0.0,m_heliyourte->hel_rayonYourte*qSin(m_heliyourte->hel_angleDivision/2.0));

    quatSelonYTraverse.setVector(0.0,1.0*qSin((m_heliyourte->phel_BetaTraverse)/2.0),0.0);quatSelonYTraverse.setScalar(qCos((m_heliyourte->phel_BetaTraverse)/2.0));
    quatSelonYTraverse.normalize();

    quatSelonYCroix.setVector(0.0,1.0*qSin((m_heliyourte->hel_angleVrilleCroix)/2.0),0.0);quatSelonYCroix.setScalar(qCos((m_heliyourte->hel_angleVrilleCroix)/2.0));
    quatSelonYCroix.normalize();

    quatSelonZCroix1.setVector(1.0*qSin((-m_heliyourte->hel_OmegaCroix)/2.0),0.0,0.0);quatSelonZCroix1.setScalar(qCos((-m_heliyourte->hel_OmegaCroix)/2.0));
    quatSelonZCroix1.normalize();

    quatSelonZCroix2.setVector(1.0*qSin((m_heliyourte->hel_OmegaCroix)/2.0),0.0,0.0);quatSelonZCroix2.setScalar(qCos((m_heliyourte->hel_OmegaCroix)/2.0));
    quatSelonZCroix2.normalize();

    quatSelonXCroix180.setVector(1.0*qSin((pi)/2.0),0.0,0.0);quatSelonXCroix180.setScalar(qCos((pi)/2.0));
    quatSelonXCroix180.normalize();

    quatSelonYCroixBeta.setVector(0.0,1.0*qSin((-m_heliyourte->hel_angleVrilleCroix2)/2.0),0.0);quatSelonYCroixBeta.setScalar(qCos((-m_heliyourte->hel_angleVrilleCroix2)/2.0));
    quatSelonYCroixBeta.normalize();

    //vue generale
    if(m_choix==0)
    {
        for(int i=0;i<m_heliyourte->hel_nbrePans;i++)
        {
            quatSelonY.setVector(0.0,1.0*qSin(i*(m_heliyourte->hel_angleDivision)/2.0),0.0);quatSelonY.setScalar(qCos(i*(m_heliyourte->hel_angleDivision)/2.0));

            quatSelonY.normalize();



            // une traverse
            RectTraverse *traverse;
            traverse = new RectTraverse(geom,m_heliyourte);
            traverses << traverse;

            traverses[i]->rotate(quatSelonY);
            double xx,zz;
            xx=m_heliyourte->hel_epaisseurTraverse/2.0+(m_heliyourte->hel_retombeePlanchesMontant-m_heliyourte->hel_EdansC.largeur-m_heliyourte->hel_EdansC.distanceRapportBordInt);
            zz=-(m_heliyourte->hel_tetonTraverse+m_heliyourte->hel_H12_H14/2.0+0.002);
            vecteurTranslation = QVector3D(xx*qCos(-m_heliyourte->phel_BetaTraverse)-zz*qSin(-m_heliyourte->phel_BetaTraverse),m_heliyourte->hel_hauteurMiBoisTraverse+m_heliyourte->hel_FdansA.bas,zz*qCos(-m_heliyourte->phel_BetaTraverse)+xx*qSin(-m_heliyourte->phel_BetaTraverse));
            traverses[i]->translate(vecteurTranslation);
            traverses[i]->translate(vecteurTranslation2);
            traverses[i]->rotate(quatSelonYTraverse);
            traverses[i]->rotate(quatSelonY_90);

            objets << 3;

            // montant A
            RectMontantA *montantA;
            montantA = new RectMontantA(geom,m_heliyourte);
            montantsA << montantA;
            montantsA[i]->rotate(quatSelonY);
            vecteurTranslation = QVector3D(0.0,0.0,m_heliyourte->hel_epaisseurPlancheAetC/2.0+m_heliyourte->hel_epaisseurPlancheB/2.0);
            montantsA[i]->translate(vecteurTranslation);
            vecteurTranslation = QVector3D(m_heliyourte->hel_retombeePlanchesMontant/2.0,m_heliyourte->hel_hauteurMurs/2.0,0.0);
            montantsA[i]->translate(vecteurTranslation);
            montantsA[i]->translate(vecteurTranslation2);

            objets << 6;

            // montant C
            RectMontantC *montantC;
            montantC = new RectMontantC(geom,m_heliyourte);
            montantsC << montantC;
            montantsC[i]->rotate(quatSelonY);
            vecteurTranslation = QVector3D(0.0,0.0,-m_heliyourte->hel_epaisseurPlancheAetC/2.0-m_heliyourte->hel_epaisseurPlancheB/2.0);
            montantsC[i]->translate(vecteurTranslation);
            vecteurTranslation = QVector3D(m_heliyourte->hel_retombeePlanchesMontant/2.0,m_heliyourte->hel_hauteurMurs/2.0,0.0);
            montantsC[i]->translate(vecteurTranslation);
            montantsC[i]->translate(vecteurTranslation2);

            objets << 6;

            // montant B
            RectMontantB *montantB;
            montantB = new RectMontantB(geom,m_heliyourte);
            montantsB << montantB;
            montantsB[i]->rotate(quatSelonY);
            vecteurTranslation = QVector3D(m_heliyourte->hel_retombeePlanchesMontant/2.0,m_heliyourte->hel_hauteurMurs/2.0,0.0);
            montantsB[i]->translate(vecteurTranslation);
            montantsB[i]->translate(vecteurTranslation2);

            objets << 3;

            // perche
            RectPerche *perche;
            perche = new RectPerche(geom,m_heliyourte);
            perches << perche;
            perches[i]->rotate(quatSelonY);
            vecteurTranslation = QVector3D(m_heliyourte->hel_perche_decx, m_heliyourte->hel_perche_decy, -m_heliyourte->hel_epaisseurPerche);
            perches[i]->translate(vecteurTranslation);
            perches[i]->translate(vecteurTranslation2);

            objets << 6;

            // croix
            RectCroix *croix;
            croix = new RectCroix(geom,m_heliyourte);
            croixs << croix;
            croixs[i]->rotate(quatSelonY);
            croixs[i]->translate(vecteurTranslation2);
            xx = m_heliyourte->hel_retombeePlanchesMontant-m_heliyourte->hel_AdansC.largeur;
            zz = -m_heliyourte->hel_epaisseurPlancheB/2.0;
            vecteurTranslation = QVector3D(xx*qCos(-m_heliyourte->hel_angleVrilleCroix)-zz*qSin(-m_heliyourte->hel_angleVrilleCroix),m_heliyourte->hel_AdansC.haut-m_heliyourte->hel_margeTrous,zz*qCos(-m_heliyourte->hel_angleVrilleCroix)+xx*qSin(-m_heliyourte->hel_angleVrilleCroix));
            croixs[i]->translate(vecteurTranslation);
            croixs[i]->rotate(quatSelonZCroix1);
            xx = m_heliyourte->hel_epaisseurCroix/2.0;
            zz = -m_heliyourte->phel_cr_distanceEntre2B/2.0;
            vecteurTranslation = QVector3D( xx*qCos(-m_heliyourte->hel_angleVrilleCroix)-zz*qSin(-m_heliyourte->hel_angleVrilleCroix),m_heliyourte->hel_retombeeCroix/2.0-m_heliyourte->hel_cr_2[1].y(),zz*qCos(-m_heliyourte->hel_angleVrilleCroix)+xx*qSin(-m_heliyourte->hel_angleVrilleCroix));
            croixs[i]->translate(vecteurTranslation);
            croixs[i]->rotate(quatSelonYCroix);
            croixs[i]->rotate(quatSelonY_90);

            objets << 3;

            // croix
            RectCroix *croix2;
            croix2 = new RectCroix(geom,m_heliyourte);
            croixs2 << croix2;
            croixs2[i]->rotate(quatSelonY);
            croixs2[i]->translate(vecteurTranslation2);
            xx = m_heliyourte->hel_retombeePlanchesMontant-m_heliyourte->hel_BdansC.largeur-m_heliyourte->hel_BdansC.distanceRapportBordInt;
            zz = -m_heliyourte->hel_epaisseurPlancheB/2.0;
            vecteurTranslation = QVector3D(xx*qCos(-m_heliyourte->hel_angleVrilleCroix2)-zz*qSin(-m_heliyourte->hel_angleVrilleCroix2),m_heliyourte->hel_BdansC.bas,zz*qCos(-m_heliyourte->hel_angleVrilleCroix2)+xx*qSin(-m_heliyourte->hel_angleVrilleCroix2));
            croixs2[i]->translate(vecteurTranslation);
            croixs2[i]->rotate(quatSelonZCroix2);
            xx = m_heliyourte->hel_epaisseurCroix/2.0;
            zz = -m_heliyourte->phel_cr_distanceEntre2B/2.0;
            vecteurTranslation = QVector3D(xx*qCos(-m_heliyourte->hel_angleVrilleCroix2)-zz*qSin(-m_heliyourte->hel_angleVrilleCroix2) ,-(m_heliyourte->hel_retombeeCroix/2.0-m_heliyourte->hel_cr_2[1].y()),zz*qCos(-m_heliyourte->hel_angleVrilleCroix2)+xx*qSin(-m_heliyourte->hel_angleVrilleCroix2));
            croixs2[i]->translate(vecteurTranslation);
            croixs2[i]->rotate(quatSelonYTraverse);
            croixs2[i]->rotate(quatSelonY_90);
            croixs2[i]->rotate(quatSelonXCroix180);

            objets << 3;

            if(couleurDistinct)
            {
                traverses[i]->setcolor(couleur(1));
                montantsA[i]->setcolor(couleur(2));
                montantsC[i]->setcolor(couleur(3));
                montantsB[i]->setcolor(couleur(4));
                perches[i]->setcolor(couleur(5));
                croixs[i]->setcolor(couleur(6));
                croixs2[i]->setcolor(couleur(7));
                couleurs << 1 << 2 << 3 << 4 << 5 << 6 << 7;
            }
            else
            {
                traverses[i]->setcolor(coulGeneral);
                montantsA[i]->setcolor(coulGeneral);
                montantsC[i]->setcolor(coulGeneral);
                montantsB[i]->setcolor(coulGeneral);
                perches[i]->setcolor(coulGeneral);
                croixs[i]->setcolor(coulGeneral);
                croixs2[i]->setcolor(coulGeneral);
                couleurs << 0 << 0 << 0 << 0 << 0 << 0 << 0;
            }

            parts << traverses[i]->parts << montantsA[i]->parts << montantsC[i]->parts << montantsB[i]->parts << perches[i]->parts << croixs[i]->parts << croixs2[i]->parts;

        }

    }
    //vue traverse
    else if(m_choix==1)
    {
        // une traverse
        RectTraverse traverse(geom,m_heliyourte);
        if(couleurDistinct)
        {
            traverse.setcolor(couleur(1));
            couleurs << 1;
        }
        else
        {
            traverse.setcolor(coulGeneral);
            couleurs << 0;
        }

        objets << 3;

        parts << traverse.parts;
    }
    else if(m_choix==2)
    {
        // une croix
        RectCroix croix(geom,m_heliyourte);
        if(couleurDistinct)
        {
            croix.setcolor(couleur(6));
            couleurs << 6;
        }
        else
        {
            croix.setcolor(coulGeneral);
            couleurs << 0;
        }
        objets << 3;
        parts << croix.parts;
    }
    else if(m_choix==3)
    {
        // le montant complet
        // montant A
        RectMontantA montantA(geom,m_heliyourte);
        vecteurTranslation = QVector3D(0.0,0.0,m_heliyourte->hel_epaisseurPlancheAetC/2.0+m_heliyourte->hel_epaisseurPlancheB/2.0);
        montantA.translate(vecteurTranslation);
        objets << 6;

        // montant C
        RectMontantC montantC(geom,m_heliyourte);
        vecteurTranslation = QVector3D(0.0,0.0,-m_heliyourte->hel_epaisseurPlancheAetC/2.0-m_heliyourte->hel_epaisseurPlancheB/2.0);
        montantC.translate(vecteurTranslation);
        objets << 6;

        // montant B
        RectMontantB montantB(geom,m_heliyourte);
        objets << 3;

        if(couleurDistinct)
        {
            montantA.setcolor(couleur(2));
            montantB.setcolor(couleur(4));
            montantC.setcolor(couleur(3));
            couleurs << 2 << 3 << 4;
        }
        else
        {
            montantA.setcolor(coulGeneral);
            montantB.setcolor(coulGeneral);
            montantC.setcolor(coulGeneral);
            couleurs << 0 << 0 << 0;
        }
        parts << montantA.parts << montantB.parts << montantC.parts;
    }
    else if(m_choix==4)
    {
        // montant A
        RectMontantA montantA(geom,m_heliyourte);

        if(couleurDistinct)
        {
            montantA.setcolor(couleur(2));
            couleurs << 2;
        }
        else
        {
            montantA.setcolor(coulGeneral);
            couleurs << 0;
        }
        objets << 6;
        parts << montantA.parts;
    }
    else if(m_choix==5)
    {

        // montant B
        RectMontantB montantB(geom,m_heliyourte);

        if(couleurDistinct)
        {
            montantB.setcolor(couleur(4));
            couleurs << 4;
        }
        else
        {
            montantB.setcolor(coulGeneral);
            couleurs << 0;
        }
        objets << 3;
        parts << montantB.parts;
    }
    else if(m_choix==6)
    {
        // montant C
        RectMontantC montantC(geom,m_heliyourte);

        if(couleurDistinct)
        {
            montantC.setcolor(couleur(3));
            couleurs << 3;
        }
        else
        {
            montantC.setcolor(coulGeneral);
            couleurs << 0;
        }
        objets << 6;
        parts << montantC.parts;
    }
    else if(m_choix==7)
    {
        // perche
        RectPerche perche(geom,m_heliyourte);

        if(couleurDistinct)
        {
            perche.setcolor(couleur(5));
            couleurs << 5;
        }
        else
        {
            perche.setcolor(coulGeneral);
            couleurs << 0;
        }
        objets << 6;
        parts << perche.parts;
    }
    //on normalise les vecteurs
    geom->finalize();


}


void heliyourte3d::draw(bool p_filaire) const
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

QString heliyourte3d::exportObj()
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
