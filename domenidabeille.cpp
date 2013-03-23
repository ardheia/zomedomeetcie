#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <qmath.h>

#include "complexes.h"
#include "mathema.h"
#include "domenidabeille.h"

domenidabeille::domenidabeille()
{
}

void domenidabeille::setParametres(int pdna_choix, double pdna_hauteur, double pdna_diametreSol, double pdna_diametreTonoo, double pdna_rayonCourbureDome, int pdna_NbreLosangesHorizontal, double pdna_diagonaleDernierLosange, double pdna_epaisseurChevron, double pdna_retombeeChevron, double pdna_epaisseurTonoo, double pdna_retombeeTonoo, double pdna_epaisseurSabliere, double pdna_decalageNoeud, double pdna_DistanceNoeudTonoo)
{
    dna_choix = pdna_choix;
    dna_NbreLosangesHorizontal = pdna_NbreLosangesHorizontal;
    dna_hauteur = pdna_hauteur;
    dna_diametreSol = pdna_diametreSol;
    dna_diametreTonoo = pdna_diametreTonoo;
    dna_diametreInterieurSabliere = dna_diametreSol-0.20;
    dna_rayonCourbureDome = pdna_rayonCourbureDome;
    dna_epaisseurChevron = pdna_epaisseurChevron/100.0;
    dna_retombeeChevron = pdna_retombeeChevron/100.0;
    dna_epaisseurTonoo = pdna_epaisseurTonoo/100.0;
    dna_retombeeTonoo = pdna_retombeeTonoo/100.0;
    dna_epaisseurSabliere = pdna_epaisseurSabliere/100.0;
    dna_decalageNoeud = pdna_decalageNoeud/100.0;
    dna_premiereDiagonaleHorizontaleLosangeHaut = pdna_diagonaleDernierLosange/100.0;
    dna_DistanceNoeudTonoo = pdna_DistanceNoeudTonoo/100.0;

    calcul();
}

int domenidabeille::orientation(int num)
{
    return chev[num].orientation;
}

QString domenidabeille::explicationChevronNumero(int num)
{
    QString resu,str;

    resu += "<p>"+QObject::tr("Il y a")+" <strong>"+str.setNum((double)dna_NbreLosangesHorizontal,'f',0)+"</strong> "+QObject::tr("chevrons de type")+" "+(char)(num+65);
    if(chev[num].orientation==0)
    {
        resu += ". "+QObject::tr("Ces chevrons sont penchés à droite")+". ";
        if(chev[num].tronque==1)
            resu += QObject::tr("Ils sont tronqués sur leur côté CD sur le Tonoo")+".</p>";
        if(chev[num].tronque==2)
            resu += QObject::tr("Ils sont tronqués sur leur côté AB sur la Sablière")+".</p>";
        if(chev[num].tronque==0)
            resu += "</p>";
    }
    else
    {
        resu += ". "+QObject::tr("Ces chevrons sont penchés à gauche")+". ";
        if(chev[num].tronque==1)
            resu += QObject::tr("Ils sont tronqués sur leur côté AB sur le Tonoo")+".</p>";
        if(chev[num].tronque==2)
            resu += QObject::tr("Ils sont tronqués sur leur côté CD sur la Sablière")+".</p>";
        if(chev[num].tronque==0)
            resu += "</p>";
    }
    resu += "<p>"+QObject::tr("Dimensions des planches dans lesquelles ces chevrons seront taillés")+" :<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("épaisseur")+" : <strong>" + str.setNum(dna_epaisseurChevron*100.0,'f',1)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("retombée")+" : <strong>" + str.setNum(chev[num].A.y()*100.0,'f',1)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("longueur")+" : <strong>" + str.setNum(chev[num].C.x()*100.0,'f',1)+"</strong> cm</p>";
    resu += "<p>"+QObject::tr("Dimensions des chevrons courbes")+" :<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("retombée")+" : <strong>" + str.setNum(dna_retombeeChevron*100,'f',1)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("rayon de courbure de l'intrados")+" : <strong>" +str.setNum(chev[num].rayonIntrados,'f',2)+"</strong> m<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("rayon de courbure de l'extrados")+" : <strong>"+str.setNum(chev[num].rayonExtrados,'f',2)+"</strong> m<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("angle de traçage alpha côté gauche AB")+" : <strong>"+str.setNum(degres(chev[num].angleTracageCoteAB),'f',1)+"</strong> °<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("angle de traçage alpha côté droit CD")+" : <strong>"+str.setNum(degres(chev[num].angleTracageCoteCD),'f',1)+"</strong> °<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("angle de coupe alpha côté gauche AB")+" : <strong>"+str.setNum(degres(chev[num].angleCoupeCoteAB),'f',1)+"</strong> °<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("angle de coupe alpha côté droit CD")+" : <strong>"+str.setNum(degres(chev[num].angleCoupeCoteCD),'f',1)+"</strong> °</p>";
    resu += "<p>"+QObject::tr("Les points notés V sont sur la face supérieure de la planche, les points notés W sont sur la face cachée de la planche")+".<br />";
    resu += QObject::tr("Le point B est pris comme origine du repère. Le schéma ci-dessous n\'est pas représentatif du chevron, il sert juste à comprendre les notations")+".</p>";
    return resu;
}

QString domenidabeille::explicationGenerale()
{
    QString str,str2,str3,resu;

    resu = "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A) "+QObject::tr("Généralités")+"</strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("La charpente en nid d\'abeille, évolution des charpentes de Philibert de l\'Orme (1510-1570), permet comme ces dernières d\'utiliser des bois de faible section, comme des planches de 30 ou 40 mm d\'épaisseur et de longueur courte (environ 2 m 30 maximum)")+".</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"color: #ff9900;\"><strong><span style=\"font-size: 12pt;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;B) "+QObject::tr("Les chevrons")+"</span></strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Précisons tout d\'abord que nous allons partir de planches rectangulaires et tailler ensuite la forme courbe de chaque chevron")+".<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; "+QObject::tr("Il est possible de ne tailler que l\'extrados du chevron courbe, en gardant l\'intrados rectiligne, cela permet, pour une même largeur de planches de départ, une meilleure solidité, mais néanmoins une esthétique différente (voute moins lisse de l\'intérieur) par rapport à des chevrons dont l\'intrados est courbe")+".<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; "+QObject::tr("Il y a 2 types de chevrons, les chevrons penchés à droite (quand on regarde le dôme de l\'extérieur, leur bas est à gauche et le haut va vers la droite) et les chevrons penchés à gauche")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("La méthode proposée pour tailler les chevrons est la suivante, à répéter pour chaque type")+" :</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>I</strong> - "+QObject::tr("marquer un chevron \"gabarit\" sur une planche, tracer le dessin d\'un chevron en marquant bien ses points les plus loins de son centre (soit W, soit V, ça dépend). Pour dessiner la bonne courbe, on pourra s\'aider des points O et O2, ainsi que du centre Ce qui correspond au centre des cercles de l\'intrados et de l\'extrados. Ne pas oublier de marquer les points s1, s2, q1, q2 sur la face V du chevron et les m1, m2, n1, n2 sur la face W du chevron. Ils correspondent respectivement à l\'arrivée du chevron de la rangée précédente et au départ du chevron de la rangée supérieure")+".</p>";
    resu += "<p align=center><img height=\"177\" width=\"400\" src=\":/images/vna/vnaAngleTracage\" /></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>II</strong> - "+QObject::tr("découper l\'extrados courbe (et l\'intrados si voulu) à la scie sauteuse ou à la scie à ruban. Ne coupez pas encore les 2 bouts en biseaux mais coupez sur les lignes les plus extérieures, avec un angle de scie normal (0°), pour obtenir des coupes bien perpendiculaires à la face du dessus du chevron")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>III</strong> - "+QObject::tr("a partir de ce chevron \"gabarit\", vous pouvez découper tous les autres chevrons du même type. Une technique, la défonceuse avec une mèche \"à copier\" qui va suivre le gabarit en découpant celui du dessus. Une autre technique, plus dangereuse, utiliser une toupie")+".</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>IV</strong> - "+QObject::tr("une fois tous les chevrons d'un même type découpés, vous pouvez biseauter les bouts. Pour cela, vous pouvez utiliser une scie à onglets à inclinaison variable, ou une scie circulaire avec l\'angle de la lame réglable")+".</p>";
    resu += "<p align=center><img height=\"162\" width=\"400\" src=\":/images/vna/vnaAngleCoupe\" /></p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"font-size: 12pt;\"><strong><span style=\"color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C) "+QObject::tr("La sablière")+"</span></strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("La sablière est un assemblage de pièces fixées sur une assise plane et stable. Le rayon extérieur de la sablière est de")+" <strong> "+str.setNum(dna_diametreSol/2.0,'f',2)+" </strong> m, "+QObject::tr("le rayon intérieur est de")+" <strong> "+str2.setNum(dna_diametreInterieurSabliere/2.0,'f',2)+" </strong> m. "+QObject::tr("Les premiers chevrons viennent s\'y fixer à intervalles réguliers de")+" <strong> "+str3.setNum(dna_diametreSol*qSin(pi/dna_NbreLosangesHorizontal)*100.0,'f',1)+" </strong> cm.</p>";
    resu += "<p align=center><img height=\"273\" width=\"250\" src=\":/images/dna/dnaSabliere\" /></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; "+QObject::tr("Afin de bloquer le mouvement des assemblages au niveau des sablières, on pourra rajouter de cales")+".</p>";
    resu += "<p align=center><img height=\"141\" width=\"250\" src=\":/images/vna/vnaFixationSabliere\" /></p>";
    resu += "<p><br /></p>";
    resu += "<p><strong><span style=\"font-size: 12pt; color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;D) "+QObject::tr("Le Tonoo")+"</span></strong></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Le Tonoo pourra être obtenu en faisant un assemblage lamellé-collé (ou vissé) en assemblant plusieurs rangées de planches jusqu\'à obtenir l\'épaisseur voulue. Les planches seront vissées ou clouées entre elles et les jonctions de chaque rangées seront décalées avec les rangées suivantes")+".<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; "+QObject::tr("Les chevrons finaux y arrivent avec un intervalle régulier de")+" <strong> "+str.setNum(dna_diametreSol*qSin(pi/dna_NbreLosangesHorizontal)*100.0,'f',1)+" </strong> cm.</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;E) "+QObject::tr("Fixations des noeuds")+"</strong></span></p>";
    resu += "<p align=center><img height=\"223\" width=\"199\" src=\":/images/vna/vnaAssemblage\" /></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("L\'assemblage ne se fait pas au milieu des chevrons, il faut respecter une valeur de décalage du noeud. Sinon, le noeud pourrait pivoter, le décalage assurant la rigidité de la structure")+".<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; "+QObject::tr("Les chevrons seront premièrement cloués ou vissés en bout au niveau des assemblages. Ensuite, si la structure est soumise à de fortes contraintes, il sera nécessaire de solidifier par un boulonnage: tige filetée, écroux, et rondelles crantées si possibles")+".</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"font-size: 12pt;\"><strong><span style=\"color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;F) "+QObject::tr("Montage structure")+"</span></strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Le montage débute par la mise en place du rond de sablière. Ensuite il suffit de monter rangées par rangées jusqu\'à pouvoir s\'approcher du tonoo, normalement, grace à la forme circulaire, ça tient pendant le montage")+".</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"font-size: 12pt;\"><strong><span style=\"color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;G) "+QObject::tr("Couverture, Ouvertures")+"</span></strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Pour couvrir ce genre de structure, on peut voliger directement sur les chevrons ou poser un liteaunage horizontal. Les matériaux de couvertures pourront être la tuile, le bardeau de bois, l\'ardoise ...")+"<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; "+QObject::tr("Si l\'on désire effectuer une ouverture style velux, on prendra soin de consolider la structure au niveau du chevêtre")+".<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; "+QObject::tr("Si une porte de grandes dimensions doit être effectuée (style hangar), on reprendra le poids de la charpente par un linteau et deux poteaux descendant au sol")+".</p>";

    return resu;
}

