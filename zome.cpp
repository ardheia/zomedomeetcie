#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <qmath.h>

#include "complexes.h"
#include "mathema.h"
#include "zome.h"

zome::zome()
{
}

void zome::setParametres(int z_methodeConstruction, int z_nombreOrdre, double z_nombreForme, double z_echelle, int z_rangees, double z_epaisseurChevronsMethode1, double z_retombeeChevronsMethode1)
{
    zom_methodeConstruction = z_methodeConstruction;
    zom_nombreOrdre = z_nombreOrdre;
    zom_nombreForme = z_nombreForme;
    zom_petiteDiagonaleRangee1 = z_echelle;
    zom_rangees = z_rangees;
    zom_epaisseurChevronsMethode1 = z_epaisseurChevronsMethode1/100.0;
    zom_retombeeChevronsMethode1 = z_retombeeChevronsMethode1/100.0;

    rang.clear();
    calcul();
}

double zome::hauteur()
{
    return zom_hauteurTotale;
}

double zome::surfaceAuSol()
{
    double surf;
    surf = zom_nombreOrdre*rang[rang.size()-1].rayonInt*qSqrt(qPow(rang[rang.size()-1].rayonExt,2.0)-qPow(rang[rang.size()-1].rayonInt,2.0));
    return surf;
}

double zome::surfaceDeToit()
{
    double surf = 0;
    for(int i=0;i<zom_rangees;i++)
    {
        if(i!=(zom_nombreOrdre-1))
        {
            double test = los[i].diagHoriz*los[i].diagVert/2.0;
            surf += zom_nombreOrdre*los[i].diagHoriz*los[i].diagVert/2.0;
        }
    }
    return surf;
}

double zome::volume()
{
    double vol = 0;
    QVector3D cent(0.0,0.0,0.0);
    for(int i=0;i<zom_rangees;i++)
    {
        if(i!=(zom_nombreOrdre-1))
            vol += zom_nombreOrdre*los[i].diagHoriz*los[i].diagVert*qAbs(cent.distanceToPlane(los[i].barycentre,los[i].Vn))/(2.0*3.0);
    }
    return vol;
}

bool zome::triangle(int num)
{
    if((num==(zom_rangees-1))&&(zom_rangees!=zom_nombreOrdre))
        return true;
    else
        return false;
}

double zome::pointPlusEloigne()
{
    return zom_hauteurTotale*2.0;
}

void zome::valeursZomeGenerales(QVector<QVector3D> &passageValeur)
{
    QVector3D tempo;
    //0
    tempo.setX(zom_nombreOrdre);tempo.setY(zom_rangees);tempo.setZ(zom_methodeConstruction);
    passageValeur << tempo;
    //1
    tempo.setX(zom_retombeeChevronsMethode1);tempo.setY(zom_epaisseurChevronsMethode1);tempo.setZ(0.0);
    passageValeur << tempo;
}

void zome::valeursSommets(QVector<rangee> &passageRangee)
{
    passageRangee.clear();
    passageRangee = rang;
}

void zome::valeursMethode1(QVector<losange_m1> &passageMethode1)
{
    passageMethode1.clear();
    passageMethode1 = los_m1;
}

