#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <qmath.h>

#include "complexes.h"
#include "mathema.h"
#include "parametres.h"
#include "geodome.h"

geodome::geodome()
{
}

void geodome::setParametres(int g_solideBase, int g_frequenceA, int g_frequenceB, double g_diametre, double g_hauteur, int g_angleRotation, int g_sabliereHorizontale, int g_affichage)
{
    //transfert parametres
    geo_solideBase = g_solideBase; //0 tetrahedre, 1 octahedre, 2 isocahedre
    geo_affichage = g_affichage;
    geo_frequenceA = (double)g_frequenceA;
    geo_frequenceB = (double)g_frequenceB;
    geo_diametre = g_diametre;
    geo_rayon = g_diametre/2.0;
    geo_hauteur = g_hauteur;
    //angle pour qu on ait 5 pointes dans le meme plan pour isocaedre
    if(geo_solideBase == 2)
        geo_angleRotation = radians(g_angleRotation+31.71747441);
    else
        geo_angleRotation = 0.0;
    //geo_ratioElliptique = g_ratioElliptique;
    geo_sabliereHorizontale = g_sabliereHorizontale;
    sommets.clear();
    triangleBase.clear();
    TsLesSomm.clear();
    TsLesSomm0.clear();
    typeArete.clear();
    TsLesAretes.clear();
    typeFace.clear();
    TsLesFaces.clear();
    typeSommet.clear();

    calcul();
}

// ca c est pour dessiner les schemas de sommet
void geodome::geoDessinSommet(QPaintDevice *des, int num)
{
    if(num==-1)
    {
        QPainter p(des);
        QRect dim = p.window();
        p.setPen(QPen(Qt::white,0));
        p.setBrush(QBrush(Qt::white)) ;
        p.drawRect(dim) ;
    }
    else
    {
        QPainter p(des);
        p.setRenderHint(QPainter::Antialiasing);
        int rCer = 60;
        int lvec = 100;
        QRect dim = p.window();
        double x0 = dim.width()/2.0;
        double y0 = dim.height()/2.0;
        // d abord on efface en blanc
        p.setPen(QPen(Qt::white,0));
        p.setBrush(QBrush(Qt::white)) ;
        p.drawRect(dim) ;
        // on trace le cercle du milieu
        p.setPen(QPen(couleur(num),3));
        p.setBrush(QBrush(Qt::NoBrush));
        p.drawEllipse(x0-rCer/2, y0-rCer/2, rCer, rCer) ;
        QString str;
        p.save();
        p.setFont(QFont("lucida",18,QFont::Bold));
        if(num<10)
            p.drawText(x0-15,y0+9,"s"+str.setNum(num));
        else
            p.drawText(x0-23,y0+9,"s"+str.setNum(num));
        p.restore();
        double angs = pi;
        double lMaxVect = 0;
        for(int j=0;j<typeSommet[num].vect.size();j++)
            if(typeArete[typeSommet[num].vect[j]].longueur>lMaxVect)
                lMaxVect = typeArete[typeSommet[num].vect[j]].longueur;
        for(int j=0;j<typeSommet[num].vect.size();j++)
        {
            p.setPen(QPen(couleur(typeSommet[num].vect[j]),2));
            double x1 = x0+qCos(angs)*(rCer/2.0+5.0);
            double y1 = y0+qSin(angs)*(rCer/2.0+5.0);
            double x2 = x0+qCos(angs)*(rCer/2.0+5.0+typeArete[typeSommet[num].vect[j]].longueur*lvec/lMaxVect);
            double y2 = y0+qSin(angs)*(rCer/2.0+5.0+typeArete[typeSommet[num].vect[j]].longueur*lvec/lMaxVect);
            double x3 = x0+qCos(angs+0.15)*(rCer/2.0+5.0+typeArete[typeSommet[num].vect[j]].longueur*lvec/(lMaxVect*1.5))-8;
            double y3 = y0+qSin(angs+0.15)*(rCer/2.0+5.0+typeArete[typeSommet[num].vect[j]].longueur*lvec/(lMaxVect*1.5))+5;
            p.drawLine(x1,y1,x2,y2);
            p.drawText(x3,y3,"a"+str.setNum(typeSommet[num].vect[j]));
            if(!((j==typeSommet[num].vect.size()-1)&&(typeSommet[num].complet==false)))
            {
                double x4 = x0+qCos(angs+typeSommet[num].angPl[j]/2.0)*(rCer/2.0+25.0)-15;
                double y4 = y0+qSin(angs+typeSommet[num].angPl[j]/2.0)*(rCer/2.0+25.0)+8;
                p.setPen(QPen(couleur(typeSommet[num].fa[j]),2));
                p.drawText(x4,y4,str.setNum(degres(typeSommet[num].angPl[j]),'f',1)+"°");
                double x5 = x0+qCos(angs+typeSommet[num].angPl[j]/2.0)*(rCer/2.0+100.0)-8;
                double y5 = y0+qSin(angs+typeSommet[num].angPl[j]/2.0)*(rCer/2.0+100.0)+5;
                p.drawText(x5,y5,"f"+str.setNum(typeSommet[num].fa[j]));
            }
            if(j!=typeSommet[num].vect.size()-1)
                angs += typeSommet[num].angPl[j];
        }
    }
}

// ca c est pour dessiner les schemas de face
void geodome::geoDessinFace(QPaintDevice *des, int num)
{
    if(num==-1)
    {
        QPainter p(des);
        QRect dim = p.window();
        p.setPen(QPen(Qt::white,0));
        p.setBrush(QBrush(Qt::white)) ;
        p.drawRect(dim) ;
    }
    else
    {
        QPainter p(des);
        p.setRenderHint(QPainter::Antialiasing);
        int lvec = 250;
        int lang = 35;
        QRect dim = p.window();
        double x0 = dim.width()/2.0;
        double y0 = dim.height()/2.0;
        // d abord on efface en blanc
        p.setPen(QPen(Qt::white,0));
        p.setBrush(QBrush(Qt::white)) ;
        p.drawRect(dim) ;
        p.setPen(QPen(couleur(num),3));
        p.setBrush(QBrush(Qt::NoBrush));
        p.save();
        p.setFont(QFont("lucida",18,QFont::Bold));
        QString str,str2;
        if(num<10)
            p.drawText(x0-14,y0+5,"f"+str.setNum(num));
        else
            p.drawText(x0-22,y0+5,"f"+str.setNum(num));
        p.restore();
        double lMaxVect = qMax(typeArete[typeFace[num].typar1].longueur,qMax(typeArete[typeFace[num].typar2].longueur,typeArete[typeFace[num].typar3].longueur));
        //on trace la premiere arete
        double x1 = x0-typeArete[typeFace[num].typar1].longueur*lvec/(lMaxVect*2.0);
        double y1 = y0+typeFace[num].h1*lvec/(lMaxVect*2.0)-15;
        double x2 = x1+typeArete[typeFace[num].typar1].longueur*lvec/lMaxVect;
        double y2 = y1;
        double x3 = x1+typeArete[typeFace[num].typar3].longueur*qCos(typeFace[num].angle1)*lvec/lMaxVect;
        double y3 = y1-typeFace[num].h1*lvec/lMaxVect;
        p.setPen(QPen(couleur(typeFace[num].typar1),2));
        p.drawLine(x1,y1,x2,y2);
        p.drawText((x1+x2)/2.0-55,y1+20,"a"+str.setNum(typeFace[num].typar1)+" L1 = "+str2.setNum(typeArete[typeFace[num].typar1].longueur*100.0,'f',1)+" cm");
        p.setPen(QPen(couleur(typeFace[num].typar2),2));
        p.drawLine(x2,y2,x3,y3);
        p.save();
        p.translate((x2+x3)/2.0,(y2+y3)/2.0);
        p.rotate(degres(typeFace[num].angle2));
        p.drawText(-55,-12,"a"+str.setNum(typeFace[num].typar2)+" L2 = "+str2.setNum(typeArete[typeFace[num].typar2].longueur*100.0,'f',1)+" cm");
        p.restore();
        p.setPen(QPen(couleur(typeFace[num].typar3),2));
        p.drawLine(x1,y1,x3,y3);
        p.save();
        p.translate((x1+x3)/2.0,(y1+y3)/2.0);
        p.rotate(-degres(typeFace[num].angle1));
        p.drawText(-55,-12,"a"+str.setNum(typeFace[num].typar3)+" L3 = "+str2.setNum(typeArete[typeFace[num].typar3].longueur*100.0,'f',1)+" cm");
        p.restore();
        p.setPen(QPen(Qt::black,2));
        p.drawText(x1+lang*qCos(typeFace[num].angle1/2.0),y1-lang*qSin(typeFace[num].angle1/2.0)+2,str.setNum(degres(typeFace[num].angle1),'f',1)+"°");
        p.drawText(x2-lang*qCos(typeFace[num].angle2/2.0)-35,y2-lang*qSin(typeFace[num].angle2/2.0)+2,str.setNum(degres(typeFace[num].angle2),'f',1)+"°");
        p.drawText(x3-17,y3+lang+14,str.setNum(degres(typeFace[num].angle3),'f',1)+"°");
    }
}

double geodome::surfaceToit()
{
    double res = 0.0;
    for(int i=0;i<typeFace.size();i++)
        res += typeFace[i].surface*typeFace[i].fa.size();
    return res;
}

double geodome::surfaceSol()
{
    double res;
    res = pi*qPow(rayon,2.0);
    return res;
}

double geodome::volume()
{
    double res = 0.0;
    res = pi*qPow(geoHauteurReelle,2.0)*(3.0*geo_diametre/2.0-geoHauteurReelle)/3.0;
    return res;
}

double geodome::diametreSol()
{
    return rayon*2.0;
}

double geodome::hauteurReelle()
{
    return geoHauteurReelle;
}

QString geodome::tableauFaces(int i)
{
    //les tableaux de coordonnees
    QString tableau,str,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13;

    QString wid;
    if(i==0)
        wid = "width=\"800\"";
    else
        wid = "";
    tableau = "<table "+wid+" cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
    tableau += "<td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Type")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Nombre")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Surface")+" (m2)</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Type Arêtes")+"</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>L1 (cm)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>L2 (cm)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>L3 (cm)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>H (m)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Alpha 1 (°)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Alpha 2 (°)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Alpha3 (°)</strong></p></td></tr>";
    for(int i=0;i<typeFace.size();i++)
        tableau += "<tr><td bgcolor=\""+couleurInternet(i)+"\"><p align=\"center\"><strong>f"+str.setNum(i)+"</strong></p></td><td><p align=\"center\">"+str2.setNum(typeFace[i].fa.size())+"</p></td><td><p align=\"center\">"+str3.setNum(typeFace[i].surface,'f',2)+"</p></td><td><p align=\"center\">"+str4.setNum(typeFace[i].typar1)+"->"+str5.setNum(typeFace[i].typar2)+"->"+str6.setNum(typeFace[i].typar3)+"</p></td><td><p align=\"center\">"+str7.setNum(typeArete[typeFace[i].typar1].longueur*100.0,'f',1)+"</p></td><td><p align=\"center\">"+str8.setNum(typeArete[typeFace[i].typar2].longueur*100.0,'f',1)+"</p></td><td><p align=\"center\">"+str9.setNum(typeArete[typeFace[i].typar3].longueur*100.0,'f',1)+"</p></td><td><p align=\"center\">"+str10.setNum(typeFace[i].h1,'f',2)+"</p></td><td><p align=\"center\">"+str11.setNum(degres(typeFace[i].angle1),'f',1)+"</p></td><td><p align=\"center\">"+str12.setNum(degres(typeFace[i].angle2),'f',1)+"</p></td><td><p align=\"center\">"+str13.setNum(degres(typeFace[i].angle3),'f',1)+"</p></td></tr>";
    int tot_faces = 0;
    for(int i = 0;i<typeFace.size();i++)
        tot_faces += typeFace[i].fa.size();
    tableau += "<tr><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("TOTAL")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+str.setNum(tot_faces)+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+str2.setNum(surfaceToit(),'f',2)+"</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td></tr>";
    tableau += "</tbody></table>";

    return tableau;
}