int domenidabeille::nbChevrons()
{
    return chev.size();
}

QString domenidabeille::debit()
{
    QString str,str2,str3,resu;
    int nb_chevrons_total;
    double m_debi, m_debitChevrons, m_debitSabliere, m_debitTonoo, m_debitTotal;
    nb_chevrons_total = dna_NbreLosangesHorizontal*chev.size();
    m_debitChevrons = 0;
    QVector<QString> carac;
    carac.clear();
    for(int i=0;i<chev.size();i++)
        carac.append(QChar(i+65));

    resu = "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Chevrons")+"</strong></span></p>";
    resu += "<p>"+QObject::tr("Il y a en tout")+" <strong>"+str.setNum((double)nb_chevrons_total,'f',0)+"</strong> "+QObject::tr("chevrons, soit")+" <strong>"+str2.setNum((double)chev.size(),'f',0)+"</strong> "+QObject::tr("rangées de")+" <strong>"+str3.setNum((double)dna_NbreLosangesHorizontal,'f',0)+"</strong> "+QObject::tr("chevrons chacune")+".<br />";
    resu += ""+QObject::tr("Les planches pour tailler tous ces chevrons auront les dimensions suivantes (par type de chevron)")+" :<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("Toutes la même epaisseur")+" : <strong>"+str.setNum(dna_epaisseurChevron*100.0,'f',0)+"</strong> cm<br /></p>";
    resu += "<table cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
    resu += "<td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Type")+"</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Retombée")+" (cm)</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Longueur")+" (cm)</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>"+QObject::tr("Débit total")+" (m3)</strong></p></td></tr>";
    for(int i=0;i<chev.size();i++)
    {
        m_debi = dna_epaisseurChevron*chev[i].retombeePlanche*chev[i].longueurPlanche*dna_NbreLosangesHorizontal;
        m_debitChevrons += m_debi;
        resu += "<tr><td bgcolor=\"#cccc99\"><strong>"+carac[i]+"</strong></td><td>"+str.setNum(chev[i].retombeePlanche*100.0,'f',1)+"</td><td>"+str2.setNum(chev[i].longueurPlanche*100.0,'f',1)+"</td><td>"+str3.setNum(m_debi,'f',2)+"</td></tr>";
    }
    resu += "</tbody></table>";
    resu += "<p>"+QObject::tr("Ce qui nous donne un débit total pour tous les chevrons de")+" <strong>"+str.setNum(m_debitChevrons,'f',2)+"</strong> m3.</p>";
    resu += "<p></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Sablière")+"</strong></span></p>";
    resu += "<p>"+QObject::tr("La sablière a pour dimensions")+" : <br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("épaisseur")+" : <strong>"+str.setNum(dna_epaisseurSabliere*100.0,'f',0)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("rayon extérieur")+" : <strong>"+str.setNum(dna_diametreSol/2.0,'f',2)+"</strong> m<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("rayon intérieur")+" : <strong>"+str.setNum(dna_diametreInterieurSabliere/2.0,'f',2)+"</strong> m</p>";
    m_debitSabliere = pi*(qPow(dna_diametreSol/2.0,2.0)-qPow(dna_diametreInterieurSabliere/2.0,2.0))*dna_epaisseurSabliere;
    resu += "<p>"+QObject::tr("Ce qui nous donne un débit exact pour la sablière de")+" <strong>"+str.setNum(m_debitSabliere,'f',2)+"</strong> m3.</p>";
    resu += "<p></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Tonoo")+"</strong></span></p>";
    resu += "<p>"+QObject::tr("Le Tonoo a les dimensions suivantes")+" : <br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("épaisseur")+" : <strong>"+str.setNum(dna_retombeeTonoo*100.0,'f',0)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("diamètre extérieur")+" : <strong>"+str.setNum(dna_diametreTonoo/2.0,'f',2)+"</strong> m<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- "+QObject::tr("diamètre intérieur")+" : <strong>"+str.setNum(dna_diametreTonoo/2.0-dna_epaisseurTonoo,'f',2)+"/strong> m</p>";
    m_debitTonoo = dna_retombeeTonoo*pi*(qPow(dna_diametreTonoo/2.0,2.0)-qPow(dna_diametreTonoo/2.0-dna_epaisseurTonoo,2.0));
    resu += "<p>"+QObject::tr("Ce qui nous donne un débit exact pour le tonoo de")+" <strong>"+str.setNum(m_debitTonoo,'f',2)+"</strong> m3.</p>";
    resu += "<p></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+QObject::tr("Débit Total")+"</strong></span></p>";
    m_debitTotal = m_debitChevrons+m_debitSabliere+m_debitTonoo;
    resu += "<p>"+QObject::tr("Soit un débit exact total de bois pour toute la charpente de")+" <strong>"+str.setNum(m_debitTotal,'f',2)+"</strong> m3.</p>";

    return resu;
}

