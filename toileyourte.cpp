#include "toileyourte.h"
#include <qmath.h>
#include "mathema.h"
#include "QPainter"
#include "parametres.h"

ToileYourte::ToileYourte()
{
}

void ToileYourte::setParametres(double toil_hauteurMur, int toil_penteYourte, double toil_diametreYourte, double toil_diametreTonoo, double toil_largeurLaize, int toil_orientation, double toil_decalage)
{
    toi_hauteurMur = toil_hauteurMur;
    toi_penteYourte = radians(toil_penteYourte);
    toi_diametreYourte = toil_diametreYourte;
    toi_diametreTonoo = toil_diametreTonoo;
    toi_largeurLaize = toil_largeurLaize;
    toi_orientation = radians(toil_orientation);
    toi_decalage = toil_decalage;

    calcul();
}

void ToileYourte::toiDessinToileGeneral(QPaintDevice *des, int taille)
{

    QPainter p(des);
    QRect dim;
    if(taille==0)
    {
        dim = QRect(0,0,700,700);
    }
    else
    {
        dim = QRect(0,0,600,600);
    }

    p.setRenderHint(QPainter::Antialiasing);
    //on cherche le centre
    double x0 = dim.width()/2.0;
    double y0 = dim.height()/2.0;
    // on cherche l echelle
    double echelle = (x0-30)/toi_rayonYourteFictif;
    // d abord on efface en blanc
    p.setPen(QPen(Qt::white,0));
    p.setBrush(QBrush(Qt::white));
    p.drawRect(dim);

    p.setPen(QPen(couleur(1),2));
    p.setBrush(QBrush(Qt::NoBrush));

    QPoint A,B,C,D;
    A = QPoint(x0+toi_A2.x()*echelle,y0-toi_A2.y()*echelle);
    B = QPoint(x0+toi_B2.x()*echelle,y0-toi_B2.y()*echelle);
    C = QPoint(x0+toi_C2.x()*echelle,y0-toi_C2.y()*echelle);
    D = QPoint(x0+toi_D2.x()*echelle,y0-toi_D2.y()*echelle);

    QRectF rectangle2(x0-toi_rayonTonooFictif*echelle,y0-toi_rayonTonooFictif*echelle,2.0*toi_rayonTonooFictif*echelle,2.0*toi_rayonTonooFictif*echelle);
    int spanAngle = degres(toi_angleTonoo)*16;
    int startAngle = degres(toi_angleDebutTonoo)*16.0;
    int nban = (int)(spanAngle/(120*16));
    if(nban>0)
        for(int i=0;i<nban;i++)
            p.drawArc(rectangle2,startAngle+i*120*16,120*16);
    p.drawArc(rectangle2, startAngle+nban*120*16, spanAngle-nban*120*16);

    QRectF rectangle(x0-toi_rayonYourteFictif*echelle,y0-toi_rayonYourteFictif*echelle,2.0*toi_rayonYourteFictif*echelle,2.0*toi_rayonYourteFictif*echelle);
    //p.drawArc(rectangle,degres(toi_angleDebutYourte)*16,degres(toi_angleYourte)*16);
    spanAngle = degres(toi_angleYourte)*16;
    startAngle = degres(toi_angleDebutYourte)*16.0;
    nban = (int)(spanAngle/(10*16));
    if(nban>0)
        for(int i=0;i<nban;i++)
            p.drawArc(rectangle,startAngle+i*10*16,10*16);
    p.drawArc(rectangle, startAngle+nban*10*16, spanAngle-nban*10*16);


    p.drawLine(A,B);
    p.drawLine(C,D);

    //dessin des morceaux
    for(int i=0;i<TsLesMorc.size();i++)
    {
        p.setPen(QPen(couleur(i+2),2));
        p.setFont(QFont("lucida",18,QFont::Bold));
        PointToile ppp = barycentreM(i);
        QPoint X111;
        X111 = QPoint(x0+ppp.x*echelle,y0-ppp.y*echelle);
        QString ste;
        ste = "";
        ste += (char)(i+65);
        p.drawText(X111.x()-14,X111.y()+5,ste);

        for(int j=0;j<TsLesMorc[i].pts.size();j++)
        {
            int k = j+1;
            if(j==TsLesMorc[i].pts.size()-1)
                k = 0;
            if(TsLesMorc[i].trait[j]==2)
            {
                QPoint X1,X2;
                X1 = QPoint(x0+TsLesMorc[i].pts[j].x*echelle,y0-TsLesMorc[i].pts[j].y*echelle);
                X2 = QPoint(x0+TsLesMorc[i].pts[k].x*echelle,y0-TsLesMorc[i].pts[k].y*echelle);
                p.drawLine(X1,X2);
            }
            if(TsLesMorc[i].trait[j]==0)
            {
                double angleDebut = angleHor(TsLesMorc[i].pts[j].x,TsLesMorc[i].pts[j].y);
                double angleParcours = angleHor(TsLesMorc[i].pts[k].x,TsLesMorc[i].pts[k].y);
                if(angleParcours<angleDebut)
                    angleParcours += 2.0*pi;
                angleParcours -= angleDebut;
               // p.drawArc(rectangle,degres(angleDebut)*16,degres(angleParcours)*16);
                spanAngle = degres(angleParcours)*16;
                startAngle = degres(angleDebut)*16.0;
                nban = (int)(spanAngle/(10*16));
                if(nban>0)
                    for(int i=0;i<nban;i++)
                        p.drawArc(rectangle,startAngle+i*10*16,10*16);
                p.drawArc(rectangle, startAngle+nban*10*16, spanAngle-nban*10*16);
            }
            if(TsLesMorc[i].trait[j]==1)
            {
                double angleDebut = angleHor(TsLesMorc[i].pts[j].x,TsLesMorc[i].pts[j].y);
                double angleParcours = angleHor(TsLesMorc[i].pts[k].x,TsLesMorc[i].pts[k].y);
                if(angleParcours<angleDebut)
                    angleParcours += 2.0*pi;
                angleParcours -= angleDebut;
              //  p.drawArc(rectangle2,degres(angleDebut)*16,degres(angleParcours)*16);
            }
            if(TsLesMorc[i].trait[j]==-1)
            {
                double angleDebut = angleHor(TsLesMorc[i].pts[k].x,TsLesMorc[i].pts[k].y);
                double angleParcours = angleHor(TsLesMorc[i].pts[j].x,TsLesMorc[i].pts[j].y);
                if(angleParcours<angleDebut)
                    angleParcours += 2.0*pi;
                angleParcours -= angleDebut;
                spanAngle = degres(angleParcours)*16;
                startAngle = degres(angleDebut)*16.0;
                nban = (int)(spanAngle/(120*16));
                if(nban>0)
                    for(int i=0;i<nban;i++)
                        p.drawArc(rectangle2,startAngle+i*120*16,120*16);
                p.drawArc(rectangle2, startAngle+nban*120*16, spanAngle-nban*120*16);
               // p.drawArc(rectangle2,degres(angleDebut)*16,degres(angleParcours)*16);
            }
        }
    }
}

