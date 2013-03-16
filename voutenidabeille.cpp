#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <qmath.h>

#include "voutenidabeille.h"
#include "mathema.h"


vouteNidAbeille::vouteNidAbeille()
{
}

QString vouteNidAbeille::explicationGenerale()
{
    QString str,str2,resu;

    resu = "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A) Généralités</strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;La charpente en nid d\'abeille, évolution des charpentes de Philibert de l\'Orme (1510-1570), permet comme ces dernières d\'utiliser des bois de faible section, comme des planches de 30 ou 40 mm d\'épaisseur et de longueur courte (environ 2 m 30 maximum).<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ce type de charpente renvoie toute sa poussée sur les sablières, comme toute voûte, et il est nécessaire d\'équilibrer cette force qui a tendance à pousser les sablières vers l\'extérieur en \"retenant\" les sablières avec des \"entraits\". <br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Un soliveage de plancher, au même niveau que les sablières et avec les solives placées perpendiculairement aux sablières remplira très bien ce rôle d\'entrait. De même si les sablières sont fixées à une dalle d\'un seul tenant.</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"color: #ff9900;\"><strong><span style=\"font-size: 12pt;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;B) Les chevrons</span></strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Précisons tout d\'abord que nous allons partir de planches rectangulaires et tailler ensuite la forme courbe de chaque chevron.<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Il est possible de ne tailler que l\'extrados du chevron courbe, en gardant l\'intrados rectiligne, cela permet, pour une même largeur de planches de départ, une meilleure solidité, mais néanmoins une esthétique différente (voute moins lisse de l\'intérieur) par rapport à des chevrons dont l\'intrados est courbe.<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Il y a 2 types de chevrons, les chevrons penchés à droite (quand on regarde la voute de l\'extérieur, leur bas est à gauche et le haut va vers la droite) et les chevrons penchés à gauche. Les chevrons de type penché à gauche sont les symétriques de chevrons de type penché à droite par rapport à un plan (comme si l\'un était le reflet de l\'autre dans un miroir).<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; N\'importe quel chevron est symétrique par rapport à un axe bissecteur qui passe dans la moitié de son épaisseur, parallèle à sa largeur.</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;La méthode proposée pour tailler les chevrons est la suivante :</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>I</strong> - marquer un chevron \"gabarit\": sur une planche, tracer le dessin d\'un chevron en marquant bien ses points les plus loins de son centre (soit W, soit V, ça dépend).</p>";
    resu += "<p align=center><img height=\"177\" width=\"400\" src=\":/images/vna/vnaAngleTracage\" /></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>II</strong> - découper l\'extrados courbe (et l\'intrados si voulu) à la scie sauteuse ou à la scie à ruban. Ne coupez pas encore les 2 bouts en biseaux mais coupez sur les lignes les plus extérieures, avec un angle de scie normal (0°), pour obtenir des coupes bien perpendiculaires à la face du dessus du chevron.</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>III</strong> - a partir de ce chevron \"gabarit\", vous pouvez découper tous les autres chevrons. Une technique, la défonceuse avec une mèche \"à copier\" qui va suivre le gabarit en découpant celui du dessus. Une autre technique, plus dangereuse, utiliser une toupie.</p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>IV</strong> - une fois tous les chevrons découpés, vous pouvez biseauter les bouts (une fois dans un sens pour les chevrons penchés à droite, une fois dans l\'autre pour les chevrons penchés à gauche). Pour cela, vous pouvez utiliser une scie à onglets à inclinaison variable, ou une scie circulaire avec l\'angle de la lame réglable.</p>";
    resu += "<p align=center><img height=\"162\" width=\"400\" src=\":/images/vna/vnaAngleCoupe\" /></p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"color: #ff9900;\"><strong><span style=\"font-size: 12pt;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C) Les chevrons de bord</span></strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ce sont juste des chevrons classiques sur lesquels on va garder un bout (ou aucun) et re-biseauter un nouveau bout ailleurs (ou les deux). Si vous vous y prenez bien, vous pouvez souvent loger plusieurs chevrons de bord dans un chevron normal.</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"font-size: 12pt;\"><strong><span style=\"color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;D) Les sablières</span></strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Les sablières sont des pièces fixées sur une assise plane et stable. Les chevrons de bord de type penché à droite viennent s\'y fixer à intervalles réguliers de <strong> "+str.setNum(vna_diagonaleHorizontaleLosange*100.0,'f',1)+" </strong> cm avec un angle d\'arrivée de <strong> "+str2.setNum(degres(vna_angleChevronBordSabliere),'f',1)+" </strong> °.</p>";
    resu += "<p align=center><img height=\"140\" width=\"400\" src=\":/images/vna/vnaArriveeSabliere\" /></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Afin de bloquer le mouvement des assemblages au niveau des sablières, on pourra rajouter de cales.</p>";
    resu += "<p align=center><img height=\"141\" width=\"250\" src=\":/images/vna/vnaFixationSabliere\" /></p>";
    resu += "<p><br /></p>";
    resu += "<p><strong><span style=\"font-size: 12pt; color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;E) Les rives</span></strong></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Les rives courbes, pourront être obtenues en \"lamellé cloué\", en assemblant plusieurs rangées de planches jusqu\'à obtenir l\'épaisseur voulue. Les planches seront vissées ou clouées entre elles et les jonctions de chaque rangées seront décalées avec les rangées suivantes.<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Les chevrons de bord d\'un même type y arrivent avec un intervalle vertical régulier de <strong> "+str.setNum(vna_diagonaleVerticaleLosange*100.0,'f',1)+" </strong> cm.</p>";
    resu += "<p align=center><img height=\"273\" width=\"250\" src=\":/images/vna/vnaRives\" /></p>";
    resu += "<p><br /></p>";
    if(vna_choixType!=1)
    {
        resu += "<p><strong><span style=\"font-size: 12pt; color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;F) La faitière</span></strong></p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;La faitière est une pièce massive sur laquelle viendront se fixer (vis ou clous) les bouts des chevrons de bord. On pourra rajouter des cales pour bloquer le mouvement latéral des bouts de chevrons.</p>";
        resu += "<p align=center><img height=\"141\" width=\"250\" src=\":/images/vna/vnaFixationFaitiere\" /></p>";
        resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Les chevrons de bord de type penchés à droite y arrivent avec un angle de<strong> "+str.setNum(degres(vna_angleChevronBordFaitiere),'f',1)+" </strong> ° et  l\'intervalle horizontal, toujours le même est de<strong> "+str2.setNum(vna_diagonaleHorizontaleLosange*100.0,'f',1)+" </strong>cm.</p>";
        resu += "<p align=center><img height=\"140\" width=\"400\" src=\":/images/vna/vnaArriveeFaitiere\" /></p>";
        resu += "<p><br /></p>";
    }
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;G) Fixations des noeuds</strong></span></p>";
    resu += "<p align=center><img height=\"223\" width=\"199\" src=\":/images/vna/vnaAssemblage\" /></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;L\'assemblage ne se fait pas au milieu des chevrons, il faut respecter une valeur de décalage du noeud. Sinon, le noeud pourrait pivoter, le décalage assurant la rigidité de la structure.<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Les chevrons seront premièrement cloués ou vissés en bout au niveau des assemblages. Ensuite, si la structure est soumise à de fortes contraintes, il sera nécessaire de solidifier par un boulonnage: tige filetée, écroux, et rondelles crantées si possibles.</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"font-size: 12pt;\"><strong><span style=\"color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;H) Montage structure</span></strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Le montage débute par la mise en place des sablières, rives et de la faitière (si il y en a une), cette dernière devant être étayée. Ensuite il suffit de choisir un coin entre la sablière et une des 2 rives et commencer à poser une rangée, puis l\'autre ... en prenant soin d\'étayer le travail tant que tout n\'a pas été posé.</p>";
    resu += "<p><br /></p>";
    resu += "<p><span style=\"font-size: 12pt;\"><strong><span style=\"color: #ff9900;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;I) Couverture, Ouvertures</span></strong></span></p>";
    resu += "<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pour couvrir ce genre de structure, on peut voliger directement sur les chevrons ou poser un liteaunage horizontal. Les matériaux de couvertures pourront être la tuile, le bardeau de bois, l\'ardoise ...<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Si l\'on désire effectuer une ouverture style velux, on prendra soin de consolider la structure au niveau du chevêtre.<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Si une porte de grandes dimensions doit être effectuées (style hangar), on reprendra le poids de la charpente par un linteau et deux poteaux descendant au sol.</p>";

    return resu;
}

QString vouteNidAbeille::explicationChevrons()
{
    QString resu,str;

    //calcul des angles de coupe
    QVector3D vn1,vn2,v1,v2;
    double m_angleCoupe,m_angleTracage;
    vn1.setX(0);vn1.setY(0);vn1.setZ(-1.0);
    v1.setX(chev_VB.x()-chev_A.x()-(chev_VA.x()-chev_A.x()));v1.setY(chev_VB.y()-chev_B.y()-(chev_VA.y()-chev_B.y()));v1.setZ(0);
    v2.setX(chev_VE.x()-chev_A.x()-(chev_VA.x()-chev_A.x()));v2.setY(chev_VE.y()-chev_B.y()-(chev_VA.y()-chev_B.y()));v2.setZ(-vna_epaisseurChevron);
    vn2 = QVector3D::crossProduct(v2,v1);
    m_angleCoupe = degres(pi/2.0-angleEntre2Vecteurs3D(vn1,vn2));
    vn1.setX(0);vn1.setY(1);vn1.setZ(0);
    vn2 = -v1;
    m_angleTracage = degres(angleEntre2Vecteurs3D(vn1,vn2));
    if(m_angleTracage>90)
    {
        m_angleTracage = -(180-m_angleTracage);
    }

    // resultats pour la page des chevrons entiers
    resu += "<p>Les chevrons gauches sont les symétriques des chevrons droits (dans un miroir, symétrie par rapport à un plan).<br />";
    resu += "Chaque chevron peut être utilisé tête-bêche (symétrie par rapport à un axe central).<p>";
    resu += "<p>Nombre de chevrons penchés à gauche : <strong>"+str.setNum((double)vna_nombreChevronGauche,'f',0)+"</strong><br />";
    resu += "Nombre de chevrons penchés à droite : <strong>"+str.setNum((double)vna_nombreChevronDroite,'f',0)+"</strong></p>";
    resu += "<p>Dimensions des planches dans lesquelles les chevrons seront taillés :<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- épaisseur : <strong>" + str.setNum(vna_epaisseurChevron*100.0,'f',1)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- retombée : <strong>" + str.setNum(vna_retombeeChevron*100.0,'f',1)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur : <strong>" + str.setNum(chev_longueurChevron,'f',1)+"</strong> cm</p>";
    resu += "<p>Dimensions des chevrons courbes :<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- retombée : <strong>" + str.setNum((chev_rayonExtrado-chev_rayonIntrado)*100,'f',1)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- rayon de courbure de l'intrados : <strong>" +str.setNum(chev_rayonIntrado,'f',2)+"</strong> m<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- rayon de courbure de l'extrados : <strong>"+str.setNum(chev_rayonExtrado,'f',2)+"</strong> m<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- angle de traçage alpha : <strong>"+str.setNum(m_angleTracage,'f',1)+"</strong> °<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- angle de coupe omega : <strong>"+str.setNum(m_angleCoupe,'f',1)+"</strong> °</p>";
    resu += "<p>Les points notés V sont sur la face supérieure de la planche, les points notés W sont sur la face cachée de la planche.<br />";
    resu += "Le point B est pris comme origine du repère.</p>";
    return resu;
}

QString vouteNidAbeille::explicationChevronTronque(int num)
{
    QString texteChevronTronque,str,str2;
    texteChevronTronque += "<p>Il y a <strong>"+str.setNum((double)chevCoupe[num].nombre,'f',0)+"</strong> chevrons de type <strong>"+(char)(num+65)+"</strong><br />";
    switch(chevCoupe[num].orientation)
    {
    case 0:
    texteChevronTronque += "Ce sont des chevrons penchés à droite et juste coupés sur leur bas au niveau de la rive gauche.</p>";
    break;
    case 1:
    texteChevronTronque += "Ce sont des chevrons penchés à droite et juste coupés sur leur bas au niveau de la sablière.</p>";
    break;
    case 2:
    texteChevronTronque += "Ce sont des chevrons penchés à droite et juste coupés sur leur haut au niveau de la rive droite.</p>";
    break;
    case 3:
    texteChevronTronque += "Ce sont des chevrons penchés à droite et juste coupés sur leur haut au niveau de la faitière.</p>";
    break;
    case 5:
        if(chevCoupe[num].nombre==1)
        {
            texteChevronTronque += "C'est un chevron penché à droite et coupé à ses deux bouts par la faitière et la rive gauche. Il va donc dans le coin en haut à gauche.</p>";
        }
        else
        {
            texteChevronTronque += "Ce sont des chevrons penchés à droite et coupés à leurs deux bouts par la faitière et la rive gauche. Ils vont donc dans le coin en haut à gauche.</p>";
        }
    break;
    case 6:
        if(chevCoupe[num].nombre==1)
        {
            texteChevronTronque += "C'est un chevron penché à droite et coupé à ses deux bouts par la rive droite et la sablière. Il va donc dans le coin en bas à droite.</p>";
        }
        else
        {
            texteChevronTronque += "Ce sont des chevrons penchés à droite et coupés à leurs deux bouts par la rive droite et la sablière. Ils vont donc dans le coin en bas à droite.</p>";
        }
    break;
    case 8:
    texteChevronTronque += "Ce sont des chevrons penchés à gauche et juste coupés sur leur haut au niveau de la rive gauche.</p>";
    break;
    case 9:
    texteChevronTronque += "Ce sont des chevrons penchés à gauche et juste coupés sur leur bas au niveau de la sablière.</p>";
    break;
    case 10:
    texteChevronTronque += "Ce sont des chevrons penchés à gauche et juste coupés sur leur bas au niveau de la rive droite.</p>";
    break;
    case 11:
    texteChevronTronque += "Ce sont des chevrons penchés à gauche et juste coupés sur leur haut au niveau de la faitière.</p>";
    break;
    case 12:
        if(chevCoupe[num].nombre==1)
        {
            texteChevronTronque += "C'est un chevron penché à gauche et coupé à ses deux bouts par la rive gauche et la sablière. Il va donc dans le coin en bas à gauche.</p>";
        }
        else
        {
            texteChevronTronque += "Ce sont des chevrons penchés à gauche et coupés à leurs deux bouts par la rive gauche et la sablière. Ils vont donc dans le coin en bas à gauche.</p>";
        }
    break;
    case 15:
        if(chevCoupe[num].nombre==1)
        {
            texteChevronTronque += "C'est un chevron penché à gauche et coupé à ses deux bouts par la faitière et la rive droite. Il va donc dans le coin en haut à droite.</p>";
        }
        else
        {
            texteChevronTronque += "Ce sont des chevrons penchés à gauche et coupés à leurs deux bouts par la faitière et la rive droite. Ils vont donc dans le coin en haut à droite.</p>";
        }
    break;
    }

    //calcul des angles de coupe
    QVector3D vn1,vn2,v1,v2;
    double m_angleCoupeGauche,m_angleCoupeDroite,m_angleTracageGauche,m_angleTracageDroite;
    vn1.setX(0);vn1.setY(0);vn1.setZ(-1.0);
    v1 = chevCoupe[num].WA-chevCoupe[num].VA;
    v2 = chevCoupe[num].VB-chevCoupe[num].VA;
    vn2 = QVector3D::crossProduct(v1,v2);
    m_angleCoupeGauche = degres(pi/2.0-angleEntre2Vecteurs3D(vn1,vn2));
    v1 = chevCoupe[num].VC-chevCoupe[num].VD;
    v2 = chevCoupe[num].WD-chevCoupe[num].VD;
    vn2 = QVector3D::crossProduct(v1,v2);
    m_angleCoupeDroite = degres(pi/2.0-angleEntre2Vecteurs3D(vn1,vn2));
    vn1.setX(0);vn1.setY(1);vn1.setZ(0);
    vn2 = chevCoupe[num].VA-chevCoupe[num].VB;
    m_angleTracageGauche = degres(angleEntre2Vecteurs3D(vn1,vn2));
    if(m_angleTracageGauche>90)
    {
        m_angleTracageGauche = -(180-m_angleTracageGauche);
    }
    vn2 = chevCoupe[num].VD-chevCoupe[num].VC;
    m_angleTracageDroite = degres(angleEntre2Vecteurs3D(vn1,vn2));
    if(m_angleTracageDroite>90)
    {
        m_angleTracageDroite = -(180-m_angleTracageDroite);
    }

    texteChevronTronque += "<p>Parametres de coupe du chevron tronque : <br />";
    texteChevronTronque += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- à gauche : angle de traçage <strong>"+str.setNum(m_angleTracageGauche,'f',1)+"</strong> ° / angle de coupe <strong>"+str2.setNum(m_angleCoupeGauche,'f',1)+"</strong> °<br />";
    texteChevronTronque += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- à droite : angle de traçage <strong>"+str.setNum(m_angleTracageDroite,'f',1)+"</strong> ° / angle de coupe <strong>"+str2.setNum(m_angleCoupeDroite,'f',1)+"</strong> °</p>";
    texteChevronTronque += "<p>Attention, l'image ci-dessus n'est pas réaliste, notamment au niveau de l'angle et du sens de la coupe. Elle sert juste à donner les notations.<br />";
    texteChevronTronque += "Comme avant, les points notés V sont sur la face supérieure de la planche, les points notés W sont sur la face cachée de la planche.</p>";

    return texteChevronTronque;
}