QString geodome::tableauMontants()
{
    //les tableaux de coordonnees
    QString tableau,str,str2,str3,str4,str5;

    tableau = "<table cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
    tableau += "<td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Type")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Nombre")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Longueur")+" (cm)</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Beta (°)</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Rapport Longueur")+"</strong></p></td bgcolor=\"#cccc99\"></tr>";
    for(int i=0;i<typeArete.size();i++)
        tableau += "<tr><td bgcolor=\""+couleurInternet(i)+"\"><p align=\"center\"><strong>a"+str.setNum(i)+"</strong></p></td><td><p align=\"center\">"+str2.setNum(typeArete[i].ar.size())+"</p></td><td><p align=\"center\">"+str3.setNum(typeArete[i].longueur*100.0,'f',1)+"</p></td><td><p align=\"center\">"+str4.setNum(degres(qAsin(typeArete[i].longueur/geo_diametre)),'f',1)+"</p></td><td><p align=\"center\">"+str5.setNum(typeArete[i].longueur/geo_diametre,'f',6)+"</p></td></tr>";
    int tot_montant = 0;
    for(int i = 0;i<typeArete.size();i++)
        tot_montant += typeArete[i].ar.size();
    double ml = 0.0;
    for(int i=0;i<typeArete.size();i++)
        ml += typeArete[i].ar.size()*typeArete[i].longueur;
    tableau += "<tr><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("TOTAL")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+str.setNum(tot_montant)+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+str2.setNum(ml,'f',2)+" m</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td bgcolor=\"#cccc99\"></tr>";
    tableau += "</tbody></table>";

    return tableau;
}

QString geodome::resultatMontants()
{
    QString str,resu;

    double ml = 0.0;
    for(int i=0;i<typeArete.size();i++)
        ml += typeArete[i].ar.size()*typeArete[i].longueur;
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Le tableau suivant détaille les arêtes (a) du géodome : pour chaque type, il donne le nombre d\'éléments, leur longueur, ainsi que l'angle de bout. Cet angle est celui avec lequel il vont arriver sur le connecteur. Si vous travaillez avec des montants en bois, il correspond à l\'angle d\'inclinaison de la scie pour couper les extrémités du montant")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Attention, les longueurs sont données sans prendre en compte les éventuelles dimensions des connecteurs. Il faudra donc en tenir compte pour tailler les montants, sinon vous allez simplement vous retrouver avec un géodome un peu plus grand")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Métrage linéaire des montants (à multiplier par la section de vos montants pour avoir le débit)")+" :<strong> "+str.setNum(ml,'f',2)+"</strong> m.</p>";
    return resu;
}

QString geodome::resultatFaces()
{
    QString str,resu;

    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Le tableau suivant détaille les faces (f) du géodome : pour chaque type, il donne le nombre d\'éléments, la surface, le type des 3 arêtes qui la constitue, leurs longueurs respectives, les mesures des 3 angles du triangle ainsi que la côte H qui peut être utile pour tracer les faces")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Surface totale des faces (donc de toute la couverture du géodome)")+" :<strong> "+str.setNum(surfaceToit(),'f',1)+"</strong> m².</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Les schémas suivants le tableau sont dessinés avec les bonnes côtes et angles, mais à échelle réduite bien sur")+". </p>";
    return resu;
}

QString geodome::resultatSommets()
{
    QString resu;

    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Le tableau suivant détaille les sommets (s) du géodome (ou connecteurs) : pour chaque type, il donne le nombre d\'éléments, s\'ils représentent un noeud complet ou un noeud qui n\'a pas de faces tout autour de lui, le type des arêtes qui partent de ce noeud, le type des faces autour de ce noeud et les angles alpha\'. Les données précedentes sont données pour chaque noeud, en tournant dans le sens horaire si on regarde le noeud depuis l\'extérieur du géodome")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Attention, les angles alpha\' sont les angles projetés sur une surface plane (leur somme fait 360°), ce ne sont pas les angles des différentes faces, ces derniers étant un peu inférieurs et donnés à l\'onglet faces")+"</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Les schémas suivants le tableau sont dessinés avec les bons angles")+".</p>";
    return resu;
}

QString geodome::aideConstruction()
{
    QString str,resu;

    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Le géodome est une stucture construite à partir d\'un des 3 polyèdres réguliers à faces triangulaires : le tetraèdre à 4 faces, l\'octaèdre à 8 faces et l\'isocaèdre à 20 faces. Chaque face est ensuite divisées régulièrement grace aux deux paramètres de fréquence A et B. Les points obtenus sont ensuite projetés sur une sphère et reliés entre eux")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Différentes méthodes sont possibles pour construire et assembler des géodomes. La documentation concernant les géodomes (ou géodes, ou domes géodesiques) est abondante sur internet")+".</p>";
    resu += "<span style=\"font-size: 12pt; color: #ff9900;\"><strong><p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Rappel des paramètres constructifs importants")+" :</p></strong></span>";
    QString cha,str2;
    if(geo_solideBase==0)
        cha = "III "+str.setNum(geo_frequenceA)+" "+str2.setNum(geo_frequenceB);
    if(geo_solideBase==1)
        cha = "IV "+str.setNum(geo_frequenceA)+" "+str2.setNum(geo_frequenceB);
    if(geo_solideBase==2)
        cha = "V "+str.setNum(geo_frequenceA)+" "+str2.setNum(geo_frequenceB);

    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Type de géodome")+" : <strong>"+cha+"</strong> ("+QObject::tr("chiffres romains pour indiquer solide de base : III pour tetraedre, IV pour octaedre, V pour isocaèdre; deuxième et troisième chiffres : fréquence A et fréquence B")+")</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Diamètre au sol, pour implantation")+" : <strong>"+str.setNum(rayon*2.0,'f',2)+" m</strong></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Diamètre de la sphère")+" : <strong>"+str.setNum(geo_diametre,'f',2)+" m</strong></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Hauteur réelle, jusqu'au faîtage")+" : <strong>"+str.setNum(geoHauteurReelle,'f',2)+" m</strong></p>";
    if(geo_sabliereHorizontale==0)
        cha = QObject::tr("non");
    else
        cha = QObject::tr("oui");
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Sablière horizontale")+" : <strong>"+cha+"</strong></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Angle de rotation")+" : <strong>"+str.setNum(degres(geo_angleRotation),'f',0)+"°</strong></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Surface au sol")+" : <strong>"+str.setNum(surfaceSol(),'f',2)+" m²</strong></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Surface de couverture")+" : <strong>"+str.setNum(surfaceToit(),'f',2)+" m²</strong></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Volume approché")+" : <strong>"+str.setNum(volume(),'f',2)+" m3</strong></p>";

    return resu;
}

QString geodome::tableauSommets(int i)
{
    //les tableaux de coordonnees
    QString tableau,str,str2;

    QString wid;
    if(i==0)
        wid = "width=\"850\"";
    else
        wid = "";
    tableau = "<table "+wid+" cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
    tableau += "<td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Type")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Nombre")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Complet")+"</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Type Arêtes")+"</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Type Faces")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Angles")+" Alpha\' °</strong></p></td></tr>";
    for(int i=0;i<typeSommet.size();i++)
    {
        QString enchVect = "";
        for(int j=0;j<typeSommet[i].vect.size();j++)
        {
            enchVect += "a"+str.setNum(typeSommet[i].vect[j]);
            if((j==typeSommet[i].vect.size()-1)&&(typeSommet[i].complet==false))
                enchVect += "";
            else
                enchVect += "->";
        }
        QString enchFace = "";
        for(int j=0;j<typeSommet[i].fa.size();j++)
        {
            enchFace += "f"+str.setNum(typeSommet[i].fa[j]);
            if((j==typeSommet[i].fa.size()-1)&&(typeSommet[i].complet==false))
                enchFace += "";
            else
                enchFace += "->";
        }
        QString enchAngPl = "";
        for(int j=0;j<typeSommet[i].angPl.size();j++)
        {
            enchAngPl += str.setNum(degres(typeSommet[i].angPl[j]),'f',1)+"°";
            if((j==typeSommet[i].angPl.size()-1)&&(typeSommet[i].complet==false))
                enchAngPl += "";
            else
                enchAngPl += "->";
        }
        QString tro;
        if(typeSommet[i].complet)
            tro = "oui";
        else
            tro = "non";
        tableau += "<tr><td bgcolor=\""+couleurInternet(i)+"\"><p align=\"center\"><strong>s"+str.setNum(i)+"</strong></p></td><td><p align=\"center\">"+str2.setNum(typeSommet[i].somet.size())+"</p></td><td><p align=\"center\">"+tro+"</p></td><td><p align=\"center\">"+enchVect+"</p></td><td><p align=\"center\">"+enchFace+"</p></td><td><p align=\"center\">"+enchAngPl+"</p></td></tr>";
    }
    int tot_sommet = 0;
    for(int i = 0;i<typeSommet.size();i++)
        tot_sommet += typeSommet[i].somet.size();
    tableau += "<tr><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("TOTAL")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+str.setNum(tot_sommet)+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong></strong></p></td></tr>";
    tableau += "</tbody></table>";

    return tableau;
}

double geodome::pointPlusEloigne()
{
    return qMax(geo_hauteur*geo_ratioElliptique,geo_diametre)*2.0;
}

int geodome::chercheFace(int ss1, int ss2, int num)
{
    int compteur = -1;
    bool sort = false;
    do
    {
        compteur++;
        if((compteur!=num)&&((ss1==triangleBase[compteur].s1)||(ss1==triangleBase[compteur].s2)||(ss1==triangleBase[compteur].s3))&&((ss2==triangleBase[compteur].s1)||(ss2==triangleBase[compteur].s2)||(ss2==triangleBase[compteur].s3)))
        {
            sort = true;
        }
    } while (sort==false);
    return compteur;
}

bool geodome::chercheSommet(sommet sss)
{
    bool trouve = false;
    int compteur = -1;
    double sens = 0.001;
    if(TsLesSomm0.size()!=0)
    {
        do
        {
            compteur++;
            if((sss.coord-TsLesSomm0[compteur].coord).length()<sens)
            {
                trouve = true;
            }
        } while ((trouve==false)&&(compteur<TsLesSomm0.size()-1));
   }
    return trouve;
}

int geodome::chercheSommmetBoucle(int xx, int yy, int face)
{
    int res = -1;
    int compteur = -1;
    do
    {
        compteur++;
        if((TsLesSomm[compteur].notX==xx)&&(TsLesSomm[compteur].notY==yy)&&(TsLesSomm[compteur].faceDeBase==face))
        {
            res = compteur;
        }
    } while ((res==-1)&&(compteur<TsLesSomm.size()-1));
    return res;
}

int geodome::faisTournerX(int face11, int face22, int xxx, int yyy)
{
    int res = xxx;
    if(triangleBase[face22].ar2==face11)
    {
        res = -(xxx+yyy)+geo_frequenceB+2*geo_frequenceA;
    } else if(triangleBase[face22].ar3==face11)
    {
        res = yyy;
    }
    return res;
}

int geodome::faisTournerY(int face11, int face22, int xxx, int yyy)
{
    int res = yyy;
    if(triangleBase[face22].ar2==face11)
    {
        res = xxx;
    } else if(triangleBase[face22].ar3==face11)
    {
        res = -(xxx+yyy)+geo_frequenceB+2*geo_frequenceA;
    }
    return res;
}