void ToileYourte::toiDessinToileMorceau(QPaintDevice *des, int ch)
{

    QPainter p(des);
    QRect dim = p.window();
    p.setRenderHint(QPainter::Antialiasing);
    //on cherche le centre
    double x0 = dim.width()/2.0;
    double y0 = dim.height();
    //on cherche le coin en bas à gauche
    double x02 = 20.0;
    double y02 = dim.height()-20.0;
    // on cherche l echelle
    double echelle = (x0-30)/toi_rayonYourteFictif;
    if(((y0-30)/toi_largeurLaize)<echelle)
        echelle = (y0-30)/toi_largeurLaize;
    x02 = (dim.width()-echelle*TsLesMorcDetail[ch].longueur)/2.0;
    y02 = dim.height()-((dim.height()-echelle*toi_largeurLaize)/2.0);
    // d abord on efface en blanc
    p.setPen(QPen(Qt::white,0));
    p.setBrush(QBrush(Qt::white));
    p.drawRect(dim);

    //dessin grille
    p.setPen(QPen(QColor(170,170,170),0.5));
    for(int j=0;j*0.1<TsLesMorcDetail[ch].longueur+0.2;j++)
    {
        QPoint p1,p2;
        p1 = QPoint(x02+j*0.1*echelle,y02);
        p2 = QPoint(p1.x(),y02-toi_largeurLaize*echelle);
        p.drawLine(p1,p2);
    }
    for(int j=0;j*0.1<toi_largeurLaize;j++)
    {
        QPoint p1,p2;
        p1 = QPoint(x02,y02-j*0.1*echelle);
        p2 = QPoint(x02+echelle*(TsLesMorcDetail[ch].longueur+0.2),p1.y());
        p.drawLine(p1,p2);
    }
    p.setPen(QPen(QColor(125,125,125),2));
    p.setFont(QFont("lucida",10));
    for(int j=0;j<TsLesMorcDetail[ch].longueur;j++)
    {
        QPoint p1,p2;
        p1 = QPoint(x02+j*echelle,y02);
        p2 = QPoint(p1.x(),y02-toi_largeurLaize*echelle);
        p.drawLine(p1,p2);
        QString str;
        p.drawText(p1.x()-5,p1.y()+16,str.setNum(j));
    }
    for(int j=0;j<toi_largeurLaize;j++)
    {
        QPoint p1,p2;
        p1 = QPoint(x02,y02-j*echelle);
        p2 = QPoint(x02+echelle*(TsLesMorcDetail[ch].longueur+0.2),p1.y());
        p.drawLine(p1,p2);
        QString str;
        if(j!=0)
            p.drawText(p1.x()-16,p1.y()+5,str.setNum(j));
    }

    QRectF rectangle2(x02+(TsLesMorcDetail[ch].centre0.x()-toi_rayonTonooFictif)*echelle,y02+(-TsLesMorcDetail[ch].centre0.y()-toi_rayonTonooFictif)*echelle,2.0*toi_rayonTonooFictif*echelle,2.0*toi_rayonTonooFictif*echelle);
    QRectF rectangle(x02+(TsLesMorcDetail[ch].centre0.x()-toi_rayonYourteFictif)*echelle,y02+(-TsLesMorcDetail[ch].centre0.y()-toi_rayonYourteFictif)*echelle,2.0*toi_rayonYourteFictif*echelle,2.0*toi_rayonYourteFictif*echelle);

    //dessin des morceaux

    p.setPen(QPen(couleur(ch+2),2));
    p.setFont(QFont("lucida",18,QFont::Bold));
    PointToile ppp = barycentreM2(ch);
    QPoint X111;
    X111 = QPoint(x02+ppp.x*echelle,y02-ppp.y*echelle);
    QString ste;
    ste = "";
    ste += (char)(ch+65);
    p.drawText(X111.x()-14,X111.y()+5,ste);
    for(int j=0;j<TsLesMorcDetail[ch].pts.size();j++)
    {


        int k = j+1;
        if(j==TsLesMorcDetail[ch].pts.size()-1)
            k = 0;
        if(TsLesMorcDetail[ch].trait[j]==2)
        {
            QPoint X1,X2;
            X1 = QPoint(x02+TsLesMorcDetail[ch].pts[j].x*echelle,y02-TsLesMorcDetail[ch].pts[j].y*echelle);
            X2 = QPoint(x02+TsLesMorcDetail[ch].pts[k].x*echelle,y02-TsLesMorcDetail[ch].pts[k].y*echelle);
            p.drawLine(X1,X2);
        }
        if(TsLesMorcDetail[ch].trait[j]==0)
        {
            double angleDebut = angleHorizontalCentrePoint2D_2(TsLesMorcDetail[ch].centre0,QVector2D(TsLesMorcDetail[ch].pts[j].x,TsLesMorcDetail[ch].pts[j].y));
            double angleParcours = angleHorizontalCentrePoint2D_2(TsLesMorcDetail[ch].centre0,QVector2D(TsLesMorcDetail[ch].pts[k].x,TsLesMorcDetail[ch].pts[k].y));
            if(angleParcours<angleDebut)
                angleParcours += 2.0*pi;
            angleParcours -= angleDebut;
            p.drawArc(rectangle,degres(angleDebut)*16,degres(angleParcours)*16);
        }
        if(TsLesMorcDetail[ch].trait[j]==-1)
        {
            double angleDebut = angleHorizontalCentrePoint2D_2(TsLesMorcDetail[ch].centre0,QVector2D(TsLesMorcDetail[ch].pts[k].x,TsLesMorcDetail[ch].pts[k].y));
            double angleParcours = angleHorizontalCentrePoint2D_2(TsLesMorcDetail[ch].centre0,QVector2D(TsLesMorcDetail[ch].pts[j].x,TsLesMorcDetail[ch].pts[j].y));
            if(angleParcours<angleDebut)
                angleParcours += 2.0*pi;
            angleParcours -= angleDebut;
            p.drawArc(rectangle2,degres(angleDebut)*16,degres(angleParcours)*16);
        }
    }
    for(int j=0;j<TsLesMorcDetail[ch].pts.size();j++)
    {
        QPoint ppp;
        ppp = QPoint(x02+TsLesMorcDetail[ch].pts[j].x*echelle,y02-TsLesMorcDetail[ch].pts[j].y*echelle);
        p.save();
        p.setPen(QPen(couleur(0),4));
        p.setFont(QFont("lucida",8));
        p.drawPoint(ppp);
        QString strr;
        if(TsLesMorcDetail[ch].pts[j].y==0.0)
            p.drawText(ppp.x()-10,ppp.y()+12,strr.setNum(j+1));
        else if(TsLesMorcDetail[ch].pts[j].y==toi_largeurLaize)
            p.drawText(ppp.x()-10,ppp.y()-8,strr.setNum(j+1));
        else
            p.drawText(ppp.x()+3,ppp.y()-3,strr.setNum(j+1));
        p.restore();
    }
}

