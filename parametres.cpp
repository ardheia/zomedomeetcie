#include "parametres.h"

int finesse()
{
    return 10;
}

QString version()
{
    return "13.03.16";
}

QColor couleur(int num)
{
    QVector<QColor> coul;
    coul.resize(19);
    coul[0].setRgb(140,91,14);
    coul[1].setRgb(107,167,20);
    coul[2].setRgb(107,167,214);
    coul[3].setRgb(184,62,151);
    coul[4].setRgb(219,214,79);
    coul[5].setRgb(234,84,93);
    coul[6].setRgb(239,170,62);
    coul[7].setRgb(37,74,169);
    coul[8].setRgb(73,118,77);
    coul[9].setRgb(162,41,0);
    coul[10].setRgb(154,139,115);
    coul[11].setRgb(239,180,238);
    coul[12].setRgb(248,220,0);
    coul[13].setRgb(122,0,18);
    coul[14].setRgb(0,193,129);
    coul[15].setRgb(132,139,243);
    coul[16].setRgb(199,238,14);
    coul[17].setRgb(88,73,87);
    coul[18].setRgb(255,138,0);
    for(int i=19;i<38;i++)
    {
        coul.append(coul[i-19]);
    }
    for(int i=38;i<57;i++)
    {
        coul.append(coul[i-19]);
    }
    for(int i=57;i<76;i++)
    {
        coul.append(coul[i-19]);
    }
    return coul[num];
}

QString couleurInternet(int um)
{
    QColor cou = couleur(um);
    return cou.name();
}