int geodome::faisTournerVect(int face11, int face22, int vect2)
{
    int res = vect2;
    if(triangleBase[face22].ar2==face11)
    {
        res = vect2+4;
    } else if(triangleBase[face22].ar3==face11)
    {
        res = vect2+2;
    }
    if(res>5)
        res = res-6;
    return res;
}
int geodome::TransfertX(int face11, int face22, int xxx, int yyy)
{
    int res;
    if(triangleBase[face11].ar2==face22)
    {
        res = xxx;
        if(triangleBase[face22].ar1==face11)
        {
            res = -(xxx+yyy)+geo_frequenceB+2*geo_frequenceA;
        } else if(triangleBase[face22].ar3==face11)
        {
            res = yyy;
        }
    }
    if(triangleBase[face11].ar3==face22)
    {
        res = xxx;
        if(triangleBase[face22].ar3==face11)
        {
            res = -(xxx+yyy)+geo_frequenceB+2*geo_frequenceA;
        } else if(triangleBase[face22].ar2==face11)
        {
            res = yyy;
        }
    }
    if(triangleBase[face11].ar1==face22)
    {
        res = xxx;
        if(triangleBase[face22].ar2==face11)
        {
            res = -(xxx+yyy)+geo_frequenceB+2*geo_frequenceA;
        } else if(triangleBase[face22].ar1==face11)
        {
            res = yyy;
        }
    }
    return res;
}

int geodome::TransfertY(int face11, int face22, int xxx, int yyy)
{
    int res;
    if(triangleBase[face11].ar2==face22)
    {
        res = yyy;
        if(triangleBase[face22].ar3==face11)
        {
            res = xxx;
        } else if(triangleBase[face22].ar2==face11)
        {
            res = -(xxx+yyy)+geo_frequenceB+2*geo_frequenceA;
        }
    }
    if(triangleBase[face11].ar3==face22)
    {
        res = yyy;
        if(triangleBase[face22].ar2==face11)
        {
            res = xxx;
        } else if(triangleBase[face22].ar1==face11)
        {
            res = -(xxx+yyy)+geo_frequenceB+2*geo_frequenceA;
        }
    }
    if(triangleBase[face11].ar1==face22)
    {
        res = yyy;
        if(triangleBase[face22].ar1==face11)
        {
            res = xxx;
        } else if(triangleBase[face22].ar3==face11)
        {
            res = -(xxx+yyy)+geo_frequenceB+2*geo_frequenceA;
        }
    }
    return res;
}

int geodome::TransfertVect(int face11, int face22, int vect2)
{
    int res;
    if(triangleBase[face11].ar2==face22)
    {
        res = vect2+5;
        if(triangleBase[face22].ar2==face11)
        {
            res = vect2+3;
        } else if(triangleBase[face22].ar3==face11)
        {
            res = vect2+1;
        }
    }
    if(triangleBase[face11].ar3==face22)
    {
        res = vect2+1;
        if(triangleBase[face22].ar2==face11)
        {
            res = vect2+5;
        } else if(triangleBase[face22].ar3==face11)
        {
            res = vect2+3;
        }
    }
    if(triangleBase[face11].ar1==face22)
    {
        res = vect2+3;
        if(triangleBase[face22].ar2==face11)
        {
            res = vect2+1;
        } else if(triangleBase[face22].ar3==face11)
        {
            res = vect2+5;
        }
    }
    if(res>5)
        res = res-6;
    return res;
}

// la on ajoute un sommet seulement si il est superieur a 0
bool geodome::ajoutSommet(int x, int y, int face, int vect, int i, int j)
{
    bool res = false;
    int a11 = chercheSommmetBoucle(x,y,face);
    if((a11!=-1)&&(TsLesSomm[a11].coord.z()>0))
    {
        TsLesSomm[i].v[j] = a11;
        TsLesSomm[a11].v[vect] = i;
        arete arr;
        arr.p1 = qMin(i,a11);
        arr.p2 = qMax(i,a11);
        // et on cree direct l arete
        TsLesAretes << arr;
        res = true;
    }
    return res;
}

double geodome::hauteurMax()
{
    double res = 0;
    for(int i=0;i<TsLesSomm0.size();i++)
        if(TsLesSomm0[i].coord.z()>res)
            res = TsLesSomm0[i].coord.z();
    return res;
}

int geodome::cherchSommet(int ii, int jj)
{
    int res = -1;
    int x1 = TsLesSomm[ii].notX;
    int y1 = TsLesSomm[ii].notY;
    int face1 = TsLesSomm[ii].faceDeBase;
    double cc = geo_frequenceA+geo_frequenceB;
    int vect;
    int x2,y2,face2;
    int x3,y3;
    int x4,y4,vect4,face4;
    bool tt;
    switch(jj)
    {
    case 0:
        tt = ajoutSommet(x1-1,y1,face1,3,ii,jj);
        if(!tt)
        {
            if(y1<=geo_frequenceA)
            {
                face2 = triangleBase[face1].ar1;
                x2 = -x1+cc+1;
                y2 = -y1+geo_frequenceA;
                x3 = faisTournerX(face1,face2,x2,y2);
                y3 = faisTournerY(face1,face2,x2,y2);
                vect = faisTournerVect(face1,face2,0);
                tt = ajoutSommet(x3,y3,face2,vect,ii,jj);
            }
            if(!tt)
            {
                face2 = triangleBase[face1].ar3;
                x2 = x1+y1-geo_frequenceA-1;
                y2 = -x1+geo_frequenceA+1;
                x3 = faisTournerX(face1,face2,x2,y2);
                y3 = faisTournerY(face1,face2,x2,y2);
                vect = faisTournerVect(face1,face2,4);
                tt = ajoutSommet(x3,y3,face2,vect,ii,jj);
            }
        }
        // procedure ajout derniers sommets
        if((((!tt)&&(x1==1)&&(y1==geo_frequenceA)))&&(geo_solideBase!=0))
        {
            x2 = 0;
            y2 = geo_frequenceA;
            face2 = triangleBase[face1].ar1;
            x3 = TransfertX(face1,face2,x2,y2);
            y3 = TransfertY(face1,face2,x2,y2);
            vect = TransfertVect(face1,face2,3);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar2;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar1;
       //
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            if(!tt)
            {
                face2 = triangleBase[face1].ar3;
                x3 = TransfertX(face1,face2,x2,y2);
                y3 = TransfertY(face1,face2,x2,y2);
                vect = TransfertVect(face1,face2,3);
                if(triangleBase[face2].ar1==face1)
                    face4 = triangleBase[face2].ar3;
                if(triangleBase[face2].ar2==face1)
                    face4 = triangleBase[face2].ar1;
                if(triangleBase[face2].ar3==face1)
                    face4 = triangleBase[face2].ar2;
                x4 = TransfertX(face2,face4,x3,y3);
                y4 = TransfertY(face2,face4,x3,y3);
                vect4 = TransfertVect(face2,face4,vect);
                tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            }
        }
        if((!tt)&&(geo_frequenceB==0)&&(x1==1)&&(y1==geo_frequenceA-1))
        {
            face2 = triangleBase[face1].ar1;
            x2 = -x1+cc+1;
            y2 = -y1+geo_frequenceA;
            x3 = faisTournerX(face1,face2,x2,y2);
            y3 = faisTournerY(face1,face2,x2,y2);
            vect = faisTournerVect(face1,face2,0);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar2;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar1;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
        }
        break;
    case 1:
        tt = ajoutSommet(x1-1,y1+1,face1,4,ii,jj);
        if(!tt)
        {
            face2 = triangleBase[face1].ar3;
            x2 = x1+y1-geo_frequenceA;
            y2 = -x1+geo_frequenceA+1;
            x3 = faisTournerX(face1,face2,x2,y2);
            y3 = faisTournerY(face1,face2,x2,y2);
            vect = faisTournerVect(face1,face2,5);
            tt = ajoutSommet(x3,y3,face2,vect,ii,jj);
        }

        if((!tt)&&(geo_frequenceB==0)&&(x1==1)&&(y1==geo_frequenceA))
        {
            face2 = triangleBase[face1].ar3;
            x2 = x1+y1-geo_frequenceA;
            y2 = -x1+geo_frequenceA+1;
            x3 = faisTournerX(face1,face2,x2,y2);
            y3 = faisTournerY(face1,face2,x2,y2);
            vect = faisTournerVect(face1,face2,5);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar1;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar2;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
        }
        if((!tt)&&(geo_frequenceB==0)&&(x1==1)&&(y1==geo_frequenceA-1))
        {
            x2 = 0;
            y2 = geo_frequenceA;
            face2 = triangleBase[face1].ar3;
            x3 = TransfertX(face1,face2,x2,y2);
            y3 = TransfertY(face1,face2,x2,y2);
            vect = TransfertVect(face1,face2,4);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar1;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar2;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            if(!tt)
            {
                face2 = triangleBase[face1].ar1;
                x3 = TransfertX(face1,face2,x2,y2);
                y3 = TransfertY(face1,face2,x2,y2);
                vect = TransfertVect(face1,face2,4);
                if(triangleBase[face2].ar1==face1)
                    face4 = triangleBase[face2].ar2;
                if(triangleBase[face2].ar2==face1)
                    face4 = triangleBase[face2].ar3;
                if(triangleBase[face2].ar3==face1)
                    face4 = triangleBase[face2].ar1;
                x4 = TransfertX(face2,face4,x3,y3);
                y4 = TransfertY(face2,face4,x3,y3);
                vect4 = TransfertVect(face2,face4,vect);
                tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            }
        }
        break;
    case 2:
        // premier cas : au cas ou le point se trouve sur la meme face
        tt = ajoutSommet(x1,y1+1,face1,5,ii,jj);
        // sinon on va chercher dans les faces juste a cote
        if(!tt)
        {
            if(x1>=geo_frequenceA)
            {
                face2 = triangleBase[face1].ar2;
                x2 = -y1+cc-1;
                y2 = x1+y1-cc+1;
                x3 = faisTournerX(face1,face2,x2,y2);
                y3 = faisTournerY(face1,face2,x2,y2);
                vect = faisTournerVect(face1,face2,4);
                tt = ajoutSommet(x3,y3,face2,vect,ii,jj);
            }
            if(!tt)
            {
                face2 = triangleBase[face1].ar3;
                x2 = x1+y1-geo_frequenceA+1;
                y2 = -x1+geo_frequenceA;
                x3 = faisTournerX(face1,face2,x2,y2);
                y3 = faisTournerY(face1,face2,x2,y2);
                vect = faisTournerVect(face1,face2,0);
                tt = ajoutSommet(x3,y3,face2,vect,ii,jj);
            }
        }
        // procedure ajout derniers sommets
        // dans le cas de l octahedre et de l icosahedre, il faut parfoit aller voir pour les sommets dans les triangles encore suivant
        if((((!tt)&&(x1==geo_frequenceA)&&(y1==(cc-1))))&&(geo_solideBase!=0))
        {
            x2 = geo_frequenceA;
            y2 = cc;
            face2 = triangleBase[face1].ar2;
            x3 = TransfertX(face1,face2,x2,y2);
            y3 = TransfertY(face1,face2,x2,y2);
            vect = TransfertVect(face1,face2,5);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar1;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar2;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            if(!tt)
            {
                face2 = triangleBase[face1].ar3;
                x3 = TransfertX(face1,face2,x2,y2);
                y3 = TransfertY(face1,face2,x2,y2);
                vect = TransfertVect(face1,face2,5);
                if(triangleBase[face2].ar1==face1)
                    face4 = triangleBase[face2].ar2;
                if(triangleBase[face2].ar2==face1)
                    face4 = triangleBase[face2].ar3;
                if(triangleBase[face2].ar3==face1)
                    face4 = triangleBase[face2].ar1;
                x4 = TransfertX(face2,face4,x3,y3);
                y4 = TransfertY(face2,face4,x3,y3);
                vect4 = TransfertVect(face2,face4,vect);
                tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            }
        }
        // et la c est pour aller voir dans le triangle suivant dans les coins quand on est en frequence b = 0
        if((!tt)&&(geo_frequenceB==0)&&(x1==geo_frequenceA-1)&&(y1==geo_frequenceA))
        {
            face2 = triangleBase[face1].ar3;
            x2 = x1+y1-geo_frequenceA+1;
            y2 = -x1+geo_frequenceA;
            x3 = faisTournerX(face1,face2,x2,y2);
            y3 = faisTournerY(face1,face2,x2,y2);
            vect = faisTournerVect(face1,face2,0);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar2;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar1;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
        }
        break;
    case 3:
        // premier cas si le point est sur la meme face
        tt = ajoutSommet(x1+1,y1,face1,0,ii,jj);
        // sinon on va chercher sur la face juste a cote
        if(!tt)
        {
            face2 = triangleBase[face1].ar2;
            x2 = cc-y1;
            y2 = x1+y1-cc+1;
            x3 = faisTournerX(face1,face2,x2,y2);
            y3 = faisTournerY(face1,face2,x2,y2);
            vect = faisTournerVect(face1,face2,5);
            tt = ajoutSommet(x3,y3,face2,vect,ii,jj);
        }
        //ici c est pour aller voir sur la face encore plus loin si jamais
        if((!tt)&&(geo_frequenceB==0)&&(y1==geo_frequenceA-1)&&(x1==geo_frequenceA))
        {
            face2 = triangleBase[face1].ar2;
            x2 = cc-y1;
            y2 = x1+y1-cc+1;
            x3 = faisTournerX(face1,face2,x2,y2);
            y3 = faisTournerY(face1,face2,x2,y2);
            vect = faisTournerVect(face1,face2,5);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar1;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar2;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
        }
        //ca c est au cas ou on pointe sur un sommet, pour aller le chercher a cote
        if((!tt)&&(geo_frequenceB==0)&&(x1==geo_frequenceA-1)&&(y1==geo_frequenceA))
        {
            x2 = geo_frequenceA;
            y2 = geo_frequenceA;
            face2 = triangleBase[face1].ar2;
            x3 = TransfertX(face1,face2,x2,y2);
            y3 = TransfertY(face1,face2,x2,y2);
            vect = TransfertVect(face1,face2,0);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar1;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar2;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            if(!tt)
            {
                face2 = triangleBase[face1].ar3;
                x3 = TransfertX(face1,face2,x2,y2);
                y3 = TransfertY(face1,face2,x2,y2);
                vect = TransfertVect(face1,face2,0);
                if(triangleBase[face2].ar1==face1)
                    face4 = triangleBase[face2].ar2;
                if(triangleBase[face2].ar2==face1)
                    face4 = triangleBase[face2].ar3;
                if(triangleBase[face2].ar3==face1)
                    face4 = triangleBase[face2].ar1;
                x4 = TransfertX(face2,face4,x3,y3);
                y4 = TransfertY(face2,face4,x3,y3);
                vect4 = TransfertVect(face2,face4,vect);
                tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            }
        }
        break;
    case 4:
        tt = ajoutSommet(x1+1,y1-1,face1,1,ii,jj);
        if(!tt)
        {
            if((x1+y1)>=cc)
            {
                face2 = triangleBase[face1].ar2;
                x2 = -y1+cc+1;
                y2 = x1+y1-cc;
                x3 = faisTournerX(face1,face2,x2,y2);
                y3 = faisTournerY(face1,face2,x2,y2);
                vect = faisTournerVect(face1,face2,0);
                tt = ajoutSommet(x3,y3,face2,vect,ii,jj);
            }
            if(!tt)
            {
                face2 = triangleBase[face1].ar1;
                x2 = -x1+cc-1;
                y2 = -y1+geo_frequenceA+1;
                x3 = faisTournerX(face1,face2,x2,y2);
                y3 = faisTournerY(face1,face2,x2,y2);
                vect = faisTournerVect(face1,face2,4);
                tt = ajoutSommet(x3,y3,face2,vect,ii,jj);
            }
        }
        // procedure ajout derniers sommets
        if((((!tt)&&(x1==(cc-1))&&(y1==1)))&&(geo_solideBase!=0))
        {
            x2 = cc;
            y2 = 0;
            face2 = triangleBase[face1].ar1;
            x3 = TransfertX(face1,face2,x2,y2);
            y3 = TransfertY(face1,face2,x2,y2);
            vect = TransfertVect(face1,face2,1);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar1;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar2;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            if(!tt)
            {
                face2 = triangleBase[face1].ar2;
                x3 = TransfertX(face1,face2,x2,y2);
                y3 = TransfertY(face1,face2,x2,y2);
                vect = TransfertVect(face1,face2,1);
                if(triangleBase[face2].ar1==face1)
                    face4 = triangleBase[face2].ar2;
                if(triangleBase[face2].ar2==face1)
                    face4 = triangleBase[face2].ar3;
                if(triangleBase[face2].ar3==face1)
                    face4 = triangleBase[face2].ar1;
                x4 = TransfertX(face2,face4,x3,y3);
                y4 = TransfertY(face2,face4,x3,y3);
                vect4 = TransfertVect(face2,face4,vect);
                tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            }
        }
        if((!tt)&&(geo_frequenceB==0)&&(y1==1)&&(x1==geo_frequenceA))
        {
            face2 = triangleBase[face1].ar2;
            x2 = -y1+cc+1;
            y2 = x1+y1-cc;
            x3 = faisTournerX(face1,face2,x2,y2);
            y3 = faisTournerY(face1,face2,x2,y2);
            vect = faisTournerVect(face1,face2,0);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar2;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar1;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
        }
        break;
    case 5:
        tt = ajoutSommet(x1,y1-1,face1,2,ii,jj);
        if(!tt)
        {
            face2 = triangleBase[face1].ar1;
            x2 = cc-x1;
            y2 = -y1+geo_frequenceA+1;
            x3 = faisTournerX(face1,face2,x2,y2);
            y3 = faisTournerY(face1,face2,x2,y2);
            vect = faisTournerVect(face1,face2,5);
            tt = ajoutSommet(x3,y3,face2,vect,ii,jj);
        }
        if((!tt)&&(geo_frequenceB==0)&&(y1==1)&&(x1==geo_frequenceA-1))
        {
            face2 = triangleBase[face1].ar1;
            x2 = cc-x1;
            y2 = -y1+geo_frequenceA+1;
            x3 = faisTournerX(face1,face2,x2,y2);
            y3 = faisTournerY(face1,face2,x2,y2);
            vect = faisTournerVect(face1,face2,5);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar1;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar2;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
        }
        //si on pointe sur un sommet
        if((!tt)&&(geo_frequenceB==0)&&(y1==1)&&(x1==geo_frequenceA))
        {
            x2 = geo_frequenceA;
            y2 = 0;
            face2 = triangleBase[face1].ar2;
            x3 = TransfertX(face1,face2,x2,y2);
            y3 = TransfertY(face1,face2,x2,y2);
            vect = TransfertVect(face1,face2,2);
            if(triangleBase[face2].ar1==face1)
                face4 = triangleBase[face2].ar2;
            if(triangleBase[face2].ar2==face1)
                face4 = triangleBase[face2].ar3;
            if(triangleBase[face2].ar3==face1)
                face4 = triangleBase[face2].ar1;
            x4 = TransfertX(face2,face4,x3,y3);
            y4 = TransfertY(face2,face4,x3,y3);
            vect4 = TransfertVect(face2,face4,vect);
            tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            if(!tt)
            {
                face2 = triangleBase[face1].ar1;
                x3 = TransfertX(face1,face2,x2,y2);
                y3 = TransfertY(face1,face2,x2,y2);
                vect = TransfertVect(face1,face2,2);
                if(triangleBase[face2].ar1==face1)
                    face4 = triangleBase[face2].ar3;
                if(triangleBase[face2].ar2==face1)
                    face4 = triangleBase[face2].ar1;
                if(triangleBase[face2].ar3==face1)
                    face4 = triangleBase[face2].ar2;
                x4 = TransfertX(face2,face4,x3,y3);
                y4 = TransfertY(face2,face4,x3,y3);
                vect4 = TransfertVect(face2,face4,vect);
                tt = ajoutSommet(x4,y4,face4,vect4,ii,jj);
            }
        }
        break;
    }
    return res;
}