QString domenidabeille::tableauChevronNumero(int num)
{
    //les tableaux de coordonnees
    QString tableau,str,str2;

    tableau = "<table cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
    tableau += "<td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td><td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td bgcolor=\"#cccc99\"></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>A</strong></td><td>"+str.setNum((chev[num].A.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].A.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if((chev[num].O.x()>0.0)&&(chev[num].O.x()<=chev[num].C.x()))
        tableau += "<td bgcolor=\"#cccc99\"><strong>O</strong></td><td>"+str.setNum((chev[num].O.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].O.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>B</strong></td><td>"+str.setNum((chev[num].B.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].B.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>O2</strong></td><td>"+str.setNum((chev[num].O2.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].O2.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>C</strong></td><td>"+str.setNum((chev[num].C.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].C.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>Ce</strong></td><td>"+str.setNum((chev[num].centre2D.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].centre2D.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>D</strong></td><td>"+str.setNum((chev[num].D.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].D.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if(num!=0)
        tableau += "<td bgcolor=\"#cccc99\"><strong>s0</strong></td><td>"+str.setNum((chev[num].s3.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].s3.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if((num!=chev.size()-1)&&(chev[num].n0.x()>0))
        tableau += "<td bgcolor=\"#cccc99\"><strong>m0</strong></td><td>"+str.setNum((chev[num].m3.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].m3.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VA</strong></td><td>"+str.setNum((chev[num].VA.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].VA.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if(num!=0)
        tableau += "<td bgcolor=\"#cccc99\"><strong>s1</strong></td><td>"+str.setNum((chev[num].s1.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].s1.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if((num!=chev.size()-1)&&(chev[num].n0.x()>0))
        tableau += "<td bgcolor=\"#cccc99\"><strong>m1</strong></td><td>"+str.setNum((chev[num].m1.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].m1.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VB</strong></td><td>"+str.setNum((chev[num].VB.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].VB.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if(num!=0)
        tableau += "<td bgcolor=\"#cccc99\"><strong>s2</strong></td><td>"+str.setNum((chev[num].s2.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].s2.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if((num!=chev.size()-1)&&(chev[num].n0.x()>0))
        tableau += "<td bgcolor=\"#cccc99\"><strong>m2</strong></td><td>"+str.setNum((chev[num].m2.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].m2.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VC</strong></td><td>"+str.setNum((chev[num].VC.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].VC.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if(num!=0)
        tableau += "<td bgcolor=\"#cccc99\"><strong>s3</strong></td><td>"+str.setNum((chev[num].s0.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].s0.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if((num!=chev.size()-1)&&(chev[num].n0.x()>0))
        tableau += "<td bgcolor=\"#cccc99\"><strong>m3</strong></td><td>"+str.setNum((chev[num].m0.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].m0.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VD</strong></td><td>"+str.setNum((chev[num].VD.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].VD.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if(num!=0)
        tableau += "<td bgcolor=\"#cccc99\"><strong>q0</strong></td><td>"+str.setNum((chev[num].q3.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].q3.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if((num!=chev.size()-1)&&(chev[num].n0.x()>0))
        tableau += "<td bgcolor=\"#cccc99\"><strong>n0</strong></td><td>"+str.setNum((chev[num].n3.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].n3.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WA</strong></td><td>"+str.setNum((chev[num].WA.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].WA.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if(num!=0)
        tableau += "<td bgcolor=\"#cccc99\"><strong>q1</strong></td><td>"+str.setNum((chev[num].q1.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].q1.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if((num!=chev.size()-1)&&(chev[num].n0.x()>0))
        tableau += "<td bgcolor=\"#cccc99\"><strong>n1</strong></td><td>"+str.setNum((chev[num].n1.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].n1.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WB</strong></td><td>"+str.setNum((chev[num].WB.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].WB.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if(num!=0)
        tableau += "<td bgcolor=\"#cccc99\"><strong>q2</strong></td><td>"+str.setNum((chev[num].q2.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].q2.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if((num!=chev.size()-1)&&(chev[num].n0.x()>0))
        tableau += "<td bgcolor=\"#cccc99\"><strong>n2</strong></td><td>"+str.setNum((chev[num].n2.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].n2.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WC</strong></td><td>"+str.setNum((chev[num].WC.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].WC.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if(num!=0)
        tableau += "<td bgcolor=\"#cccc99\"><strong>q3</strong></td><td>"+str.setNum((chev[num].q0.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].q0.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    if((num!=chev.size()-1)&&(chev[num].n0.x()>0))
        tableau += "<td bgcolor=\"#cccc99\"><strong>n3</strong></td><td>"+str.setNum((chev[num].n0.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].n0.y()-chev[num].B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WD</strong></td><td>"+str.setNum((chev[num].WD.x()-chev[num].A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev[num].WD.y()-chev[num].B.y())*100.0,'f',1)+"</td>";
    tableau += "<td></td><td></td><td></td></tr></tbody></table>";

    return tableau;
}

void domenidabeille::valeursDomeGenerales(QVector<QVector3D> &passageValeur)
{
    QVector3D tempo;
    //0
    tempo.setX(dna_diametreSol);tempo.setY(dna_epaisseurSabliere);tempo.setZ(dna_diametreInterieurSabliere);
    passageValeur << tempo;
    //1
    tempo.setX(dna_diametreTonoo);tempo.setY(dna_epaisseurTonoo);tempo.setZ(dna_retombeeTonoo);
    passageValeur << tempo;
    //2
    tempo.setX(dna_NbreLosangesHorizontal);tempo.setY(dna_hauteur);
    passageValeur << tempo;
}

void domenidabeille::valeursChevrons(QVector<chevronDNA> &passageChevron)
{
    passageChevron.clear();
    passageChevron = chev;
}

void domenidabeille::calcul()
{
    QVector2D a,b,c,d;
    a.setX(dna_diametreSol/2.0);a.setY(dna_epaisseurSabliere);
    b.setX(dna_diametreTonoo/2.0);b.setY(dna_hauteur);
    c.setX(-dna_diametreSol/2.0);c.setY(dna_epaisseurSabliere);

    if(dna_choix==0)
    {
        dna_rayonCourbureDome = rayonCercle3Points2D(a,b,c);
    }

    d = centreCercle2Points2DavecRayon1(b,a,dna_rayonCourbureDome);
    dna_centreCourbe = centreCercle2Points2DavecRayon2(b,a,dna_rayonCourbureDome);
    if(d.x()<dna_centreCourbe.x()){dna_centreCourbe = d;}

    //calculs angles
    dna_angleHorizontalCentreSabliere = qAtan((a.y()-dna_centreCourbe.y())/(a.x()-dna_centreCourbe.x()));
    dna_angleHorizontalCentreTonoo = angleHorizontalCentrePoint2D(dna_centreCourbe,b);
    dna_angleSabliereCentreTonoo = dna_angleHorizontalCentreTonoo-dna_angleHorizontalCentreSabliere;

    dna_angleDiagonaleHorizontal = 2.0*pi/dna_NbreLosangesHorizontal;
    dna_angleDemiDiagonaleHorizontal = dna_angleDiagonaleHorizontal/2.0;

    //longueurs courbes
    dna_longueurArcCourbeSabliereTonoo = dna_angleSabliereCentreTonoo*dna_rayonCourbureDome;

    QVector3D noeudA,noeudB,noeudC,AB,Bcentre,VnormalPlanABcentre;

    dna_centreCourbe3D.setX(dna_centreCourbe.x());
    dna_centreCourbe3D.setY(0);
    dna_centreCourbe3D.setZ(dna_centreCourbe.y());

    //ici on cherche tous les noeuds,en partant du haut

    dna_angleVerticalNoeuds.clear();
    //on rajoute un noeud fictif qui est celui du bout demi chevron du haut, on le calculera apres
    dna_angleVerticalNoeuds.append(0);
    //puis le premier noeud a cote du tonoo
    dna_angleVerticalNoeuds.append(dna_angleHorizontalCentreTonoo-dna_DistanceNoeudTonoo/dna_rayonCourbureDome);
    //le second est determine par rapport a l angle beta
    dna_angleVerticalNoeuds.append(dna_angleVerticalNoeuds[1]-dna_premiereDiagonaleHorizontaleLosangeHaut/(2.0*dna_rayonCourbureDome));

    //ensuite la boucle pour trouver les noeuds
    int compteurNoeud,sortie;

    compteurNoeud = 3;
    sortie = 0;
    do
    {
        noeudA = point3D(dna_angleVerticalNoeuds[compteurNoeud-2],dna_angleDiagonaleHorizontal);
        noeudB = point3D(dna_angleVerticalNoeuds[compteurNoeud-1],dna_angleDemiDiagonaleHorizontal);
        AB = noeudB-noeudA;
        Bcentre.setX(dna_centreCourbe.x()*qCos(dna_angleDemiDiagonaleHorizontal)-noeudB.x());
        Bcentre.setY(dna_centreCourbe.x()*qSin(dna_angleDemiDiagonaleHorizontal)-noeudB.y());
        Bcentre.setZ(dna_centreCourbe.y()-noeudB.z());
        VnormalPlanABcentre = QVector3D::crossProduct(AB,Bcentre);

        noeudC = intersection1Plan1CercleDansPlanOxz3D(noeudB,VnormalPlanABcentre,dna_centreCourbe3D,dna_rayonCourbureDome);
        dna_angleVerticalNoeuds.append(angleHorizontalCentrePoint3D(dna_centreCourbe3D,noeudC));

        compteurNoeud += 1;
        if(dna_angleVerticalNoeuds[compteurNoeud-1]<dna_angleHorizontalCentreSabliere)
        {
            sortie = 1;
        }

    } while(sortie == 0);

    //maintenant le point 0
    noeudA = point3D(dna_angleVerticalNoeuds[1],-dna_angleDemiDiagonaleHorizontal);
    noeudB = point3D(dna_angleVerticalNoeuds[2],-dna_angleDiagonaleHorizontal);
    AB = noeudB-noeudA;
    //ce devrait s appeler ACentre mais bon on fait comme si
    Bcentre.setX(dna_centreCourbe.x()*qCos(-dna_angleDemiDiagonaleHorizontal)-noeudA.x());
    Bcentre.setY(dna_centreCourbe.x()*qSin(-dna_angleDemiDiagonaleHorizontal)-noeudA.y());
    Bcentre.setZ(dna_centreCourbe.y()-noeudA.z());
    VnormalPlanABcentre = QVector3D::crossProduct(AB,Bcentre);
    noeudC = intersection1Plan1CercleDansPlanOxz3D(noeudA,VnormalPlanABcentre,dna_centreCourbe3D,dna_rayonCourbureDome);
    dna_angleVerticalNoeuds[0] = angleHorizontalCentrePoint3D(dna_centreCourbe3D,noeudC);

    //on transfert dans un tableau ou on va aussi inclure les primes et les secondes
    dna_noeud.clear();
    dna_noeud.resize(dna_angleVerticalNoeuds.size());
    for(int i=0;i<dna_angleVerticalNoeuds.size();i++)
    {
        dna_noeud[i].alphaZero = dna_angleVerticalNoeuds[dna_angleVerticalNoeuds.size()-1-i];
        dna_noeud[i].phiZero = 0;
    }

    // on cherche B seconde, d abord on cherche de combien il est decale en phi
    VnormalPlanABcentre = QVector3D::crossProduct((point3D(dna_noeud[1].alphaZero,dna_angleDemiDiagonaleHorizontal)-point3D(dna_noeud[2].alphaZero,0)),(dna_centreCourbe3D-point3D(dna_noeud[2].alphaZero,0)));
    double ro;
    ro = angleEntre2Vecteurs3D(VnormalPlanABcentre,QVector3D(0,1,0));
    dna_noeud[2].phiSeconde = dna_decalageNoeud*qSin(ro)/(dna_rayonCourbureDome*2.0);
    // apres on cherche sa position exacte en alpha
    noeudA = point3D(dna_noeud[1].alphaZero,dna_angleDemiDiagonaleHorizontal+dna_noeud[2].phiSeconde);
    noeudB = point3D(dna_noeud[2].alphaZero,dna_noeud[2].phiSeconde);
    AB = noeudB-noeudA;
    Bcentre.setX(dna_centreCourbe.x()*qCos(dna_noeud[2].phiSeconde)-noeudB.x());
    Bcentre.setY(dna_centreCourbe.x()*qSin(dna_noeud[2].phiSeconde)-noeudB.y());
    Bcentre.setZ(dna_centreCourbe.y()-noeudB.z());
    VnormalPlanABcentre = QVector3D::crossProduct(AB,Bcentre);
    noeudC = intersection1Plan1CercleDansPlanOxz3D(noeudB,VnormalPlanABcentre,dna_centreCourbe3D,dna_rayonCourbureDome);
    dna_noeud[2].alphaSeconde = angleHorizontalCentrePoint3D(dna_centreCourbe3D,noeudC);
    //et voila
    // maintenant qu'on a ca on va chercher le A prime
    calculPointPrime(1);
    //c est fait
    //et puis on va chercher le Aseconde pour le chevron du bas / le noeud a en fait c'est Aprime et le noeud B c'est en fait le A
    noeudA = point3D(dna_noeud[1].alphaPrime,2.0*dna_noeud[1].phiPrime);
    noeudB = point3D(dna_noeud[1].alphaZero,dna_noeud[1].phiPrime);
    AB = noeudB-noeudA;
    Bcentre.setX(dna_centreCourbe.x()*qCos(dna_noeud[1].phiPrime)-noeudB.x());
    Bcentre.setY(dna_centreCourbe.x()*qSin(dna_noeud[1].phiPrime)-noeudB.y());
    Bcentre.setZ(dna_centreCourbe.y()-noeudB.z());
    VnormalPlanABcentre = QVector3D::crossProduct(AB,Bcentre);
    noeudC = intersection1Plan1CercleDansPlanOxz3D(noeudB,VnormalPlanABcentre,dna_centreCourbe3D,dna_rayonCourbureDome);
    dna_noeud[1].alphaSeconde = angleHorizontalCentrePoint3D(dna_centreCourbe3D,noeudC);
    dna_noeud[1].phiSeconde = dna_noeud[1].phiPrime;

    // maintenant on recalcule le B seconde
    for(int i=2;i<dna_noeud.size()-1;i++)
    {
        calculPointSeconde(i);
        calculPointPrime(i);
    }
    calculPointSeconde(dna_noeud.size()-1);
    //sans oublier le 0 prime
    calculPointPrime(0);

    //Et maintenant qu'on a nos points on va calculer les chevrons
    chev.clear();
    chev.resize(dna_angleVerticalNoeuds.size());


    for(int i=0;i<dna_angleVerticalNoeuds.size();i++)
    {
        //cas particulier du bout de chevron tronque du bas
        if(i==0)
        {
            chev[i].orientation = 0; //penche a droite
            chev[i].tronque = 2;
            chev[i].pointN = point3D(dna_noeud[i].alphaZero, dna_noeud[i].phiZero);
            chev[i].pointNplus1seconde = point3D(dna_noeud[i+1].alphaSeconde, dna_angleDemiDiagonaleHorizontal-dna_noeud[i+1].phiSeconde);
            noeudA = point3D(dna_noeud[i].alphaZero,-dna_angleDemiDiagonaleHorizontal);
            noeudB = point3D(dna_noeud[i+1].alphaPrime,-dna_angleDiagonaleHorizontal+dna_noeud[i+1].phiPrime);
            AB = noeudB-noeudA;
            Bcentre.setX(dna_centreCourbe.x()*qCos(-dna_angleDemiDiagonaleHorizontal)-noeudA.x());
            Bcentre.setY(dna_centreCourbe.x()*qSin(-dna_angleDemiDiagonaleHorizontal)-noeudA.y());
            Bcentre.setZ(dna_centreCourbe.y()-noeudA.z());
            VnormalPlanABcentre = QVector3D::crossProduct(AB,Bcentre);
            noeudC = intersection1Plan1CercleDansPlanOxz3D(noeudA,VnormalPlanABcentre,dna_centreCourbe3D,dna_rayonCourbureDome);
            chev[i].pointNmoins1prime = point3D(angleHorizontalCentrePoint3D(dna_centreCourbe3D,noeudC),-dna_angleDemiDiagonaleHorizontal);
        }
        //cas particulier du bout de chevron tronque du haut
        else if(i==dna_angleVerticalNoeuds.size()-1)
        {
            chev[i].tronque = 1;
            noeudA = point3D(dna_noeud[i-1].alphaPrime,dna_angleDiagonaleHorizontal-dna_noeud[i-1].phiPrime);
            noeudB = point3D(dna_noeud[i].alphaZero,dna_angleDemiDiagonaleHorizontal+dna_noeud[i].phiZero);
            AB = noeudB-noeudA;
            Bcentre.setX(dna_centreCourbe.x()*qCos(dna_angleDemiDiagonaleHorizontal)-noeudB.x());
            Bcentre.setY(dna_centreCourbe.x()*qSin(dna_angleDemiDiagonaleHorizontal)-noeudB.y());
            Bcentre.setZ(dna_centreCourbe.y()-noeudB.z());
            VnormalPlanABcentre = QVector3D::crossProduct(AB,Bcentre);
            noeudC = intersection1Plan1CercleDansPlanOxz3D(noeudB,VnormalPlanABcentre,dna_centreCourbe3D,dna_rayonCourbureDome);
            if(pair(i))
            {
                chev[i].orientation = 0;//penche a droite
                chev[i].pointN = point3D(dna_noeud[i].alphaZero, dna_noeud[i].phiZero);
                chev[i].pointNmoins1prime = point3D(dna_noeud[i-1].alphaPrime, -dna_angleDemiDiagonaleHorizontal+dna_noeud[i-1].phiPrime);
                chev[i].pointNplus1seconde = point3D(angleHorizontalCentrePoint3D(dna_centreCourbe3D,noeudC), dna_angleDemiDiagonaleHorizontal);
            }
            else
            {
                chev[i].orientation = 1;//penche a gauche
                chev[i].pointN = point3D(dna_noeud[i].alphaZero, dna_noeud[i].phiZero+dna_angleDemiDiagonaleHorizontal);
                chev[i].pointNmoins1prime = point3D(dna_noeud[i-1].alphaPrime, dna_angleDiagonaleHorizontal-dna_noeud[i-1].phiPrime);
                chev[i].pointNplus1seconde = point3D(angleHorizontalCentrePoint3D(dna_centreCourbe3D,noeudC), 0.0);
            }
        }
        // et les autres chevrons
        else
        {
            chev[i].tronque = 0;
            if(pair(i))
            {
                chev[i].orientation = 0;//penche a droite
                chev[i].pointN = point3D(dna_noeud[i].alphaZero, dna_noeud[i].phiZero);
                chev[i].pointNmoins1prime = point3D(dna_noeud[i-1].alphaPrime, -dna_angleDemiDiagonaleHorizontal+dna_noeud[i-1].phiPrime);
                chev[i].pointNplus1seconde = point3D(dna_noeud[i+1].alphaSeconde, dna_angleDemiDiagonaleHorizontal-dna_noeud[i+1].phiSeconde);
            }
            else
            {
                chev[i].orientation = 1;//penche a gauche
                chev[i].pointN = point3D(dna_noeud[i].alphaZero, dna_noeud[i].phiZero+dna_angleDemiDiagonaleHorizontal);
                chev[i].pointNmoins1prime = point3D(dna_noeud[i-1].alphaPrime, dna_angleDiagonaleHorizontal-dna_noeud[i-1].phiPrime);
                chev[i].pointNplus1seconde = point3D(dna_noeud[i+1].alphaSeconde, dna_noeud[i+1].phiSeconde);
            }
        }

        chev[i].rayonExtrados = rayonCercle3Points3D(chev[i].pointN, chev[i].pointNmoins1prime, chev[i].pointNplus1seconde);
        chev[i].rayonIntrados = chev[i].rayonExtrados-dna_retombeeChevron;
        chev[i].centre = centreCercle3Points3D(chev[i].pointN, chev[i].pointNmoins1prime, chev[i].pointNplus1seconde);
        chev[i].VNormal = QVector3D::crossProduct((chev[i].pointNplus1seconde-chev[i].pointNmoins1prime),(chev[i].centre-chev[i].pointN));
        chev[i].VNormal.normalize();
        if(pair(i))
        {
            //penche a droite
            chev[i].pointNcoteV = chev[i].pointN-chev[i].VNormal*dna_epaisseurChevron/2.0;
            chev[i].pointNcoteW = chev[i].pointN+chev[i].VNormal*dna_epaisseurChevron/2.0;
            chev[i].centreCoteV = chev[i].centre-chev[i].VNormal*dna_epaisseurChevron/2.0;
            chev[i].centreCoteW = chev[i].centre+chev[i].VNormal*dna_epaisseurChevron/2.0;
        }
        else
        {
            chev[i].pointNcoteV = chev[i].pointN+chev[i].VNormal*dna_epaisseurChevron/2.0;
            chev[i].pointNcoteW = chev[i].pointN-chev[i].VNormal*dna_epaisseurChevron/2.0;
            chev[i].centreCoteV = chev[i].centre+chev[i].VNormal*dna_epaisseurChevron/2.0;
            chev[i].centreCoteW = chev[i].centre-chev[i].VNormal*dna_epaisseurChevron/2.0;

        }


    }

    //maintenant on va calculer les points pour le chevron tronque du bas
    //d abord les points du haut
    chev[0].VD3D = intersection2Plans1Sphere3D(chev[0].pointNcoteV,chev[0].VNormal,chev[1].pointNcoteV,chev[1].VNormal,chev[0].centreCoteV,chev[0].rayonExtrados);
    chev[0].WD3D = intersection2Plans1Sphere3D(chev[0].pointNcoteW,chev[0].VNormal,chev[1].pointNcoteV,chev[1].VNormal,chev[0].centreCoteW,chev[0].rayonExtrados);
    chev[0].VC3D = intersection2Plans1Sphere3D(chev[0].pointNcoteV,chev[0].VNormal,chev[1].pointNcoteV,chev[1].VNormal,chev[0].centreCoteV,chev[0].rayonIntrados);
    chev[0].WC3D = intersection2Plans1Sphere3D(chev[0].pointNcoteW,chev[0].VNormal,chev[1].pointNcoteV,chev[1].VNormal,chev[0].centreCoteW,chev[0].rayonIntrados);
    chev[1].s1_3D = chev[0].VD3D;
    chev[1].s2_3D = chev[0].VC3D;
    chev[1].q1_3D = chev[0].WD3D;
    chev[1].q2_3D = chev[0].WC3D;
    //les points d'intersection avec la sabliere
    QVector3D pointSabliere, VNsabliere;
    pointSabliere.setX(2.0);pointSabliere.setY(3.0);pointSabliere.setZ(dna_epaisseurSabliere);
    VNsabliere.setX(0.0001);VNsabliere.setY(0.0001);VNsabliere.setZ(1);
    VNsabliere.normalize();
    chev[0].VA3D = intersection2Plans1Sphere3D(chev[0].pointNcoteV,chev[0].VNormal,pointSabliere,VNsabliere,chev[0].centreCoteV,chev[0].rayonExtrados);
    chev[0].WA3D = intersection2Plans1Sphere3D(chev[0].pointNcoteW,chev[0].VNormal,pointSabliere,VNsabliere,chev[0].centreCoteW,chev[0].rayonExtrados);
    chev[0].VB3D = intersection2Plans1Sphere3D(chev[0].pointNcoteV,chev[0].VNormal,pointSabliere,VNsabliere,chev[0].centreCoteV,chev[0].rayonIntrados);
    chev[0].WB3D = intersection2Plans1Sphere3D(chev[0].pointNcoteW,chev[0].VNormal,pointSabliere,VNsabliere,chev[0].centreCoteW,chev[0].rayonIntrados);

    chevronDNA chevRot;
    //et puis c est parti pour tous les points de tous les chevrons
    for(int i=1;i<dna_angleVerticalNoeuds.size()-1;i++)
    {
        if(pair(i))
        {
            //penche a droite
            chev[i].VD3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,chev[i+1].pointNcoteV,chev[i+1].VNormal,chev[i].centreCoteV,chev[i].rayonExtrados);
            chev[i].WD3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,chev[i+1].pointNcoteV,chev[i+1].VNormal,chev[i].centreCoteW,chev[i].rayonExtrados);
            chev[i].VC3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,chev[i+1].pointNcoteV,chev[i+1].VNormal,chev[i].centreCoteV,chev[i].rayonIntrados);
            chev[i].WC3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,chev[i+1].pointNcoteV,chev[i+1].VNormal,chev[i].centreCoteW,chev[i].rayonIntrados);
            chev[i+1].s1_3D = chev[i].VD3D;
            chev[i+1].s2_3D = chev[i].VC3D;
            chev[i+1].q1_3D = chev[i].WD3D;
            chev[i+1].q2_3D = chev[i].WC3D;
            chevRot = rotateChevron(chev[i-1],-dna_angleDiagonaleHorizontal);
            chev[i].VA3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[i].centreCoteV,chev[i].rayonExtrados);
            chev[i].WA3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[i].centreCoteW,chev[i].rayonExtrados);
            chev[i].VB3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[i].centreCoteV,chev[i].rayonIntrados);
            chev[i].WB3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[i].centreCoteW,chev[i].rayonIntrados);
            chev[i-1].n1_3D = rotationAutourDeZ(chev[i].VA3D,dna_angleDiagonaleHorizontal);
            chev[i-1].n2_3D = rotationAutourDeZ(chev[i].VB3D,dna_angleDiagonaleHorizontal);
            chev[i-1].m1_3D = rotationAutourDeZ(chev[i].WA3D,dna_angleDiagonaleHorizontal);
            chev[i-1].m2_3D = rotationAutourDeZ(chev[i].WB3D,dna_angleDiagonaleHorizontal);
        }
        else
        {
            //penche a gauche
            chev[i].VA3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,chev[i+1].pointNcoteV,chev[i+1].VNormal,chev[i].centreCoteV,chev[i].rayonExtrados);
            chev[i].WA3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,chev[i+1].pointNcoteV,chev[i+1].VNormal,chev[i].centreCoteW,chev[i].rayonExtrados);
            chev[i].VB3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,chev[i+1].pointNcoteV,chev[i+1].VNormal,chev[i].centreCoteV,chev[i].rayonIntrados);
            chev[i].WB3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,chev[i+1].pointNcoteV,chev[i+1].VNormal,chev[i].centreCoteW,chev[i].rayonIntrados);
            chev[i+1].s1_3D = chev[i].VA3D;
            chev[i+1].s2_3D = chev[i].VB3D;
            chev[i+1].q1_3D = chev[i].WA3D;
            chev[i+1].q2_3D = chev[i].WB3D;
            chevRot = rotateChevron(chev[i-1],dna_angleDiagonaleHorizontal);
            chev[i].VD3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[i].centreCoteV,chev[i].rayonExtrados);
            chev[i].WD3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[i].centreCoteW,chev[i].rayonExtrados);
            chev[i].VC3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[i].centreCoteV,chev[i].rayonIntrados);
            chev[i].WC3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[i].centreCoteW,chev[i].rayonIntrados);
            chev[i-1].n1_3D = rotationAutourDeZ(chev[i].VD3D,-dna_angleDiagonaleHorizontal);
            chev[i-1].n2_3D = rotationAutourDeZ(chev[i].VC3D,-dna_angleDiagonaleHorizontal);
            chev[i-1].m1_3D = rotationAutourDeZ(chev[i].WD3D,-dna_angleDiagonaleHorizontal);
            chev[i-1].m2_3D = rotationAutourDeZ(chev[i].WC3D,-dna_angleDiagonaleHorizontal);
        }
        //condition tronque contre sabliere
        if(pair(i))
        {
            if((chev[i].VA3D.z()<dna_epaisseurSabliere)||(chev[i].WA3D.z()<dna_epaisseurSabliere)||(chev[i].WB3D.z()<dna_epaisseurSabliere)||(chev[i].VB3D.z()<dna_epaisseurSabliere))
            {
                chev[i].VA3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,pointSabliere,VNsabliere,chev[i].centreCoteV,chev[i].rayonExtrados);
                chev[i].WA3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,pointSabliere,VNsabliere,chev[i].centreCoteW,chev[i].rayonExtrados);
                chev[i].VB3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,pointSabliere,VNsabliere,chev[i].centreCoteV,chev[i].rayonIntrados);
                chev[i].WB3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,pointSabliere,VNsabliere,chev[i].centreCoteW,chev[i].rayonIntrados);
                chev[i].tronque = 2;
            }
        }
        else
        {
            if((chev[i].VD3D.z()<dna_epaisseurSabliere)||(chev[i].WD3D.z()<dna_epaisseurSabliere)||(chev[i].WC3D.z()<dna_epaisseurSabliere)||(chev[i].VC3D.z()<dna_epaisseurSabliere))
            {
                chev[i].VD3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,pointSabliere,VNsabliere,chev[i].centreCoteV,chev[i].rayonExtrados);
                chev[i].WD3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,pointSabliere,VNsabliere,chev[i].centreCoteW,chev[i].rayonExtrados);
                chev[i].VC3D = intersection2Plans1Sphere3D(chev[i].pointNcoteV,chev[i].VNormal,pointSabliere,VNsabliere,chev[i].centreCoteV,chev[i].rayonIntrados);
                chev[i].WC3D = intersection2Plans1Sphere3D(chev[i].pointNcoteW,chev[i].VNormal,pointSabliere,VNsabliere,chev[i].centreCoteW,chev[i].rayonIntrados);
                chev[i].tronque = 2;
            }
        }
        //condition tronque contre tonoo
        if(pair(i))
        {
            if((distanceAuCentre(chev[i].VD3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].WD3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].VC3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].WC3D)<(dna_diametreTonoo/2.0)))
            {
                chev[i].VD3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[i].rayonExtrados,chev[i].centreCoteV,chev[i].pointNcoteV,chev[i].VNormal);
                chev[i].WD3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[i].rayonExtrados,chev[i].centreCoteW,chev[i].pointNcoteW,chev[i].VNormal);
                chev[i].VC3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[i].rayonIntrados,chev[i].centreCoteV,chev[i].pointNcoteV,chev[i].VNormal);
                chev[i].WC3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[i].rayonIntrados,chev[i].centreCoteW,chev[i].pointNcoteW,chev[i].VNormal);
                chev[i].tronque = 1;
            }
        }
        else
        {
            if((distanceAuCentre(chev[i].VA3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].WA3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].VB3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].WB3D)<(dna_diametreTonoo/2.0)))
            {
                chev[i].VA3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[i].rayonExtrados,chev[i].centreCoteV,chev[i].pointNcoteV,chev[i].VNormal);
                chev[i].WA3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[i].rayonExtrados,chev[i].centreCoteW,chev[i].pointNcoteW,chev[i].VNormal);
                chev[i].VB3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[i].rayonIntrados,chev[i].centreCoteV,chev[i].pointNcoteV,chev[i].VNormal);
                chev[i].WB3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[i].rayonIntrados,chev[i].centreCoteW,chev[i].pointNcoteW,chev[i].VNormal);
                chev[i].tronque = 1;
            }
        }

    }

    //et le dernier chevron tronque du haut
    int ii = dna_angleVerticalNoeuds.size()-1;
    if(pair(ii))
    {
        //penche a droite
        chev[ii].VD3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[ii].rayonExtrados,chev[ii].centreCoteV,chev[ii].pointNcoteV,chev[ii].VNormal);
        chev[ii].WD3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[ii].rayonExtrados,chev[ii].centreCoteW,chev[ii].pointNcoteW,chev[ii].VNormal);
        chev[ii].VC3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[ii].rayonIntrados,chev[ii].centreCoteV,chev[ii].pointNcoteV,chev[ii].VNormal);
        chev[ii].WC3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[ii].rayonIntrados,chev[ii].centreCoteW,chev[ii].pointNcoteW,chev[ii].VNormal);
        chev[ii].tronque = 1;
        chevRot = rotateChevron(chev[ii-1],-dna_angleDiagonaleHorizontal);
        chev[ii].VA3D = intersection2Plans1Sphere3D(chev[ii].pointNcoteV,chev[ii].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[ii].centreCoteV,chev[ii].rayonExtrados);
        chev[ii].WA3D = intersection2Plans1Sphere3D(chev[ii].pointNcoteW,chev[ii].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[ii].centreCoteW,chev[ii].rayonExtrados);
        chev[ii].VB3D = intersection2Plans1Sphere3D(chev[ii].pointNcoteV,chev[ii].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[ii].centreCoteV,chev[ii].rayonIntrados);
        chev[ii].WB3D = intersection2Plans1Sphere3D(chev[ii].pointNcoteW,chev[ii].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[ii].centreCoteW,chev[ii].rayonIntrados);
        chev[ii-1].n1_3D = rotationAutourDeZ(chev[ii].VA3D,dna_angleDiagonaleHorizontal);
        chev[ii-1].n2_3D = rotationAutourDeZ(chev[ii].VB3D,dna_angleDiagonaleHorizontal);
        chev[ii-1].m1_3D = rotationAutourDeZ(chev[ii].WA3D,dna_angleDiagonaleHorizontal);
        chev[ii-1].m2_3D = rotationAutourDeZ(chev[ii].WB3D,dna_angleDiagonaleHorizontal);
    }
    else
    {
        //penche a gauche
        chev[ii].VA3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[ii].rayonExtrados,chev[ii].centreCoteV,chev[ii].pointNcoteV,chev[ii].VNormal);
        chev[ii].WA3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[ii].rayonExtrados,chev[ii].centreCoteW,chev[ii].pointNcoteW,chev[ii].VNormal);
        chev[ii].VB3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[ii].rayonIntrados,chev[ii].centreCoteV,chev[ii].pointNcoteV,chev[ii].VNormal);
        chev[ii].WB3D = intersection1CylindreSelonOzEtUnCercleDansUnPlan(dna_diametreTonoo/2.0,chev[ii].rayonIntrados,chev[ii].centreCoteW,chev[ii].pointNcoteW,chev[ii].VNormal);
        chev[ii].tronque = 1;
        chevRot = rotateChevron(chev[ii-1],dna_angleDiagonaleHorizontal);
        chev[ii].VD3D = intersection2Plans1Sphere3D(chev[ii].pointNcoteV,chev[ii].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[ii].centreCoteV,chev[ii].rayonExtrados);
        chev[ii].WD3D = intersection2Plans1Sphere3D(chev[ii].pointNcoteW,chev[ii].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[ii].centreCoteW,chev[ii].rayonExtrados);
        chev[ii].VC3D = intersection2Plans1Sphere3D(chev[ii].pointNcoteV,chev[ii].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[ii].centreCoteV,chev[ii].rayonIntrados);
        chev[ii].WC3D = intersection2Plans1Sphere3D(chev[ii].pointNcoteW,chev[ii].VNormal,chevRot.pointNcoteW,chevRot.VNormal,chev[ii].centreCoteW,chev[ii].rayonIntrados);
        chev[ii-1].n1_3D = rotationAutourDeZ(chev[ii].VA3D,-dna_angleDiagonaleHorizontal);
        chev[ii-1].n2_3D = rotationAutourDeZ(chev[ii].VB3D,-dna_angleDiagonaleHorizontal);
        chev[ii-1].m1_3D = rotationAutourDeZ(chev[ii].WA3D,-dna_angleDiagonaleHorizontal);
        chev[ii-1].m2_3D = rotationAutourDeZ(chev[ii].WB3D,-dna_angleDiagonaleHorizontal);
    }

    //la on va supprimer les chevrons qui ont ne serait ce qu un point de leur bas a l interieur du tonoo
    for(int i=1;i<dna_angleVerticalNoeuds.size();i++)
    {
        //condition tronque contre tonoo
        if(pair(i))
        {
            if((distanceAuCentre(chev[i].VA3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].WA3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].VB3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].WB3D)<(dna_diametreTonoo/2.0)))
            {
                chev[i].tronque = 3; //3 ca veut dire qu on le supprime
            }
        }
        else
        {
            if((distanceAuCentre(chev[i].VD3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].WD3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].VC3D)<(dna_diametreTonoo/2.0))||(distanceAuCentre(chev[i].WC3D)<(dna_diametreTonoo/2.0)))
            {
                chev[i].tronque = 3;
            }
        }
    }

    int ol;
    int efface;
    do
    {
        efface = 0;
        ol = 0;
        do
        {
            if(chev[ol].tronque==3)
            {
                chev.remove(ol);
                efface = 1;
            }
            ol++;
        }
        while((efface == 0)&&(ol<chev.size()));
    }
    while(efface!=0);

    QVector3D centreTemp;
    centreTemp = QVector3D(0.0,0.0,dna_epaisseurSabliere);
    //on ajuste la largeur de la sabliere
    dna_diametreInterieurSabliere = 2.0*qMin((chev[0].VB3D-centreTemp).length(),qMin((chev[0].WB3D-centreTemp).length(),qMin((chev[1].VC3D-centreTemp).length(),(chev[1].WC3D-centreTemp).length())));

    //maintenant qu'on a tous les points en 3D
    //on va les passer en 2D

    QMatrix4x4 final,initial,rotation;
    QQuaternion quatChev, quatChevOz, quatChevTotal;
    QVector3D vecteurX;
    for(int i=0;i<chev.size();i++)
    {
        final(0,0) = 1;
        final(1,0) = 0;
        final(2,0) = 0;
        final(3,0) = 0;
        final(0,1) = 0;
        final(1,1) = 1;
        final(2,1) = 0;
        final(3,1) = 0;
        final(0,2) = 0;
        final(1,2) = 0;
        final(2,2) = 1;
        final(3,2) = 0;
        final(0,3) = 0;
        final(1,3) = 0;
        final(2,3) = 0;
        final(3,3) = 1;
        if((chev[i].VB3D-chev[i].pointN).length()>(chev[i].WB3D-chev[i].pointN).length())
        {
            vecteurX = (chev[i].VC3D-chev[i].VB3D).normalized();
        }
        else
        {
            vecteurX = (chev[i].WC3D-chev[i].WB3D).normalized();
        }
        if(chev[i].orientation==0)
        {
            initial(0,0) = vecteurX.x();
            initial(1,0) = vecteurX.y();
            initial(2,0) = vecteurX.z();
            initial(3,0) = 0;
            initial(0,2) = -chev[i].VNormal.x();
            initial(1,2) = -chev[i].VNormal.y();
            initial(2,2) = -chev[i].VNormal.z();
            initial(3,2) = 0;
            initial(0,1) = QVector3D::crossProduct(-chev[i].VNormal,vecteurX).normalized().x();
            initial(1,1) = QVector3D::crossProduct(-chev[i].VNormal,vecteurX).normalized().y();
            initial(2,1) = QVector3D::crossProduct(-chev[i].VNormal,vecteurX).normalized().z();
        }
        else
        {
            initial(0,0) = vecteurX.x();
            initial(1,0) = vecteurX.y();
            initial(2,0) = vecteurX.z();
            initial(3,0) = 0;
            initial(0,2) = chev[i].VNormal.x();
            initial(1,2) = chev[i].VNormal.y();
            initial(2,2) = chev[i].VNormal.z();
            initial(3,2) = 0;
            initial(0,1) = QVector3D::crossProduct(chev[i].VNormal,vecteurX).normalized().x();
            initial(1,1) = QVector3D::crossProduct(chev[i].VNormal,vecteurX).normalized().y();
            initial(2,1) = QVector3D::crossProduct(chev[i].VNormal,vecteurX).normalized().z();
        }
        initial(3,1) = 0;
        initial(0,3) = 0;
        initial(1,3) = 0;
        initial(2,3) = 0;
        initial(3,3) = 1;
        rotation = (final*(initial.inverted()));
        quatChev = convertirMatriceRotation(rotation);
        quatChev.normalize();
        chev[i].VA = rotationSelonQuaternion(chev[i].VA3D,quatChev);
        chev[i].VB = rotationSelonQuaternion(chev[i].VB3D,quatChev);
        chev[i].VC = rotationSelonQuaternion(chev[i].VC3D,quatChev);
        chev[i].VD = rotationSelonQuaternion(chev[i].VD3D,quatChev);
        chev[i].WA = rotationSelonQuaternion(chev[i].WA3D,quatChev);
        chev[i].WB = rotationSelonQuaternion(chev[i].WB3D,quatChev);
        chev[i].WC = rotationSelonQuaternion(chev[i].WC3D,quatChev);
        chev[i].WD = rotationSelonQuaternion(chev[i].WD3D,quatChev);
        chev[i].s1 = rotationSelonQuaternion(chev[i].s1_3D,quatChev);
        chev[i].s2 = rotationSelonQuaternion(chev[i].s2_3D,quatChev);
        chev[i].q1 = rotationSelonQuaternion(chev[i].q1_3D,quatChev);
        chev[i].q2 = rotationSelonQuaternion(chev[i].q2_3D,quatChev);
        chev[i].m1 = rotationSelonQuaternion(chev[i].m1_3D,quatChev);
        chev[i].m2 = rotationSelonQuaternion(chev[i].m2_3D,quatChev);
        chev[i].n1 = rotationSelonQuaternion(chev[i].n1_3D,quatChev);
        chev[i].n2 = rotationSelonQuaternion(chev[i].n2_3D,quatChev);
        chev[i].centre2D = rotationSelonQuaternion(chev[i].centre,quatChev);
        //et la on redresse le chevron si jamais il est trop bas du cote droit
        quatChevTotal = quatChev.normalized();
        if((chev[i].VC.y()<0)||(chev[i].WC.y()<0))
        {
            double ang;
            QVector3D centreRot, pointM;
            if(chev[i].VC.y()<chev[i].WC.y())
            {
                pointM = chev[i].VC;
            }
            else
            {
                pointM = chev[i].WC;
            }
            if(chev[i].VB.y()==0)
            {
                centreRot = chev[i].VB;
            }
            else
            {
                centreRot = chev[i].WB;
            }
            ang = qAtan(qAbs(pointM.y())/(pointM.x()-centreRot.x()));
            quatChevOz.setScalar(ang);
            quatChevOz.setVector(0.0,0.0,1.0);
            quatChevOz.normalize();
            quatChevTotal = quatChev*quatChevOz;
            quatChevTotal.normalize();
            chev[i].VA = rotationAutourDeZetCentre(chev[i].VA,ang,centreRot);
            chev[i].VB = rotationAutourDeZetCentre(chev[i].VB,ang,centreRot);
            chev[i].VC = rotationAutourDeZetCentre(chev[i].VC,ang,centreRot);
            chev[i].VD = rotationAutourDeZetCentre(chev[i].VD,ang,centreRot);
            chev[i].WA = rotationAutourDeZetCentre(chev[i].WA,ang,centreRot);
            chev[i].WB = rotationAutourDeZetCentre(chev[i].WB,ang,centreRot);
            chev[i].WC = rotationAutourDeZetCentre(chev[i].WC,ang,centreRot);
            chev[i].WD = rotationAutourDeZetCentre(chev[i].WD,ang,centreRot);
            chev[i].s1 = rotationAutourDeZetCentre(chev[i].s1,ang,centreRot);
            chev[i].s2 = rotationAutourDeZetCentre(chev[i].s2,ang,centreRot);
            chev[i].q1 = rotationAutourDeZetCentre(chev[i].q1,ang,centreRot);
            chev[i].q2 = rotationAutourDeZetCentre(chev[i].q2,ang,centreRot);
            chev[i].m1 = rotationAutourDeZetCentre(chev[i].m1,ang,centreRot);
            chev[i].m2 = rotationAutourDeZetCentre(chev[i].m2,ang,centreRot);
            chev[i].n1 = rotationAutourDeZetCentre(chev[i].n1,ang,centreRot);
            chev[i].n2 = rotationAutourDeZetCentre(chev[i].n2,ang,centreRot);
            chev[i].centre2D = rotationAutourDeZetCentre(chev[i].centre2D,ang,centreRot);
        }

        //on stocke le quaternion pour passer de la 3d a la 2d
        chev[i].quatRot = quatChevTotal.conjugate();

        //maintenant on cherche les 4 coins de la planche
        chev[i].A.setZ(chev[i].VA.z());chev[i].A.setX(valeurLaPlusAGauche(chev[i].VA,chev[i].VB,chev[i].WA,chev[i].WB));
        chev[i].B.setZ(chev[i].VA.z());chev[i].B.setX(chev[i].A.x());
        chev[i].C.setZ(chev[i].VA.z());chev[i].C.setX(valeurLaPlusADroite(chev[i].VC,chev[i].VD,chev[i].WC,chev[i].WD));
        chev[i].D.setZ(chev[i].VA.z());chev[i].D.setX(chev[i].C.x());
        chev[i].B.setY(valeurLaPlusEnBas(chev[i].VB,chev[i].VC,chev[i].WB,chev[i].WC));
        chev[i].C.setY(chev[i].B.y());
        if(chev[i].C.x()<chev[i].centre2D.x())
        {
            chev[i].A.setY(qMax(chev[i].VD.y(),chev[i].WD.y()));
        }
        else if(chev[i].A.x()>chev[i].centre2D.x())
        {
            chev[i].A.setY(qMax(chev[i].VA.y(),chev[i].WA.y()));
        }
        else
        {
            chev[i].A.setY(chev[i].centre2D.y()+chev[i].rayonExtrados);
        }
        chev[i].D.setY(chev[i].A.y());

        chev[i].translation = chev[i].B;
        //et on redecale tous les autres points pour obtenir le point B comme reference
        chev[i].VA -= chev[i].B;
        chev[i].VB -= chev[i].B;
        chev[i].VC -= chev[i].B;
        chev[i].VD -= chev[i].B;
        chev[i].WA -= chev[i].B;
        chev[i].WB -= chev[i].B;
        chev[i].WC -= chev[i].B;
        chev[i].WD -= chev[i].B;
        chev[i].s1 -= chev[i].B;
        chev[i].s2 -= chev[i].B;
        chev[i].q1 -= chev[i].B;
        chev[i].q2 -= chev[i].B;
        chev[i].m1 -= chev[i].B;
        chev[i].m2 -= chev[i].B;
        chev[i].n1 -= chev[i].B;
        chev[i].n2 -= chev[i].B;
        chev[i].centre2D -= chev[i].B;
        chev[i].A -= chev[i].B;
        chev[i].C -= chev[i].B;
        chev[i].D -= chev[i].B;
        chev[i].B -= chev[i].B;

        //calcul des so,s3 ...
        chev[i].s0 = intersectionDroiteAvecDroiteHorizontale(chev[i].s1,chev[i].s2,0.0);
        chev[i].q0 = intersectionDroiteAvecDroiteHorizontale(chev[i].q1,chev[i].q2,0.0);
        chev[i].m0 = intersectionDroiteAvecDroiteHorizontale(chev[i].m1,chev[i].m2,0.0);
        chev[i].n0 = intersectionDroiteAvecDroiteHorizontale(chev[i].n1,chev[i].n2,0.0);
        chev[i].s3 = intersectionDroiteAvecDroiteHorizontale(chev[i].s1,chev[i].s2,chev[i].A.y());
        chev[i].q3 = intersectionDroiteAvecDroiteHorizontale(chev[i].q1,chev[i].q2,chev[i].A.y());
        chev[i].m3 = intersectionDroiteAvecDroiteHorizontale(chev[i].m1,chev[i].m2,chev[i].A.y());
        chev[i].n3 = intersectionDroiteAvecDroiteHorizontale(chev[i].n1,chev[i].n2,chev[i].A.y());

        //calcul du point haut
        chev[i].O = QVector3D(chev[i].centre2D.x(),chev[i].A.y(),0.0);
        chev[i].O2 = QVector3D(chev[i].centre2D.x(),chev[i].A.y()-dna_retombeeChevron,0.0);

        //calcul des angles de tracage
        chev[i].angleTracageCoteAB = angleEntre2Vecteurs3D(QVector3D(1.0,0.0,0.0),chev[i].VB-chev[i].VA);
        chev[i].angleTracageCoteCD = angleEntre2Vecteurs3D(chev[i].VC-chev[i].VD,QVector3D(-1.0,0.0,0.0));

        //calcul angles de coupe
        QVector3D vplan;
        vplan = QVector3D::crossProduct((chev[i].VA-chev[i].VB),(chev[i].WB-chev[i].VB));
        chev[i].angleCoupeCoteAB = angleEntre2Vecteurs3D(QVector3D(0.0,0.0,1.0),vplan)-pi/2.0;
        vplan = QVector3D::crossProduct((chev[i].WC-chev[i].VC),(chev[i].VD-chev[i].VC));
        chev[i].angleCoupeCoteCD = angleEntre2Vecteurs3D(QVector3D(0.0,0.0,1.0),vplan)-pi/2.0;

        //et on calcule facilement la retombee et la longueur de la planche dans laquelle devra etre taille le chevron
        chev[i].longueurPlanche = chev[i].C.length();
        chev[i].retombeePlanche = chev[i].A.length();
        chev[i].epaisseurChevron = dna_epaisseurChevron;
    }

}