QString zome::debit()
{
    QString resu,str,str2,str3;
    double longueurBrutMax,mldiagvert,mldiaghoriz;

    switch(zom_methodeConstruction)
    {
    case 0: //methode losange
        resu = "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Les losanges</strong></span></p>";
        if(zom_rangees<zom_nombreOrdre)
        {
            resu += "<p>Il y a en tout <strong>"+str.setNum((double)(zom_rangees-1),'f',0)+"</strong> rangees de losanges complets, avec à chaque fois <strong>"+str2.setNum((double)zom_nombreOrdre,'f',0)+"</strong> losanges par rangée soit un total de <strong>"+str3.setNum((double)(zom_nombreOrdre*(zom_rangees-1)),'f',0)+"</strong> losanges complets.<br />";
            resu += "Il y a aussi la première rangée constituée de <strong>"+str.setNum((double)zom_nombreOrdre,'f',0)+"</strong> triangles</p>";
        }
        else
        {
            resu += "<p>Il y a en tout <strong>"+str.setNum((double)(zom_rangees-1),'f',0)+"</strong> rangees de losanges complets, avec à chaque fois <strong>"+str2.setNum((double)zom_nombreOrdre,'f',0)+"</strong> losanges par rangée soit un total de <strong>"+str3.setNum((double)(zom_nombreOrdre*(zom_rangees-1)),'f',0)+"</strong> losanges complets.</p>";
         }
        resu += "<p>Les planches (4 par losange) pour tailler ces losanges auront au maximum les dimensions suivantes :<br />";
        resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- epaisseur : <strong>"+str.setNum(zom_epaisseurChevronsMethode1*100.0,'f',1)+"</strong> cm<br />";
        resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- retombee : <strong>"+str.setNum(zom_retombeeChevronsMethode1*100.0,'f',1)+"</strong> cm<br />";
        longueurBrutMax = 0;
        for(int i=0;i<zom_rangees;i++)
        {
            if(los_m1[i].longueurPlancheBrut>longueurBrutMax)
                longueurBrutMax = los_m1[i].longueurPlancheBrut;
        }
        resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur la plus grande : <strong>"+str.setNum(longueurBrutMax*100.0,'f',1)+"</strong> cm</p>";
        double debitLos,metrageLin;
        if(zom_rangees<zom_nombreOrdre)
        {
            metrageLin = ((zom_rangees-1)*4+2)*zom_nombreOrdre*longueurBrutMax+zom_nombreOrdre*los_m1[zom_rangees-1].DiagHorizInt;
            debitLos = metrageLin*zom_epaisseurChevronsMethode1*zom_retombeeChevronsMethode1;
            resu += "<p>Il faut donc <strong>"+str.setNum((double)((zom_rangees-1)*4+2)*zom_nombreOrdre,'f',0)+"</strong> planches de cette longueur, et aussi <strong>"+str2.setNum((double)zom_nombreOrdre,'f',0)+"</strong> planches de longueur <strong>"+str3.setNum(los_m1[zom_rangees-1].DiagHorizInt*100.0,'f',2)+"</strong> cm qui constitueront les bas des triangles.</p>";
        }
        else
        {
             metrageLin = (zom_rangees-1)*4*zom_nombreOrdre*longueurBrutMax;
             debitLos = metrageLin*zom_epaisseurChevronsMethode1*zom_retombeeChevronsMethode1;
             resu += "<p>Il faut donc <strong>"+str.setNum((double)((zom_rangees-1)*4)*zom_nombreOrdre,'f',0)+"</strong> planches de cette longueur.</p>";
        }
        resu += "<p>Ce qui nous donne un métrage linéaire total de <strong>"+str2.setNum(metrageLin,'f',2)+"</strong> m et un débit total de <strong>"+str.setNum(debitLos,'f',2)+"</strong> m3.</p>";
        resu += "<p></p>";

        mldiagvert = 0;
        mldiaghoriz = 0;
        for(int i=0;i<(zom_rangees-1);i++)
        {
            mldiagvert += los_m1[i].DiagVertInt;
            mldiaghoriz += los_m1[i].DiagHorizInt;
        }

        resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Les diagonales pour contreventer les losanges</strong></span></p>";
        resu += "<p>Si vous souhaitez contreventer les losanges avec des diagonales verticales, vous aurez alors <strong>"+str.setNum(mldiagvert*zom_nombreOrdre,'f',1)+"</strong> ml de planches supplémentaire à prévoir, soit un débit de <strong>"+str2.setNum(mldiagvert*zom_nombreOrdre*zom_retombeeChevronsMethode1*zom_epaisseurChevronsMethode1,'f',2)+"</strong> m3.</p>";
        resu += "<p>Si vous souhaitez contreventer les losanges avec des diagonales horizontales, vous aurez alors <strong>"+str.setNum(mldiaghoriz*zom_nombreOrdre,'f',1)+"</strong> ml de planches supplémentaire à prévoir, soit un débit de <strong>"+str2.setNum(mldiaghoriz*zom_nombreOrdre*zom_retombeeChevronsMethode1*zom_epaisseurChevronsMethode1,'f',2)+"</strong> m3.</p>";

        break;
    case 1: //juste visualisation
        resu = "<p>Pour cette méthode de construction, toutes les infos sont rassemblées dans l\'onglet \"Aide à la construction\"</p>";
        break;
    }

    return resu;
}