QString ToileYourte::toiCoordMorceau(int ch)
{
    //les tableaux de coordonnees
    QString tableau,str,str2,str3;
    int taille = TsLesMorcDetail[ch].pts.size();
    if(taille/2.0!=qRound(taille/2.0))
        taille +=1;
    taille = taille/2.0;
    tableau = "<table cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
    tableau += "<td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td></tr>";
    for(int i=0;i<taille;i++)
    {
        tableau += "<tr><td bgcolor=\"#cccc99\"><strong>"+str3.setNum(i+1)+"</strong></td><td>"+str.setNum(TsLesMorcDetail[ch].pts[i].x,'f',3)+"</td><td>"+str2.setNum(TsLesMorcDetail[ch].pts[i].y,'f',3)+"</td>";
        if(i+taille<TsLesMorcDetail[ch].pts.size())
            tableau += "<td bgcolor=\"#cccc99\"><strong>"+str3.setNum(i+taille+1)+"</strong></td><td>"+str.setNum(TsLesMorcDetail[ch].pts[i+taille].x,'f',3)+"</td><td>"+str2.setNum(TsLesMorcDetail[ch].pts[i+taille].y,'f',3)+"</td></tr>";
        else
            tableau += "<td bgcolor=\"#cccc99\"></td><td></td><td></td></tr>";
    }
    tableau += "</tbody></table>";

    return tableau;
}

int ToileYourte::toiNombMorceaux()
{
    return TsLesMorcDetail.size();
}

QString ToileYourte::toiExplicationToile()
{
    QString str,str2,str3,str4,str5,resu;
    resu = "<html><p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Avant toute chose, une précision : ce logiciel donne la découpe de la toile pour une héliyourte mais aussi pour une yourte classique si on choisit les bons paramètres (pente de toit, hauteur des murs, diamètre extérieur). Le seul paramètre que l\'on n\'a pas directement dans le cas d\'une héliyourte et que l\'on connait dans le cas d\'une yourte classique est le diamètre du tonoo. Dans ce logiciel, il suffit d\'augmenter le nombre de pans pour obtenir le diamètre souhaité !")+"</p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A) "+QObject::tr("Les Murs")+"</strong></span></p>";
    double peri = pi*toi_diametreYourte;
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Le périmètre de cette héliyourte est de ")+"<strong>"+str.setNum(peri,'f',2)+" m</strong>"+QObject::tr(", la hauteur des murs est de ")+str2.setNum(toi_hauteurMur,'f',2)+QObject::tr(" m, soit une surface minimum de ")+"<strong>"+str3.setNum(toi_hauteurMur*peri,'f',2)+" m²</strong>"+QObject::tr(". Surface à laquelle il faut retirer les surfaces de la porte et des vitres. Toutefois, selon la configuration de votre yourte, il peut être intéressant de rajouter de la surface de toile en bas et en haut des murs. De notre côté, nous avons l\'habitude de compter 15 cm de plus en haut, pour faire un ourlet avec une cordelette à l`intérieur afin de pouvoir serrer le haut du mur juste en bas du toit, au dessus du début des perches. De même, nous rajoutons environ 25 cm en bas de mur, pour venir serrer de la même façon la toile sous le plancher de la yourte (si celle-ci est posée en hauteur)")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Le plus important étant de s\'économiser des coutures, l\'idéal est de trouver le tissu avec une largeur de laize suffisante pour couvrir toute la hauteur du mur. Ou au pire, une largeur de laize égale à la moitié (par ex, pour une yourte avec des murs haut de 1 m 90, une largeur de laize de 1 m 20 est pratique, car une fois deux largeurs cousues ensemble, cela représente environ 2 m 35 soit 45 cm de plus que la hauteur des murs, pour réaliser les ourlets du haut et du bas)")+".</p>";
    resu += "<p align=center><img height=\"565\" width=\"243\" src=\":/images/hly/hlyToileMur\" /></p>";
    resu += "<p><br /></p>";
    double longmax = 0.0;
    for(int i=0;i<TsLesMorcDetail.size();i++)
        longmax += TsLesMorcDetail[i].longueur;
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;B) "+QObject::tr("Le Toit")+"</strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Pour découper tous les morceaux, il faut, au maximum  ")+"<strong>"+str.setNum(longmax,'f',2)+" ml</strong>"+QObject::tr(" (mètres linéaires) de tissu de largeur de laize ")+str2.setNum(toi_largeurLaize,'f',2)+QObject::tr(" m, soit une surface maximum de ")+"<strong>"+str3.setNum(toi_largeurLaize*longmax,'f',2)+" m²</strong>.</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Cette surface peut être réduite d\'une façon simple : après avoir exporté vos résultats en pdf, imprimer le plan global de la toile de toit (celui avec tous les morceaux) puis découpez tous les morceaux séparément. Ensuite il suffit de les placer à la suite, sur une laize de tissu fictive, en essayant de limiter les pertes. Une fois que vous avez placé toutes vos pièces, en mesurant la longueur totale, vous pouvez facilement déduire le métrage de tissu à prévoir (N\'hésitez pas à coller votre \'calpinage\', il vous sera utile pour découper la toile pour de vrai !)")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Les morceaux de toile de toit donnés par ce logiciel ne suffisent pas pour couvrir le toit : il faut rajouter en bas de toit une bande de tissu d\'environ 25 cm de largeur, qui sera d'un côté cousue au toit, et de l\'autre côté avec un gros ourlet dans lequel passe une corde (genre cordelette de 8mm de diamètre) afin de serrer la toile de toit bien proprement sur le haut des murs")+".</p>";
    resu += "<p align=center><img height=\"400\" width=\"600\" src=\":/images/hly/hlyToileVue\" /></p>";
    int divlaize = qRound(toi_largeurLaize/0.25);
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Pour la bande de bas de toit, dans le cas de cette yourte, il vous faut compter ")+"<strong>"+str.setNum(peri/divlaize,'f',2)+" ml</strong>"+QObject::tr(" supplémentaires de tissu de largeur de laize ")+str2.setNum(toi_largeurLaize,'f',2)+QObject::tr(" m, soit ")+"<strong>"+str3.setNum(toi_largeurLaize*peri/divlaize,'f',2)+" m²</strong>"+QObject::tr(", que vous découperez en ")+str4.setNum(divlaize)+QObject::tr(" bandes de ")+str5.setNum(toi_largeurLaize/divlaize*100.0,'f',1)+QObject::tr(" cm de largeur, qui une fois assemblées bout à bout, feront le tour de la yourte.")+".</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C) "+QObject::tr("La Capote")+"</strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Jusqu\'ici nous n\'avons pas abordé le problème de la couverture du haut du tonoo. Cela va dépendre de la finition de votre tonoo, s\'il est vitré et étanche, peut-être que vous n\'aurait pas besoin de capote. Si jamais il vous en faut une (un jour j\'intégrerai une fonction pour dessiner les plans de la capote sur mesure !!!), sachez qu\'il vous faut prévoir au minimum un carré de toile supplémentaire de côté égal à trois fois le diamètre de votre tonoo.")+".</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;D) "+QObject::tr("La Couture")+"</strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Nous vous conseillons d\'utiliser le type de couture D que nous expliquons dans notre document dédié à la couture de toile que vous trouverez ")+"<a href=\"http://ardheia.free.fr/documents_ardheia/toiles.pdf\">"+QObject::tr("ici")+"</a>.</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Après avoir découpé tous les morceaux de toit, vous pouvez les assembler en veillant à respecter le sens d'écoulement de l\'eau. Une fois tous les morceaux assemblés, vous pouvez faire la couture qui \'ferme\' le cone de toit, en gardant toujours en tête l\'écoulement de l\'eau. Puis vous ferez un petit ourlet au niveau du tonoo, et vous rajouterez la bande en bas de toit, sur laquelle vous aurez préalablement fais un ourlet avec la corde dedans (lorsque vous faites un ourlet avec une corde qui passe à l\'intérieur, n'\hésitez pas à faire un ourlet suffisamment grand pour que la corde puisse coulisser, et veillez à passer la corde au fur et à mesure de la réalisation de l\'ourlet, ça sera beaucoup plus simple que de la faire passer à la fin !!!)")+".</p>";



    resu += "<p><br /></p></html>";
    return resu;
}