chevronDNA domenidabeille::rotateChevron(chevronDNA chev, double angPhi)
{
    chevronDNA res;
    res.pointNcoteV = rotationAutourDeZ(chev.pointNcoteV,angPhi);
    res.pointNcoteW = rotationAutourDeZ(chev.pointNcoteW,angPhi);
    res.VNormal = rotationAutourDeZ(chev.VNormal,angPhi);
    res.centreCoteV = rotationAutourDeZ(chev.centreCoteV,angPhi);
    res.centreCoteW = rotationAutourDeZ(chev.centreCoteW,angPhi);
    res.rayonExtrados = chev.rayonExtrados;
    res.rayonIntrados = chev.rayonIntrados;
    return res;
}

double domenidabeille::distanceAuCentre(QVector3D chev)
{
    double resu;
    resu = qSqrt(qPow(chev.x(),2.0)+qPow(chev.y(),2.0));
    return resu;
}

double domenidabeille::pointPlusEloigne()
{
    return qMax(qMax(dna_diametreSol,dna_hauteur),dna_diametreTonoo);
}

double domenidabeille::volume()
{
    double res;
    res = pi*qPow(dna_diametreSol/2.0,2.0)*dna_epaisseurSabliere+pi*dna_rayonCourbureDome*qPow(dna_centreCourbe.x(),2.0)*(qSin(dna_angleHorizontalCentreTonoo)-qSin(dna_angleHorizontalCentreSabliere))+pi*qPow(dna_rayonCourbureDome,3.0)*(qSin(dna_angleHorizontalCentreTonoo)*qPow(qCos(dna_angleHorizontalCentreTonoo),2.0)+2.0*qSin(dna_angleHorizontalCentreTonoo)-qSin(dna_angleHorizontalCentreSabliere)*qPow(qCos(dna_angleHorizontalCentreSabliere),2.0)-2.0*qSin(dna_angleHorizontalCentreSabliere))/3.0+pi*dna_centreCourbe.x()*qPow(dna_rayonCourbureDome,2.0)*(qSin(dna_angleHorizontalCentreTonoo)*qCos(dna_angleHorizontalCentreTonoo)+dna_angleHorizontalCentreTonoo-qSin(dna_angleHorizontalCentreSabliere)*qCos(dna_angleHorizontalCentreSabliere)-dna_angleHorizontalCentreSabliere);
    return res;
}