QString zome::explicationLosange(int num)
{
    QString resu;
    QString str,str2,str3;
    switch(zom_methodeConstruction)
    {
    case 0: //methode losange
        if((zom_nombreOrdre!=zom_rangees)&&(num==(zom_rangees-1)))
        {
            resu = "<p>Il y a <strong>"+str.setNum((double)zom_nombreOrdre,'f',0)+"</strong> triangles sur la rangée n° <strong>"+str2.setNum((double)(num+1),'f',0)+"</strong>.<br />Un triangle est composé de 3 chevrons différents, aux dimensions suivantes :<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- épaisseur (commune aux 3) : <strong>" + str.setNum(zom_epaisseurChevronsMethode1*100.0,'f',1)+"</strong> cm<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- retombée (commune aux 3) : <strong>" + str.setNum(zom_retombeeChevronsMethode1*100.0,'f',1)+"</strong> cm<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur L1 = L1\' : <strong>" + str.setNum(los_m1[num].longueur*100.0,'f',1)+"</strong> cm<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur L3 : <strong>" + str.setNum(los_m1[num].DiagHorizInt*100.0,'f',1)+"</strong> cm<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- angle alpha1 : <strong>" + str.setNum(degres(los_m1[num].beta),'f',1)+"</strong> °<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- angle alpha2 = alpha : <strong>" + str.setNum(degres(los_m1[num].alpha/2.0),'f',1)+"</strong> °</p>";
            resu += "<p>Les triangles de cette rangée (<strong>"+str.setNum((double)(num+1),'f',0)+"</strong>) font un angle Beta de <strong>"+str2.setNum(degres(los_m1[num].angleAvecRangeePrecedente),'f',0)+"</strong> ° avec les losanges de la rangée précédente (<strong>"+str3.setNum((double)(num),'f',0)+"</strong>).<br />";
            resu += "Les triangles de cette rangée (<strong>"+str.setNum((double)(num+1),'f',0)+"</strong>) font un angle Omega avec l'horizontale de <strong>"+str2.setNum(degres(los_m1[num].angleAvecHorizontale),'f',1)+"</strong> °.<br />";

        }
        else
        {
            resu = "<p>Il y a <strong>"+str.setNum((double)zom_nombreOrdre,'f',0)+"</strong> losanges sur la rangée n° <strong>"+str2.setNum((double)(num+1),'f',0)+"</strong>.<br />Un losange est constitué de 4 chevrons identiques, aux dimensions suivantes :<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- épaisseur : <strong>" + str.setNum(zom_epaisseurChevronsMethode1*100.0,'f',1)+"</strong> cm<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- retombée : <strong>" + str.setNum(zom_retombeeChevronsMethode1*100.0,'f',1)+"</strong> cm<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur \'brute, hors coupe\' : <strong>" + str.setNum(los_m1[num].longueurPlancheBrut*100.0,'f',1)+"</strong> cm<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur L1 : <strong>" + str.setNum(los_m1[num].longueur*100.0,'f',1)+"</strong> cm<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- angle alpha1 : <strong>" + str.setNum(degres(los_m1[num].beta),'f',1)+"</strong> °<br />";
            resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- angle inclinaison lame : <strong>" + str.setNum(degres(los_m1[num].angleReglageLame),'f',1)+"</strong> °</p>";
            if(num==0)
                resu += "<p>Les losanges la première rangée (<strong>"+str.setNum((double)(num+1),'f',0)+"</strong>) font un angle Beta entre eux de <strong>"+str2.setNum(degres(los_m1[num].angleAvecRangeePrecedente),'f',0)+"</strong> °.<br />";
            else
                resu += "<p>Les losanges de cette rangée (<strong>"+str.setNum((double)(num+1),'f',0)+"</strong>) font un angle Beta de <strong>"+str2.setNum(degres(los_m1[num].angleAvecRangeePrecedente),'f',0)+"</strong> ° avec les losanges de la rangée précédente (<strong>"+str3.setNum((double)(num),'f',0)+"</strong>).<br />";
            if(num==(zom_nombreOrdre-2)&&(zom_nombreOrdre==zom_rangees))
                resu += "Les losanges de la dernière rangée (<strong>"+str.setNum((double)(num+1),'f',0)+"</strong>) font un angle Beta entre eux de <strong>"+str2.setNum(degres(los_m1[num].angleAvecRangeeSuivante),'f',0)+"</strong> °.<br />";
            else
                resu += "Les losanges de cette rangée (<strong>"+str.setNum((double)(num+1),'f',0)+"</strong>) font un angle Beta de <strong>"+str2.setNum(degres(los_m1[num].angleAvecRangeeSuivante),'f',0)+"</strong> ° avec ceux de la rangée suivante (<strong>"+str3.setNum((double)(num+2),'f',0)+"</strong>).<br />";
            resu += "Les losanges de cette rangée (<strong>"+str.setNum((double)(num+1),'f',0)+"</strong>) font un angle Omega avec l'horizontale de <strong>"+str2.setNum(degres(los_m1[num].angleAvecHorizontale),'f',1)+"</strong> °.<br />";
            resu += "Si vous souhaitez contreventer avec des diagonales verticales : la longueur L2 (point à pointe) est de <strong>"+str.setNum(los_m1[num].DiagVertInt*100.0,'f',1)+"</strong> cm et l'angle des pointes est de <strong>"+str2.setNum(degres(los_m1[num].beta),'f',1)+"</strong> °.<br />";
            resu += "Si vous souhaitez contreventer avec des diagonales horizontales : la longueur L3 (point à pointe) est de <strong>"+str.setNum(los_m1[num].DiagHorizInt*100.0,'f',1)+"</strong> cm et l'angle des pointes est de <strong>"+str2.setNum(degres(los_m1[num].alpha),'f',1)+"</strong> °.</p>";
        }
        break;
    case 1: //juste visualisation
        resu ="";
        break;
    }
    return resu;
}