void ToileYourte::calcul()
{
    toi_perteCouture = 0.045;
    toi_perteCoutureOurlet = 0.03;
    toi_rayonYourteFictif = toi_diametreYourte/(2.0*qCos(toi_penteYourte))+0.04+toi_perteCouture;
    toi_rayonTonooFictif = toi_diametreTonoo/(2.0*qCos(toi_penteYourte))-0.04;
    toi_largeurLaizeUtile = toi_largeurLaize-toi_perteCouture;

    toi_perimetreBasFictif = 2.0*pi*toi_rayonYourteFictif;
    toi_perimetreBas =  toi_perimetreBasFictif*qCos(toi_penteYourte);
    toi_perimetreHautFictif = 2.0*pi*toi_rayonTonooFictif;
    toi_perimetreHaut =  toi_perimetreHautFictif*qCos(toi_penteYourte);

    double angle = radians(60);

    double angleCalcul = angle;
    toi_angleDebutTonoo = qAsin((toi_rayonTonooFictif*qSin(angleCalcul)-toi_perteCouture)/toi_rayonTonooFictif);
    toi_A2 = QVector2D(toi_rayonTonooFictif*qCos(toi_angleDebutTonoo),toi_rayonTonooFictif*qSin(toi_angleDebutTonoo));
    toi_angleTonoo = 2.0*pi*toi_perimetreBas/toi_perimetreBasFictif+angleCalcul-toi_angleDebutTonoo;
    toi_C2 = QVector2D(toi_rayonTonooFictif*qCos(toi_angleDebutTonoo+toi_angleTonoo),toi_rayonTonooFictif*qSin(toi_angleDebutTonoo+toi_angleTonoo));
    angleCalcul = qAsin(toi_rayonTonooFictif*qSin(angle)/toi_rayonYourteFictif);
    toi_angleDebutYourte = qAsin((toi_rayonYourteFictif*qSin(angleCalcul)-toi_perteCouture)/toi_rayonYourteFictif);
    toi_B2 = QVector2D(toi_rayonYourteFictif*qCos(toi_angleDebutYourte),toi_rayonYourteFictif*qSin(toi_angleDebutYourte));
    toi_angleYourte = 2.0*pi*toi_perimetreHaut/toi_perimetreHautFictif+angleCalcul-toi_angleDebutYourte;
    toi_D2 = QVector2D(toi_rayonYourteFictif*qCos(toi_angleDebutYourte+toi_angleYourte),toi_rayonYourteFictif*qSin(toi_angleDebutYourte+toi_angleYourte));

    int deb_compt = -(int)((toi_rayonYourteFictif+3.00)/toi_largeurLaizeUtile+1.00);
    int fin_compt = -deb_compt;

    // alors dans l idee on va tourner notre plan pour que orientation soit à 0
    toi_A2 = rotation2DautourOrigine(toi_A2,-toi_orientation);
    toi_B2 = rotation2DautourOrigine(toi_B2,-toi_orientation);
    toi_C2 = rotation2DautourOrigine(toi_C2,-toi_orientation);
    toi_D2 = rotation2DautourOrigine(toi_D2,-toi_orientation);
    toi_angleDebutTonoo += toi_orientation;
    toi_angleDebutYourte += toi_orientation;

    TsLesMorc.clear();
    TsLesMorcDetail.clear();
    tablLigne.clear();
    for(int i = deb_compt;i<=fin_compt;i++)
    {
        double eq_b = i*toi_largeurLaizeUtile+toi_A2.y()+toi_decalage;
        PointToile ess;
        temp.pts.clear();
        ess = cherchePointGrandCercle(0,eq_b);
        // type 0 grand cercle 1 petit cercle 2 seg AB 3 seg AB droit 4 seg CD 5 seg CD droit
        if(ess.type!=100)
        {
            temp.pts << ess;
            ess = cherchePointGrandCercle(1,eq_b);
            if(ess.type!=100)
                temp.pts << ess;
        }
        ess = cherchePointPetitCercle(0,eq_b);
        if(ess.type!=100)
            {
            temp.pts << ess;
            ess = cherchePointPetitCercle(1,eq_b);
            if(ess.type!=100)
                temp.pts << ess;
        }
        ess = cherchePointSegmentAB(eq_b);
        if(ess.type!=100)
        {
            if(ess.type!=50)
                temp.pts << ess;
            else
            {
                ess.x = toi_A2.x();
                ess.y = toi_A2.y();
                ess.type = 3;
                temp.pts << ess;
                ess.x = toi_B2.x();
                ess.y = toi_B2.y();
                ess.type = 3;
                temp.pts << ess;
            }
        }
        ess = cherchePointSegmentCD(eq_b);
        if(ess.type!=100)
        {
            if(ess.type!=50)
                temp.pts << ess;
            else
            {
                ess.x = toi_C2.x();
                ess.y = toi_C2.y();
                ess.type = 5;
                temp.pts << ess;
                ess.x = toi_D2.x();
                ess.y = toi_D2.y();
                ess.type = 5;
                temp.pts << ess;
            }
        }
        // la on les ordonne
        LignePointToile tempTrie;
        int lon = temp.pts.size();
        for(int j = 0;j<lon;j++)
        {
            int k = cherchePlusAGauche();
            tempTrie.pts << temp.pts[k];
            temp.pts.remove(k);
        }
        if(lon!=0)
            tablLigne << tempTrie;
    }
    //on vient d ajouter tous les points rencontres sur les lignes maintenant faut creer les morceaux de toile
    bool deca = false;
    for(int i=0;i<(tablLigne.size());i++)
    {
        QString str;
        tablLigne[i].ch = "";
        for(int j=0;j<tablLigne[i].pts.size();j++)
            tablLigne[i].ch += str.setNum(tablLigne[i].pts[j].type);
        if(tablLigne[i].ch=="2")
            deca = true;
        if(tablLigne[i].ch=="012")
            tablLigne[i].ch="01";
        if((tablLigne[i].ch=="01210")&&(toi_orientation>0))
        {
            tablLigne[i].ch="0110";
            tablLigne[i].pts[2]=tablLigne[i].pts[3];
            tablLigne[i].pts[3]=tablLigne[i].pts[4];
        }
        if((tablLigne[i].ch=="01210")&&(toi_orientation<0))
        {
            tablLigne[i].ch="0120";
            tablLigne[i].pts[3]=tablLigne[i].pts[4];
        }
        if(tablLigne[i].ch=="01240")
        {
            tablLigne[i].ch="0240";
            tablLigne[i].pts[1]=tablLigne[i].pts[2];
            tablLigne[i].pts[2]=tablLigne[i].pts[3];
            tablLigne[i].pts[3]=tablLigne[i].pts[4];
        }
    }
    if(deca)
    {
        for(int i=0;i<(tablLigne.size()-1);i++)
            tablLigne[i]=tablLigne[i+1];
        tablLigne.remove(tablLigne.size()-1);
    }
    // en fonction des type de points
    PointToile ptA,ptB,ptC,ptD;
    ptA.x = toi_A2.x();
    ptA.y = toi_A2.y();
    ptA.type = 6;
    ptB.x = toi_B2.x();
    ptB.y = toi_B2.y();
    ptB.type = 6;
    ptC.x = toi_C2.x();
    ptC.y = toi_C2.y();
    ptC.type = 6;
    ptD.x = toi_D2.x();
    ptD.y = toi_D2.y();
    ptD.type = 6;

    if(tablLigne.size()==0)
    {
        MorceauToile mt;
        mt.pts << ptD << ptC << ptA << ptB;
        mt.trait << 2 << -1 << 2 << 0;
        TsLesMorc << mt;
    }
    else
    {
        //en bas
        if((tablLigne[0].ch=="00"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << tablLigne[0].pts[1];
            mt.trait << 0 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="04"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="01"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << ptC << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << -1 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="0112"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << ptC << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << -1 << 2;
            TsLesMorc << mt;
            mt.pts.clear();mt.trait.clear();
            mt.pts << ptA << tablLigne[0].pts[3] << tablLigne[0].pts[2];
            mt.trait << 2 << 2 << -1;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="0114"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << tablLigne[0].pts[3] << tablLigne[0].pts[2] << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << 2 << -1 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="0133"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << ptC << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << -1 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="0155"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << ptC << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << -1 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="0110"))
        {
            MorceauToile mt;
            if(tablLigne[0].pts[0].y<ptD.y)
            {
                mt.pts << tablLigne[0].pts[0] << tablLigne[0].pts[3] << tablLigne[0].pts[2] << tablLigne[0].pts[1];
                mt.trait << 0 << 2 << -1 << 2;
                TsLesMorc << mt;
            }
            else
            {
                mt.pts << tablLigne[0].pts[0] << ptD << ptC << tablLigne[0].pts[1];
                mt.trait << 0 << 2 << -1 << 2;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << ptB << tablLigne[0].pts[3] << tablLigne[0].pts[2];
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
        }
        if((tablLigne[0].ch=="0420"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << 2;
            TsLesMorc << mt;
            mt.pts.clear();mt.trait.clear();
            mt.pts << tablLigne[0].pts[2] << ptB << tablLigne[0].pts[3];
            mt.trait << 2 << 0 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="02"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << ptC << ptA << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << -1 << 2 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="0210"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << tablLigne[0].pts[3] << tablLigne[0].pts[2] << ptA << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << -1 << 2 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="0240"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0]  << tablLigne[0].pts[3] << tablLigne[0].pts[2] << ptC << ptA << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << 2 << -1 << 2 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="0120"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << ptC << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << -1 << 2;
            TsLesMorc << mt;
            mt.pts.clear();mt.trait.clear();
            mt.pts << tablLigne[0].pts[2] << ptB << tablLigne[0].pts[3];
            mt.trait << 2 << 0 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="20"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptB << tablLigne[0].pts[1];
            mt.trait << 2 << 0 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[0].ch=="011420"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[0].pts[0] << ptD << tablLigne[0].pts[3] << tablLigne[0].pts[2] << tablLigne[0].pts[1];
            mt.trait << 0 << 2 << 2 << -1 << 2;
            TsLesMorc << mt;
            mt.pts.clear();mt.trait.clear();
            mt.pts << tablLigne[0].pts[4] << ptB << tablLigne[0].pts[5];
            mt.trait << 2 << 0 << 2;
            TsLesMorc << mt;
        }
        //au milieu
        for(int i=0;i<(tablLigne.size()-1);i++)
        {
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if(((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="04"))||((tablLigne[i].ch=="02")&&(tablLigne[i+1].ch=="02")))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="20")&&(tablLigne[i+1].ch=="20"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="02"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << ptC;
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if(((tablLigne[i].ch=="0420")&&(tablLigne[i+1].ch=="0420"))||((tablLigne[i].ch=="0240")&&(tablLigne[i+1].ch=="0240")))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="04"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0133")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if(((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="0133"))||((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="01")))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="0114"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if(((tablLigne[i].ch=="0114")&&(tablLigne[i+1].ch=="01"))||((tablLigne[i].ch=="0114")&&(tablLigne[i+1].ch=="0133")))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptC;
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0114")&&(tablLigne[i+1].ch=="0114"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0114")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptC;
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0114")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << ptB << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptC;
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="01")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << ptB << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << ptA << ptB << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0114")&&(tablLigne[i+1].ch=="02"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptC;
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="02")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptB << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="01")&&(tablLigne[i+1].ch=="02"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="02"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0112")&&(tablLigne[i+1].ch=="02"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0112")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptB << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0114")&&(tablLigne[i+1].ch=="0112"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptC;
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="01")&&(tablLigne[i+1].ch=="0112"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="0112"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="0112"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="01"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="0114"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="0133"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="0155"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="0420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="02"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << ptC << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="0210"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="0240"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << ptC << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="0120"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="00")&&(tablLigne[i+1].ch=="011420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptD << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 0 << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptB << tablLigne[i+1].pts[5] << tablLigne[i+1].pts[4];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="0420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="0120"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="20"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="04")&&(tablLigne[i+1].ch=="011420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptB << tablLigne[i+1].pts[5] << tablLigne[i+1].pts[4];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="01")&&(tablLigne[i+1].ch=="01"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="01")&&(tablLigne[i+1].ch=="0133"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="01")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="01")&&(tablLigne[i+1].ch=="0120"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts <<  ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0112")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] <<  ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0112")&&(tablLigne[i+1].ch=="0112"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0114")&&(tablLigne[i+1].ch=="0120"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptC;
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0114")&&(tablLigne[i+1].ch=="011420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << 2 << -1;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptB << tablLigne[i+1].pts[5] << tablLigne[i+1].pts[4];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0133")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0155")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << ptB << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0155")&&(tablLigne[i+1].ch=="02"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0420")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << ptA << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0420")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << ptA;
                mt.trait << 2 << 0 << 2 << -1 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0420")&&(tablLigne[i+1].ch=="0120"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0420")&&(tablLigne[i+1].ch=="011420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[5] << tablLigne[i+1].pts[4];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0210")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptB << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << ptC;
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0210")&&(tablLigne[i+1].ch=="02"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << ptC;
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0210")&&(tablLigne[i+1].ch=="0210"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0210")&&(tablLigne[i+1].ch=="0240"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << ptC;
                mt.trait << 2 << 0 << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0240")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptB << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD;
                mt.trait << 2 << 0 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0240")&&(tablLigne[i+1].ch=="02"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD;
                mt.trait << 2 << 0 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0120")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0120")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << ptA;
                mt.trait << 2 << 0 << 2 << -1 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0120")&&(tablLigne[i+1].ch=="0120"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="20")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0] << ptD << ptC << ptA;
                mt.trait << 2 << 0 << 2 << 0 << 2 << -1 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="20")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << ptA;
                mt.trait << 2 << 0 << 2 << -1 << 2;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptD << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="20")&&(tablLigne[i+1].ch=="0420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptD << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="20")&&(tablLigne[i+1].ch=="0120"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptD << ptC << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="20")&&(tablLigne[i+1].ch=="011420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[5] << tablLigne[i+1].pts[4];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptD << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="011420")&&(tablLigne[i+1].ch=="00"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << tablLigne[i].pts[4] << tablLigne[i].pts[5] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 0 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptC;
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="011420")&&(tablLigne[i+1].ch=="0110"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[4] << tablLigne[i].pts[5] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << ptA;
                mt.trait << 2 << 0 << 2 << -1 << 2;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptC;
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="011420")&&(tablLigne[i+1].ch=="0120"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[4] << tablLigne[i].pts[5] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptC;
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="011420")&&(tablLigne[i+1].ch=="011420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[4] << tablLigne[i].pts[5] << tablLigne[i+1].pts[5] << tablLigne[i+1].pts[4];
                mt.trait << 2 << 0 << 2 << 2;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="01"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << ptC;
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="0112"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << ptC;
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptA << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="0114"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="0133"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << ptC;
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="0155"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << ptC;
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="0210"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="0240"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << ptA << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2] << ptC;
                mt.trait << 2 << 0 << 2 << 2 << -1;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="0120"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << ptC;
                mt.trait << 2 << 0 << 2 << -1;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptB << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }
            if((tablLigne[i].ch=="0110")&&(tablLigne[i+1].ch=="011420"))
            {
                MorceauToile mt;
                mt.pts << tablLigne[i].pts[0] << tablLigne[i].pts[1] << tablLigne[i+1].pts[1] << tablLigne[i+1].pts[0];
                mt.trait << 2 << -1 << 2 << 0;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << tablLigne[i].pts[2] << tablLigne[i].pts[3] << ptD << tablLigne[i+1].pts[3] << tablLigne[i+1].pts[2];
                mt.trait << 2 << 0 << 2 << 2 << -1;
                TsLesMorc << mt;
                mt.pts.clear();mt.trait.clear();
                mt.pts << ptB << tablLigne[i+1].pts[5] << tablLigne[i+1].pts[4];
                mt.trait << 0 << 2 << 2;
                TsLesMorc << mt;
            }

        }
        //en haut
        int hhh = tablLigne.size()-1;
        if((tablLigne[hhh].ch=="00"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1];
            mt.trait << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="02"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1]<< ptB;
            mt.trait << 2 << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="04"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptC << ptA << ptB;
            mt.trait << 2 << 2 << -1 << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="01"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptA << ptB;
            mt.trait << 2 << -1 << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="0112"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << tablLigne[hhh].pts[2] << tablLigne[hhh].pts[3] << ptB;
            mt.trait << 2 << -1 << 2 << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="0114"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptA << ptB;
            mt.trait << 2 << -1 << 2 << 0;
            TsLesMorc << mt;
            mt.pts.clear();mt.trait.clear();
            mt.pts << tablLigne[hhh].pts[2] << tablLigne[hhh].pts[3] << ptC;
            mt.trait << 2 << 2 << -1;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="0133"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptA << ptB;
            mt.trait << 2 << -1 << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="0155"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptA << ptB;
            mt.trait << 2 << -1 << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="0110"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << tablLigne[hhh].pts[2] << tablLigne[hhh].pts[3];
            mt.trait << 2 << -1 << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="0420"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptC << ptA << tablLigne[hhh].pts[2] << tablLigne[hhh].pts[3];
            mt.trait << 2 << 2 << -1 << 2 << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="0210"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptB;
            mt.trait << 2 << 2 << 0;
            TsLesMorc << mt;
            mt.pts.clear();mt.trait.clear();
            mt.pts << tablLigne[hhh].pts[2] << tablLigne[hhh].pts[3] << ptD << ptC;
            mt.trait << 2 << 0 << 2 << -1;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="0240"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptB;
            mt.trait << 2 << 2 << 0;
            TsLesMorc << mt;
            mt.pts.clear();mt.trait.clear();
            mt.pts << tablLigne[hhh].pts[2] << tablLigne[hhh].pts[3] << ptD;
            mt.trait << 2 << 0 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="0120"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptA << tablLigne[hhh].pts[2] << tablLigne[hhh].pts[3];
            mt.trait << 2 << -1 << 2 << 2 << 0;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="20"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptD << ptC << ptA;
            mt.trait << 2 << 0 << 2 << -1 << 2;
            TsLesMorc << mt;
        }
        if((tablLigne[hhh].ch=="011420"))
        {
            MorceauToile mt;
            mt.pts << tablLigne[hhh].pts[0] << tablLigne[hhh].pts[1] << ptA << tablLigne[hhh].pts[4] << tablLigne[hhh].pts[5];
            mt.trait << 2 << -1 << 2 << 2 << 0;
            TsLesMorc << mt;
            mt.pts.clear();mt.trait.clear();
            mt.pts << tablLigne[hhh].pts[2] << tablLigne[hhh].pts[3] << ptC;
            mt.trait << 2 << 2 << -1;
            TsLesMorc << mt;
        }
    }

    for(int i=0;i<TsLesMorc.size();i++)
    {
        MorceauToile mt;
        for(int j=0;j<TsLesMorc[i].pts.size();j++)
        {
            if(TsLesMorc[i].trait[j]==2)
            {
                mt.trait << 2;
                mt.pts << TsLesMorc[i].pts[j];
            }
            if(TsLesMorc[i].trait[j]==0)
            {
                QVector2D pdeb,pfin;
                pdeb = QVector2D(TsLesMorc[i].pts[j].x,TsLesMorc[i].pts[j].y);
                if(j==TsLesMorc[i].pts.size()-1)
                    pfin = QVector2D(TsLesMorc[i].pts[0].x,TsLesMorc[i].pts[0].y);
                else
                    pfin = QVector2D(TsLesMorc[i].pts[j+1].x,TsLesMorc[i].pts[j+1].y);
                double distance = (pfin-pdeb).length();
                int nbdivisions = qRound(distance/0.3);
                double angptdeb,angptfin,angdiff;
                angptdeb = angleHorizontalCentrePoint2D_2(QVector2D(0.0,0.0),pdeb);
                angptfin = angleHorizontalCentrePoint2D_2(QVector2D(0.0,0.0),pfin);
                angdiff = angleDeDifferenceDuPremierVersLeDeuxieme(angptdeb,angptfin,false);
                mt.trait << 0;
                mt.pts << TsLesMorc[i].pts[j];
                for(int k=1;k<nbdivisions;k++)
                {
                    PointToile pttt;
                    pttt.x = toi_rayonYourteFictif*qCos(angptdeb+(k)*angdiff/nbdivisions);
                    pttt.y = toi_rayonYourteFictif*qSin(angptdeb+(k)*angdiff/nbdivisions);
                    mt.trait << 0;
                    mt.pts << pttt;
                }
            }
            if(TsLesMorc[i].trait[j]==-1)
            {
                QVector2D pdeb,pfin;
                pdeb = QVector2D(TsLesMorc[i].pts[j].x,TsLesMorc[i].pts[j].y);
                if(j==TsLesMorc[i].pts.size()-1)
                    pfin = QVector2D(TsLesMorc[i].pts[0].x,TsLesMorc[i].pts[0].y);
                else
                    pfin = QVector2D(TsLesMorc[i].pts[j+1].x,TsLesMorc[i].pts[j+1].y);
                double distance = (pfin-pdeb).length();
                int nbdivisions = qRound(distance/0.3);
                double angptdeb,angptfin,angdiff;
                angptdeb = angleHorizontalCentrePoint2D_2(QVector2D(0.0,0.0),pdeb);
                angptfin = angleHorizontalCentrePoint2D_2(QVector2D(0.0,0.0),pfin);
                angdiff = angleDeDifferenceDuPremierVersLeDeuxieme(angptdeb,angptfin,true);
                mt.trait << -1;
                mt.pts << TsLesMorc[i].pts[j];
                for(int k=1;k<nbdivisions;k++)
                {
                    PointToile pttt;
                    pttt.x = toi_rayonTonooFictif*qCos(angptdeb-(k)*angdiff/nbdivisions);
                    pttt.y = toi_rayonTonooFictif*qSin(angptdeb-(k)*angdiff/nbdivisions);
                    mt.trait << -1;
                    mt.pts << pttt;
                }
            }
        }
        TsLesMorcDetail << mt;
    }

    // ca y est on a les morceaux separes a centrer sur la laize
    // maintenant il faut les translater
    // ne pas oublier de prevoir la bande de retour en bas

    for(int i=0;i<TsLesMorcDetail.size();i++)
    {
        double xmin,xmax,ymin,ymax;
        xmin = 3000;
        xmax = -3000;
        ymin = 3000;
        ymax = -3000;
        for(int j=0;j<TsLesMorcDetail[i].pts.size();j++)
        {
            TsLesMorcDetail[i].pts0 << TsLesMorcDetail[i].pts[j];
            if(TsLesMorcDetail[i].pts[j].x<xmin)
                xmin = TsLesMorcDetail[i].pts[j].x;
            if(TsLesMorcDetail[i].pts[j].x>xmax)
                xmax = TsLesMorcDetail[i].pts[j].x;
            if(TsLesMorcDetail[i].pts[j].y<ymin)
                ymin = TsLesMorcDetail[i].pts[j].y;
            if(TsLesMorcDetail[i].pts[j].y>ymax)
                ymax = TsLesMorcDetail[i].pts[j].y;
        }
        TsLesMorcDetail[i].longueur = xmax-xmin;
        for(int j=0;j<TsLesMorcDetail[i].pts.size();j++)
        {
            TsLesMorcDetail[i].pts[j].x -= xmin;
            TsLesMorcDetail[i].centre0.setX(-xmin);
            if((ymax-ymin)==toi_largeurLaizeUtile)
            {
                TsLesMorcDetail[i].pts[j].y -= ymin;
                TsLesMorcDetail[i].centre0.setY(-ymin);
            }
            else if(TsLesMorcDetail[i].trait[TsLesMorcDetail[i].trait.size()-1]==2)
            {
                TsLesMorcDetail[i].pts[j].y -= ymax-toi_largeurLaizeUtile;
                TsLesMorcDetail[i].centre0.setY(-(ymax-toi_largeurLaizeUtile));
            }
            else
            {
                TsLesMorcDetail[i].pts[j].y -= ymin;
                TsLesMorcDetail[i].centre0.setY(-ymin);
            }
        }
    }
    for(int i=0;i<TsLesMorcDetail.size();i++)
    {
        TsLesMorcDetail[i].centre0.setY(TsLesMorcDetail[i].centre0.y()+toi_perteCouture/2.0);
        for(int j=0;j<TsLesMorcDetail[i].pts.size();j++)
        {
            TsLesMorcDetail[i].pts[j].y += toi_perteCouture/2.0;
        }
    }
    for(int i=0;i<TsLesMorcDetail.size();i++)
    {
        MorceauToile morctamp;
        morctamp.pts << TsLesMorcDetail[i].pts[0];
        morctamp.trait << TsLesMorcDetail[i].trait[0];
        for(int j=1;j<TsLesMorcDetail[i].pts.size();j++)
        {
            if((TsLesMorcDetail[i].pts[j].y==TsLesMorcDetail[i].pts[j-1].y)&&(TsLesMorcDetail[i].pts[j].y<=toi_perteCouture/2.0+0.1))
            {
                PointToile pp1,pp2;
                pp1.x = TsLesMorcDetail[i].pts[j-1].x;
                pp2.x = TsLesMorcDetail[i].pts[j].x;
                pp1.y = 0.0;
                pp2.y = 0.0;
                morctamp.pts << pp1 << pp2;
                morctamp.trait << 2 << 2;
            }
            if((TsLesMorcDetail[i].pts[j].y==TsLesMorcDetail[i].pts[j-1].y)&&(TsLesMorcDetail[i].pts[j].y>=(-toi_perteCouture/2.0+toi_largeurLaize-0.01)))
            {
                PointToile pp1,pp2;
                pp1.x = TsLesMorcDetail[i].pts[j-1].x;
                pp2.x = TsLesMorcDetail[i].pts[j].x;
                pp1.y = toi_largeurLaize;
                pp2.y = toi_largeurLaize;
                morctamp.pts << pp1 << pp2;
                morctamp.trait << 2 << 2;
            }
            morctamp.pts << TsLesMorcDetail[i].pts[j];
            morctamp.trait << TsLesMorcDetail[i].trait[j];
            if((j==(TsLesMorcDetail[i].pts.size()-1))&&(TsLesMorcDetail[i].pts[j].y==TsLesMorcDetail[i].pts[0].y)&&(TsLesMorcDetail[i].pts[j].y>=(-toi_perteCouture/2.0+toi_largeurLaize-0.01)))
            {
                PointToile pp1,pp2;
                pp1.x = TsLesMorcDetail[i].pts[j].x;
                pp2.x = TsLesMorcDetail[i].pts[0].x;
                pp1.y = toi_largeurLaize;
                pp2.y = toi_largeurLaize;
                morctamp.pts << pp1 << pp2;
                morctamp.trait << 2 << 2;
            }
        }
        TsLesMorcDetail[i].pts.clear();
        TsLesMorcDetail[i].pts = morctamp.pts;
        TsLesMorcDetail[i].trait.clear();
        TsLesMorcDetail[i].trait = morctamp.trait;
    }
}