int geodome::cherchePointCommun(int p1, int p2)
{
    int res = -1;
    int compte1 = -1;
    int compte2;
    do
    {
        compte1++;
        compte2 = -1;
        do
        {
            compte2++;
            if((TsLesSomm[p1].v[compte1]==TsLesSomm[p2].v[compte2])&&(TsLesSomm[p1].v[compte1]!=-1))
                res = TsLesSomm[p1].v[compte1];
        } while ((res==-1)&&(compte2<5));
    } while ((res==-1)&&(compte1<5));
    return res;
}

int geodome::cherchePointCommun2(int p1, int p2, int resu1)
{
    int res = -1;
    int res2 = resu1;
    if(res2!=-1)
    {
        int compte1 = -1;
        int compte2;
        do
        {
            compte1++;
            compte2 = -1;
            do
            {
                compte2++;
                if((TsLesSomm[p1].v[compte1]==TsLesSomm[p2].v[compte2])&&(TsLesSomm[p1].v[compte1]!=-1)&&(TsLesSomm[p1].v[compte1]!=res2))
                    res = TsLesSomm[p1].v[compte1];
            } while ((res==-1)&&(compte2<5));
        } while ((res==-1)&&(compte1<5));
    }
    return res;
}

int geodome::cherchePointCommun3(int p1, int p2, int resu1, int resu2)
{
    int res = -1;
    int res2 = resu1;
    int res3 = resu2;
    if(res2!=-1)
    {
        int compte1 = -1;
        int compte2;
        do
        {
            compte1++;
            compte2 = -1;
            do
            {
                compte2++;
                if((TsLesSomm[p1].v[compte1]==TsLesSomm[p2].v[compte2])&&(TsLesSomm[p1].v[compte1]!=-1)&&(TsLesSomm[p1].v[compte1]!=res2)&&(TsLesSomm[p1].v[compte1]!=res3))
                    res = TsLesSomm[p1].v[compte1];
            } while ((res==-1)&&(compte2<5));
        } while ((res==-1)&&(compte1<5));
    }
    return res;
}

int geodome::chercheTypeFace(int a1, int a2, int a3)
{
    int res = -1;
    int aar1 = qMin(a1,qMin(a2,a3));
    int aar2,aar3;
    if(aar1==a1)
    {
        aar2 = a2;
        aar3 = a3;
        if(aar1==aar3)
        {
            aar3 = aar2;
            aar2 = aar1;
        }
    } else if(aar1==a2)
    {
        aar2 = a3;
        aar3 = a1;
    } else if(aar1==a3)
    {
        aar2 = a1;
        aar3 = a2;
    }
    if(typeFace.size()!=0)
    {
        int compteur = -1;
        do
        {
            compteur++;
            if((typeFace[compteur].typar1==aar1)&&(typeFace[compteur].typar2==aar2)&&(typeFace[compteur].typar3==aar3))
                res = compteur;
        } while ((res==-1)&&(compteur<typeFace.size()-1));
    }
    return res;
}