QString zome::explicationGenerale()
{
    QString str,str2,str3,str4,str5,str6,resu;
    switch(zom_methodeConstruction)
    {
    case 0: //methode losange

        resu = "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A) Généralités</strong></span></p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C\'est l\'américain Steve Bear qui fût le premier (?), dans les années soixante, à étudier sa géométrie et le développer pour l\'habitat. C'est lui aussi qui lui donna son nom, mariant le ZO de rhombizonaèdre et le ME du dôme.<br />";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Solide géométrique rythmé par les nombres (nombre de forme, nombre d\'ordre..), il rappelle des structures observables dans la nature, architectures minérales et végétales fondées sur le nombre d\'or. Il constitue en effet un assemblage de rangées de losanges successifs, ayant tous la même longueur de côté mais des rapport entre diagonales différents, enroulés en double hélice.</p>";
        resu += "<p><br /></p>";
        resu += "<p><span style=\"color: #ff9900;\"><strong><span style=\"font-size: 12pt;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;B) Les losanges</span></strong></span></p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Nous allons dans un premier temps faire du travail à la chaîne, c\'est-à-dire fabriquer tous les losanges.</p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;La méthode proposée pour tailler un losange est la suivante :</p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>I</strong> - couper les planches à la longueur brute.</p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>II</strong> - découper les angles des 2 bouts.</p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>III</strong> - assembler les 4 planches en les vissant en bout afin de constituer un losange.</p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>IV</strong> - renforcer les losanges avec un contreventement horizontal ou vertical si vous le souhaitez.</p>";
        resu += "<p><br /></p>";
        resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C) Fixations des Losanges entre eux</strong></span></p>";
        resu += "<p align=center><img height=\"143\" width=\"208\" src=\":/images/zome/zomAngle\" /></p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;La méthode proposée pour l'assemblage des losanges entre eux est d'utiliser des tiges filetées préalablement coudées à l'angle Beta indiqué. Selon la taille de la structure, il peut être pertinent de mettre 2 ou 3 tiges filetées par côté de losange. <br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pour couder une tige fileté sans l'abimer, on utilisera par exemple un etau qui tient un côté entre des cales en bois, et de l'autre, un tube assez long pour avoir un bras de levier suffisant afin de couder.</p>";
        resu += "<p><br /></p>";
        resu += "<p><span style=\"font-size: 12pt;\"><strong><span style=\"color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;D) Montage structure</span></strong></span></p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Le montage débute par la rangée du bas (les triangles) et on monte en tournant rangée par rangée, pour arriver au sommet à la rangée n°1.</p>";
        if(zom_nombreOrdre!=zom_rangees)
            resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Le diamètre au sol du cercle circonscrit au zome est de <strong>"+str2.setNum(zom_diametreSolInt,'f',2)+"</strong> m, le diamètre au sol du cercle passant par les pointes des triangles est de <strong>"+str.setNum(zom_diametreSolExt,'f',2)+"</strong> m.</p>";
        resu += "<p><br /></p>";
        resu += "<p><span style=\"font-size: 12pt;\"><strong><span style=\"color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;E) Couverture, Ouvertures, Isolation</span></strong></span></p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pour couvrir ce genre de structure, on peut utiliser tous les types de couverture traditionnelle. On apportera cependant un soin tout particulier à toutes les arêtes, potentielles sources de fuite ...<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Si l\'on désire effectuer une ouverture style velux, on prendra soin de consolider la structure au niveau du chevêtre.<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Si une porte de grandes dimensions doit être effectuée (style hangar), on reprendra le poids de la charpente par un linteau et deux poteaux descendant au sol.<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;L'isolation peut se faire directement dans les caissons avec un isolant rigide ou semi-rigide. Pour les jonctions, il peut être utile d\'utiliser un isolant souple que l\'on pourra comprimer comme de la laine de chanvre</p>";
        resu += "<p><br /></p>";
        resu += "<p align=center><img height=\"502\" width=\"601\" src=\":/images/zome/zomLosangePhoto\" /></p>";

        break;
    case 1: //juste visualisation
        int nbrbarres;
        resu = "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A) Généralités</strong></span></p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C\'est l\'américain Steve Bear qui fût le premier (?), dans les années soixante, à étudier sa géométrie et le développer pour l\'habitat. C'est lui aussi qui lui donna son nom, mariant le ZO de rhombizonaèdre et le ME du dôme.<br />";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Solide géométrique rythmé par les nombres (nombre de forme, nombre d\'ordre..), il rappelle des structures observables dans la nature, architectures minérales et végétales fondées sur le nombre d\'or. Il constitue en effet un assemblage de rangées de losanges successifs, ayant tous la même longueur de côté mais des rapport entre diagonales différents, enroulés en double hélice.</p>";
        resu += "<p><br /></p>";
        resu += "<p><span style=\"color: #ff9900;\"><strong><span style=\"font-size: 12pt;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;B) Dimensions</span></strong></span></p>";
        if(zom_nombreOrdre!=zom_rangees)
            resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Le diamètre au sol du cercle inscrit au zome est de <strong>"+str2.setNum(zom_diametreSolInt,'f',2)+"</strong> m, le diamètre au sol du cercle circonscrit, c'est à dire passant par les pointes des triangles est de <strong>"+str.setNum(zom_diametreSolExt,'f',2)+"</strong> m.</p>";
        if(zom_nombreOrdre==zom_rangees)
        {
            nbrbarres = (zom_rangees-1)*2*zom_nombreOrdre;
            resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pour construire ce zome, il vous faut <strong>"+str.setNum((double)nbrbarres,'f',0)+"</strong> barres de <strong>"+str2.setNum(zom_longeurCoteLosange*100.0,'f',1)+"</strong> cm de long, soit <strong>"+str3.setNum(zom_longeurCoteLosange*nbrbarres,'f',2)+"</strong> ml de barre.</p>";
        }
        else
        {
            nbrbarres = ((zom_rangees-1)*2+1)*zom_nombreOrdre;
            resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pour construire ce zome, il vous faut <strong>"+str.setNum((double)nbrbarres,'f',0)+"</strong> barres de <strong>"+str2.setNum(zom_longeurCoteLosange*100.0,'f',1)+"</strong> cm de long, soit <strong>"+str3.setNum(zom_longeurCoteLosange*nbrbarres,'f',2)+"</strong> ml de barre.</p>";
            resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Il vous faudra aussi <strong>"+str.setNum((double)zom_nombreOrdre,'f',0)+"</strong> barres de <strong>"+str2.setNum(los[zom_rangees-1].diagHoriz*100.0,'f',1)+"</strong> cm de long afin de fermer les triangles du bas (diagonales horizontales), soit <strong>"+str3.setNum(zom_nombreOrdre*los[zom_rangees-1].diagHoriz,'f',2)+"</strong> ml de barre supplémentaires.</p>";
        }
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Voici, rangée par rangée, quelques dimensions caractéristiques afin de pouvoir par exemple fabriquer les connecteurs :</p>";
        resu += "<p><br /></p>";
        resu += "<table cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
        resu += "<td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Rangée n°</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Type</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Diagonale Horizontale (cm)</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Diagonale Verticale (cm)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Angle Alpha (°)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Angle Beta (°)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Angle Omega / horizontale (°)</strong></p></td></tr>";
        for(int i=0;i<zom_rangees;i++)
        {
            if((i==(zom_rangees-1))&&(zom_nombreOrdre!=zom_rangees)) // triangle
                resu += "<tr><td bgcolor=\"#cccc99\"><strong>"+str.setNum((double)i+1,'f',0)+"</strong></td><td>"+"triangle"+"</td><td>"+str2.setNum(los[i].diagHoriz*100.0,'f',1)+"</td><td>"+str3.setNum(los[i].diagVert*100.0,'f',1)+"</td><td>"+str4.setNum(degres(los[i].alpha),'f',1)+"</td><td>"+str5.setNum(degres(los[i].beta),'f',1)+"</td><td>"+str6.setNum(degres(los[i].angleAvecHorizontale),'f',1)+"</td></tr>";
            else if (!((i==(zom_rangees-1))&&(zom_nombreOrdre==zom_rangees)))
                resu += "<tr><td bgcolor=\"#cccc99\"><strong>"+str.setNum((double)i+1,'f',0)+"</strong></td><td>"+"losange"+"</td><td>"+str2.setNum(los[i].diagHoriz*100.0,'f',1)+"</td><td>"+str3.setNum(los[i].diagVert*100.0,'f',1)+"</td><td>"+str4.setNum(degres(los[i].alpha),'f',1)+"</td><td>"+str5.setNum(degres(los[i].beta),'f',1)+"</td><td>"+str6.setNum(degres(los[i].angleAvecHorizontale),'f',1)+"</td></tr>";
        }
        resu += "</tbody></table>";
        resu += "<p><br /></p>";
        resu += "<p align=center><img height=\"400\" width=\"693\" src=\":/images/zome/zomMethode0\" /></p>";
        resu += "<p align=center><img height=\"390\" width=\"308\" src=\":/images/zome/zomOmega\" /></p>";


        break;
    }
    return resu;
}

void zome::calcul()
{
    zom_longeurCoteLosange = zom_petiteDiagonaleRangee1*qSqrt(1.0+qPow(zom_nombreForme,2.0))/2.0;
    zom_demiHauteurLosangeRangee1 = zom_petiteDiagonaleRangee1*qSqrt(qPow(zom_nombreForme,2.0)-qPow(1.0/qTan(pi/zom_nombreOrdre),2.0))/2.0;
    zom_hauteurTotale = zom_rangees*zom_demiHauteurLosangeRangee1;

    //calcul de tous les sommets du zome
    rang.resize(zom_rangees+1);
    rang[0].rayonExt = 0;
    rang[0].rayonInt = 0;
    rang[0].z = zom_hauteurTotale;

    rang[1].z = zom_hauteurTotale-zom_demiHauteurLosangeRangee1;
    rang[1].rayonInt = zom_petiteDiagonaleRangee1/(2.0*qTan(pi/zom_nombreOrdre));
    rang[1].rayonExt = rang[1].rayonInt/(qCos(pi/zom_nombreOrdre));

    rang[2].z = zom_hauteurTotale-2*zom_demiHauteurLosangeRangee1;
    rang[2].rayonExt = 2.0*rang[1].rayonInt;
    rang[2].rayonInt = qCos(pi/zom_nombreOrdre)*rang[2].rayonExt;

    if(zom_rangees>2)
    {
        for(int i = 3;i<=zom_rangees;i++)
        {
            rang[i].z = zom_hauteurTotale-i*zom_demiHauteurLosangeRangee1;
            rang[i].rayonExt = rang[i-2].rayonExt+2.0*(rang[i-1].rayonInt-rang[i-2].rayonExt);
            rang[i].rayonInt = qCos(pi/zom_nombreOrdre)*rang[i].rayonExt;
        }
    }


    QVector3D m_a, m_b, m_c, m_d;
    double alpha, dec;
    dec = pi/zom_nombreOrdre;

    los.resize(zom_rangees);
    for(int i=0;i<zom_rangees;i++)
    {
        if(pair(i)==false)
                alpha = 0;
            else
                alpha = dec;
        m_a = QVector3D(rang[i].rayonExt*qCos(alpha),rang[i].rayonExt*qSin(alpha),rang[i].z);
        m_b = QVector3D(rang[i+1].rayonExt*qCos(alpha-dec),rang[i+1].rayonExt*qSin(alpha-dec),rang[i+1].z);
        if(i==(zom_rangees-1))
            m_c = QVector3D(rang[i+1].rayonInt*qCos(alpha),rang[i+1].rayonInt*qSin(alpha),rang[i+1].z);
        else
            m_c = QVector3D(rang[i+2].rayonExt*qCos(alpha),rang[i+2].rayonExt*qSin(alpha),rang[i+2].z);
        m_d = QVector3D(rang[i+1].rayonExt*qCos(alpha+dec),rang[i+1].rayonExt*qSin(alpha+dec),rang[i+1].z);
        los[i].barycentre = QVector3D(rang[i+1].rayonInt*qCos(alpha),rang[i+1].rayonInt*qSin(alpha),rang[i+1].z);

        QVector3D losX,losY;
        losX = m_d-los[i].barycentre;
        losY = m_a-los[i].barycentre;
        los[i].Vn = QVector3D::crossProduct(losX,losY).normalized();
        QVector3D vHor(0.0,0.0,-1.0);
        los[i].angleAvecHorizontale = angleEntre2Vecteurs3D(los[i].Vn,vHor);


        los[i].diagHoriz = (m_b-m_d).length();
        los[i].diagVert = (m_a-m_c).length();
        if((i==(zom_rangees-1))&&(zom_rangees!=zom_nombreOrdre))
        {
            los[i].alpha = qAtan(2*los[i].diagVert/los[i].diagHoriz);
            los[i].beta = 2.0*(pi/2.0-los[i].alpha);
        }
        else
        {
            los[i].alpha = 2.0*qAtan(los[i].diagVert/los[i].diagHoriz);
            los[i].beta = pi-los[i].alpha;
        }
    }

    //la on fait les calculs de la methode 1
    rang_m1.resize(zom_rangees+1);
    zom_hauteurTotale_m1 = zom_hauteurTotale-zom_retombeeChevronsMethode1*zom_nombreForme*qTan(pi/zom_nombreOrdre);
    zom_petiteDiagonaleRangee1_m1 = 2.0*zom_hauteurTotale_m1/(zom_rangees*qSqrt(qPow(zom_nombreForme,2.0)-1/qPow(qTan(pi/zom_nombreOrdre),2.0)));
    zom_longeurCoteLosange_m1 = zom_petiteDiagonaleRangee1_m1*qSqrt(1.0+qPow(zom_nombreForme,2.0))/2.0;
    zom_demiHauteurLosangeRangee1_m1 = zom_petiteDiagonaleRangee1_m1*qSqrt(qPow(zom_nombreForme,2.0)-qPow(1.0/qTan(pi/zom_nombreOrdre),2.0))/2.0;


    rang_m1[0].rayonExt = 0;
    rang_m1[0].rayonInt = 0;
    rang_m1[0].z = zom_hauteurTotale_m1;

    rang_m1[1].z = zom_hauteurTotale_m1-zom_demiHauteurLosangeRangee1_m1;
    rang_m1[1].rayonInt = zom_petiteDiagonaleRangee1_m1/(2.0*qTan(pi/zom_nombreOrdre));
    rang_m1[1].rayonExt = rang_m1[1].rayonInt/(qCos(pi/zom_nombreOrdre));

    rang_m1[2].z = zom_hauteurTotale_m1-2*zom_demiHauteurLosangeRangee1_m1;
    rang_m1[2].rayonExt = 2.0*rang_m1[1].rayonInt;
    rang_m1[2].rayonInt = qCos(pi/zom_nombreOrdre)*rang_m1[2].rayonExt;

    if(zom_rangees>2)
    {
        for(int i = 3;i<=zom_rangees;i++)
        {
            rang_m1[i].z = zom_hauteurTotale_m1-i*zom_demiHauteurLosangeRangee1_m1;
            rang_m1[i].rayonExt = rang_m1[i-2].rayonExt+2.0*(rang_m1[i-1].rayonInt-rang_m1[i-2].rayonExt);
            rang_m1[i].rayonInt = qCos(pi/zom_nombreOrdre)*rang_m1[i].rayonExt;
        }
    }

    zom_diametreSolInt = 2.0*rang[zom_rangees-1].rayonInt;
    zom_diametreSolExt = 2.0*rang[zom_rangees-1].rayonExt;
    //et voila on a tous nos sommets decales un peu vers l interieur

    los_m1.resize(zom_rangees);
    for(int i=0;i<zom_rangees;i++)
    {
        if(pair(i))
                alpha = 0;
            else
                alpha = dec;
        m_a = QVector3D(rang_m1[i].rayonExt*qCos(alpha),rang_m1[i].rayonExt*qSin(alpha),rang_m1[i].z-rang_m1[0].z/2.0);
        m_b = QVector3D(rang_m1[i+1].rayonExt*qCos(alpha-dec),rang_m1[i+1].rayonExt*qSin(alpha-dec),rang_m1[i+1].z-rang_m1[0].z/2.0);
        if(i==(zom_rangees-1))
            m_c = QVector3D(rang_m1[i+1].rayonInt*qCos(alpha),rang_m1[i+1].rayonInt*qSin(alpha),rang_m1[i+1].z-rang_m1[0].z/2.0);
        else
            m_c = QVector3D(rang_m1[i+2].rayonExt*qCos(alpha),rang_m1[i+2].rayonExt*qSin(alpha),rang_m1[i+2].z-rang_m1[0].z/2.0);
        m_d = QVector3D(rang_m1[i+1].rayonExt*qCos(alpha+dec),rang_m1[i+1].rayonExt*qSin(alpha+dec),rang_m1[i+1].z-rang_m1[0].z/2.0);
        los_m1[i].barycentre = QVector3D(rang_m1[i+1].rayonInt*qCos(alpha),rang_m1[i+1].rayonInt*qSin(alpha),rang_m1[i+1].z-rang_m1[0].z/2.0);

        QMatrix4x4 initial, final, rotation;
        QVector3D losX,losY;
        losX = m_d-los_m1[i].barycentre;
        losY = m_a-los_m1[i].barycentre;
        los_m1[i].Vn = QVector3D::crossProduct(losX,losY).normalized();
        QVector3D vHor(0.0,0.0,-1.0);
        los_m1[i].angleAvecHorizontale = angleEntre2Vecteurs3D(los_m1[i].Vn,vHor);
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
        initial(0,1) = 0;
        initial(1,1) = 1;
        initial(2,1) = 0;
        initial(3,1) = 0;
        initial(0,2) = 0;
        initial(1,2) = 0;
        initial(2,2) = 1;
        initial(3,2) = 0;
        initial(0,3) = 0;
        initial(1,3) = 0;
        initial(2,3) = 0;
        initial(3,3) = 1;
        rotation = (final*(initial.inverted()));
        los_m1[i].quatRot = convertirMatriceRotation(rotation);

        //
        los_m1[i].diagHoriz = (m_b-m_d).length();
        los_m1[i].diagVert = (m_a-m_c).length();
        if((i==(zom_rangees-1))&&(i!=(zom_nombreOrdre-1)))
        {
            los_m1[i].alpha = 2.0*qAtan(2*los_m1[i].diagVert/los_m1[i].diagHoriz);
            los_m1[i].beta = pi-los_m1[i].alpha;
            los_m1[i].angleReglageLame = qAbs(los_m1[i].alpha-pi/2.0);
            los_m1[i].longueur = zom_longeurCoteLosange_m1-zom_epaisseurChevronsMethode1/qCos(los_m1[i].alpha-pi/2.0);
            los_m1[i].longueurPlancheBrut = los_m1[i].longueur+zom_epaisseurChevronsMethode1*qAbs(qTan(los_m1[i].alpha-pi/2.0));
            los_m1[i].DiagHorizInt = los_m1[i].diagHoriz-2.0*zom_epaisseurChevronsMethode1/qSin(los_m1[i].alpha/2.0);
            los_m1[i].DiagVertInt = los_m1[i].diagVert-zom_epaisseurChevronsMethode1/qSin(los_m1[i].beta/2.0);
        }
        else
        {
            los_m1[i].alpha = 2.0*qAtan(los_m1[i].diagVert/los_m1[i].diagHoriz);
            los_m1[i].beta = pi-los_m1[i].alpha;
            los_m1[i].angleReglageLame = qAbs(los_m1[i].alpha-pi/2.0);
            los_m1[i].longueur = zom_longeurCoteLosange_m1-zom_epaisseurChevronsMethode1/qCos(los_m1[i].alpha-pi/2.0);
            los_m1[i].longueurPlancheBrut = los_m1[i].longueur+zom_epaisseurChevronsMethode1*qAbs(qTan(los_m1[i].alpha-pi/2.0));
            los_m1[i].DiagHorizInt = los_m1[i].diagHoriz-2.0*zom_epaisseurChevronsMethode1/qSin(los_m1[i].alpha/2.0);
            los_m1[i].DiagVertInt = los_m1[i].diagVert-2.0*zom_epaisseurChevronsMethode1/qSin(los_m1[i].beta/2.0);
        }
    }
    for(int i=0;i<zom_rangees;i++)
    {
        if(i==0)
        {
            QVector3D vtemp = rotationAutourDeZ(los_m1[i].Vn,2.0*pi/zom_nombreOrdre);
            los_m1[i].angleAvecRangeePrecedente = pi-angleEntre2Vecteurs3D(los_m1[i].Vn,vtemp);
        }
        else if((zom_rangees==zom_nombreOrdre)&&(i==(zom_rangees-2)))
        {
            los_m1[i].angleAvecRangeePrecedente = pi-angleEntre2Vecteurs3D(los_m1[i-1].Vn,los_m1[i].Vn);
            los_m1[i-1].angleAvecRangeeSuivante = los_m1[i].angleAvecRangeePrecedente;
            QVector3D vtemp = rotationAutourDeZ(los_m1[i].Vn,2.0*pi/zom_nombreOrdre);
            los_m1[i].angleAvecRangeeSuivante = pi-angleEntre2Vecteurs3D(los_m1[i].Vn,vtemp);
        }
        else if(!((zom_rangees==zom_nombreOrdre)&&(i==(zom_rangees-1))))
        {
            los_m1[i].angleAvecRangeePrecedente = pi-angleEntre2Vecteurs3D(los_m1[i-1].Vn,los_m1[i].Vn);
            los_m1[i-1].angleAvecRangeeSuivante = los_m1[i].angleAvecRangeePrecedente;
        }
    }



}

QStringList zome::listeLosanges()
{
    QStringList m_liste;
    QString str, str2;
    switch(zom_methodeConstruction)
    {
    case 0:
        for(int i=0;i<zom_rangees;i++)
        {
            if((i==(zom_rangees-1))&&(zom_rangees!=zom_nombreOrdre))
                m_liste << str2.setNum((double)zom_nombreOrdre,'f',0)+" triangles de la rangée n°"+str.setNum((double)(i+1),'f',0);
            else if((i==(zom_rangees-1))&&(zom_rangees==zom_nombreOrdre))
                int h;
            else
                m_liste << str2.setNum((double)zom_nombreOrdre,'f',0)+" losanges de la rangée n°"+str.setNum((double)(i+1),'f',0);
        }
        break;
    case 1:
        m_liste.clear();
        break;
    }

    return m_liste;
}