double domenidabeille::surfaceDeToit()
{
    double res;
    res = dna_diametreSol*pi*dna_epaisseurSabliere+2.0*pi*dna_rayonCourbureDome*dna_centreCourbe.x()*(dna_angleHorizontalCentreTonoo-dna_angleHorizontalCentreSabliere)+2.0*pi*qPow(dna_rayonCourbureDome,2.0)*(qSin(dna_angleHorizontalCentreTonoo)-qSin(dna_angleHorizontalCentreSabliere));
    return res;
}

double domenidabeille::surfaceAuSol()
{
    return pi*qPow(dna_diametreSol/2.0,2.0);
}

QVector3D domenidabeille::point3D(double alpha, double phi)
{
    QVector3D res;
    res.setX((dna_centreCourbe.x()+dna_rayonCourbureDome*qCos(alpha))*qCos(phi));
    res.setY((dna_centreCourbe.x()+dna_rayonCourbureDome*qCos(alpha))*qSin(phi));
    res.setZ((dna_centreCourbe.y()+dna_rayonCourbureDome*qSin(alpha)));
    return res;
}

void domenidabeille::calculPointPrime(int numeroDuPoint)
{
    double roo;
    QVector3D Vn,noeud0,noeud1,V01,V0Centre,noeudPrime;
    if(numeroDuPoint <2)
    {
        Vn = QVector3D::crossProduct((point3D(dna_noeud[numeroDuPoint].alphaZero,0)-point3D(dna_noeud[numeroDuPoint+1].alphaSeconde,dna_angleDemiDiagonaleHorizontal-dna_noeud[numeroDuPoint+1].phiSeconde)),(dna_centreCourbe3D-point3D(dna_noeud[numeroDuPoint].alphaZero,0)));
    }
    else
    {
        Vn = QVector3D::crossProduct(-(point3D(dna_noeud[numeroDuPoint].alphaZero,0)-point3D(dna_noeud[numeroDuPoint-1].alphaPrime,dna_angleDemiDiagonaleHorizontal-dna_noeud[numeroDuPoint-1].phiPrime)),(dna_centreCourbe3D-point3D(dna_noeud[numeroDuPoint].alphaZero,0)));
    }
    roo = angleEntre2Vecteurs3D(Vn,QVector3D(0,1,0));
    dna_noeud[numeroDuPoint].phiPrime = dna_decalageNoeud*qSin(roo)/(dna_rayonCourbureDome*2.0);
    // apres on cherche sa position exacte en alpha
    if(numeroDuPoint <2)
    {
        noeud0 = point3D(dna_noeud[numeroDuPoint].alphaZero,dna_noeud[numeroDuPoint].phiPrime);
        noeud1 = point3D(dna_noeud[numeroDuPoint+1].alphaSeconde,dna_angleDemiDiagonaleHorizontal-dna_noeud[numeroDuPoint+1].phiSeconde+dna_noeud[numeroDuPoint].phiPrime);
    }
    else
    {
        noeud0 = point3D(dna_noeud[numeroDuPoint].alphaZero,dna_noeud[numeroDuPoint].phiPrime);
        noeud1 = point3D(dna_noeud[numeroDuPoint-1].alphaPrime,-dna_angleDemiDiagonaleHorizontal+dna_noeud[numeroDuPoint-1].phiPrime-dna_noeud[numeroDuPoint].phiPrime);
     }
    V01 = noeud1-noeud0;
    V0Centre.setX(dna_centreCourbe.x()*qCos(dna_noeud[numeroDuPoint].phiPrime)-noeud0.x());
    V0Centre.setY(dna_centreCourbe.x()*qSin(dna_noeud[numeroDuPoint].phiPrime)-noeud0.y());
    V0Centre.setZ(dna_centreCourbe.y()-noeud0.z());
    Vn = QVector3D::crossProduct(V01,V0Centre);
    noeudPrime = intersection1Plan1CercleDansPlanOxz3D(noeud0,Vn,dna_centreCourbe3D,dna_rayonCourbureDome);
    dna_noeud[numeroDuPoint].alphaPrime = angleHorizontalCentrePoint3D(dna_centreCourbe3D,noeudPrime);

    //et voila
}