void geodome::ajouteFace(int s1, int s2, int s3)
{
    // on ordonne les 3 sommets en commencant par numero plus petit
    int p1 = qMin(s1,qMin(s2,s3));
    int pe2, pe3;
    int p2, p3;
    if(p1==s1)
    {
        pe2 = s2;
        pe3 = s3;
    }else if(p1==s2)
    {
        pe2 = s1;
        pe3 = s3;
    }else if(p1==s3)
    {
        pe2 = s1;
        pe3 = s2;
    }

    // on finit d ordonner en les mettant dans le sens contraire des aiguilles d une montre
    QVector3D normal = QVector3D::crossProduct((TsLesSomm[pe2].coord-TsLesSomm[p1].coord),(TsLesSomm[pe3].coord-TsLesSomm[p1].coord));
    if(QVector3D::dotProduct(normal,(TsLesSomm[p1].coord-centreGeo))>0)
    {
        p2 = pe2;
        p3 = pe3;
    }else
    {
        p2 = pe3;
        p3 = pe2;
    }

    // on verifit si on n a pas deja ajoute cette face
    bool testFace = true;
    if(TsLesFaces.size()!=0)
    {
        int compteur = -1;
        do
        {
            compteur++;
            if((TsLesFaces[compteur].s1==p1)&&(TsLesFaces[compteur].s2==p2)&&(TsLesFaces[compteur].s3==p3))
                testFace = false;
        }while((compteur<TsLesFaces.size()-1)&&(testFace));
    }

    // sinon on la rajoute
    if(testFace)
    {
        face f;
        f.s1 = p1;
        f.s2 = p2;
        f.s3 = p3;
        // on cherche les aretes
        f.ar1 = chercheArete(p1,p2);
        f.ar2 = chercheArete(p2,p3);
        f.ar3 = chercheArete(p3,p1);
        // et de quel type elles sont
        int tar1 = TsLesAretes[f.ar1].typ;
        int tar2 = TsLesAretes[f.ar2].typ;
        int tar3 = TsLesAretes[f.ar3].typ;
        // on cherche si le type de face (definit par 3 types d aretes ordonnees) existe deja
        int typ = chercheTypeFace(tar1,tar2,tar3);
        // si non, on le cree
        if(typ==-1)
        {
            // en mettant le plus petit type d arete en premier
            type_face tf;
            tf.typar1 = qMin(tar1,qMin(tar2,tar3));
            if(tf.typar1==tar1)
            {
                tf.typar2 = tar2;
                tf.typar3 = tar3;
                // ca c est pour les cas genre 0 1 0 -> 0 0 1
                if(tf.typar3==tf.typar1)
                {
                    tf.typar3 = tar2;
                    tf.typar2 = tar1;
                }
            } else if(tf.typar1==tar2)
            {
                tf.typar2 = tar3;
                tf.typar3 = tar1;
            } else if(tf.typar1==tar3)
            {
                tf.typar2 = tar1;
                tf.typar3 = tar2;
            }
            // on rajoute la face courante au type
            tf.fa << TsLesFaces.size();
            typeFace << tf;
            // on ajoute le type a la face courante
            f.typ = typeFace.size()-1;
            // la on rajoute le caractere orientation
            if((tf.typar1==tar1)&&(tf.typar2==tar2)&&(tf.typar3==tar3))
            {
                f.orient = 1;
            }
            else if((tf.typar1==tar2)&&(tf.typar2==tar3)&&(tf.typar3==tar1))
            {
                f.orient = 2;
            }
            else if((tf.typar1==tar3)&&(tf.typar2==tar1)&&(tf.typar3==tar2))
            {
                f.orient = 3;
            }
            // ca c est une ligne au cas ou on ait 0 0 0 par exemple
            if((tf.typar1==tf.typar2)&&(tf.typar1==tf.typar3))
            {
                f.orient = 4;
            }
        }
        else
        // si le type de face existe deja
        {
            if((typeFace[typ].typar1==tar1)&&(typeFace[typ].typar2==tar2)&&(typeFace[typ].typar3==tar3))
            {
                f.orient = 1;
            }
            else if((typeFace[typ].typar1==tar2)&&(typeFace[typ].typar2==tar3)&&(typeFace[typ].typar3==tar1))
            {
                f.orient = 2;
            }
            else if((typeFace[typ].typar1==tar3)&&(typeFace[typ].typar2==tar1)&&(typeFace[typ].typar3==tar2))
            {
                f.orient = 3;
            }
            // ca c est une ligne au cas ou on ait 0 0 0 par exemple
            if((typeFace[typ].typar1==typeFace[typ].typar2)&&(typeFace[typ].typar1==typeFace[typ].typar3))
            {
                f.orient = 4;
            }
            f.typ = typ;
            typeFace[typ].fa << TsLesFaces.size();
        }
        // on rajoute la face
        TsLesFaces << f;
    }
}

int geodome::chercheArete(int s1, int s2)
{
    int res = -1;
    int compteur = -1;
    int s3 = qMin(s1,s2);
    int s4 = qMax(s1,s2);
    do
    {
        compteur++;
        if((s3==TsLesAretes[compteur].p1)&&(s4==TsLesAretes[compteur].p2))
            res = compteur;
    }while((compteur<TsLesAretes.size()-1)&&(res==-1));
    return res;
}

int geodome::chercheFac(int s1, int s2, int s3)
{
    int res = -1;
    //on oriente les points
    int p1 = qMin(s1,qMin(s2,s3));
    int p2, p3;
    int pe2, pe3;
    if(p1==s1)
    {
        pe2 = s2;
        pe3 = s3;
    }else if(p1==s2)
    {
        pe2 = s1;
        pe3 = s3;
    }else if(p1==s3)
    {
        pe2 = s1;
        pe3 = s2;
    }
    QVector3D normal = QVector3D::crossProduct((TsLesSomm[pe2].coord-TsLesSomm[p1].coord),(TsLesSomm[pe3].coord-TsLesSomm[p1].coord));
    if(QVector3D::dotProduct(normal,(TsLesSomm[p1].coord-centreGeo))>0)
    {
        p2 = pe2;
        p3 = pe3;
    }else
    {
        p2 = pe3;
        p3 = pe2;
    }
    //et on cherche
    int compteur = -1;
    do
    {
        compteur++;
        if((TsLesFaces[compteur].s1==p1)&&(TsLesFaces[compteur].s2==p2)&&(TsLesFaces[compteur].s3==p3))
            res = compteur;
    }while((compteur<TsLesFaces.size()-1)&&(res==-1));
    return res;
}