double ToileYourte::angleHor(double x, double y)
{
    double ret;
    if(x==0)
        if(y>0)
            ret = pi/2.0;
        else
            ret = 3.0*pi/2.0;
    else if(y==0)
        if(x>0)
            ret = 0;
        else
            ret = pi;
    else
    {
        ret = qAtan(qAbs(y/x));
        if((x<0)&&(y>0))
            ret = pi-ret;
        if((x<0)&&(y<0))
            ret += pi;
        if((x>0)&&(y<0))
            ret = 2.0*pi-ret;
    }
    return ret;
}

PointToile ToileYourte::cherchePointGrandCercle(int num, double y)
{
    PointToile ret;
    ret.type = 100;
    if((y<toi_rayonYourteFictif)&&(y>-toi_rayonYourteFictif))
    {
        ret.y = y;
        double alp;
        if(num==0)
            alp = pi-qAsin(ret.y/toi_rayonYourteFictif);
        else
            alp = qAsin(ret.y/toi_rayonYourteFictif);
        if(alp<0)
            alp += 2.0*pi;
        double alp2 = alp + 2.0*pi;
        double alp3 = alp - 2.0*pi;
        ret.x = toi_rayonYourteFictif*qCos(alp);
        if(((alp>toi_angleDebutYourte)&&(alp<(toi_angleDebutYourte+toi_angleYourte)))||((alp2>toi_angleDebutYourte)&&(alp2<(toi_angleDebutYourte+toi_angleYourte)))||((alp3>toi_angleDebutYourte)&&(alp3<(toi_angleDebutYourte+toi_angleYourte))))
            ret.type = 0;
    }
    return ret;
}