void domenidabeille::calculPointSeconde(int numeroDuPoint)
{
    QVector3D noeA, noeA1prime, noeA2prime, noeB, centreA, centreB;
    QVector3D VAA1prime, VAcentreA, VA2primeB, VBcentreB, VN1, VN2;

    //les points
    noeA = point3D(dna_noeud[numeroDuPoint-1].alphaZero,0);
    noeA1prime = point3D(dna_noeud[numeroDuPoint-1].alphaPrime,-dna_noeud[numeroDuPoint-1].phiPrime);
    noeA2prime = point3D(dna_noeud[numeroDuPoint-1].alphaPrime,dna_angleDiagonaleHorizontal-dna_noeud[numeroDuPoint-1].phiPrime);
    noeB = point3D(dna_noeud[numeroDuPoint].alphaZero,dna_angleDemiDiagonaleHorizontal);
    centreA.setX(dna_centreCourbe3D.x());
    centreA.setY(0);
    centreA.setZ(dna_centreCourbe3D.z());
    centreB.setX(dna_centreCourbe3D.x()*qCos(dna_angleDemiDiagonaleHorizontal));
    centreB.setY(dna_centreCourbe3D.x()*qSin(dna_angleDemiDiagonaleHorizontal));
    centreB.setZ(dna_centreCourbe3D.z());

    //les vecteurs
    VAA1prime = noeA1prime-noeA;
    VAcentreA = centreA-noeA;
    VN1 = QVector3D::crossProduct(VAA1prime,VAcentreA);
    VA2primeB = noeB-noeA2prime;
    VBcentreB = centreB-noeB;
    VN2 = QVector3D::crossProduct(VA2primeB,VBcentreB);

    double m_Bprime,m_Cprime,m_Dprime,m_F,m_G,m_H,m_D,m_E,m_I,m_J,m_K,m_L,m_M,m_N,m_P,m_Q,m_S,m_T,m_U,m_Gprime,m_Hprime,m_W,m_a,m_b,m_c,m_d,m_e;
    m_Bprime = -VN1.y()/VN1.x();
    m_Cprime = -VN1.z()/VN1.x();
    m_D = QVector3D::dotProduct(noeA,VN1);
    m_Dprime = m_D/VN1.x();
    m_E = QVector3D::dotProduct(noeB,VN2);
    m_F = -VN2.y()/VN2.x();
    m_G = -VN2.z()/VN2.x();
    m_H = m_E/VN2.x();
    m_K = (m_Cprime-m_G)/(m_F-m_Bprime);
    m_L = (m_Dprime-m_H)/(m_F-m_Bprime);
    m_I = m_Bprime*m_K+m_Cprime;
    m_J = m_Bprime*m_L+m_Dprime;
    m_M = qPow(dna_centreCourbe3D.z(),2.0)-qPow(dna_centreCourbe3D.x(),2.0)-qPow(dna_rayonCourbureDome,2.0);
    m_N = 4.0*qPow(dna_centreCourbe3D.x(),2.0)*(qPow(dna_rayonCourbureDome,2.0)-qPow(dna_centreCourbe3D.z(),2.0));
    m_Q = 4.0*qPow(dna_centreCourbe3D.z(),2.0)+4.0*qPow(dna_centreCourbe3D.x(),2.0);
    m_P = -4.0*dna_centreCourbe3D.z()*m_M+8.0*qPow(dna_centreCourbe3D.x(),2.0)*dna_centreCourbe3D.z();
    m_T = m_N-qPow(m_M,2.0);
    m_S = 2.0*m_M;
    m_U = -4.0*dna_centreCourbe3D.z();
    m_Gprime = qPow(m_I,2.0)+qPow(m_K,2.0)+1;
    m_Hprime = 2*m_I*m_J+2*m_K*m_L;
    m_W = qPow(m_J,2.0)+qPow(m_L,2.0);
    m_a = qPow(m_Gprime,2.0);
    m_b = 2.0*m_Gprime*m_Hprime+m_U*m_Gprime;
    m_c = qPow(m_Hprime,2.0)+2*m_Gprime*m_W+m_S*m_Gprime+m_Q+m_U*m_Hprime;
    m_d = 2*m_Hprime*m_W+m_S*m_Hprime+m_U*m_W+m_P;
    m_e = qPow(m_W,2.0)+m_S*m_W-m_T;
    //resolution equation du 4 eme degre az4+bz3+cz2+dz+e=0 d'apres ferrari
    //QVector<double> sol_x;
    //sol_x = resolutionDegre4Ferrari(m_a,m_b,m_c,m_d,m_e);
    QVector<complexes> sol_xx;
    sol_xx = resolutionDegre4Lagrange(m_a,m_b,m_c,m_d,m_e);
        //et la on a 4 solutions
    QVector<QVector3D> sol;
    QVector<double> dist;
    sol.clear();
    sol.resize(4);
    dist.clear();
    dist.resize(4);
    double distmin = 90000.0;
    for(int i=0;i<4;i++)
    {
        sol[i].setZ(sol_xx[i].re());
        sol[i].setX(m_I*sol[i].z()+m_J);
        sol[i].setY(m_K*sol[i].z()+m_L);
        dist[i] = (sol[i]-noeB).length();
        if(dist[i]<distmin)
        {
            distmin = dist[i];
        }
    }
    for(int i=0;i<4;i++)
    {
        if(dist[i]==distmin)
        {
            dna_noeud[numeroDuPoint].alphaSeconde = angleHorizontalCentrePoint3D(dna_centreCourbe3D,sol[i]);
            dna_noeud[numeroDuPoint].phiSeconde = anglePhiHorizontalEntre2PointsEt1Centre3D(dna_centreCourbe3D,sol[i],noeB);
        }
    }

}

QStringList domenidabeille::listeChevrons()
{
    QStringList m_liste;
    QString str, str2, ligneAjout;
    for(int i=0;i<chev.size();i++)
    {
        m_liste << str.setNum(dna_NbreLosangesHorizontal)+" chevrons de type "+(char)(i+65);
    }
    return m_liste;
}