void geodome::ajouteTypeSommet(int fa[], int nbfa, bool tronq, int num)
{
    int rang = -1;
    if(typeSommet.size()!=0)
    {
        int compteur = -1;
        do
        {
            compteur++;
            if((nbfa==1)&(typeSommet[compteur].fa.size()==1)&&(fa[0]==typeSommet[compteur].fa[0])&&(fa[0+6]==typeSommet[compteur].fa2[0]))
                rang = compteur;

            if((nbfa==2)&(typeSommet[compteur].fa.size()==2)&&(fa[0]==typeSommet[compteur].fa[0])&&(fa[0+6]==typeSommet[compteur].fa2[0])&&(fa[1]==typeSommet[compteur].fa[1])&&(fa[1+6]==typeSommet[compteur].fa2[1]))
                rang = compteur;
            if((nbfa==2)&(typeSommet[compteur].fa.size()==2)&&(fa[0]==typeSommet[compteur].fa[1])&&(fa[0+6]==typeSommet[compteur].fa2[1])&&(fa[1]==typeSommet[compteur].fa[0])&&(fa[1+6]==typeSommet[compteur].fa2[0]))
                rang = compteur;

            if((nbfa==3)&(typeSommet[compteur].fa.size()==3)&&(fa[0]==typeSommet[compteur].fa[0])&&(fa[0+6]==typeSommet[compteur].fa2[0])&&(fa[1]==typeSommet[compteur].fa[1])&&(fa[1+6]==typeSommet[compteur].fa2[1])&&(fa[2]==typeSommet[compteur].fa[2])&&(fa[2+6]==typeSommet[compteur].fa2[2]))
                rang = compteur;
            if((nbfa==3)&(typeSommet[compteur].fa.size()==3)&&(fa[0]==typeSommet[compteur].fa[1])&&(fa[0+6]==typeSommet[compteur].fa2[1])&&(fa[1]==typeSommet[compteur].fa[2])&&(fa[1+6]==typeSommet[compteur].fa2[2])&&(fa[2]==typeSommet[compteur].fa[0])&&(fa[2+6]==typeSommet[compteur].fa2[0]))
                rang = compteur;
            if((nbfa==3)&(typeSommet[compteur].fa.size()==3)&&(fa[0]==typeSommet[compteur].fa[2])&&(fa[0+6]==typeSommet[compteur].fa2[2])&&(fa[1]==typeSommet[compteur].fa[0])&&(fa[1+6]==typeSommet[compteur].fa2[0])&&(fa[2]==typeSommet[compteur].fa[1])&&(fa[2+6]==typeSommet[compteur].fa2[1]))
                rang = compteur;

            if((nbfa==4)&(typeSommet[compteur].fa.size()==4)&&(fa[0]==typeSommet[compteur].fa[0])&&(fa[0+6]==typeSommet[compteur].fa2[0])&&(fa[1]==typeSommet[compteur].fa[1])&&(fa[1+6]==typeSommet[compteur].fa2[1])&&(fa[2]==typeSommet[compteur].fa[2])&&(fa[2+6]==typeSommet[compteur].fa2[2])&&(fa[3]==typeSommet[compteur].fa[3])&&(fa[3+6]==typeSommet[compteur].fa2[3]))
                rang = compteur;
            if((nbfa==4)&(typeSommet[compteur].fa.size()==4)&&(fa[0]==typeSommet[compteur].fa[1])&&(fa[0+6]==typeSommet[compteur].fa2[1])&&(fa[1]==typeSommet[compteur].fa[2])&&(fa[1+6]==typeSommet[compteur].fa2[2])&&(fa[2]==typeSommet[compteur].fa[3])&&(fa[2+6]==typeSommet[compteur].fa2[3])&&(fa[3]==typeSommet[compteur].fa[0])&&(fa[3+6]==typeSommet[compteur].fa2[0]))
                rang = compteur;
            if((nbfa==4)&(typeSommet[compteur].fa.size()==4)&&(fa[0]==typeSommet[compteur].fa[2])&&(fa[0+6]==typeSommet[compteur].fa2[2])&&(fa[1]==typeSommet[compteur].fa[3])&&(fa[1+6]==typeSommet[compteur].fa2[3])&&(fa[2]==typeSommet[compteur].fa[0])&&(fa[2+6]==typeSommet[compteur].fa2[0])&&(fa[3]==typeSommet[compteur].fa[1])&&(fa[3+6]==typeSommet[compteur].fa2[1]))
                rang = compteur;
            if((nbfa==4)&(typeSommet[compteur].fa.size()==4)&&(fa[0]==typeSommet[compteur].fa[3])&&(fa[0+6]==typeSommet[compteur].fa2[3])&&(fa[1]==typeSommet[compteur].fa[0])&&(fa[1+6]==typeSommet[compteur].fa2[0])&&(fa[2]==typeSommet[compteur].fa[1])&&(fa[2+6]==typeSommet[compteur].fa2[1])&&(fa[3]==typeSommet[compteur].fa[2])&&(fa[3+6]==typeSommet[compteur].fa2[2]))
                rang = compteur;

            if((nbfa==5)&(typeSommet[compteur].fa.size()==5)&&(fa[0]==typeSommet[compteur].fa[0])&&(fa[0+6]==typeSommet[compteur].fa2[0])&&(fa[1]==typeSommet[compteur].fa[1])&&(fa[1+6]==typeSommet[compteur].fa2[1])&&(fa[2]==typeSommet[compteur].fa[2])&&(fa[2+6]==typeSommet[compteur].fa2[2])&&(fa[3]==typeSommet[compteur].fa[3])&&(fa[3+6]==typeSommet[compteur].fa2[3])&&(fa[4]==typeSommet[compteur].fa[4])&&(fa[4+6]==typeSommet[compteur].fa2[4]))
                rang = compteur;
            if((nbfa==5)&(typeSommet[compteur].fa.size()==5)&&(fa[0]==typeSommet[compteur].fa[1])&&(fa[0+6]==typeSommet[compteur].fa2[1])&&(fa[1]==typeSommet[compteur].fa[2])&&(fa[1+6]==typeSommet[compteur].fa2[2])&&(fa[2]==typeSommet[compteur].fa[3])&&(fa[2+6]==typeSommet[compteur].fa2[3])&&(fa[3]==typeSommet[compteur].fa[4])&&(fa[3+6]==typeSommet[compteur].fa2[4])&&(fa[4]==typeSommet[compteur].fa[0])&&(fa[4+6]==typeSommet[compteur].fa2[0]))
                rang = compteur;
            if((nbfa==5)&(typeSommet[compteur].fa.size()==5)&&(fa[0]==typeSommet[compteur].fa[2])&&(fa[0+6]==typeSommet[compteur].fa2[2])&&(fa[1]==typeSommet[compteur].fa[3])&&(fa[1+6]==typeSommet[compteur].fa2[3])&&(fa[2]==typeSommet[compteur].fa[4])&&(fa[2+6]==typeSommet[compteur].fa2[4])&&(fa[3]==typeSommet[compteur].fa[0])&&(fa[3+6]==typeSommet[compteur].fa2[0])&&(fa[4]==typeSommet[compteur].fa[1])&&(fa[4+6]==typeSommet[compteur].fa2[1]))
                rang = compteur;
            if((nbfa==5)&(typeSommet[compteur].fa.size()==5)&&(fa[0]==typeSommet[compteur].fa[3])&&(fa[0+6]==typeSommet[compteur].fa2[3])&&(fa[1]==typeSommet[compteur].fa[4])&&(fa[1+6]==typeSommet[compteur].fa2[4])&&(fa[2]==typeSommet[compteur].fa[0])&&(fa[2+6]==typeSommet[compteur].fa2[0])&&(fa[3]==typeSommet[compteur].fa[1])&&(fa[3+6]==typeSommet[compteur].fa2[1])&&(fa[4]==typeSommet[compteur].fa[2])&&(fa[4+6]==typeSommet[compteur].fa2[2]))
                rang = compteur;
            if((nbfa==5)&(typeSommet[compteur].fa.size()==5)&&(fa[0]==typeSommet[compteur].fa[4])&&(fa[0+6]==typeSommet[compteur].fa2[4])&&(fa[1]==typeSommet[compteur].fa[0])&&(fa[1+6]==typeSommet[compteur].fa2[0])&&(fa[2]==typeSommet[compteur].fa[1])&&(fa[2+6]==typeSommet[compteur].fa2[1])&&(fa[3]==typeSommet[compteur].fa[2])&&(fa[3+6]==typeSommet[compteur].fa2[2])&&(fa[4]==typeSommet[compteur].fa[3])&&(fa[4+6]==typeSommet[compteur].fa2[3]))
                rang = compteur;

            if((nbfa==6)&(typeSommet[compteur].fa.size()==6)&&(fa[0]==typeSommet[compteur].fa[0])&&(fa[0+6]==typeSommet[compteur].fa2[0])&&(fa[1]==typeSommet[compteur].fa[1])&&(fa[1+6]==typeSommet[compteur].fa2[1])&&(fa[2]==typeSommet[compteur].fa[2])&&(fa[2+6]==typeSommet[compteur].fa2[2])&&(fa[3]==typeSommet[compteur].fa[3])&&(fa[3+6]==typeSommet[compteur].fa2[3])&&(fa[4]==typeSommet[compteur].fa[4])&&(fa[4+6]==typeSommet[compteur].fa2[4])&&(fa[5]==typeSommet[compteur].fa[5])&&(fa[5+6]==typeSommet[compteur].fa2[5]))
                rang = compteur;
            if((nbfa==6)&(typeSommet[compteur].fa.size()==6)&&(fa[0]==typeSommet[compteur].fa[5])&&(fa[0+6]==typeSommet[compteur].fa2[5])&&(fa[1]==typeSommet[compteur].fa[0])&&(fa[1+6]==typeSommet[compteur].fa2[0])&&(fa[2]==typeSommet[compteur].fa[1])&&(fa[2+6]==typeSommet[compteur].fa2[1])&&(fa[3]==typeSommet[compteur].fa[2])&&(fa[3+6]==typeSommet[compteur].fa2[2])&&(fa[4]==typeSommet[compteur].fa[3])&&(fa[4+6]==typeSommet[compteur].fa2[3])&&(fa[5]==typeSommet[compteur].fa[4])&&(fa[5+6]==typeSommet[compteur].fa2[4]))
                rang = compteur;
            if((nbfa==6)&(typeSommet[compteur].fa.size()==6)&&(fa[0]==typeSommet[compteur].fa[4])&&(fa[0+6]==typeSommet[compteur].fa2[4])&&(fa[1]==typeSommet[compteur].fa[5])&&(fa[1+6]==typeSommet[compteur].fa2[5])&&(fa[2]==typeSommet[compteur].fa[0])&&(fa[2+6]==typeSommet[compteur].fa2[0])&&(fa[3]==typeSommet[compteur].fa[1])&&(fa[3+6]==typeSommet[compteur].fa2[1])&&(fa[4]==typeSommet[compteur].fa[2])&&(fa[4+6]==typeSommet[compteur].fa2[2])&&(fa[5]==typeSommet[compteur].fa[3])&&(fa[5+6]==typeSommet[compteur].fa2[3]))
                rang = compteur;
            if((nbfa==6)&(typeSommet[compteur].fa.size()==6)&&(fa[0]==typeSommet[compteur].fa[3])&&(fa[0+6]==typeSommet[compteur].fa2[3])&&(fa[1]==typeSommet[compteur].fa[4])&&(fa[1+6]==typeSommet[compteur].fa2[4])&&(fa[2]==typeSommet[compteur].fa[5])&&(fa[2+6]==typeSommet[compteur].fa2[5])&&(fa[3]==typeSommet[compteur].fa[0])&&(fa[3+6]==typeSommet[compteur].fa2[0])&&(fa[4]==typeSommet[compteur].fa[1])&&(fa[4+6]==typeSommet[compteur].fa2[1])&&(fa[5]==typeSommet[compteur].fa[2])&&(fa[5+6]==typeSommet[compteur].fa2[2]))
                rang = compteur;
            if((nbfa==6)&(typeSommet[compteur].fa.size()==6)&&(fa[0]==typeSommet[compteur].fa[2])&&(fa[0+6]==typeSommet[compteur].fa2[2])&&(fa[1]==typeSommet[compteur].fa[3])&&(fa[1+6]==typeSommet[compteur].fa2[3])&&(fa[2]==typeSommet[compteur].fa[4])&&(fa[2+6]==typeSommet[compteur].fa2[4])&&(fa[3]==typeSommet[compteur].fa[5])&&(fa[3+6]==typeSommet[compteur].fa2[5])&&(fa[4]==typeSommet[compteur].fa[0])&&(fa[4+6]==typeSommet[compteur].fa2[0])&&(fa[5]==typeSommet[compteur].fa[1])&&(fa[5+6]==typeSommet[compteur].fa2[1]))
                rang = compteur;
            if((nbfa==6)&(typeSommet[compteur].fa.size()==6)&&(fa[0]==typeSommet[compteur].fa[1])&&(fa[0+6]==typeSommet[compteur].fa2[1])&&(fa[1]==typeSommet[compteur].fa[2])&&(fa[1+6]==typeSommet[compteur].fa2[2])&&(fa[2]==typeSommet[compteur].fa[3])&&(fa[2+6]==typeSommet[compteur].fa2[3])&&(fa[3]==typeSommet[compteur].fa[4])&&(fa[3+6]==typeSommet[compteur].fa2[4])&&(fa[4]==typeSommet[compteur].fa[5])&&(fa[4+6]==typeSommet[compteur].fa2[5])&&(fa[5]==typeSommet[compteur].fa[0])&&(fa[5+6]==typeSommet[compteur].fa2[0]))
                rang = compteur;
        } while ((compteur<typeSommet.size()-1)&&(rang==-1));
    }
    if(rang==-1)
    {
        type_sommet ts;
        ts.complet = !(tronq);
        for(int i=0;i<nbfa;i++)
        {
            ts.fa << fa[i];
            ts.fa2 << fa[i+6];
        }
        typeSommet << ts;
        rang = typeSommet.size()-1;
    }
    TsLesSomm[num].typ = rang;
    typeSommet[rang].somet << num;
}