PointToile ToileYourte::cherchePointPetitCercle(int num, double y)
{
    PointToile ret;
    ret.type = 100;
    if((y<toi_rayonTonooFictif)&&(y>-toi_rayonTonooFictif))
    {
        ret.y = y;
        double alp;
        if(num==0)
            alp = pi-qAsin(ret.y/toi_rayonTonooFictif);
        else
            alp = qAsin(ret.y/toi_rayonTonooFictif);
        if(alp<0)
            alp += 2.0*pi;
        double alp2 = alp + 2.0*pi;
        double alp3 = alp - 2.0*pi;
        ret.x = toi_rayonTonooFictif*qCos(alp);
        if(((alp>toi_angleDebutTonoo)&&(alp<(toi_angleDebutTonoo+toi_angleTonoo)))||((alp2>toi_angleDebutTonoo)&&(alp2<(toi_angleDebutTonoo+toi_angleTonoo)))||((alp3>toi_angleDebutTonoo)&&(alp3<(toi_angleDebutTonoo+toi_angleTonoo))))
            ret.type = 1;
    }
    return ret;
}

PointToile ToileYourte::cherchePointSegmentAB(double y)
{
    PointToile ret;
    ret.type = 100;
    double a,b;
    if(toi_B2.x()!=toi_A2.x())
    {
        a = (toi_B2.y()-toi_A2.y())/(toi_B2.x()-toi_A2.x());
        b = toi_A2.y()-a*toi_A2.x();
    }
    if(((toi_B2.y()>toi_A2.y())&&(y<=toi_B2.y())&&(y>=toi_A2.y()))||((toi_B2.y()<toi_A2.y())&&(y>=toi_B2.y())&&(y<=toi_A2.y())))
    {
        ret.y = y;
        if(toi_B2.x()!=toi_A2.x())
            ret.x = (y-b)/a;
        else
            ret.x = toi_A2.x();
        ret.type = 2;
    }
    if((toi_B2.y()==toi_A2.y())&&(toi_B2.y()==y))
        ret.type = 50;
    return ret;
}