QString vouteNidAbeille::tableauChevronDroite()
{
    //les tableaux de coordonnees
    QString tableau,str,str2;

    tableau = "<table cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
    tableau += "<td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>A</strong></td><td>"+str.setNum((chev_A.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_A.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>O1</strong></td><td>"+str.setNum((chev_O1.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_O1.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>B</strong></td><td>"+str.setNum((chev_B.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_B.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>O2</strong></td><td>"+str.setNum((chev_O2.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_O2.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>C</strong></td><td>"+str.setNum((chev_C.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_C.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>O3</strong></td><td>"+str.setNum((chev_O3.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_O3.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>D</strong></td><td>"+str.setNum((chev_D.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_D.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>s0</strong></td><td>"+str.setNum((chev_s0.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_s0.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VA</strong></td><td>"+str.setNum((chev_VA.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VA.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>s1</strong></td><td>"+str.setNum((chev_s1.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_s1.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VB</strong></td><td>"+str.setNum((chev_VB.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VB.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>s2</strong></td><td>"+str.setNum((chev_s2.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_s2.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VC</strong></td><td>"+str.setNum((chev_VC.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VC.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>s3</strong></td><td>"+str.setNum((chev_s3.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_s3.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VD</strong></td><td>"+str.setNum((chev_VD.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VD.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>q0</strong></td><td>"+str.setNum((chev_q0.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_q0.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WA</strong></td><td>"+str.setNum((chev_VE.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VE.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>q1</strong></td><td>"+str.setNum((chev_q1.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_q1.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WB</strong></td><td>"+str.setNum((chev_VF.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VF.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>q2</strong></td><td>"+str.setNum((chev_q2.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_q2.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WC</strong></td><td>"+str.setNum((chev_VG.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VG.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>q3</strong></td><td>"+str.setNum((chev_q3.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_q3.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WD</strong></td><td>"+str.setNum((chev_VH.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VH.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td></td><td></td><td></td></tr></tbody></table>";

    return tableau;
}

QString vouteNidAbeille::tableauChevronGauche()
{
    //les tableaux de coordonnees
    QString tableau,str,str2;

    tableau = "<table cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
    tableau += "<td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>A</strong></td><td>"+str.setNum((chev_A.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_A.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>O1</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_O1.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_O1.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>B</strong></td><td>"+str.setNum((chev_B.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_B.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>O2</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_O2.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_O2.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>C</strong></td><td>"+str.setNum((chev_C.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_C.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>O3</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_O3.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_O3.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>D</strong></td><td>"+str.setNum((chev_D.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_D.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>s0</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_s0.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_s0.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WA</strong></td><td>"+str.setNum((chev_VA.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VA.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>s1</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_s1.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_s1.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WB</strong></td><td>"+str.setNum((chev_VB.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VB.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>s2</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_s2.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_s2.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WC</strong></td><td>"+str.setNum((chev_VC.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VC.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>s3</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_s3.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_s3.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>WD</strong></td><td>"+str.setNum((chev_VD.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VD.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>q0</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_q0.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_q0.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VA</strong></td><td>"+str.setNum((chev_VE.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VE.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>q1</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_q1.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_q1.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VB</strong></td><td>"+str.setNum((chev_VF.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VF.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>q2</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_q2.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_q2.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VC</strong></td><td>"+str.setNum((chev_VG.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VG.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>q3</strong></td><td>"+str.setNum((2.0*(chev_O1.x()-chev_A.x())-(chev_q3.x()-chev_A.x()))*100.0,'f',1)+"</td><td>"+str2.setNum((chev_q3.y()-chev_B.y())*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VD</strong></td><td>"+str.setNum((chev_VH.x()-chev_A.x())*100.0,'f',1)+"</td><td>"+str2.setNum((chev_VH.y()-chev_B.y())*100.0,'f',1)+"</td>";
    tableau += "<td></td><td></td><td></td></tr></tbody></table>";

    return tableau;
}

QString vouteNidAbeille::debit()
{
    QString str,str2,resu;
    int nb_chevrons_total;
    int nb_chevrons_entiers_total, nb_chevrons_bord;
    nb_chevrons_entiers_total = vna_nombreChevronGauche+vna_nombreChevronDroite;
    nb_chevrons_bord = 0;
    for(int i=0;i<chevCoupe.size();i++)
    {
        nb_chevrons_bord += chevCoupe[i].nombre;
    }

    if(vna_choixType==1)
    {
        nb_chevrons_total = nb_chevrons_entiers_total+nb_chevrons_bord;
    }
    else
    {
        nb_chevrons_total = nb_chevrons_entiers_total+2*nb_chevrons_bord;
    }
    resu = "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Chevrons</strong></span></p>";
    resu += "<p>Il faut <strong>"+str.setNum((double)nb_chevrons_total,'f',0)+"</strong> planches aux dimensions suivantes :<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- epaisseur : <strong>"+str.setNum(vna_epaisseurChevron*100.0,'f',0)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- retombée : <strong>"+str.setNum(vna_retombeeChevron*100.0,'f',0)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur : <strong>"+str.setNum(chev_longueurChevron,'f',3)+"</strong> m<br />";
    resu += "(on a compté une planche entière pour chaque chevron tronqué, pour se permettre de choisir les meilleures planches)</p>";
    double vna_debitChevrons, vna_debitSabliere, vna_debitFaitiere, vna_debitRive, vna_debitTotal;
    vna_debitChevrons = nb_chevrons_total*chev_longueurChevron*vna_epaisseurChevron*vna_retombeeChevron;
    resu += "<p>Ce qui nous donne un débit pour les chevrons de <strong>"+str.setNum(vna_debitChevrons,'f',2)+"</strong> m3.</p>";
    resu += "<p></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Sablières</strong></span></p>";
    resu += "<p>Il y a <strong>2</strong> sablières avec pour dimensions : <br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- épaisseur : <strong>"+str.setNum(vna_epaisseurSabliere*100.0,'f',0)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- largeur minimum : <strong>"+str.setNum(vna_largeurSabliere*100.0,'f',1)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur : <strong>"+str.setNum(vna_profondeurVoute,'f',2)+"</strong> m</p>";
    vna_debitSabliere = 2.0*vna_epaisseurSabliere*vna_largeurSabliere*vna_profondeurVoute;
    resu += "<p>Ce qui nous donne un débit pour les sablières de <strong>"+str.setNum(vna_debitSabliere,'f',2)+"</strong> m3.</p>";
    resu += "<p></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Faitière</strong></span></p>";
    if(vna_choixType==1)
    {
        vna_debitFaitiere = 0;
        resu += "<p>Il n'y a pas de faitière.</p>";
    }
    else
    {
        resu += "<p>Il y a <strong>une</strong> faitière aux dimensions suivantes : <br />";
        resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- épaisseur : <strong>"+str.setNum(vna_epaisseurFaitiere*100.0,'f',0)+"</strong> cm<br />";
        resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- retombée : <strong>"+str.setNum(vna_retombeeFaitiere*100.0,'f',0)+"</strong> cm<br />";
        resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur : <strong>"+str.setNum(vna_profondeurVoute,'f',2)+"</strong> m</p>";
        vna_debitFaitiere = vna_epaisseurFaitiere*vna_retombeeFaitiere*vna_profondeurVoute;
        resu += "<p>Ce qui nous donne un débit pour la faitière de <strong>"+str.setNum(vna_debitFaitiere,'f',2)+"</strong> m3.</p>";
    }
    resu += "<p></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Rives</strong></span></p>";
    double m_longueurExtradosRive;
    m_longueurExtradosRive = (vna_angleHorizontalCentreCoinFaitiere-vna_angleHorizontalCentreCoinSabliere)*vna_rayonCourbureVoute;
    vna_debitRive = (pi*qPow(vna_rayonCourbureVoute,2.0)-pi*qPow((vna_rayonCourbureVoute-vna_retombeeRive),2.0))*(vna_angleHorizontalCentreCoinFaitiere-vna_angleHorizontalCentreCoinSabliere)/(2*pi)*vna_epaisseurRive;
    if(vna_choixType==1)
    {
        resu += "<p>Il y a <strong>2</strong> rives de dimensions :<br />";
        vna_debitRive *= 2;
    }
    else
    {
        resu += "<p>Il y a <strong>4</strong> rives de dimensions :<br />";
        vna_debitRive *= 4;
    }
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- épaisseur : <strong>"+str.setNum(vna_epaisseurRive*100.0,'f',0)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- retombée minimale : <strong>"+str.setNum(vna_retombeeRive*100.0,'f',0)+"</strong> cm<br />";
    resu += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- longueur courbe extrados de la rive : <strong>"+str.setNum(m_longueurExtradosRive,'f',2)+"</strong> m</p>";
    resu += "<p>Ce qui nous donne un débit pour les rives de <strong>"+str.setNum(vna_debitRive,'f',2)+"</strong> m3.</p>";
    resu += "<p></p>";
    resu += "<p><span style=\"font-size: 12pt; color: #ff9900;\"><strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Débit Total</strong></span></p>";
    vna_debitTotal = vna_debitChevrons+vna_debitSabliere+vna_debitFaitiere+vna_debitRive;
    resu += "<p>Soit un débit total de bois pour toute la charpente de <strong>"+str.setNum(vna_debitTotal,'f',2)+"</strong> m3.</p>";

    return resu;
}

QString vouteNidAbeille::tableauChevronTronque(int num)
{
    //les tableaux de coordonnees
    QString tableau,str,str2;

    tableau = "<table cellpadding=\"2\" border=\"1\" align=\"center\"><tbody><tr>";
    tableau += "<td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td bgcolor=\"#cccc99\"><td bgcolor=\"#cccc99\"></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>X</strong></p></td><td bgcolor=\"#cccc99\"><p align=\"center\"><strong>Y</strong></p></td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>A</strong></td><td>"+str.setNum(chevCoupe[num].A.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].A.y()*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>VC</strong></td><td>"+str.setNum(chevCoupe[num].VC.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].VC.y()*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>B</strong></td><td>"+str.setNum(chevCoupe[num].B.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].B.y()*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>VD</strong></td><td>"+str.setNum(chevCoupe[num].VD.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].VD.y()*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>C</strong></td><td>"+str.setNum(chevCoupe[num].C.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].C.y()*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>WA</strong></td><td>"+str.setNum(chevCoupe[num].WA.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].WA.y()*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>D</strong></td><td>"+str.setNum(chevCoupe[num].D.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].D.y()*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>WB</strong></td><td>"+str.setNum(chevCoupe[num].WB.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].WB.y()*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VA</strong></td><td>"+str.setNum(chevCoupe[num].VA.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].VA.y()*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>WC</strong></td><td>"+str.setNum(chevCoupe[num].WC.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].WC.y()*100.0,'f',1)+"</td></tr>";
    tableau += "<tr><td bgcolor=\"#cccc99\"><strong>VB</strong></td><td>"+str.setNum(chevCoupe[num].VB.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].VB.y()*100.0,'f',1)+"</td>";
    tableau += "<td bgcolor=\"#cccc99\"><strong>WD</strong></td><td>"+str.setNum(chevCoupe[num].WD.x()*100.0,'f',1)+"</td><td>"+str2.setNum(chevCoupe[num].WD.y()*100.0,'f',1)+"</td></tr>";
    tableau += "</tbody></table>";

    return tableau;
}

void vouteNidAbeille::setParametres(int pvna_choixType, double pvna_angleBeta, double pvna_profondeurVoute, double pvna_hauteurVoute, double pvna_portee, double pvna_rayonCourbureVoute, int pvna_rangeesVerticales, double pvna_epaisseurChevron, double pvna_retombeeChevron, double pvna_epaisseurFaitiere, double pvna_retombeeFaitiere, double pvna_epaisseurRive, double pvna_epaisseurSabliere, double pvna_decalageNoeud, double pvna_distanceNoeudFaitiere, double pvna_distanceNoeudSabliere, double pvna_distanceNoeudRiveGauche)
{
    // 0 voute reguliere avec faitiere
    // 1 voute reguliere sans faitiere
    // 2 voute irreguliere avec faitiere
    vna_choixType = pvna_choixType;
    vna_angleBeta = radians(pvna_angleBeta);
    vna_angleAlpha = pi/2-vna_angleBeta;
    vna_profondeurVoute = pvna_profondeurVoute;
    vna_hauteurVoute = pvna_hauteurVoute;
    vna_portee = pvna_portee;
    vna_rayonCourbureVoute = pvna_rayonCourbureVoute;
    vna_rangeesVerticales = pvna_rangeesVerticales;
    vna_epaisseurChevron = pvna_epaisseurChevron/100.0;
    vna_demiEpaisseurChevron = vna_epaisseurChevron/2.0;
    vna_retombeeChevron = pvna_retombeeChevron/100.0;
    vna_epaisseurFaitiere = pvna_epaisseurFaitiere/100.0;
    vna_retombeeFaitiere = pvna_retombeeFaitiere/100.0;
    vna_epaisseurRive = pvna_epaisseurRive/100.0;
    vna_epaisseurSabliere = pvna_epaisseurSabliere/100.0;
    vna_decalageNoeud = pvna_decalageNoeud/100.0;
    vna_distanceNoeudSabliere = pvna_distanceNoeudSabliere/100.0;
    vna_distanceNoeudFaitiere = pvna_distanceNoeudFaitiere/100.0;
    if(vna_choixType ==1)
    {vna_distanceNoeudFaitiere = vna_distanceNoeudSabliere;}
    vna_distanceNoeudRiveGauche = pvna_distanceNoeudRiveGauche/100.0;

    calcul();
}

QStringList vouteNidAbeille::listeChevronsTronques()
{
    QStringList m_liste;
    QString str, str2, ligneAjout;
    for(int i=0;i<chevCoupe.size();i++)
    {
        m_liste << str.setNum(chevCoupe[i].nombre)+" chevrons de type "+(char)(i+65);
    }
    return m_liste;
}

int vouteNidAbeille::nbreChevronsTronques()
{
    return chevCoupe.size();
}

void vouteNidAbeille::valeursChevron(QVector<QVector3D>& passageValeur)
{
    passageValeur << chev_VA << chev_VB << chev_VC << chev_VD << chev_VE << chev_VF << chev_VG << chev_VH;
    passageValeur << chev_centreChevron << chev_centreChevronPrime;
}

void vouteNidAbeille::valeursChevronTronque(QVector<QVector3D> &passageValeur, int numero)
{
    QVector3D tempo;
    //0,1,2,3,4,5,6,7
    passageValeur << chevCoupe[numero].VA3D << chevCoupe[numero].VB3D << chevCoupe[numero].VC3D << chevCoupe[numero].VD3D << chevCoupe[numero].VE3D << chevCoupe[numero].VF3D << chevCoupe[numero].VG3D << chevCoupe[numero].VH3D;
    //8,9,10,11
    passageValeur << chev_centreChevron << chev_centreChevronPrime << O1prime << O1seconde;
    //12
    tempo.setX(chevCoupe[numero].ihor);tempo.setY(chevCoupe[numero].jhor);tempo.setZ(chevCoupe[numero].nombre);
    passageValeur << tempo;
    //13
    tempo.setX(chevCoupe[numero].iver);tempo.setY(chevCoupe[numero].jver);tempo.setZ(vna_epaisseurChevron);
    passageValeur << tempo;
    //14
    tempo.setX(chev_rayonExtrado);tempo.setY(chev_rayonIntrado);tempo.setZ(nombreTronqueaDroite);
    passageValeur << tempo;
    //15,16
    passageValeur << OintradosPrime << OintradosSeconde;
    //17,18 centre chevron en 3d
    passageValeur << centreChevron1prime << centreChevron1seconde;
    //19
    tempo.setX(chevCoupe[numero].orientation);
    passageValeur << tempo;
    //20,21,22,23
    passageValeur << chevCoupe[numero].A << chevCoupe[numero].B << chevCoupe[numero].C << chevCoupe[numero].D;
    //24,25,26,27
    passageValeur << chevCoupe[numero].VA << chevCoupe[numero].VB << chevCoupe[numero].VC << chevCoupe[numero].VD;
    //28,29,30,31
    passageValeur << chevCoupe[numero].WA << chevCoupe[numero].WB << chevCoupe[numero].WC << chevCoupe[numero].WD;


}

void vouteNidAbeille::valeursVoute(QVector<QVector3D>& passageValeur)
{
    QVector3D tempo;
    //0,1,2
    tempo = (V1haut - V1bas);
    passageValeur << vecteurNormalChevron1 << tempo << V1haut;
    //3
    tempo.setX(vna_rangeesVerticales);tempo.setY(vna_rangeesHorizontales);tempo.setZ(vna_diagonaleHorizontaleLosange);
    passageValeur << tempo;
    //a modifier automatiquement par la suite
    //4
    tempo.setX(vna_profondeurVoute);tempo.setY(vna_epaisseurSabliere);tempo.setZ(vna_largeurSabliere);
    passageValeur << tempo;
    //5
    tempo.setX(vna_portee);tempo.setY(vna_epaisseurRive);tempo.setZ(vna_distanceNoeudRiveGauche);
    passageValeur << tempo;
    //6
    tempo.setX(vna_epaisseurFaitiere/2.0);tempo.setY(vna_retombeeFaitiere);tempo.setZ(vna_hauteurVoute);
    passageValeur << tempo;
    //7
    tempo.setX(O1.x());tempo.setY(O1.y());tempo.setZ(vna_profondeurVoute/2.0-vna_epaisseurRive-vna_distanceNoeudRiveGauche);
    passageValeur << tempo;
    //8
    tempo.setX(vna_choixType);tempo.setY(vna_demiAngleDiagonaleVerticaleLosange);
    passageValeur << tempo;
    //9
    tempo.setX(vna_centre.x());tempo.setY(vna_centre.y());tempo.setZ(vna_demiDiagonaleHorizontaleLosange);
    passageValeur << tempo;
    //10
    tempo.setX(vna_rayonCourbureVoute);tempo.setY(vna_angleDiagonaleVerticaleLosange);tempo.setZ(vna_angleHorizontalCentreNoeudDepart);
    passageValeur << tempo;
    //11,12
    passageValeur << chev_VD << chev_VA;
}

void vouteNidAbeille::valeursVoute2(QVector<QVector3D> &passageValeur)
{
    QVector3D tempo;
    tempo.setY(vna_nombreChevronDroiteHorizontal);
    tempo.setX(vna_debutChevronHorizontalDroite);
    passageValeur << tempo;
    //1 valeurs extremes en z des chevrons penches a gauche
    tempo.setY(vna_nombreChevronGaucheHorizontal);
    tempo.setX(vna_debutChevronHorizontalGauche);
    passageValeur << tempo;
    //2 valeurs extremes en angle des chevrons penches a droite
    tempo.setX(vna_debutChevronVerticalDroite);
    tempo.setY(vna_nombreChevronDroiteVertical);
    passageValeur << tempo;
    //3 valeurs extremes en angle des chevrons penches a gauche
    tempo.setX(vna_debutChevronVerticalGauche);
    tempo.setY(vna_nombreChevronGaucheVertical);
    passageValeur << tempo;
    //4 nombre de chevrons tronques differents
    tempo.setX(chevCoupe.size());tempo.setY(nombreTronqueaDroite);
    passageValeur << tempo;
}

void vouteNidAbeille::valeursRive(QVector<QVector3D>& passageValeur)
{
    QVector3D tempo;
    tempo.setX(vna_centre.x());tempo.setY(vna_centre.y());tempo.setZ(vna_rayonCourbureVoute);
    passageValeur << tempo;
    tempo.setX(vna_epaisseurRive);tempo.setY(vna_retombeeRive);tempo.setZ(vna_angleHorizontalCentreCoinSabliere);
    passageValeur << tempo;
    tempo.setX(vna_angleHorizontalCentreCoinFaitiere);tempo.setY(vna_choixType);
    passageValeur << tempo;
}

//calcul du volume de la voute
double vouteNidAbeille::volume()
{
    double result;
    result = vna_hauteurVoute*vna_epaisseurFaitiere + (vna_portee-vna_epaisseurFaitiere)*vna_epaisseurSabliere;
    if((vna_centre.x()<(vna_epaisseurFaitiere/2.0))&&(vna_centre.y()>vna_epaisseurSabliere))
    {
        result += vna_angleCoinSabliereCentreCoinFaitiere*qPow(vna_rayonCourbureVoute,2.0);
        result += (vna_portee/2.0-vna_centre.x())*(vna_centre.y()-vna_epaisseurSabliere);
        result -= 2.0*(vna_epaisseurFaitiere/2.0-vna_centre.x())*(vna_centre.y()-vna_epaisseurSabliere);
        result -= (vna_hauteurVoute-vna_centre.y())*(vna_epaisseurFaitiere/2.0-vna_centre.x());
    }
    else if((vna_centre.x()<(vna_epaisseurFaitiere/2.0))&&(vna_centre.y()<vna_epaisseurSabliere))
    {
        result += vna_angleCoinSabliereCentreCoinFaitiere*qPow(vna_rayonCourbureVoute,2.0);
        result -= (vna_epaisseurFaitiere/2.0-vna_centre.x())*(vna_hauteurVoute-vna_centre.y());
        result += (vna_epaisseurFaitiere/2.0-vna_centre.x())*(vna_epaisseurSabliere-vna_centre.y());
        result -= (vna_portee/2.0-vna_centre.x())*(vna_epaisseurSabliere-vna_centre.y());
        result += (vna_epaisseurFaitiere/2.0-vna_centre.x())*(vna_epaisseurSabliere-vna_centre.y());
    }
    else if((vna_centre.x()>(vna_epaisseurFaitiere/2.0))&&(vna_centre.y()<vna_epaisseurSabliere))
    {
        result += vna_angleCoinSabliereCentreCoinFaitiere*qPow(vna_rayonCourbureVoute,2.0);
        result += (vna_centre.x()-vna_epaisseurFaitiere/2.0)*(vna_hauteurVoute-vna_centre.y());
        result -= 2.0*(vna_centre.x()-vna_epaisseurFaitiere/2.0)*(vna_epaisseurSabliere-vna_centre.y());
        result -= (vna_portee/2.0-vna_centre.x())*(vna_epaisseurSabliere-vna_centre.y());
    }
    else if((vna_centre.x()>(vna_epaisseurFaitiere/2.0))&&(vna_centre.y()>vna_epaisseurSabliere))
    {
        result += vna_angleCoinSabliereCentreCoinFaitiere*qPow(vna_rayonCourbureVoute,2.0);
        result += (vna_centre.x()-vna_epaisseurFaitiere/2.0)*(vna_hauteurVoute-vna_centre.y());
        result += 2.0*(vna_centre.x()-vna_epaisseurFaitiere/2.0)*(vna_centre.y()-vna_epaisseurSabliere);
        result += (vna_portee/2.0-vna_centre.x())*(vna_centre.y()-vna_epaisseurSabliere);
    }
    result = result*vna_profondeurVoute;
    return result;
}

double vouteNidAbeille::surfaceDeToit()
{
    double result;
    result = vna_angleCoinSabliereCentreCoinFaitiere*vna_rayonCourbureVoute*2.0*vna_profondeurVoute;
    return result;
}

int vouteNidAbeille::nbreChevronsGauche()
{
    return vna_nombreChevronGauche;
}

int vouteNidAbeille::nbreChevronsDroite()
{
    return vna_nombreChevronDroite;
}

void vouteNidAbeille::calcul()
{
    QVector2D point1,point2,point3,centre1,centre2,vect;

    point1.setX(-vna_portee/2);
    point1.setY(vna_epaisseurSabliere);
    point2.setX(vna_epaisseurFaitiere/2);
    point2.setY(vna_hauteurVoute);;
    point3.setX(vna_portee/2);
    point3.setY(vna_epaisseurSabliere);

    //si c'est une voute reguliere, on calcule le rayon de courbure et trouve le centre
    if(vna_choixType==0||vna_choixType==1)
    {
        vna_rayonCourbureVoute = rayonCercle3Points2D(point1,point2,point3);
        vna_centre = centreCercle3Points2D(point1,point2,point3);
    }
    //si c est irregulier on cherche le centre de la partie droite de la voute (pour x>0)
    else
    {
        centre1 = centreCercle2Points2DavecRayon1(point2,point3,vna_rayonCourbureVoute);
        centre2 = centreCercle2Points2DavecRayon2(point2,point3,vna_rayonCourbureVoute);
        if(centre1.x()>=centre2.x()){vna_centre = centre2;}else{vna_centre = centre1;}
    }


    //calcul distance droite coin sabliere superieur exterieur / coin superieur faitiere interieur
    if(vna_choixType==0||vna_choixType==2)
    {
        vect = point3-point2;
        vna_distanceDroiteCoinSabliereCoinFaitiere = vect.length();
    }
    else
    {
        vect = point3-point1;
        vna_distanceDroiteCoinSabliereCoinFaitiere = vect.length();
    }

    //calcul angles
    vna_angleHorizontalCentreCoinSabliere = qAtan((point3.y()-vna_centre.y())/(point3.x()-vna_centre.x()));
    if(vna_choixType==1)
    {
        vna_angleHorizontalCentreCoinFaitiere = pi-vna_angleHorizontalCentreCoinSabliere;
    }
    else
    {
        if(point2.x()==vna_centre.x())
        {
            vna_angleHorizontalCentreCoinFaitiere = qAtan((point2.y()-vna_centre.y())/0.0001);
        }
        else if(vna_centre.x()<point2.x())
        {
            vna_angleHorizontalCentreCoinFaitiere = qAtan((point2.y()-vna_centre.y())/(point2.x()-vna_centre.x()));
        }
        else if(vna_centre.x()>point2.x())
        {
            vna_angleHorizontalCentreCoinFaitiere = pi-qAbs(qAtan((point2.y()-vna_centre.y())/(point2.x()-vna_centre.x())));
        }
    }
    vna_angleCoinSabliereCentreCoinFaitiere = vna_angleHorizontalCentreCoinFaitiere-vna_angleHorizontalCentreCoinSabliere;

    //longueurs courbes
    vna_longueurCourbeCoinFaitiereCoinSabliere = vna_angleCoinSabliereCentreCoinFaitiere*vna_rayonCourbureVoute;
    vna_longueurCourbeNoeudDepartNoeudArrivee = vna_longueurCourbeCoinFaitiereCoinSabliere-vna_distanceNoeudFaitiere-vna_distanceNoeudSabliere;

    //diagonale verticale losange
    vna_diagonaleVerticaleLosange = vna_longueurCourbeNoeudDepartNoeudArrivee/((vna_rangeesVerticales+1)/2.0);
    vna_angleDiagonaleVerticaleLosange = vna_diagonaleVerticaleLosange/vna_rayonCourbureVoute;
    vna_demiAngleDiagonaleVerticaleLosange = vna_angleDiagonaleVerticaleLosange/2.0;

    //calculs des decalages des noeuds
    vna_decalageNoeudHorizontal = vna_decalageNoeud*qCos(vna_angleAlpha);
    vna_demiDecalageNoeudHorizontal = vna_decalageNoeudHorizontal/2.0;
    vna_angleDecalageNoeudVertical = vna_decalageNoeud*qSin(vna_angleAlpha)/vna_rayonCourbureVoute;
    vna_demiAngleDecalageNoeudVertical = vna_angleDecalageNoeudVertical/2.0;

    //diagonale horizontale losange
    vna_longueurChevronFilaire = (vna_diagonaleVerticaleLosange+vna_decalageNoeud*qSin(vna_angleAlpha))/qSin(vna_angleAlpha);
    vna_diagonaleHorizontaleLosange = (vna_longueurChevronFilaire+vna_decalageNoeud)*qCos(vna_angleAlpha);
    vna_demiDiagonaleHorizontaleLosange = vna_diagonaleHorizontaleLosange/2.0;

    //encore des calculs d'angles
    vna_angleCoinSabliereCentreNoeudDepart = vna_distanceNoeudSabliere/vna_rayonCourbureVoute;
    vna_angleNoeudArriveeCentreCoinFaitiere = vna_distanceNoeudFaitiere/vna_rayonCourbureVoute;
    vna_angleNoeudDepartCentreNoeudArrivee = vna_longueurCourbeNoeudDepartNoeudArrivee/vna_rayonCourbureVoute;
    vna_angleHorizontalCentreNoeudDepart = vna_angleHorizontalCentreCoinSabliere+vna_angleCoinSabliereCentreNoeudDepart;
    vna_angleHorizontalCentreNoeudArrivee = vna_angleHorizontalCentreCoinFaitiere+vna_angleNoeudArriveeCentreCoinFaitiere;

    //calcul nombre de rangees horizontales
    vna_rangeesHorizontales = trunc((vna_profondeurVoute-2*vna_epaisseurRive-vna_distanceNoeudRiveGauche)/vna_diagonaleHorizontaleLosange);

    //calculs des points 3D
    O1.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart));
    O1.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart));
    O1.setZ(0.0);
    U1ass.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDecalageNoeudVertical));
    U1ass.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDecalageNoeudVertical));
    U1ass.setZ(vna_demiDecalageNoeudHorizontal);
    T1ass.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDecalageNoeudVertical));
    T1ass.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDecalageNoeudVertical));
    T1ass.setZ(-vna_demiDecalageNoeudHorizontal);
    T1.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDecalageNoeudVertical));
    T1.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDecalageNoeudVertical));
    T1.setZ(vna_diagonaleHorizontaleLosange-vna_demiDecalageNoeudHorizontal);
    U1.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDecalageNoeudVertical));
    U1.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDecalageNoeudVertical));
    U1.setZ(-vna_diagonaleHorizontaleLosange+vna_demiDecalageNoeudHorizontal);
    O2ass.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange));
    O2ass.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange));
    O2ass.setZ(vna_demiDiagonaleHorizontaleLosange);
    T2.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+vna_demiAngleDecalageNoeudVertical));
    T2.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+vna_demiAngleDecalageNoeudVertical));
    T2.setZ(-vna_demiDiagonaleHorizontaleLosange+vna_demiDecalageNoeudHorizontal);
    O2.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange));
    O2.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange));
    O2.setZ(-vna_demiDiagonaleHorizontaleLosange);
    T3ass.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart+vna_angleDiagonaleVerticaleLosange+vna_demiAngleDecalageNoeudVertical));
    T3ass.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart+vna_angleDiagonaleVerticaleLosange+vna_demiAngleDecalageNoeudVertical));
    T3ass.setZ(vna_diagonaleHorizontaleLosange-vna_demiDecalageNoeudHorizontal);
    T3.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart+vna_angleDiagonaleVerticaleLosange+vna_demiAngleDecalageNoeudVertical));
    T3.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart+vna_angleDiagonaleVerticaleLosange+vna_demiAngleDecalageNoeudVertical));
    T3.setZ(-vna_demiDecalageNoeudHorizontal);
    O0.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDiagonaleVerticaleLosange));
    O0.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDiagonaleVerticaleLosange));
    O0.setZ(vna_demiDiagonaleHorizontaleLosange);
    O0ass.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDiagonaleVerticaleLosange));
    O0ass.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDiagonaleVerticaleLosange));
    O0ass.setZ(-vna_demiDiagonaleHorizontaleLosange);
    U0.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDiagonaleVerticaleLosange-vna_demiAngleDecalageNoeudVertical));
    U0.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart-vna_demiAngleDiagonaleVerticaleLosange-vna_demiAngleDecalageNoeudVertical));
    U0.setZ(vna_demiDiagonaleHorizontaleLosange-vna_demiDecalageNoeudHorizontal);
    Umoins1.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart-vna_angleDiagonaleVerticaleLosange-vna_demiAngleDecalageNoeudVertical));
    Umoins1.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart-vna_angleDiagonaleVerticaleLosange-vna_demiAngleDecalageNoeudVertical));
    Umoins1.setZ(vna_demiDecalageNoeudHorizontal);
    Umoins1ass.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreNoeudDepart-vna_angleDiagonaleVerticaleLosange-vna_demiAngleDecalageNoeudVertical));
    Umoins1ass.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreNoeudDepart-vna_angleDiagonaleVerticaleLosange-vna_demiAngleDecalageNoeudVertical));
    Umoins1ass.setZ(-vna_diagonaleHorizontaleLosange+vna_demiDecalageNoeudHorizontal);
    centreTracageChevron1 = vna_centre;
    centreTracageChevron1.setZ(0.0);
    centreTracageChevron0et2ass = vna_centre;
    centreTracageChevron0et2ass.setZ(vna_demiDiagonaleHorizontaleLosange);
    centreTracageChevron0asset2 = vna_centre;
    centreTracageChevron0asset2.setZ(-vna_demiDiagonaleHorizontaleLosange);

    //calcul pour chaque chevron de leur centre, leur rayon courbure extrados et leur vecteur normal

    centreChevron1 = centreCercle3Points3D(U0,O1,T2);
    centreChevron0 = centreCercle3Points3D(Umoins1,O0,T1);
    centreChevron2 = centreCercle3Points3D(U1,O2,T3);
    centreChevron0ass = centreCercle3Points3D(Umoins1ass,O0ass,T1ass);
    centreChevron2ass = centreCercle3Points3D(U1ass,O2ass,T3ass);
    rayonChevron1 = rayonCercle3Points3D(U0,O1,T2);
    rayonChevron0 = rayonCercle3Points3D(Umoins1,O0,T1);
    rayonChevron2 = rayonCercle3Points3D(U1,O2,T3);
    rayonChevron0ass = rayonCercle3Points3D(Umoins1ass,O0ass,T1ass);
    rayonChevron2ass = rayonCercle3Points3D(U1ass,O2ass,T3ass);

    // alors la avant j'avais mis les centreTracageChevrons donc a voir mais a mon avis ils servent a rien.
    vecteurNormalChevron1 = vecteurNormalNorme3Points3D(O1,centreChevron1,T2);
    vecteurNormalChevron0 = vecteurNormalNorme3Points3D(O0,centreChevron0,T1);
    vecteurNormalChevron2 = vecteurNormalNorme3Points3D(O2,centreChevron2,U1);
    vecteurNormalChevron0ass = vecteurNormalNorme3Points3D(O0ass,centreChevron0ass,Umoins1ass);
    vecteurNormalChevron2ass = vecteurNormalNorme3Points3D(O2ass,centreChevron2ass,T3ass);

    //la on decale les centres avec le vecteur normal
    QVector3D O0assprime, O0assseconde, centreChevron0assprime, centreChevron0assseconde, O2assprime, O2assseconde, centreChevron2assprime, centreChevron2assseconde;
    O1prime = O1+vna_demiEpaisseurChevron*vecteurNormalChevron1;
    O1seconde = O1-vna_demiEpaisseurChevron*vecteurNormalChevron1;
    centreChevron1prime = centreChevron1+vna_demiEpaisseurChevron*vecteurNormalChevron1;
    centreChevron1seconde = centreChevron1-vna_demiEpaisseurChevron*vecteurNormalChevron1;
    O2prime = O2+vna_demiEpaisseurChevron*vecteurNormalChevron2;
    O0prime = O0+vna_demiEpaisseurChevron*vecteurNormalChevron0;
    centreChevron2prime = centreChevron2+vna_demiEpaisseurChevron*vecteurNormalChevron2;
    centreChevron0prime = centreChevron0+vna_demiEpaisseurChevron*vecteurNormalChevron0;
    O0assprime = O0ass+vna_demiEpaisseurChevron*vecteurNormalChevron0ass;
    O0assseconde = O0ass-vna_demiEpaisseurChevron*vecteurNormalChevron0ass;
    centreChevron0assprime = centreChevron0ass+vna_demiEpaisseurChevron*vecteurNormalChevron0ass;
    centreChevron0assseconde = centreChevron0ass-vna_demiEpaisseurChevron*vecteurNormalChevron0ass;
    O2assprime = O2ass+vna_demiEpaisseurChevron*vecteurNormalChevron2ass;
    O2assseconde = O2ass-vna_demiEpaisseurChevron*vecteurNormalChevron2ass;
    centreChevron2assprime = centreChevron2ass+vna_demiEpaisseurChevron*vecteurNormalChevron2ass;
    centreChevron2assseconde = centreChevron2ass-vna_demiEpaisseurChevron*vecteurNormalChevron2ass;

    //la on cherche les coordonnes des points d'intersections entre les chevrons
    V1haut = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,O2prime,vecteurNormalChevron2,centreChevron1prime,rayonChevron1);
    W1haut = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,O2prime,vecteurNormalChevron2,centreChevron1seconde,rayonChevron1);
    V2 = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,O2prime,vecteurNormalChevron2,centreChevron2prime,rayonChevron2);
    W2 = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,O2prime,vecteurNormalChevron2,centreChevron2prime,rayonChevron2);
    V1bas = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,O0prime,vecteurNormalChevron0,centreChevron1prime,rayonChevron1);
    W1bas = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,O0prime,vecteurNormalChevron0,centreChevron1seconde,rayonChevron1);
    V0 = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,O0prime,vecteurNormalChevron0,centreChevron0prime,rayonChevron0);
    W0 = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,O0prime,vecteurNormalChevron0,centreChevron0prime,rayonChevron0);
    V1ass2 = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,O2assprime,vecteurNormalChevron2ass,centreChevron1seconde,rayonChevron1);
    W1ass2 = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,O2assseconde,vecteurNormalChevron2ass,centreChevron1seconde,rayonChevron1);
    V2ass = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,O2assprime,vecteurNormalChevron2ass,centreChevron2assprime,rayonChevron2ass);
    W2ass = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,O2assseconde,vecteurNormalChevron2ass,centreChevron2assseconde,rayonChevron2ass);
    V1ass0 = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,O0assprime,vecteurNormalChevron0ass,centreChevron1prime,rayonChevron1);
    W1ass0 = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,O0assseconde,vecteurNormalChevron0ass,centreChevron1prime,rayonChevron1);
    V0ass = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,O0assprime,vecteurNormalChevron0ass,centreChevron0assprime,rayonChevron0ass);
    W0ass = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,O0assseconde,vecteurNormalChevron0ass,centreChevron0assseconde,rayonChevron0ass);

    //vecteur intersections entre chevrons
    QVector3D vecteurIntersection1et2, vecteurIntersection1et0, vecteurIntersection1et2ass, vecteurIntersection1et0ass;
    vecteurIntersection1et2 = QVector3D::crossProduct(vecteurNormalChevron1,vecteurNormalChevron2);
    vecteurIntersection1et0 = QVector3D::crossProduct(vecteurNormalChevron0,vecteurNormalChevron1);
    vecteurIntersection1et2ass = QVector3D::crossProduct(vecteurNormalChevron2ass,vecteurNormalChevron1);
    vecteurIntersection1et0ass = QVector3D::crossProduct(vecteurNormalChevron1,vecteurNormalChevron0ass);

    //angles sur le chevron1
    double angleEntreV1hautC1primeEtVecteurIntersection1et2, angleEntreV1hautC1primeEtO1primeC1prime, angleEntreO1primeC1primeEtVecteurIntersection1et2;
    double angleEntreV1basC1primeEtVecteurIntersection1et0, angleEntreV1basC1primeEtO1primeC1prime, angleEntreO1primeC1primeEtVecteurIntersection1et0;
    double angleEntreV1ass2C1secondeEtVecteurIntersection1et2ass, angleEntreV1ass2C1secondeEtO1secondeC1seconde, angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass;
    double angleEntreV1ass0C1primeEtVecteurIntersection1et0ass, angleEntreV1ass0C1primeEtO1primeC1prime, angleEntreO1primeC1primeEtVecteurIntersection1et0ass;

    angleEntreV1hautC1primeEtVecteurIntersection1et2 = angleEntre2Vecteurs3D((centreChevron1prime-V1haut),vecteurIntersection1et2);
    angleEntreV1hautC1primeEtO1primeC1prime = angleEntre2Vecteurs3D((centreChevron1prime-V1haut),(centreChevron1prime-O1prime));
    angleEntreO1primeC1primeEtVecteurIntersection1et2 = angleEntre2Vecteurs3D(vecteurIntersection1et2,(centreChevron1prime-O1prime));

    angleEntreV1basC1primeEtVecteurIntersection1et0 = angleEntre2Vecteurs3D((centreChevron1prime-V1bas),vecteurIntersection1et0);
    angleEntreV1basC1primeEtO1primeC1prime = angleEntre2Vecteurs3D((centreChevron1prime-V1bas),(centreChevron1prime-O1prime));
    angleEntreO1primeC1primeEtVecteurIntersection1et0 = angleEntre2Vecteurs3D((centreChevron1prime-O1prime),vecteurIntersection1et0);

    angleEntreV1ass2C1secondeEtVecteurIntersection1et2ass = angleEntre2Vecteurs3D((centreChevron1seconde-V1ass2),vecteurIntersection1et2ass);
    angleEntreV1ass2C1secondeEtO1secondeC1seconde = angleEntre2Vecteurs3D((centreChevron1seconde-V1ass2),(centreChevron1seconde-O1seconde));
    angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass = angleEntre2Vecteurs3D((centreChevron1seconde-O1seconde),vecteurIntersection1et2ass);

    angleEntreV1ass0C1primeEtVecteurIntersection1et0ass = angleEntre2Vecteurs3D((centreChevron1prime-V1ass0),vecteurIntersection1et0ass);
    angleEntreV1ass0C1primeEtO1primeC1prime = angleEntre2Vecteurs3D((centreChevron1prime-V1ass0),(centreChevron1prime-O1prime));
    angleEntreO1primeC1primeEtVecteurIntersection1et0ass = angleEntre2Vecteurs3D((centreChevron1prime-O1prime),vecteurIntersection1et0ass);

    //jusque la tout va bien
    //calculs chevron 2D
    QVector3D var_vecteur, var_vecteur2;
    double var_angle, var_longueur1;

    chev_centreChevron.setX(0);chev_centreChevron.setY(-rayonChevron1);chev_centreChevron.setZ(vna_demiEpaisseurChevron);
    chev_centreChevronPrime = chev_centreChevron;chev_centreChevronPrime.setZ(-vna_demiEpaisseurChevron);
    chev_O1.setX(0);chev_O1.setY(0);chev_O1.setZ(vna_demiEpaisseurChevron);
    chev_O1prime = chev_O1; chev_O1prime.setZ(-vna_demiEpaisseurChevron);
    chev_O3 = chev_O1; chev_O3.setY(-vna_retombeeChevron);
    chev_O3prime = chev_O3; chev_O3prime.setZ(-vna_demiEpaisseurChevron);


    var_vecteur = O1prime-V1bas; var_longueur1 = var_vecteur.length(); var_angle = qAsin(var_longueur1/(2.0*rayonChevron1));
    chev_VA.setX(-rayonChevron1*qSin(2*var_angle));chev_VA.setY(-(rayonChevron1-rayonChevron1*qCos(2*var_angle)));chev_VA.setZ(vna_demiEpaisseurChevron);
    var_vecteur = O1prime-V1haut; var_longueur1 = var_vecteur.length(); var_angle = qAsin(var_longueur1/(2.0*rayonChevron1));
    chev_VD.setX(rayonChevron1*qSin(2*var_angle));chev_VD.setY(-(rayonChevron1-rayonChevron1*qCos(2*var_angle)));chev_VD.setZ(vna_demiEpaisseurChevron);
    var_vecteur = O1seconde-W1bas; var_longueur1 = var_vecteur.length(); var_angle = qAsin(var_longueur1/(2.0*rayonChevron1));
    chev_VE.setX(-rayonChevron1*qSin(2*var_angle));chev_VE.setY(-(rayonChevron1-rayonChevron1*qCos(2*var_angle)));chev_VE.setZ(-vna_demiEpaisseurChevron);
    var_vecteur = O1seconde-W1haut; var_longueur1 = var_vecteur.length(); var_angle = qAsin(var_longueur1/(2.0*rayonChevron1));
    chev_VH.setX(rayonChevron1*qSin(2*var_angle));chev_VH.setY(-(rayonChevron1-rayonChevron1*qCos(2*var_angle)));chev_VH.setZ(-vna_demiEpaisseurChevron);

    chev_longueurChevron = qMax(chev_VD.x(),chev_VH.x())+qMax(-chev_VA.x(),-chev_VE.x());
    chev_A.setX(-qMax(-chev_VA.x(),-chev_VE.x()));chev_A.setY(0);chev_A.setZ(vna_demiEpaisseurChevron);
    chev_B = chev_A; chev_B.setY(-vna_retombeeChevron);
    chev_E = chev_A; chev_E.setZ(-vna_demiEpaisseurChevron);
    chev_F = chev_B; chev_F.setZ(-vna_demiEpaisseurChevron);
    chev_D.setX(qMax(chev_VD.x(),chev_VH.x()));chev_D.setY(0);chev_D.setZ(vna_demiEpaisseurChevron);
    chev_C = chev_D; chev_C.setY(-vna_retombeeChevron);
    chev_H = chev_D; chev_H.setZ(-vna_demiEpaisseurChevron);
    chev_G = chev_C; chev_G.setZ(-vna_demiEpaisseurChevron);

    var_vecteur = V1ass0-O1prime; var_longueur1 = var_vecteur.length(); var_angle = qAcos((2*qPow(rayonChevron1,2.0)-qPow(var_longueur1,2.0))/(2*qPow(rayonChevron1,2.0)));
    chev_s1.setX(rayonChevron1*qSin(var_angle));chev_s1.setY(-(rayonChevron1-rayonChevron1*qCos(var_angle)));chev_s1.setZ(vna_demiEpaisseurChevron);
    var_vecteur = W1ass0-O1prime; var_longueur1 = var_vecteur.length(); var_angle = qAcos((2*qPow(rayonChevron1,2.0)-qPow(var_longueur1,2.0))/(2*qPow(rayonChevron1,2.0)));
    chev_q1.setX(rayonChevron1*qSin(var_angle));chev_q1.setY(-(rayonChevron1-rayonChevron1*qCos(var_angle)));chev_q1.setZ(vna_demiEpaisseurChevron);
    var_vecteur = V1ass2-O1seconde; var_longueur1 = var_vecteur.length(); var_angle = qAcos((2*qPow(rayonChevron1,2.0)-qPow(var_longueur1,2.0))/(2*qPow(rayonChevron1,2.0)));
    chev_m1.setX(-rayonChevron1*qSin(var_angle));chev_m1.setY(-(rayonChevron1-rayonChevron1*qCos(var_angle)));chev_m1.setZ(vna_demiEpaisseurChevron);
    var_vecteur = W1ass2-O1seconde; var_longueur1 = var_vecteur.length(); var_angle = qAcos((2*qPow(rayonChevron1,2.0)-qPow(var_longueur1,2.0))/(2*qPow(rayonChevron1,2.0)));
    chev_n1.setX(-rayonChevron1*qSin(var_angle));chev_n1.setY(-(rayonChevron1-rayonChevron1*qCos(var_angle)));chev_n1.setZ(vna_demiEpaisseurChevron);
    //a gauche
    if(angleEntreV1basC1primeEtVecteurIntersection1et0==angleEntreV1basC1primeEtO1primeC1prime+angleEntreO1primeC1primeEtVecteurIntersection1et0)
    {
        chev_VB.setX(chev_VA.x()-qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(vna_retombeeChevron+chev_VA.y()));
        chev_VF.setX(chev_VE.x()-qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(vna_retombeeChevron+chev_VE.y()));
    }
    else
    {
        chev_VB.setX(chev_VA.x()+qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(vna_retombeeChevron+chev_VA.y()));
        chev_VF.setX(chev_VE.x()+qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(vna_retombeeChevron+chev_VE.y()));
    }
    chev_VB.setY(-vna_retombeeChevron);chev_VB.setZ(vna_demiEpaisseurChevron);
    chev_VF.setY(-vna_retombeeChevron);chev_VF.setZ(-vna_demiEpaisseurChevron);
    var_vecteur = chev_VF-chev_VE; var_vecteur2 = chev_VB-chev_VA;
    if(var_vecteur.length()>var_vecteur2.length())
    {
        chev_VF.setY(chev_VE.y()-qCos(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(var_vecteur2.length()));
        if(angleEntreV1basC1primeEtVecteurIntersection1et0==angleEntreV1basC1primeEtO1primeC1prime+angleEntreO1primeC1primeEtVecteurIntersection1et0)
        {
            chev_VF.setX(chev_VE.x()-qSin(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(var_vecteur2.length()));
        }
        else
        {
            chev_VF.setX(chev_VE.x()+qSin(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(var_vecteur2.length()));
        }
    }
    else
    {
        chev_VB.setY(chev_VA.y()-qCos(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(var_vecteur.length()));
        if(angleEntreV1basC1primeEtVecteurIntersection1et0==angleEntreV1basC1primeEtO1primeC1prime+angleEntreO1primeC1primeEtVecteurIntersection1et0)
        {
            chev_VB.setX(chev_VA.x()-qSin(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(var_vecteur.length()));
        }
        else
        {
            chev_VB.setX(chev_VA.x()+qSin(angleEntreO1primeC1primeEtVecteurIntersection1et0)*(var_vecteur.length()));
        }
    }
    if(-qMax(-chev_VB.x(),-chev_VF.x())<chev_A.x())
    {
        chev_longueurChevron = chev_longueurChevron+qMax(-chev_VB.x(),-chev_VF.x())+chev_A.x();
        chev_A.setX(-qMax(-chev_VB.x(),-chev_VF.x()));
        chev_B.setX(-qMax(-chev_VB.x(),-chev_VF.x()));
        chev_E.setX(-qMax(-chev_VB.x(),-chev_VF.x()));
        chev_F.setX(-qMax(-chev_VB.x(),-chev_VF.x()));
    }
    //a droite
    if(angleEntreV1hautC1primeEtVecteurIntersection1et2==angleEntreV1hautC1primeEtO1primeC1prime+angleEntreO1primeC1primeEtVecteurIntersection1et2)
    {
        chev_VC.setX(chev_VD.x()+qTan(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(vna_retombeeChevron+chev_VD.y()));
        chev_VG.setX(chev_VH.x()+qTan(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(vna_retombeeChevron+chev_VH.y()));
    }
    else
    {
        chev_VC.setX(chev_VD.x()-qTan(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(vna_retombeeChevron+chev_VD.y()));
        chev_VG.setX(chev_VH.x()-qTan(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(vna_retombeeChevron+chev_VH.y()));
    }
    chev_VC.setY(-vna_retombeeChevron);chev_VG.setZ(vna_demiEpaisseurChevron);
    chev_VG.setY(-vna_retombeeChevron);chev_VG.setZ(-vna_demiEpaisseurChevron);
    var_vecteur = chev_VG-chev_VH; var_vecteur2 = chev_VC-chev_VD;
    if(var_vecteur.length()>var_vecteur2.length())
    {
        chev_VG.setY(chev_VH.y()-qCos(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(var_vecteur2.length()));
        if(angleEntreV1hautC1primeEtVecteurIntersection1et2==angleEntreV1hautC1primeEtO1primeC1prime+angleEntreO1primeC1primeEtVecteurIntersection1et2)
        {
            chev_VG.setX(chev_VH.x()+qSin(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(var_vecteur2.length()));
        }
        else
        {
            chev_VG.setX(chev_VH.x()-qSin(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(var_vecteur2.length()));
        }
    }
    else
    {
        chev_VC.setY(chev_VD.y()-qCos(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(var_vecteur.length()));
        if(angleEntreV1hautC1primeEtVecteurIntersection1et2==angleEntreV1hautC1primeEtO1primeC1prime+angleEntreO1primeC1primeEtVecteurIntersection1et2)
        {
            chev_VC.setX(chev_VD.x()+qSin(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(var_vecteur.length()));
        }
        else
        {
            chev_VC.setX(chev_VD.x()-qSin(angleEntreO1primeC1primeEtVecteurIntersection1et2)*(var_vecteur.length()));
        }
    }
    if(qMax(chev_VC.x(),chev_VG.x())>-chev_A.x())
    {
        chev_longueurChevron = chev_longueurChevron+qMax(chev_VC.x(),chev_VG.x())-chev_D.x();
        chev_D.setX(qMax(chev_VC.x(),chev_VG.x()));
        chev_C.setX(qMax(chev_VC.x(),chev_VG.x()));
        chev_G.setX(qMax(chev_VC.x(),chev_VG.x()));
        chev_H.setX(qMax(chev_VC.x(),chev_VG.x()));
    }


    var_vecteur = chev_centreChevron-chev_VB;
    chev_rayonIntrado = var_vecteur.length();
    chev_rayonExtrado = rayonChevron1;
    chev_O2.setX(0); chev_O2.setY(-(chev_rayonExtrado-chev_rayonIntrado)); chev_O2.setZ(vna_demiEpaisseurChevron);
    chev_O2prime = chev_O2; chev_O2prime.setZ(-vna_demiEpaisseurChevron);

    if(angleEntreV1ass2C1secondeEtVecteurIntersection1et2ass==angleEntreV1ass2C1secondeEtO1secondeC1seconde+angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass)
    {
        chev_m0.setX(chev_m1.x()+qTan(angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass)*(-chev_m1.y()));
        chev_n0.setX(chev_n1.x()+qTan(angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass)*(-chev_n1.y()));
        chev_m3.setX(chev_m1.x()-qTan(angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass)*(vna_retombeeChevron+chev_m1.y()));
        chev_n3.setX(chev_n1.x()-qTan(angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass)*(vna_retombeeChevron+chev_n1.y()));
    }
    else
    {
        chev_m0.setX(chev_m1.x()-qTan(angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass)*(-chev_m1.y()));
        chev_n0.setX(chev_n1.x()-qTan(angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass)*(-chev_n1.y()));
        chev_m3.setX(chev_m1.x()+qTan(angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass)*(vna_retombeeChevron+chev_m1.y()));
        chev_n3.setX(chev_n1.x()+qTan(angleEntreO1secondeC1secondeEtVecteurIntersection1et2ass)*(vna_retombeeChevron+chev_n1.y()));
    }
    chev_m0.setY(0);chev_m0.setZ(-vna_demiEpaisseurChevron);
    chev_n0.setY(0);chev_n0.setZ(-vna_demiEpaisseurChevron);
    chev_m3.setY(-vna_retombeeChevron);chev_m3.setZ(-vna_demiEpaisseurChevron);
    chev_n3.setY(-vna_retombeeChevron);chev_n3.setZ(-vna_demiEpaisseurChevron);

    if(angleEntreV1ass0C1primeEtVecteurIntersection1et0ass==angleEntreV1ass0C1primeEtO1primeC1prime+angleEntreO1primeC1primeEtVecteurIntersection1et0ass)
    {
        chev_s0.setX(chev_s1.x()-qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0ass)*(-chev_s1.y()));
        chev_q0.setX(chev_q1.x()-qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0ass)*(-chev_q1.y()));
        chev_s3.setX(chev_s1.x()+qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0ass)*(vna_retombeeChevron+chev_s1.y()));
        chev_q3.setX(chev_q1.x()+qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0ass)*(vna_retombeeChevron+chev_q1.y()));
    }
    else
    {
        chev_s0.setX(chev_s1.x()+qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0ass)*(-chev_s1.y()));
        chev_q0.setX(chev_q1.x()+qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0ass)*(-chev_q1.y()));
        chev_s3.setX(chev_s1.x()-qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0ass)*(vna_retombeeChevron+chev_s1.y()));
        chev_q3.setX(chev_q1.x()-qTan(angleEntreO1primeC1primeEtVecteurIntersection1et0ass)*(vna_retombeeChevron+chev_q1.y()));
    }
    chev_s0.setY(0);chev_s0.setZ(vna_demiEpaisseurChevron);
    chev_q0.setY(0);chev_q0.setZ(vna_demiEpaisseurChevron);
    chev_s3.setY(-vna_retombeeChevron);chev_s3.setZ(vna_demiEpaisseurChevron);
    chev_q3.setY(-vna_retombeeChevron);chev_q3.setZ(vna_demiEpaisseurChevron);

    chev_s2 = intersectionDroite1Cercle2D(chev_s0,chev_s3,chev_centreChevron,chev_rayonIntrado);
    chev_q2 = intersectionDroite1Cercle2D(chev_q0,chev_q3,chev_centreChevron,chev_rayonIntrado);
    chev_n2 = intersectionDroite1Cercle2D(chev_n0,chev_n3,chev_centreChevronPrime,chev_rayonIntrado);
    chev_m2 = intersectionDroite1Cercle2D(chev_m0,chev_m3,chev_centreChevronPrime,chev_rayonIntrado);

    chev_VA3D = V1bas; chev_VE3D = W1bas; chev_VD3D = V1haut; chev_VH3D = W1haut;
    chev_VB3D = chev_VA3D + vecteurIntersection1et0.normalized()*(chev_VB-chev_VA).length();
    chev_VF3D = chev_VE3D + vecteurIntersection1et0.normalized()*(chev_VF-chev_VE).length();
    chev_VC3D = chev_VD3D + vecteurIntersection1et2.normalized()*(chev_VC-chev_VD).length();
    chev_VG3D = chev_VH3D + vecteurIntersection1et2.normalized()*(chev_VG-chev_VH).length();
    OintradosPrime = O1prime + (centreChevron1prime-O1prime).normalized()*(chev_rayonExtrado-chev_rayonIntrado);
    OintradosSeconde = O1seconde + (centreChevron1seconde-O1seconde).normalized()*(chev_rayonExtrado-chev_rayonIntrado);

    QVector3D vecTemp,vecTemp2;
    vecTemp.setX(1.0);vecTemp.setY(0);vecTemp.setZ(0);
    chev_angleTracage = angleEntre2Vecteurs3D(vecTemp,(chev_VB-chev_VA));
    vecTemp = QVector3D::crossProduct((chev_VE-chev_VA),(chev_VB-chev_VA));
    vecTemp2.setX(0);vecTemp2.setY(0);vecTemp2.setZ(-1.0);
    chev_angleCoupe = pi/2.0-angleEntre2Vecteurs3D(vecTemp2,vecTemp);

    //a partir de la on va calculer les chevrons tronques :
    double distance0, angle0;
    double distance1, angle1;
    double demiEpaisseurSelonY;
    demiEpaisseurSelonY = vna_epaisseurChevron/(2.0*qCos(vna_angleAlpha));
    QVector3D vect0,vect1,vect2,vect3,vect4;
    int ii,jj;
    vect0.setX(O1.x()-vna_centre.x());vect0.setY(O1.y()-vna_centre.y());vect0.setZ(0);
    vect1.setX(V1bas.x()-vna_centre.x());vect1.setY(V1bas.y()-vna_centre.y());vect1.setZ(0);
    vect2.setX(W1bas.x()-vna_centre.x());vect2.setY(W1bas.y()-vna_centre.y());vect2.setZ(0);
    vect3.setX(chev_VB3D.x()-vna_centre.x());vect3.setY(chev_VB3D.y()-vna_centre.y());vect3.setZ(0);
    vect4.setX(chev_VF3D.x()-vna_centre.x());vect4.setY(chev_VF3D.y()-vna_centre.y());vect4.setZ(0);
    angle0 = qMax(angleEntre2Vecteurs3D(vect3,vect0),qMax(angleEntre2Vecteurs3D(vect4,vect0),qMax(angleEntre2Vecteurs3D(vect1,vect0),angleEntre2Vecteurs3D(vect2,vect0))));
    angle1 = qMin(angleEntre2Vecteurs3D(vect3,vect0),qMin(angleEntre2Vecteurs3D(vect4,vect0),qMin(angleEntre2Vecteurs3D(vect1,vect0),angleEntre2Vecteurs3D(vect2,vect0))));
    distance0 = qMax(-chev_VG3D.z(),qMax(-chev_VC3D.z(),qMax(-V1haut.z(),-W1haut.z())))-O1.z();
    distance1 = qMin(-chev_VG3D.z(),qMin(-chev_VC3D.z(),qMin(-V1haut.z(),-W1haut.z())))-O1.z();

    ccc.clear();
    chevCoupe.clear();
    ccc.resize(16);
    for(int i = 0;i<16;i++)
    {
        ccc[i].nombre = 0;
    }

    QVector3D vecteurIntersection,vecteurPlanCoupe;
    QVector3D pointPlanCoupe;
    // c'est parti pour les chevrons penches a droite
    jj = -3;
    while((vna_angleHorizontalCentreNoeudDepart-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)
    {
        ii = -3;
        while((vna_distanceNoeudRiveGauche-distance0+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))
            {
                //0 juste coupe sur rive gauche ok
                if(((vna_distanceNoeudRiveGauche-distance0+ii*vna_diagonaleHorizontaleLosange)<0)&&((vna_distanceNoeudRiveGauche+distance1+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_angleHorizontalCentreNoeudDepart-angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere))
                {
                    int mm,nn;
                    mm = 0;
                    nn = 0;
                    for(int n=0;n<ccc[nn].nombre;n++)
                    {
                        //le chevron existe deja, on lui rajoute un
                        if(ii == ccc[nn].position[n])
                        {
                            mm++;
                            chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                            chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                        }
                    }
                    //pas de chevron pareil existant, on en rajoute un
                    if(mm == 0)
                    {
                        ccc[nn].nombre++;
                        chevCoupe.resize(chevCoupe.size()+1);
                        ccc[nn].pointeurs.append(chevCoupe.size()-1);
                        ccc[nn].position.append(ii);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 0;
                        vecteurPlanCoupe.setX(0.00001);vecteurPlanCoupe.setY(0.00001);vecteurPlanCoupe.setZ(-1.0);
                        vecteurIntersection = QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                        pointPlanCoupe.setX(0);pointPlanCoupe.setY(0);pointPlanCoupe.setZ(vna_distanceNoeudRiveGauche+ii*vna_diagonaleHorizontaleLosange);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = chev_VD3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = chev_VC3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = chev_VH3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = chev_VG3D;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                        QVector3D centreVoute;
                        centreVoute.setX(vna_centre.x());centreVoute.setY(vna_centre.y());centreVoute.setZ(chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D.z());
                        vna_retombeeRive = vna_rayonCourbureVoute-qMin((centreVoute-chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D).length(),(centreVoute-chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D).length());
                    }
                }
                //5 coupe sur rive gauche et faitiere ok
                if(((vna_distanceNoeudRiveGauche-distance0+ii*vna_diagonaleHorizontaleLosange)<0)&&((vna_distanceNoeudRiveGauche+distance1+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_angleHorizontalCentreNoeudDepart-angle1+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)&&((vna_angleHorizontalCentreNoeudDepart+angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinFaitiere))
                {
                    double t_a,t_y,t_x;
                    t_a = angle0/distance0;
                    t_x = vna_distanceNoeudRiveGauche+ii*vna_diagonaleHorizontaleLosange;
                    t_y = -vna_angleHorizontalCentreCoinFaitiere+vna_angleHorizontalCentreNoeudDepart+jj*vna_angleDiagonaleVerticaleLosange;
                    if(t_y<(t_a*t_x-demiEpaisseurSelonY))
                    {
                        int mm,nn;
                        nn = 5;
                        mm = 0;
                        /*for(int n=0;n<ccc[nn].nombre;n++)
                        {

                            if(ii == ccc[nn].position[n])
                            {
                               mm++;
                                chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                                chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                                chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                            }
                        }*/
                        if(mm == 0)
                        {
                            ccc[nn].nombre++;
                            chevCoupe.resize(chevCoupe.size()+1);
                            ccc[nn].pointeurs.append(chevCoupe.size()-1);
                            ccc[nn].position.append(ii);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 5;
                            vecteurPlanCoupe.setX(0.00001);vecteurPlanCoupe.setY(0.00001);vecteurPlanCoupe.setZ(-1.0);
                            vecteurIntersection = QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(0);pointPlanCoupe.setY(0);pointPlanCoupe.setZ(vna_distanceNoeudRiveGauche+ii*vna_diagonaleHorizontaleLosange);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                            if(vna_choixType==1)
                            {
                                vecteurPlanCoupe.setX(qCos(jj*vna_angleDiagonaleVerticaleLosange-pi/2));vecteurPlanCoupe.setY(-qSin(jj*vna_angleDiagonaleVerticaleLosange-pi/2));vecteurPlanCoupe.setZ(0.0);
                                vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                                pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                            }
                            else
                            {
                                vecteurPlanCoupe.setX(qCos(jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setY(-qSin(jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setZ(0.0);
                                vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                                pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                            }
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);

                        }
                    }


                }
                //1 coupe juste sur sabliere ok
                if(((vna_distanceNoeudRiveGauche-distance0+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_distanceNoeudRiveGauche+distance0+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+angle1+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere))
                {
                    int mm,nn;
                    mm = 0;
                    nn = 1;
                    for(int n=0;n<ccc[nn].nombre;n++)
                    {
                        //le chevron existe deja, on lui rajoute un
                        if(jj == ccc[nn].position[n])
                        {
                            mm++;
                            chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                            chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                        }
                    }
                    //pas de chevron pareil existant, on en rajoute un
                    if(mm == 0)
                    {
                        ccc[nn].nombre++;
                        chevCoupe.resize(chevCoupe.size()+1);
                        ccc[nn].pointeurs.append(chevCoupe.size()-1);
                        ccc[nn].position.append(jj);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 1;
                        vecteurPlanCoupe.setX(qSin(jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setY(qCos(jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setZ(0.0);
                        vecteurIntersection = QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                        pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreCoinSabliere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreCoinSabliere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = chev_VD3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = chev_VC3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = chev_VH3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = chev_VG3D;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                        vna_largeurSabliere = qMax(chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D.x(),chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D.x())-qMin(chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D.x(),chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D.x());
                        QVector3D v1,v2;
                        v1 = chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D-chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D;
                        v2.setX(0);v2.setY(0);v2.setZ(1);
                        vna_angleChevronBordSabliere = angleEntre2Vecteurs3D(v1,v2);
                    }
                }
                //3 coupe juste sur faitiere ok
                if(((vna_distanceNoeudRiveGauche-distance0+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_distanceNoeudRiveGauche+distance0+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart-angle1+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)&&((vna_angleHorizontalCentreNoeudDepart+angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinFaitiere))
                {
                    int mm,nn;
                    mm = 0;
                    nn = 3;
                    for(int n=0;n<ccc[nn].nombre;n++)
                    {
                        //le chevron existe deja, on lui rajoute un
                        if(jj == ccc[nn].position[n])
                        {
                            mm++;
                            chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                            chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                        }
                    }
                    //pas de chevron pareil existant, on en rajoute un
                    if(mm == 0)
                    {
                        ccc[nn].nombre++;
                        chevCoupe.resize(chevCoupe.size()+1);
                        ccc[nn].pointeurs.append(chevCoupe.size()-1);
                        ccc[nn].position.append(jj);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 3;
                        QVector3D v1,v2;
                        if(vna_choixType==1)
                        {
                            v2.setX(0);v2.setY(0);v2.setZ(1);
                            vecteurPlanCoupe.setX(qCos(jj*vna_angleDiagonaleVerticaleLosange-pi/2));vecteurPlanCoupe.setY(-qSin(jj*vna_angleDiagonaleVerticaleLosange-pi/2));vecteurPlanCoupe.setZ(0.0);
                            vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                        }
                        else
                        {
                            v2.setX(0);v2.setY(0);v2.setZ(1);
                            vecteurPlanCoupe.setX(qCos(jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setY(-qSin(jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setZ(0.0);
                            vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                        }
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = chev_VA3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = chev_VB3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = chev_VE3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = chev_VF3D;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                        v1 = chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D-chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D;
                        vna_angleChevronBordFaitiere = angleEntre2Vecteurs3D(v1,v2);
                    }
                }
                //2 juste coupe sur rive droite ok
                if(((vna_distanceNoeudRiveGauche-distance1+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_distanceNoeudRiveGauche+distance0+ii*vna_diagonaleHorizontaleLosange)>(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart-angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere))
                {
                    int mm,nn;
                    mm = 0;
                    nn = 2;
                    for(int n=0;n<ccc[nn].nombre;n++)
                    {
                        //le chevron existe deja, on lui rajoute un
                        if(ii == ccc[nn].position[n])
                        {
                            mm++;
                            chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                            chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                        }
                    }
                    //pas de chevron pareil existant, on en rajoute un
                    if(mm == 0)
                    {
                        ccc[nn].nombre++;
                        chevCoupe.resize(chevCoupe.size()+1);
                        ccc[nn].pointeurs.append(chevCoupe.size()-1);
                        ccc[nn].position.append(ii);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 2;
                        vecteurPlanCoupe.setX(0.00001);vecteurPlanCoupe.setY(0.00001);vecteurPlanCoupe.setZ(1.0);
                        vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                        pointPlanCoupe.setX(0);pointPlanCoupe.setY(0);pointPlanCoupe.setZ(-(vna_profondeurVoute-2.0*vna_epaisseurRive)+(vna_distanceNoeudRiveGauche+(ii)*vna_diagonaleHorizontaleLosange));
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = chev_VA3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = chev_VB3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = chev_VE3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = chev_VF3D;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                    }
                }
                //6 coupe sur rive droite et sabliere ok
                if(((vna_distanceNoeudRiveGauche-distance1+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_distanceNoeudRiveGauche+distance0+ii*vna_diagonaleHorizontaleLosange)>(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+angle1+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere))
                {
                    double t_a,t_y,t_x;
                    t_a = angle0/distance0;
                    t_x = -(vna_profondeurVoute-2.0*vna_epaisseurRive)+vna_distanceNoeudRiveGauche+ii*vna_diagonaleHorizontaleLosange;
                    t_y = -vna_angleHorizontalCentreCoinSabliere+vna_angleHorizontalCentreNoeudDepart+jj*vna_angleDiagonaleVerticaleLosange;
                    if(t_y>(t_a*t_x+demiEpaisseurSelonY))
                    {
                        int mm,nn;
                        nn = 6;
                        mm = 0;
                        /*for(int n=0;n<ccc[nn].nombre;n++)
                        {
                            if(ii == ccc[nn].position[n])
                            {
                                mm++;
                                chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                                chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                                chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                            }
                        }*/
                        if(mm == 0)
                        {
                            ccc[nn].nombre++;
                            chevCoupe.resize(chevCoupe.size()+1);
                            ccc[nn].pointeurs.append(chevCoupe.size()-1);
                            ccc[nn].position.append(ii);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 6;
                            vecteurPlanCoupe.setX(0.00001);vecteurPlanCoupe.setY(0.00001);vecteurPlanCoupe.setZ(1.0);
                            vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(0);pointPlanCoupe.setY(0);pointPlanCoupe.setZ(-(vna_profondeurVoute-2.0*vna_epaisseurRive)+(vna_distanceNoeudRiveGauche+(ii)*vna_diagonaleHorizontaleLosange));
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                            vecteurPlanCoupe.setX(qSin(jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setY(qCos(jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setZ(0.0);
                            vecteurIntersection = QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreCoinSabliere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreCoinSabliere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                        }
                    }
                }
                ii++;
            };
        jj++;
    };
    jj = -3;
    while((vna_angleHorizontalCentreNoeudDepart-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)
    {
        ii = -3;
        while((vna_distanceNoeudRiveGauche-distance0+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))
            {
                //4 coupe sur rive gauche et sabliere ok
                if(((vna_distanceNoeudRiveGauche-distance0+ii*vna_diagonaleHorizontaleLosange)<0)&&((vna_distanceNoeudRiveGauche+distance1+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_angleHorizontalCentreNoeudDepart-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+angle1+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere))
                {
                    double t_a,t_y,t_x;
                    t_a = angle0/distance0;
                    t_x = vna_distanceNoeudRiveGauche+ii*vna_diagonaleHorizontaleLosange;
                    t_y = -vna_angleHorizontalCentreCoinSabliere+vna_angleHorizontalCentreNoeudDepart+jj*vna_angleDiagonaleVerticaleLosange;
                    if(t_y>t_a*t_x)
                    {
                        //on rajoute un chevron penche a droite tronque sur rive gauche
                        for(int n=0;n<ccc[0].nombre;n++)
                        {
                            if(ii == ccc[0].position[n])
                            {
                                chevCoupe[ccc[0].pointeurs[n]].nombre++;
                                chevCoupe[ccc[0].pointeurs[n]].iver = jj;
                            }
                        }
                    }
                    else
                    {
                        //on rajoute un chevron penche a droite tronque sur sabliere
                        for(int n=0;n<ccc[1].nombre;n++)
                        {
                            if(jj == ccc[1].position[n])
                            {
                                chevCoupe[ccc[1].pointeurs[n]].nombre++;
                                chevCoupe[ccc[1].pointeurs[n]].ihor = ii;
                            }
                        }
                    }
                }
                //7 coupe sur rive droite et faitiere ok
                if(((vna_distanceNoeudRiveGauche-distance1+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_distanceNoeudRiveGauche+distance0+ii*vna_diagonaleHorizontaleLosange)>(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart-angle1+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)&&((vna_angleHorizontalCentreNoeudDepart+angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinFaitiere))
                {
                    double t_a,t_y,t_x;
                    t_a = angle0/distance0;
                    t_x = -(vna_profondeurVoute-2.0*vna_epaisseurRive)+vna_distanceNoeudRiveGauche+ii*vna_diagonaleHorizontaleLosange;
                    t_y = -vna_angleHorizontalCentreCoinFaitiere+vna_angleHorizontalCentreNoeudDepart+jj*vna_angleDiagonaleVerticaleLosange;
                    if(t_y>t_a*t_x)
                    {
                        //on rajoute un chevron penche a droite tronque sur faitiere
                        for(int n=0;n<ccc[3].nombre;n++)
                        {
                            if(jj == ccc[3].position[n])
                            {
                                chevCoupe[ccc[3].pointeurs[n]].nombre++;
                                chevCoupe[ccc[3].pointeurs[n]].jhor = ii;
                            }
                        }
                    }
                    else
                    {
                        //on rajoute un chevron penche a droite tronque sur rive droite
                        for(int n=0;n<ccc[2].nombre;n++)
                        {
                            if(ii == ccc[2].position[n])
                            {
                                chevCoupe[ccc[2].pointeurs[n]].nombre++;
                                chevCoupe[ccc[2].pointeurs[n]].jver = jj;
                            }
                        }
                    }
                }
                ii++;
            };
        jj++;
    };

    nombreTronqueaDroite = chevCoupe.size();

    //et les chevrons penches a gauche
    jj = -3;
    while((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)
    {
        ii = -3;
        while((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance0+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))
            {
                //8 juste coupe sur rive gauche ok
                if(((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance0+ii*vna_diagonaleHorizontaleLosange)<0)&&((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+distance1+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere))
                {
                    int mm,nn;
                    mm = 0;
                    nn = 8;
                    for(int n=0;n<ccc[nn].nombre;n++)
                    {
                        //le chevron existe deja, on lui rajoute un
                        if(ii == ccc[nn].position[n])
                        {
                            mm++;
                            chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                            chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                        }
                    }
                    //pas de chevron pareil existant, on en rajoute un
                    if(mm == 0)
                    {
                        ccc[nn].nombre++;
                        chevCoupe.resize(chevCoupe.size()+1);
                        ccc[nn].pointeurs.append(chevCoupe.size()-1);
                        ccc[nn].position.append(ii);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 8;
                        vecteurPlanCoupe.setX(0.00001);vecteurPlanCoupe.setY(0.00001);vecteurPlanCoupe.setZ(-1.0);
                        vecteurIntersection = QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                        pointPlanCoupe.setX(0);pointPlanCoupe.setY(0);pointPlanCoupe.setZ(-(vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+ii*vna_diagonaleHorizontaleLosange));
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = chev_VA3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = chev_VE3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = chev_VB3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = chev_VF3D;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                    }
                }
                //12 coupe sur rive gauche et sabliere ok
                if(((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance0+ii*vna_diagonaleHorizontaleLosange)<0)&&((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+distance1+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+angle1+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere))
                {
                    double t_a,t_y,t_x;
                    t_a = -angle0/distance0;
                    t_x = vna_distanceNoeudRiveGauche+ii*vna_diagonaleHorizontaleLosange+vna_demiDiagonaleHorizontaleLosange;
                    t_y = -vna_angleHorizontalCentreCoinSabliere+vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange;
                    if(t_y>(t_a*t_x+demiEpaisseurSelonY))
                    {
                        int mm,nn;
                        mm = 0;
                        nn = 12;
                        /*for(int n=0;n<ccc[nn].nombre;n++)
                        {
                            if(ii == ccc[nn].position[n])
                            {
                                mm++;
                                chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                                chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                                chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                            }
                        }*/
                        if(mm == 0)
                        {
                            ccc[nn].nombre++;
                            chevCoupe.resize(chevCoupe.size()+1);
                            ccc[nn].pointeurs.append(chevCoupe.size()-1);
                            ccc[nn].position.append(ii);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 12;
                            vecteurPlanCoupe.setX(0.00001);vecteurPlanCoupe.setY(0.00001);vecteurPlanCoupe.setZ(-1.0);
                            vecteurIntersection = QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(0);pointPlanCoupe.setY(0);pointPlanCoupe.setZ(-(vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+ii*vna_diagonaleHorizontaleLosange));
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                            vecteurPlanCoupe.setX(qSin(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setY(qCos(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setZ(0.0);
                            vecteurIntersection = QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreCoinSabliere-vna_demiAngleDiagonaleVerticaleLosange-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreCoinSabliere-vna_demiAngleDiagonaleVerticaleLosange-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);

                        }
                    }


                }
                //9 coupe juste sur sabliere ok
                if(((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance0+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+distance0+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+angle1+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere))
                {
                    int mm,nn;
                    mm = 0;
                    nn = 9;
                    for(int n=0;n<ccc[nn].nombre;n++)
                    {
                        if(jj == ccc[nn].position[n])
                        {
                            mm++;
                            chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                            chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                        }
                    }

                    if(mm == 0)
                    {
                        ccc[nn].nombre++;
                        chevCoupe.resize(chevCoupe.size()+1);
                        ccc[nn].pointeurs.append(chevCoupe.size()-1);
                        ccc[nn].position.append(jj);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 9;
                        vecteurPlanCoupe.setX(qSin(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setY(qCos(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setZ(0.0);
                        vecteurIntersection = QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                        pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(vna_angleHorizontalCentreCoinSabliere-vna_demiAngleDiagonaleVerticaleLosange-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(vna_angleHorizontalCentreCoinSabliere-vna_demiAngleDiagonaleVerticaleLosange-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = chev_VD3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = chev_VC3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = chev_VH3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = chev_VG3D;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                    }
                }
                //11 coupe juste sur faitiere ok
                if(((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance0+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+distance0+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle1+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinFaitiere))
                {
                    int mm,nn;
                    nn = 11;
                    mm = 0;
                    for(int n=0;n<ccc[nn].nombre;n++)
                    {

                        if(jj == ccc[nn].position[n])
                        {
                            mm++;
                            chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                            chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                        }
                    }

                    if(mm == 0)
                    {
                        ccc[nn].nombre++;
                        chevCoupe.resize(chevCoupe.size()+1);
                        ccc[nn].pointeurs.append(chevCoupe.size()-1);
                        ccc[nn].position.append(jj);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 11;
                        if(vna_choixType==1)
                        {
                            vecteurPlanCoupe.setX(qCos(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange-pi/2));vecteurPlanCoupe.setY(-qSin(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange-pi/2));vecteurPlanCoupe.setZ(0.0);
                            vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(-vna_demiAngleDiagonaleVerticaleLosange+vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(-vna_demiAngleDiagonaleVerticaleLosange+vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                        }
                        else
                        {
                            vecteurPlanCoupe.setX(qCos(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setY(-qSin(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setZ(0.0);
                            vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(-vna_demiAngleDiagonaleVerticaleLosange+vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(-vna_demiAngleDiagonaleVerticaleLosange+vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                        }
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = chev_VA3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = chev_VB3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = chev_VE3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = chev_VF3D;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);

                    }
                }
                //10 juste coupe sur rive droite ok
                if(((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance1+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+distance0+ii*vna_diagonaleHorizontaleLosange)>(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere))
                {
                    int mm,nn;
                    mm = 0;
                    nn = 10;
                    for(int n=0;n<ccc[nn].nombre;n++)
                    {

                        if(ii == ccc[nn].position[n])
                        {
                            mm++;
                            chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                            chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                        }
                    }

                    if(mm == 0)
                    {
                        ccc[nn].nombre++;
                        chevCoupe.resize(chevCoupe.size()+1);
                        ccc[nn].pointeurs.append(chevCoupe.size()-1);
                        ccc[nn].position.append(ii);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 10;
                        vecteurPlanCoupe.setX(0.00001);vecteurPlanCoupe.setY(0.00001);vecteurPlanCoupe.setZ(1.0);
                        vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                        pointPlanCoupe.setX(0);pointPlanCoupe.setY(0);pointPlanCoupe.setZ(-(-(vna_profondeurVoute-2.0*vna_epaisseurRive)+(vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+(ii)*vna_diagonaleHorizontaleLosange)));
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = chev_VD3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = chev_VH3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = chev_VC3D;chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = chev_VG3D;
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                        chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                    }
                }
                //15 coupe sur rive droite et faitiere ok
                if(((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance1+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+distance0+ii*vna_diagonaleHorizontaleLosange)>(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle1+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinFaitiere))
                {
                    double t_a,t_y,t_x;
                    t_a = -angle0/distance0;
                    t_x = -(vna_profondeurVoute-2.0*vna_epaisseurRive)+vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+ii*vna_diagonaleHorizontaleLosange;
                    t_y = -vna_angleHorizontalCentreCoinFaitiere+vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange;
                    if(t_y<(t_a*t_x-demiEpaisseurSelonY))
                    {
                        int mm,nn;
                        nn = 15;
                        mm = 0;
                        /*for(int n=0;n<ccc[nn].nombre;n++)
                        {
                            if(ii == ccc[nn].position[n])
                            {
                                mm++;
                                chevCoupe[ccc[nn].pointeurs[n]].nombre++;
                                chevCoupe[ccc[nn].pointeurs[n]].jhor = ii;
                                chevCoupe[ccc[nn].pointeurs[n]].jver = jj;
                           }
                        }*/
                        if(mm == 0)
                        {
                            ccc[nn].nombre++;
                            chevCoupe.resize(chevCoupe.size()+1);
                            ccc[nn].pointeurs.append(chevCoupe.size()-1);
                            ccc[nn].position.append(ii);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].ihor = ii;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].iver = jj;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jhor = ii;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].jver = jj;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].nombre = 1;
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].orientation = 15;
                            vecteurPlanCoupe.setX(0.00001);vecteurPlanCoupe.setY(0.00001);vecteurPlanCoupe.setZ(1.0);
                            vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                            pointPlanCoupe.setX(0);pointPlanCoupe.setY(0);pointPlanCoupe.setZ(-(-(vna_profondeurVoute-2.0*vna_epaisseurRive)+(vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+(ii)*vna_diagonaleHorizontaleLosange)));
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VA3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VE3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VB3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VF3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);
                            if(vna_choixType==1)
                            {
                                vecteurPlanCoupe.setX(qCos(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange-pi/2));vecteurPlanCoupe.setY(-qSin(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange-pi/2));vecteurPlanCoupe.setZ(0.0);
                                vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                                pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(-vna_demiAngleDiagonaleVerticaleLosange+vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(-vna_demiAngleDiagonaleVerticaleLosange+vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                            }
                            else
                            {
                                vecteurPlanCoupe.setX(qCos(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setY(-qSin(vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange));vecteurPlanCoupe.setZ(0.0);
                                vecteurIntersection = -QVector3D::crossProduct(vecteurPlanCoupe,vecteurNormalChevron1);
                                pointPlanCoupe.setX(vna_centre.x()+vna_rayonCourbureVoute*qCos(-vna_demiAngleDiagonaleVerticaleLosange+vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setY(vna_centre.y()+vna_rayonCourbureVoute*qSin(-vna_demiAngleDiagonaleVerticaleLosange+vna_angleHorizontalCentreCoinFaitiere-jj*vna_angleDiagonaleVerticaleLosange));pointPlanCoupe.setZ(0.0);
                            }
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VD3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VH3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonExtrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VC3D = intersection2Plans1Sphere3D(O1prime,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1prime,chev_rayonIntrado);
                            chevCoupe[ccc[nn].pointeurs[ccc[nn].nombre-1]].VG3D = intersection2Plans1Sphere3D(O1seconde,vecteurNormalChevron1,pointPlanCoupe,vecteurPlanCoupe,centreChevron1seconde,chev_rayonIntrado);

                        }
                    }


                }

                ii++;
            };
        jj++;
    };
    jj = -3;
    while((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)
    {
        ii = -3;
        while((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance0+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))
            {
                //13 coupe sur rive gauche et faitiere ok
                if(((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance0+ii*vna_diagonaleHorizontaleLosange)<0)&&((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+distance1+ii*vna_diagonaleHorizontaleLosange)>0)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle1+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+angle0+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinFaitiere))
                {
                    double t_a,t_y,t_x;
                    t_a = -angle0/distance0;
                    t_x = vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+ii*vna_diagonaleHorizontaleLosange;
                    t_y = -vna_angleHorizontalCentreCoinFaitiere+vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange;
                    if(t_y<t_a*t_x)
                    {
                        //on rajoute un chevron penche a droite tronque sur rive gauche
                        for(int n=0;n<ccc[8].nombre;n++)
                        {
                            if(ii == ccc[8].position[n])
                            {
                                chevCoupe[ccc[8].pointeurs[n]].nombre++;
                                chevCoupe[ccc[8].pointeurs[n]].jver = jj;
                            }
                        }
                    }
                    else
                    {
                        //on rajoute un chevron penche a gauche tronque sur faitiere
                        for(int n=0;n<ccc[11].nombre;n++)
                        {
                            if(jj == ccc[11].position[n])
                            {
                                chevCoupe[ccc[11].pointeurs[n]].nombre++;
                                chevCoupe[ccc[11].pointeurs[n]].ihor = ii;
                            }
                        }
                    }
                }
                //14 coupe sur rive droite et sabliere ok
                if(((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange-distance1+ii*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+distance0+ii*vna_diagonaleHorizontaleLosange)>(vna_profondeurVoute-2.0*vna_epaisseurRive))&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-angle0+jj*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinSabliere)&&((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+angle1+jj*vna_angleDiagonaleVerticaleLosange)>vna_angleHorizontalCentreCoinSabliere))
                {
                    double t_a,t_y,t_x;
                    t_a = -angle0/distance0;
                    t_x = -(vna_profondeurVoute-2.0*vna_epaisseurRive)+vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange+ii*vna_diagonaleHorizontaleLosange;
                    t_y = -vna_angleHorizontalCentreCoinSabliere+vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+jj*vna_angleDiagonaleVerticaleLosange;
                    if(t_y>t_a*t_x)
                    {
                        //on rajoute un chevron penche a gauche tronque sur rive droite
                        for(int n=0;n<ccc[10].nombre;n++)
                        {
                            if(ii == ccc[10].position[n])
                            {
                                chevCoupe[ccc[10].pointeurs[n]].nombre++;
                                chevCoupe[ccc[10].pointeurs[n]].iver = jj;
                            }
                        }
                    }
                    else
                    {
                        //on rajoute un chevron penche a gauche tronque sur sabliere
                        for(int n=0;n<ccc[9].nombre;n++)
                        {
                            if(jj == ccc[9].position[n])
                            {
                                chevCoupe[ccc[9].pointeurs[n]].nombre++;
                                chevCoupe[ccc[9].pointeurs[n]].jhor = ii;
                            }
                        }
                    }
                }
                ii++;
            };
        jj++;
    };

    //calculs 2D chevrons tronques

    for(int i=0;i<chevCoupe.size();i++)
    {
        int jjj;
        jjj = chevCoupe[i].orientation;
        //0 penche a droite coupe sur rive gauche
        //1 penche a droite coupe sur sabliere
        //8 penche a gauche coupe sur rive gauche
        //11 penche a gauche coupe sur faitiere
        if(jjj==0||jjj==1||jjj==8||jjj==11)
        {
            chevCoupe[i].VC = chev_VC;chevCoupe[i].VD = chev_VD;chevCoupe[i].WC = chev_VG;chevCoupe[i].WD = chev_VH;
            chevCoupe[i].C = chev_C;chevCoupe[i].D = chev_D;
            chevCoupe[i].VA = intersection2Cercles2D(chevCoupe[i].VD,(chevCoupe[i].VA3D-chevCoupe[i].VD3D).length(),chev_centreChevron,chev_rayonExtrado,0);
            chevCoupe[i].VB = intersection2Cercles2D(chevCoupe[i].VC,(chevCoupe[i].VB3D-chevCoupe[i].VC3D).length(),chev_centreChevron,chev_rayonIntrado,0);
            chevCoupe[i].WA = intersection2Cercles2D(chevCoupe[i].WD,(chevCoupe[i].VH3D-chevCoupe[i].VE3D).length(),chev_centreChevronPrime,chev_rayonExtrado,0);
            chevCoupe[i].WB = intersection2Cercles2D(chevCoupe[i].WC,(chevCoupe[i].VG3D-chevCoupe[i].VF3D).length(),chev_centreChevronPrime,chev_rayonIntrado,0);
            chevCoupe[i].A.setX(valeurLaPlusAGauche(chevCoupe[i].VA,chevCoupe[i].VB,chevCoupe[i].WA,chevCoupe[i].WB));chevCoupe[i].A.setY(0);chevCoupe[i].A.setZ(vna_demiEpaisseurChevron);
            chevCoupe[i].B = chevCoupe[i].A;chevCoupe[i].B.setY(-vna_retombeeChevron);
        }
        //2 penche a droite coupe sur rive droite
        //3 penche a droite coupe sur faitiere
        //9 penche a gauche coupe sur sabliere
        //10 penche a gauche coupe sur rive droite
        if(jjj==2||jjj==3||jjj==9||jjj==10)
        {
            chevCoupe[i].VA = chev_VA;chevCoupe[i].VB = chev_VB;chevCoupe[i].WA = chev_VE;chevCoupe[i].WB = chev_VF;
            chevCoupe[i].A = chev_A;chevCoupe[i].B = chev_B;
            chevCoupe[i].VD = intersection2Cercles2D(chevCoupe[i].VA,(chevCoupe[i].VA3D-chevCoupe[i].VD3D).length(),chev_centreChevron,chev_rayonExtrado,1);
            chevCoupe[i].VC = intersection2Cercles2D(chevCoupe[i].VB,(chevCoupe[i].VB3D-chevCoupe[i].VC3D).length(),chev_centreChevron,chev_rayonIntrado,1);
            chevCoupe[i].WD = intersection2Cercles2D(chevCoupe[i].WA,(chevCoupe[i].VH3D-chevCoupe[i].VE3D).length(),chev_centreChevronPrime,chev_rayonExtrado,1);
            chevCoupe[i].WC = intersection2Cercles2D(chevCoupe[i].WB,(chevCoupe[i].VG3D-chevCoupe[i].VF3D).length(),chev_centreChevronPrime,chev_rayonIntrado,1);
            chevCoupe[i].D.setX(valeurLaPlusADroite(chevCoupe[i].VD,chevCoupe[i].VC,chevCoupe[i].WD,chevCoupe[i].WC));chevCoupe[i].D.setY(0);chevCoupe[i].D.setZ(vna_demiEpaisseurChevron);
            chevCoupe[i].C = chevCoupe[i].D;chevCoupe[i].C.setY(-vna_retombeeChevron);
        }
        //5 penche a droite coupe sur rive gauche et faitiere
        //6 penche a droite coupe sur rive droite et sabliere
        //12 penche a gauche coupe sur rive gauche et sabliere
        //15 penche a gauche coupe sur rive droite et faitiere
        if(jjj==5||jjj==6||jjj==12||jjj==15)
        {
            chevCoupe[i].VD = intersection2Cercles2D(chev_VD,(chevCoupe[i].VD3D-chev_VD3D).length(),chev_centreChevron,chev_rayonExtrado,0);
            chevCoupe[i].VC = intersection2Cercles2D(chev_VC,(chevCoupe[i].VC3D-chev_VC3D).length(),chev_centreChevron,chev_rayonIntrado,0);
            chevCoupe[i].WD = intersection2Cercles2D(chev_VH,(chevCoupe[i].VH3D-chev_VH3D).length(),chev_centreChevronPrime,chev_rayonExtrado,0);
            chevCoupe[i].WC = intersection2Cercles2D(chev_VG,(chevCoupe[i].VG3D-chev_VG3D).length(),chev_centreChevronPrime,chev_rayonIntrado,0);
            chevCoupe[i].VA = intersection2Cercles2D(chev_VA,(chevCoupe[i].VA3D-chev_VA3D).length(),chev_centreChevron,chev_rayonExtrado,1);
            chevCoupe[i].VB = intersection2Cercles2D(chev_VB,(chevCoupe[i].VB3D-chev_VB3D).length(),chev_centreChevron,chev_rayonIntrado,1);
            chevCoupe[i].WA = intersection2Cercles2D(chev_VE,(chevCoupe[i].VE3D-chev_VE3D).length(),chev_centreChevronPrime,chev_rayonExtrado,1);
            chevCoupe[i].WB = intersection2Cercles2D(chev_VF,(chevCoupe[i].VF3D-chev_VF3D).length(),chev_centreChevronPrime,chev_rayonIntrado,1);
            chevCoupe[i].D.setX(valeurLaPlusADroite(chevCoupe[i].VD,chevCoupe[i].VC,chevCoupe[i].WD,chevCoupe[i].WC));chevCoupe[i].D.setY(0);chevCoupe[i].D.setZ(vna_demiEpaisseurChevron);
            chevCoupe[i].C = chevCoupe[i].D;chevCoupe[i].C.setY(-vna_retombeeChevron);
            chevCoupe[i].A.setX(valeurLaPlusAGauche(chevCoupe[i].VA,chevCoupe[i].VB,chevCoupe[i].WA,chevCoupe[i].WB));chevCoupe[i].A.setY(0);chevCoupe[i].A.setZ(vna_demiEpaisseurChevron);
            chevCoupe[i].B = chevCoupe[i].A;chevCoupe[i].B.setY(-vna_retombeeChevron);
        }

    }

    //la on change toutes les coordonnes pour avoir B comme origine de repere
    for(int i=0;i<chevCoupe.size();i++)
    {
        chevCoupe[i].A.setX(0);chevCoupe[i].A.setY(vna_retombeeChevron);
        chevCoupe[i].C.setX(chevCoupe[i].C.x()-chevCoupe[i].B.x());chevCoupe[i].C.setY(0);
        chevCoupe[i].D.setX(chevCoupe[i].D.x()-chevCoupe[i].B.x());chevCoupe[i].D.setY(vna_retombeeChevron);
        chevCoupe[i].VA.setX(chevCoupe[i].VA.x()-chevCoupe[i].B.x());chevCoupe[i].VA.setY(chevCoupe[i].VA.y()-chevCoupe[i].B.y());chevCoupe[i].VA.setZ(vna_demiEpaisseurChevron);
        chevCoupe[i].VB.setX(chevCoupe[i].VB.x()-chevCoupe[i].B.x());chevCoupe[i].VB.setY(chevCoupe[i].VB.y()-chevCoupe[i].B.y());chevCoupe[i].VB.setZ(vna_demiEpaisseurChevron);
        chevCoupe[i].VC.setX(chevCoupe[i].VC.x()-chevCoupe[i].B.x());chevCoupe[i].VC.setY(chevCoupe[i].VC.y()-chevCoupe[i].B.y());chevCoupe[i].VC.setZ(vna_demiEpaisseurChevron);
        chevCoupe[i].VD.setX(chevCoupe[i].VD.x()-chevCoupe[i].B.x());chevCoupe[i].VD.setY(chevCoupe[i].VD.y()-chevCoupe[i].B.y());chevCoupe[i].VD.setZ(vna_demiEpaisseurChevron);
        chevCoupe[i].WA.setX(chevCoupe[i].WA.x()-chevCoupe[i].B.x());chevCoupe[i].WA.setY(chevCoupe[i].WA.y()-chevCoupe[i].B.y());chevCoupe[i].WA.setZ(-vna_demiEpaisseurChevron);
        chevCoupe[i].WB.setX(chevCoupe[i].WB.x()-chevCoupe[i].B.x());chevCoupe[i].WB.setY(chevCoupe[i].WB.y()-chevCoupe[i].B.y());chevCoupe[i].WB.setZ(-vna_demiEpaisseurChevron);
        chevCoupe[i].WC.setX(chevCoupe[i].WC.x()-chevCoupe[i].B.x());chevCoupe[i].WC.setY(chevCoupe[i].WC.y()-chevCoupe[i].B.y());chevCoupe[i].WC.setZ(-vna_demiEpaisseurChevron);
        chevCoupe[i].WD.setX(chevCoupe[i].WD.x()-chevCoupe[i].B.x());chevCoupe[i].WD.setY(chevCoupe[i].WD.y()-chevCoupe[i].B.y());chevCoupe[i].WD.setZ(-vna_demiEpaisseurChevron);
        chevCoupe[i].B.setX(0);chevCoupe[i].B.setY(0);
    }

    //calcul du nombre de chevrons
    int m_i,m_j;
    double m_distance;
    //0 valeurs extremes en z des chevrons penches a droite
    m_i = 0;
    m_distance = qMax(-chev_VG3D.z(),qMax(-chev_VC3D.z(),qMax(-V1haut.z(),-W1haut.z())))-O1.z();
    while((-m_distance+m_i*vna_diagonaleHorizontaleLosange+vna_distanceNoeudRiveGauche)<0)
    {m_i++;};
    m_j = 0;
    while((m_distance+vna_distanceNoeudRiveGauche+m_j*vna_diagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))
    {m_j++;};
    vna_nombreChevronDroiteHorizontal = m_j-m_i;
    vna_debutChevronHorizontalDroite = m_i;
    vna_nombreChevronDroite = m_j-m_i;
    //1 valeurs extremes en z des chevrons penches a gauche
    m_i = 0;
    while((-m_distance+m_i*vna_diagonaleHorizontaleLosange+vna_distanceNoeudRiveGauche+vna_demiDiagonaleHorizontaleLosange)<0)
    {m_i++;};
    m_j = 0;
    while((m_distance+vna_distanceNoeudRiveGauche+m_j*vna_diagonaleHorizontaleLosange+vna_demiDiagonaleHorizontaleLosange)<(vna_profondeurVoute-2.0*vna_epaisseurRive))
    {m_j++;};
    vna_nombreChevronGaucheHorizontal = m_j-m_i;
    vna_debutChevronHorizontalGauche = m_i;
    vna_nombreChevronGauche =  m_j-m_i;
    //2 valeurs extremes en angle des chevrons penches a droite
    m_i = 0;
    double m_angle0;
    QVector3D m_vect0,m_vect1,m_vect2,m_vect3,m_vect4;
    m_vect0.setX(O1.x()-vna_centre.x());m_vect0.setY(O1.y()-vna_centre.y());m_vect0.setZ(0);
    m_vect1.setX(V1bas.x()-vna_centre.x());m_vect1.setY(V1bas.y()-vna_centre.y());m_vect1.setZ(0);
    m_vect2.setX(W1bas.x()-vna_centre.x());m_vect2.setY(W1bas.y()-vna_centre.y());m_vect2.setZ(0);
    m_vect3.setX(chev_VB3D.x()-vna_centre.x());m_vect3.setY(chev_VB3D.y()-vna_centre.y());m_vect3.setZ(0);
    m_vect4.setX(chev_VF3D.x()-vna_centre.x());m_vect4.setY(chev_VF3D.y()-vna_centre.y());m_vect4.setZ(0);
    m_angle0 = qMax(angleEntre2Vecteurs3D(m_vect3,m_vect0),qMax(angleEntre2Vecteurs3D(m_vect4,m_vect0),qMax(angleEntre2Vecteurs3D(m_vect1,m_vect0),angleEntre2Vecteurs3D(m_vect2,m_vect0))));
    while((vna_angleHorizontalCentreNoeudDepart-m_angle0+m_i*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinSabliere)
    {m_i++;};
    m_j = 0;
    while((vna_angleHorizontalCentreNoeudDepart+m_angle0+m_j*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)
    {m_j++;};
    vna_debutChevronVerticalDroite = m_i;
    vna_nombreChevronDroiteVertical = m_j-m_i;
    vna_nombreChevronDroite = vna_nombreChevronDroite*(m_j-m_i);
    //3 valeurs extremes en angle des chevrons penches a gauche
    m_i = 0;
    while((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange-m_angle0+m_i*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinSabliere)
    {m_i++;};
    m_j = 0;
    while((vna_angleHorizontalCentreNoeudDepart+vna_demiAngleDiagonaleVerticaleLosange+m_angle0+m_j*vna_angleDiagonaleVerticaleLosange)<vna_angleHorizontalCentreCoinFaitiere)
    {m_j++;};
    vna_debutChevronVerticalGauche = m_i;
    vna_nombreChevronGaucheVertical = m_j-m_i;
    vna_nombreChevronGauche = vna_nombreChevronGauche*(m_j-m_i);

    if(vna_choixType != 1)
    {
        vna_nombreChevronGauche *= 2;
        vna_nombreChevronDroite *= 2;
    }

}

double vouteNidAbeille::pointPlusEloigne()
{
    return qMax(qMax(vna_profondeurVoute,vna_portee),vna_hauteurVoute);
}

int vouteNidAbeille::orientationChevronTronque(int num)
{
    return chevCoupe[num].orientation;
}

double vouteNidAbeille::retombeeChevron()
{
    double retombeePlancheMin;
    retombeePlancheMin = qMax(absol(chev_A.y()-chev_VA.y()),qMax(absol(chev_A.y()-chev_VE.y()),qMax(absol(chev_D.y()-chev_VD.y()),absol(chev_D.y()-chev_VH.y()))));
    return retombeePlancheMin;
}