void geodome::calcul()
{

    sommet s;
    for(int i=0;i<6;i++)
        s.v[i] = -1;
    triangle t;
    // definition du solide de base

    switch(geo_solideBase)
    {
    case 0: // tetrahehre
        s.coord = QVector3D(0.0,0.0,geo_rayon);
        sommets << s;
        s.coord = QVector3D(-4.0*geo_rayon/(3.0*qSqrt(2.0)),0.0,-geo_rayon/3.0);
        sommets << s;
        s.coord = QVector3D(4.0*geo_rayon*qCos(radians(60.0))/(3.0*qSqrt(2.0)),4.0*geo_rayon*qSin(radians(60.0))/(3.0*qSqrt(2.0)),-geo_rayon/3.0);
        sommets << s;
        s.coord = QVector3D(4.0*geo_rayon*qCos(radians(60.0))/(3.0*qSqrt(2.0)),-4.0*geo_rayon*qSin(radians(60.0))/(3.0*qSqrt(2.0)),-geo_rayon/3.0);
        sommets << s;
        t.s1 = 0;t.s2 = 2;t.s3 = 1;t.ar1 = 2;t.ar2 = 3;t.ar3 = 1;
        triangleBase << t;
        t.s1 = 0;t.s2 = 1;t.s3 = 3;t.ar1 = 0;t.ar2 = 3;t.ar3 = 2;
        triangleBase << t;
        t.s1 = 0;t.s2 = 3;t.s3 = 2;t.ar1 = 1;t.ar2 = 3;t.ar3 = 0;
        triangleBase << t;
        t.s1 = 3;t.s2 = 1;t.s3 = 2;t.ar1 = 1;t.ar2 = 0;t.ar3 = 2;
        triangleBase << t;
        break;
    case 1: // octahedre
        s.coord = QVector3D(0.0,0.0,geo_rayon);
        sommets << s;
        s.coord = QVector3D(0.0,0.0,-geo_rayon);
        sommets << s;
        s.coord = QVector3D(0.0,geo_rayon,0.0);
        sommets << s;
        s.coord = QVector3D(0.0,-geo_rayon,0.0);
        sommets << s;
        s.coord = QVector3D(geo_rayon,0.0,0.0);
        sommets << s;
        s.coord = QVector3D(-geo_rayon,0.0,0.0);
        sommets << s;
        t.s1 = 0;t.s2 = 3;t.s3 = 4;t.ar1 = 3;t.ar2 = 4;t.ar3 = 1;
        triangleBase << t;
        t.s1 = 0;t.s2 = 4;t.s3 = 2;t.ar1 = 0;t.ar2 = 5;t.ar3 = 2;
        triangleBase << t;
        t.s1 = 0;t.s2 = 2;t.s3 = 5;t.ar1 = 1;t.ar2 = 6;t.ar3 = 3;
        triangleBase << t;
        t.s1 = 0;t.s2 = 5;t.s3 = 3;t.ar1 = 2;t.ar2 = 7;t.ar3 = 0;
        triangleBase << t;
        t.s1 = 1;t.s2 = 4;t.s3 = 3;t.ar1 = 5;t.ar2 = 0;t.ar3 = 7;
        triangleBase << t;
        t.s1 = 1;t.s2 = 2;t.s3 = 4;t.ar1 = 6;t.ar2 = 1;t.ar3 = 4;
        triangleBase << t;
        t.s1 = 1;t.s2 = 5;t.s3 = 2;t.ar1 = 7;t.ar2 = 2;t.ar3 = 5;
        triangleBase << t;
        t.s1 = 1;t.s2 = 3;t.s3 = 5;t.ar1 = 4;t.ar2 = 3;t.ar3 = 6;
        triangleBase << t;
        break;
    case 2: // isocahedre
        double jj = geo_rayon/qSqrt(1.0+qPow((1.0+qSqrt(5.0))/2.0,2.0));
        double kk = ((1.0+qSqrt(5.0))/2.0)*jj;
        s.coord = QVector3D(kk,jj,0.0);
        sommets << s;
        s.coord = QVector3D(-kk,jj,0.0);
        sommets << s;
        s.coord = QVector3D(-kk,-jj,0.0);
        sommets << s;
        s.coord = QVector3D(kk,-jj,0.0);
        sommets << s;
        s.coord = QVector3D(0.0,kk,jj);
        sommets << s;
        s.coord = QVector3D(0.0,-kk,jj);
        sommets << s;
        s.coord = QVector3D(0.0,-kk,-jj);
        sommets << s;
        s.coord = QVector3D(0.0,kk,-jj);
        sommets << s;
        s.coord = QVector3D(jj,0.0,kk);
        sommets << s;
        s.coord = QVector3D(jj,0.0,-kk);
        sommets << s;
        s.coord = QVector3D(-jj,0.0,-kk);
        sommets << s;
        s.coord = QVector3D(-jj,0.0,kk);
        sommets << s;
        t.s1 = 11;t.s2 = 5;t.s3 = 8;
        triangleBase << t;
        t.s1 = 8;t.s2 = 4;t.s3 = 11;
        triangleBase << t;
        t.s1 = 9;t.s2 = 6;t.s3 = 10;
        triangleBase << t;
        t.s1 = 10;t.s2 = 7;t.s3 = 9;
        triangleBase << t;
        t.s1 = 6;t.s2 = 3;t.s3 = 5;
        triangleBase << t;
        t.s1 = 5;t.s2 = 2;t.s3 = 6;
        triangleBase << t;
        t.s1 = 4;t.s2 = 0;t.s3 = 7;
        triangleBase << t;
        t.s1 = 7;t.s2 = 1;t.s3 = 4;
        triangleBase << t;
        t.s1 = 2;t.s2 = 11;t.s3 = 1;
        triangleBase << t;
        t.s1 = 1;t.s2 = 10;t.s3 = 2;
        triangleBase << t;
        t.s1 = 3;t.s2 = 9;t.s3 = 0;
        triangleBase << t;
        t.s1 = 0;t.s2 = 8;t.s3 = 3;
        triangleBase << t;
        t.s1 = 8;t.s2 = 5;t.s3 = 3;
        triangleBase << t;
        t.s1 = 8;t.s2 = 0;t.s3 = 4;
        triangleBase << t;
        t.s1 = 11;t.s2 = 4;t.s3 = 1;
        triangleBase << t;
        t.s1 = 11;t.s2 = 2;t.s3 = 5;
        triangleBase << t;
        t.s1 = 9;t.s2 = 3;t.s3 = 6;
        triangleBase << t;
        t.s1 = 9;t.s2 = 7;t.s3 = 0;
        triangleBase << t;
        t.s1 = 10;t.s2 = 1;t.s3 = 7;
        triangleBase << t;
        t.s1 = 10;t.s2 = 6;t.s3 = 2;
        triangleBase << t;
        for(int i=0;i<triangleBase.size();i++)
        {
            triangleBase[i].ar1 = chercheFace(triangleBase[i].s1,triangleBase[i].s2,i);
            triangleBase[i].ar2 = chercheFace(triangleBase[i].s2,triangleBase[i].s3,i);
            triangleBase[i].ar3 = chercheFace(triangleBase[i].s3,triangleBase[i].s1,i);
        }
        break;
    }

    //rotation du solide
    if(geo_angleRotation!=0)
    {
        for(int i=0;i<sommets.size();i++)
        {
            double x,z;
            x = sommets[i].coord.x();
            z = sommets[i].coord.z();
            sommets[i].coord.setX(x*qCos(geo_angleRotation)+z*qSin(geo_angleRotation));
            sommets[i].coord.setZ(-x*qSin(geo_angleRotation)+z*qCos(geo_angleRotation));
        }
    }


    double geo_longueurArete = (sommets[triangleBase[0].s1].coord-sommets[triangleBase[0].s2].coord).length();
    QVector<sommet> divTriangle;
    sommet st;
    for(int i=0;i<6;i++)
        st.v[i] = -1;
    double a1,a2,b2_0,b1,b2,a3,a4,b4,rapp;
    rapp = geo_frequenceA/(geo_frequenceA+geo_frequenceB);
    a3 = 2*qSin(radians(60.0));
    a4 = -2*qSin(radians(60.0));
    b4 = -a4*geo_longueurArete;
    if(geo_frequenceA!=geo_frequenceB)
    {
        a1 = qSin(radians(60.0))/(qCos(radians(60.0))-rapp);
        b1 = -a1*geo_longueurArete/(geo_frequenceA+geo_frequenceB);
    }
    a2 = rapp*qSin(radians(60.0))/(1-rapp*qCos(radians(60.0)));
    b2_0 = -a2*geo_longueurArete;
    b2 = b2_0/geo_frequenceA;
    for(int i=0;i<=(geo_frequenceA+geo_frequenceB+1);i++)
    {
        for(int j=0;j<=(geo_frequenceA+geo_frequenceB+1);j++)
        {
            double x,y;
            if(geo_frequenceA!=geo_frequenceB)
            {
                x = (b2_0-j*b2-i*b1)/(a1-a2);
                y = a1*x+i*b1;
            }
            else
            {
                x = i*geo_longueurArete/(geo_frequenceA+geo_frequenceB);
                y = a2*x+b2_0-j*b2;
            }
            if((y>=-0.00001)&&(y<=a3*x+0.00001)&&(y<=(a4*x+b4+0.00001)))
            {
                st.coord.setX(x);st.coord.setY(y);st.coord.setZ(0);
                st.notX = i;st.notY = j;
                if(((i==0)&&(j==geo_frequenceA))||((i==(geo_frequenceA+geo_frequenceB))&&(j==0))||((i==geo_frequenceA)&&(j==(geo_frequenceA+geo_frequenceB))))
                {
                    switch(geo_solideBase)
                    {
                    case 0:
                        st.nbVoisins = 3;
                        break;
                    case 1:
                        st.nbVoisins = 4;
                        break;
                    case 2:
                        st.nbVoisins = 5;
                        break;
                    }
                }
                else
                {
                    st.nbVoisins = 6;
                }
                divTriangle << st;
            }
        }
    }
    int comptSommetParTriangle = divTriangle.size();

    // ici on ajoute tous les points dans l'espace et on les projette sur la sphere directement
    for(int i=0;i<triangleBase.size();i++)
    {
        QMatrix4x4 final,initial,rotation;
        QVector3D losX,losY;
        bool test = false;
        losX = sommets[triangleBase[i].s2].coord-sommets[triangleBase[i].s1].coord;
        losY = sommets[triangleBase[i].s3].coord-milieu2Points3D(sommets[triangleBase[i].s1].coord,sommets[triangleBase[i].s2].coord);
        if(QVector3D::crossProduct(losX,losY).normalized()==QVector3D(0.0,0.0,-1.0))
        {
            test = true;
        }
        //le quaternion de rotation
        final(0,0) = losX.normalized().x();
        final(1,0) = losX.normalized().y();
        final(2,0) = losX.normalized().z();
        final(3,0) = 0;
        final(0,1) = losY.normalized().x();
        final(1,1) = losY.normalized().y();
        final(2,1) = losY.normalized().z();
        final(3,1) = 0;
        final(0,2) = QVector3D::crossProduct(losX,losY).normalized().x();
        final(1,2) = QVector3D::crossProduct(losX,losY).normalized().y();
        final(2,2) = QVector3D::crossProduct(losX,losY).normalized().z();
        final(3,2) = 0;
        final(0,3) = 0;
        final(1,3) = 0;
        final(2,3) = 0;
        final(3,3) = 1;
        initial(0,0) = 1;
        initial(1,0) = 0;
        initial(2,0) = 0;
        initial(3,0) = 0;
        if(test)
        {
            initial(0,1) = 0;
            initial(1,1) = 0;
            initial(2,1) = 1;
            initial(3,1) = 0;
            initial(0,2) = 0;
            initial(1,2) = -1;
            initial(2,2) = 0;
            initial(3,2) = 0;
        }
        else
        {
            initial(0,1) = 0;
            initial(1,1) = 1;
            initial(2,1) = 0;
            initial(3,1) = 0;
            initial(0,2) = 0;
            initial(1,2) = 0;
            initial(2,2) = 1;
            initial(3,2) = 0;
        }
        initial(0,3) = 0;
        initial(1,3) = 0;
        initial(2,3) = 0;
        initial(3,3) = 1;
        rotation = (final*(initial.inverted()));
        QQuaternion quatRot = convertirMatriceRotation(rotation);
        QVector3D vectTrans = sommets[triangleBase[i].s1].coord;
        for(int j=0;j<comptSommetParTriangle;j++)
        {
            sommet so;
            so.typ = 0;
            for(int i=0;i<6;i++)
                so.v[i] = -1;
            if(test)
            {
                so.coord = QVector3D(divTriangle[j].coord.x(),0.0,divTriangle[j].coord.y());
                so.notX=divTriangle[j].notX;
                so.notY=divTriangle[j].notY;
                so.nbVoisins=divTriangle[j].nbVoisins;
            }
            else
            {
            so = divTriangle[j];
            }
            so.coord = rotationSelonQuaternion(so.coord,quatRot);
            so.coord = so.coord+vectTrans;
            so.coord = so.coord*(geo_rayon/so.coord.length());
            so.faceDeBase = i;
            if(chercheSommet(so)==false)
            {
                TsLesSomm0 << so;
            }
        }
    }

    //le truc elliptique
  /*  if(geo_ratioElliptique!=1.0)
        for(int i=0;i<TsLesSomm0.size();i++)
            TsLesSomm0[i].coord.setZ(TsLesSomm0[i].coord.z()*geo_ratioElliptique);*/

    // la on cherche le sommet actuel et on met tout a la bonne hauteur
    double hh = hauteurMax();
    for(int i=0;i<TsLesSomm0.size();i++)
        TsLesSomm0[i].coord.setZ(TsLesSomm0[i].coord.z()+geo_hauteur-hh);
    centreGeo = QVector3D(0.0,0.0,geo_hauteur-hh);

    // la on supprime tous les points trop bas
    for(int i=0;i<TsLesSomm0.size();i++)
        if(TsLesSomm0[i].coord.z()>0)
            TsLesSomm << TsLesSomm0[i];

    // maintenant faut relier les points entre eux
    for(int i=0;i<TsLesSomm.size();i++)
    {
        if((geo_frequenceA==1)&&(geo_frequenceB==0))
        {
        // a faire
        }
        if(TsLesSomm[i].nbVoisins==6)
            for(int j=0;j<6;j++)
                if(TsLesSomm[i].v[j] == -1)
                    int a = cherchSommet(i,j);
    }

    for(int i=0;i<TsLesSomm.size();i++)
    {
        int hop = 0;
        for(int j=0;j<6;j++)
        {
            if(TsLesSomm[i].v[j]!=-1)
            {
                hop++;
            }
        }
        TsLesSomm[i].tronque = false;
        if(TsLesSomm[i].nbVoisins!=hop)
            TsLesSomm[i].tronque = true;
        TsLesSomm[i].nbVoisins = hop;
    }

    //la on met a plat

    bool tron = false;
    double bas = 5000;
    rayon = 0.0;
    for(int i=0;i<TsLesSomm.size();i++)
    {
        if((TsLesSomm[i].tronque)&&(TsLesSomm[i].coord.z()<bas))
        {
            tron = true;
            bas = TsLesSomm[i].coord.z();
            rayon = (TsLesSomm[i].coord-QVector3D(0.0,0.0,bas)).length();
        }
    }
    geoHauteurReelle = geo_hauteur-bas;

    if((tron)&&(geo_sabliereHorizontale==1))
        for(int i=0;i<TsLesSomm.size();i++)
        {
            if((TsLesSomm[i].tronque))
            {
                TsLesSomm[i].coord.setZ(bas);
                double rayon2 = (TsLesSomm[i].coord-QVector3D(0.0,0.0,bas)).length();
                TsLesSomm[i].coord.setX(rayon*TsLesSomm[i].coord.x()/rayon2);
                TsLesSomm[i].coord.setY(rayon*TsLesSomm[i].coord.y()/rayon2);
            }
        }


    //on classe les aretes par type
    for(int i=0;i<TsLesAretes.size();i++)
    {
        double taille = (TsLesSomm[TsLesAretes[i].p1].coord-TsLesSomm[TsLesAretes[i].p2].coord).length();
        int cherchtype = -1;
        int compteur = -1;
        if(typeArete.size()!=0)
        {
            do
            {
                compteur++;
                if(qAbs(typeArete[compteur].longueur-taille)<0.01)
                {
                    typeArete[compteur].ar << i;
                    TsLesAretes[i].typ = compteur;
                    cherchtype = compteur;
                }
            } while((compteur<typeArete.size()-1)&&(cherchtype==-1));
            if(cherchtype == -1)
            {
                type_arete ta;
                ta.longueur = taille;
                ta.ar << i;
                typeArete << ta;
                TsLesAretes[i].typ = typeArete.size()-1;
            }
        }
        else
        {
            type_arete ta;
            ta.longueur = taille;
            ta.ar << i;
            typeArete << ta;
            TsLesAretes[i].typ = 0;
        }
    }

    //et maintenant les faces
    for(int i=0;i<TsLesAretes.size();i++)
    {
        int s1 = TsLesAretes[i].p1;
        int s2 = TsLesAretes[i].p2;
        int s3 = cherchePointCommun(s1,s2);
        int s4 = cherchePointCommun2(s1,s2,s3);
        int s5 = -1;
        if((s4!=-1)&&(geo_solideBase==0))
            s5 = cherchePointCommun3(s1,s2,s3,s4);
        if((s5==-1)&&(geo_solideBase==0)&&(s3!=-1)&&(s4!=-1))
        {
            if(TsLesSomm[s3].nbVoisins==3)
            {
                bool tof = false;
                for(int k=0;k<6;k++)
                    if(TsLesSomm[s3].v[k]==s4)
                        tof = true;
                if(tof)
                    s4 = -1;
            }
            else if(TsLesSomm[s4].nbVoisins==3)
            {
                bool tof = false;
                for(int k=0;k<6;k++)
                    if(TsLesSomm[s4].v[k]==s3)
                        tof = true;
                if(tof)
                {
                    s3 = s4;
                    s4 = -1;
                }
            }
        }
        if(s5!=-1)
        {
            if(TsLesSomm[s3].nbVoisins==3)
            {
                bool tof = false;
                for(int k=0;k<6;k++)
                    if(TsLesSomm[s3].v[k]==s4)
                        tof = true;
                if(tof)
                    s4 = s5;
            }
            else if(TsLesSomm[s4].nbVoisins==3)
            {
                bool tof = false;
                for(int k=0;k<6;k++)
                    if(TsLesSomm[s4].v[k]==s3)
                        tof = true;
                if(tof)
                    s3 = s5;
            }
            else if(TsLesSomm[s5].nbVoisins==3)
            {
                bool tof = false;
                for(int k=0;k<6;k++)
                    if(TsLesSomm[s5].v[k]==s4)
                        tof = true;
                if(tof)
                {
                    s4 = s5;
                } else
                {
                    s3 = s5;
                }
            }
        }
        if(s3!=-1)
            ajouteFace(s1,s2,s3);
        if(s4!=-1)
            ajouteFace(s1,s2,s4);
    }

    // puis les calculs subsidiaires et un peu de nettoyage
    for(int i=0;i<typeArete.size();i++)
        typeArete[i].angleCoupeBout = qAcos(typeArete[i].longueur/geo_diametre);

    for(int i=0;i<typeFace.size();i++)
    {
        typeFace[i].angle1 = qAcos((qPow(typeArete[typeFace[i].typar1].longueur,2.0)+qPow(typeArete[typeFace[i].typar3].longueur,2.0)-qPow(typeArete[typeFace[i].typar2].longueur,2.0))/(2.0*typeArete[typeFace[i].typar1].longueur*typeArete[typeFace[i].typar3].longueur));
        typeFace[i].angle2 = qAcos((qPow(typeArete[typeFace[i].typar1].longueur,2.0)+qPow(typeArete[typeFace[i].typar2].longueur,2.0)-qPow(typeArete[typeFace[i].typar3].longueur,2.0))/(2.0*typeArete[typeFace[i].typar1].longueur*typeArete[typeFace[i].typar2].longueur));
        typeFace[i].angle3 = qAcos((qPow(typeArete[typeFace[i].typar2].longueur,2.0)+qPow(typeArete[typeFace[i].typar3].longueur,2.0)-qPow(typeArete[typeFace[i].typar1].longueur,2.0))/(2.0*typeArete[typeFace[i].typar2].longueur*typeArete[typeFace[i].typar3].longueur));
        double DemiPer = (typeArete[typeFace[i].typar1].longueur+typeArete[typeFace[i].typar2].longueur+typeArete[typeFace[i].typar3].longueur)/2.0;
        typeFace[i].surface = qSqrt(DemiPer*(DemiPer-typeArete[typeFace[i].typar1].longueur)*(DemiPer-typeArete[typeFace[i].typar2].longueur)*(DemiPer-typeArete[typeFace[i].typar3].longueur));
        typeFace[i].h1 = typeArete[typeFace[i].typar3].longueur*qSin(typeFace[i].angle1);
    }

    // la partie sur les sommets

    for(int i=0;i<TsLesSomm.size();i++)
    {
        int rot2[6];
        for(int j=0;j<6;j++)
            rot2[j] = TsLesSomm[i].v[j];
        if((TsLesSomm[i].nbVoisins!=6))
        {
            int rot[6];
            int gg;
            gg = 1;
            do
            {
                for(int j=0;j<6;j++)
                    rot[j] = rot2[j];
                rot2[0]=rot[1];
                rot2[1]=rot[2];
                rot2[2]=rot[3];
                rot2[3]=rot[4];
                rot2[4]=rot[5];
                rot2[5]=rot[0];
            }while(rot2[0]!=-1);
            do
            {
                for(int j=0;j<6;j++)
                    rot[j] = rot2[j];
                rot2[0]=rot[1];
                rot2[1]=rot[2];
                rot2[2]=rot[3];
                rot2[3]=rot[4];
                rot2[4]=rot[5];
                rot2[5]=rot[0];
            }while(rot2[0]==-1);
            for(int k=1;k<5;k++)
                if(TsLesSomm[i].nbVoisins>k)
                    while(rot2[k]==-1)
                    {
                        for(int j=k;j<6;j++)
                            rot[j] = rot2[j];
                        for(int j=k;j<5;j++)
                            rot2[j]=rot[j+1];
                        rot2[5]=rot[k];
                    }
        }
        if(TsLesSomm[i].nbVoisins!=0)
        {
            int fac[12];
            for(int j=0;j<12;j++)
                fac[j] = -1;
            int nbFac;
            if(TsLesSomm[i].tronque)
                nbFac = TsLesSomm[i].nbVoisins-1;
            else
                nbFac = TsLesSomm[i].nbVoisins;
            for(int j=0;j<nbFac;j++)
            {
                if((TsLesSomm[i].tronque==false)&&(j==nbFac-1))
                {
                    int h = chercheFac(i,rot2[j],rot2[0]);
                    //on retourne le numero de la face
                    if(h!=-1)
                    {
                        fac[j] = TsLesFaces[h].typ;
                        fac[j+6] = TsLesFaces[h].orient;
                        if((rot2[j]==TsLesFaces[h].s1)&&(fac[j+6]!=4))
                        {
                            fac[j+6] = TsLesFaces[h].orient+2;
                            if(fac[j+6]>3)
                                fac[j+6] -= 3;
                        }else if((rot2[0]==TsLesFaces[h].s1)&&(fac[j+6]!=4))
                        {
                            fac[j+6] = TsLesFaces[h].orient+1;
                            if(fac[j+6]>3)
                                fac[j+6] -= 3;
                        }
                    }
                }
                else
                {
                   int h  = chercheFac(i,rot2[j],rot2[j+1]);
                   if(h!=-1)
                   {
                       fac[j] = TsLesFaces[h].typ;
                       fac[j+6] = TsLesFaces[h].orient;
                       if((rot2[j]==TsLesFaces[h].s1)&&(fac[j+6]!=4))
                       {
                           fac[j+6] = TsLesFaces[h].orient+2;
                           if(fac[j+6]>3)
                               fac[j+6] -= 3;
                       }else if((rot2[j+1]==TsLesFaces[h].s1)&&(fac[j+6]!=4))
                       {
                           fac[j+6] = TsLesFaces[h].orient+1;
                           if(fac[j+6]>3)
                               fac[j+6] -= 3;
                       }
                   }
                }
            }
            ajouteTypeSommet(fac,nbFac,TsLesSomm[i].tronque,i);
        }
    }

    // on fait des calculs subsidiaires sur les types de sommets
    for(int i=0;i<typeSommet.size();i++)
    {
        if(typeSommet[i].fa2[0]==4)
            typeSommet[i].vect << typeFace[typeSommet[i].fa[0]].typar1;
        if(typeSommet[i].fa2[0]==1)
            typeSommet[i].vect << typeFace[typeSommet[i].fa[0]].typar3;
        if(typeSommet[i].fa2[0]==2)
            typeSommet[i].vect << typeFace[typeSommet[i].fa[0]].typar2;
        if(typeSommet[i].fa2[0]==3)
            typeSommet[i].vect << typeFace[typeSommet[i].fa[0]].typar1;
        for(int j=0;j<typeSommet[i].fa.size();j++)
        {
            if((typeSommet[i].complet==true)&&(j==typeSommet[i].fa.size()-1))
            {
                if(typeSommet[i].fa2[j]==4)
                    typeSommet[i].ang << typeFace[typeSommet[i].fa[j]].angle1;
                if(typeSommet[i].fa2[j]==1)
                    typeSommet[i].ang << typeFace[typeSommet[i].fa[j]].angle1;
                if(typeSommet[i].fa2[j]==2)
                    typeSommet[i].ang << typeFace[typeSommet[i].fa[j]].angle3;
                if(typeSommet[i].fa2[j]==3)
                    typeSommet[i].ang << typeFace[typeSommet[i].fa[j]].angle2;
            }
            else
            {
                if(typeSommet[i].fa2[j]==4)
                {
                    typeSommet[i].vect << typeFace[typeSommet[i].fa[j]].typar1;
                    typeSommet[i].ang << typeFace[typeSommet[i].fa[j]].angle1;
                }
                if(typeSommet[i].fa2[j]==1)
                {
                    typeSommet[i].vect << typeFace[typeSommet[i].fa[j]].typar1;
                    typeSommet[i].ang << typeFace[typeSommet[i].fa[j]].angle1;
                }
                if(typeSommet[i].fa2[j]==2)
                {
                    typeSommet[i].vect << typeFace[typeSommet[i].fa[j]].typar3;
                    typeSommet[i].ang << typeFace[typeSommet[i].fa[j]].angle3;
                }
                if(typeSommet[i].fa2[j]==3)
                {
                    typeSommet[i].vect << typeFace[typeSommet[i].fa[j]].typar2;
                    typeSommet[i].ang << typeFace[typeSommet[i].fa[j]].angle2;
                }
            }
        }
    }

    //la on met les angles des sommets a plat
    for(int i=0;i<typeSommet.size();i++)
    {
        for(int j=0;j<typeSommet[i].ang.size();j++)
        {

            double l1 = typeArete[typeSommet[i].vect[j]].longueur;
            double l2;
            if((j==typeSommet[i].ang.size()-1)&&(typeSommet[i].complet==true))
                l2 = typeArete[typeSommet[i].vect[0]].longueur;
            else
                l2 = typeArete[typeSommet[i].vect[j+1]].longueur;
            double r = geo_diametre/2.0;
            double beta1 = qAcos(l1/(2.0*r));
            double beta2 = qAcos(l2/(2.0*r));
            double l1pr = r/qCos(beta1);
            double l2pr = r/qCos(beta2);
            double M1 = sin(beta1)*l1pr;
            double M2 = sin(beta2)*l2pr;
            double ccarre = qPow(l1pr,2.0)+qPow(l2pr,2.0)-2.0*l1pr*l2pr*qCos(typeSommet[i].ang[j]);
            double phi = qAcos((qPow(M1,2.0)+qPow(M2,2.0)-ccarre)/(2.0*M1*M2));
            typeSommet[i].angPl << phi;
        }
    }


}