PointToile ToileYourte::cherchePointSegmentCD(double y)
{
    PointToile ret;
    ret.type = 100;
    double a,b;
    if(toi_D2.x()!=toi_C2.x())
    {
        a = (toi_D2.y()-toi_C2.y())/(toi_D2.x()-toi_C2.x());
        b = toi_C2.y()-a*toi_C2.x();
    }
    if(((toi_D2.y()>toi_C2.y())&&(y<=toi_D2.y())&&(y>=toi_C2.y()))||((toi_D2.y()<toi_C2.y())&&(y>=toi_D2.y())&&(y<=toi_C2.y())))
    {
        ret.y = y;
        if(toi_D2.x()!=toi_C2.x())
            ret.x = (y-b)/a;
        else
            ret.x = toi_C2.x();
        ret.type = 4;
    }
    if((toi_D2.y()==toi_C2.y())&&(toi_D2.y()==y))
        ret.type = 50;
    return ret;
}

int ToileYourte::cherchePlusAGauche()
{
    int res = 0;
    if(temp.pts.size()>1)
        for(int l=1;l<temp.pts.size();l++)
            if(temp.pts[l].x<temp.pts[res].x)
                res = l;
    return res;
}

PointToile ToileYourte::barycentreM(int ij)
{
    PointToile res;
    double x = 0.0;
    double y = 0.0;
    for(int i = 0;i<TsLesMorcDetail[ij].pts0.size();i++)
    {
        x +=TsLesMorcDetail[ij].pts0[i].x;
        y +=TsLesMorcDetail[ij].pts0[i].y;
    }
    res.x = x/TsLesMorcDetail[ij].pts0.size();
    res.y = y/TsLesMorcDetail[ij].pts0.size();
    return res;
}

PointToile ToileYourte::barycentreM2(int ij)
{
    PointToile res;
    double x = 0.0;
    double y = 0.0;
    for(int i = 0;i<TsLesMorcDetail[ij].pts.size();i++)
    {
        x +=TsLesMorcDetail[ij].pts[i].x;
        y +=TsLesMorcDetail[ij].pts[i].y;
    }
    res.x = x/TsLesMorcDetail[ij].pts.size();
    res.y = y/TsLesMorcDetail[ij].pts.size();
    return res;
}

QStringList ToileYourte::listeMorceaux()
{
    QStringList m_liste;
    for(int i=0;i<TsLesMorc.size();i++)
    {
        m_liste << QObject::tr("Morceau")+" "+(char)(i+65);
    }
    return m_liste;
}
