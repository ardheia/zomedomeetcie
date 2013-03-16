#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>

#include <qmath.h>
#include "objet3d.h"

//
void Geometry::loadArrays() const
{
    // fonction qui dit a opengl ou aller chercher les coordonnes des sommets
    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    // fonction qui dit a open gl ou aller chercher les vecteurs normaux a chaque sommet
    // vecteur normaux qui servent a  gerer l'eclairage surtout
    glNormalPointer(GL_FLOAT, 0, normals.constData());
}

void Geometry::finalize()
{
    // normalise tous les vecteurs normaux, c'est une obligation
    for (int i = 0; i < normals.count(); ++i)
        normals[i].normalize();
}

//ca c'est une fonction pour ajouter un sommet de maniere smooth, c'est a dire que s'il existe deja pour une autre face
// on additionne leurs normales pour obtenir une normale moyenne et on ne redefinit pas le sommet

//remarquer le fait que la boucle part de la fin du tableau car c'est souvent dans les derniers sommets ajoutes que
//l'on va trouver le sommet similaire
QVector3D Geometry::appendSmooth(const QVector3D &a, const QVector3D &n, int from)
{
    double tt = 0.0;
    //la fonction .count donne le nombre de cases occupees dans un tableau dynamique
    int v = vertices.count() - 1;
    // regarde si le point a existe deja, si oui, sort de la boucle for
    for ( ; v >= from; --v)
        if (qFuzzyCompare(vertices[v], a))
            break;
    // on recupere alors la valeur de v en sortie (forcee ou pas) de boucle
    if (v < from)
    {
        // le sommet n'a pas ete trouve donc on l'ajoute ainsi que sa normale a la fin des tableaux avec append
        v = vertices.count();
        vertices.append(a);
        normals.append(n);
        tt = 1.0;
    }
    else
    {
        // si on a trouve le sommet, on additionne alors a la normale du sommet deja en place la normale du sommet
        // que l'on voulait rajouter, pas bete, et apres il faudra finalize pour normaliser
        normals[v] += n;
    }
    // dans les deux cas, on reference le point par son index
    // le tableau faces contient autant de cases que de points, mais pour eviter qu'on definisse certains points en doubles
    // ce tableau relit le deuxieme sommet au premier defini pareil
    faces.append(v);
    return QVector3D((double)v,tt,0.0);
}


//la on ajoute un sommet de maniere brutale, pour creer des aretes franches, on rajoute le point et sa normale meme si il
// existe deja
int Geometry::appendFaceted(const QVector3D &a, const QVector3D &n)
{
    int v = vertices.count();
    vertices.append(a);
    normals.append(n);
    faces.append(v);
    return v;
}


//ca c'est une fonction qui convertit un format de couleur en un autre
void qSetColor(float colorVec[], QColor c)
{
    colorVec[0] = c.redF();
    colorVec[1] = c.greenF();
    colorVec[2] = c.blueF();
    colorVec[3] = c.alphaF();
}



//
Patch::Patch(Geometry *g)
   : start(g->faces.count())
   , count(0)
   , initv(g->vertices.count())
   , sm(Patch::Smooth)
   , startlignes(g->lignes.count())
   , countlignes(0)
   , countv(0)
   , geom(g)
{
    //definit la couleur des faces
    qSetColor(faceColor, QColor(Qt::darkGray));
}

//fonction qui ajoute une rotation a la matrice
void Patch::rotate(qreal deg, QVector3D axis)
{
    mat.rotate(deg, axis);
}

//fonction qui rajoute une translation a la matrice
void Patch::translate(const QVector3D &t)
{
    mat.translate(t);
}

void Patch::rotate(QQuaternion quater)
{
    mat.rotate(quater);
}

void Patch::rotateRapportVecteurEtCentre(qreal deg, QVector3D axis, QVector3D centre)
{

}


//
static inline void qMultMatrix(const QMatrix4x4 &mat)
{
    if (sizeof(qreal) == sizeof(GLfloat))
        glMultMatrixf((GLfloat*)mat.constData());
#ifndef QT_OPENGL_ES
    else if (sizeof(qreal) == sizeof(GLdouble))
        glMultMatrixd((GLdouble*)mat.constData());
#endif
    else
    {
        GLfloat fmat[16];
        qreal const *r = mat.constData();
        for (int i = 0; i < 16; ++i)
            fmat[i] = r[i];
        glMultMatrixf(fmat);
    }
}

//
//! [2]
void Patch::draw(bool p_filaire) const
{
    //on copie la matrice d'affichage en la pushant
    glPushMatrix();
    //on la multiplie localement par la matrice de transformation
    qMultMatrix(mat);
    //definit le materiau de notre objet
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, faceColor);
    if(p_filaire)
    {
        // constdata retourne un pointeur vers la variable choisie
        const GLushort *indices2 = geom->lignes.constData();
        //cree la geometrie avec des lignes
        glDrawElements(GL_LINES,countlignes,GL_UNSIGNED_SHORT,indices2+startlignes);
    }
    else
    {
        // constdata retourne un pointeur vers la variable choisie
        const GLushort *indices = geom->faces.constData();
        //cree la geometrie avec des triangles
        // count c'est le nombre de points qu'il va falloir dessiner, ou nombre de triangle/3, qui n'est pas egal au nombre de points
        // definit dans le tableau car on a fusionne plusieurs points
        // charge le tableau des indices (ou faces)
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indices + start);
    }
    //on recupere la matrice ?
    glPopMatrix();
}
//! [2]

// fonction qui ajoute un triangle, avec ses 3 points et sa normale
void Patch::addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n, bool ligneab, bool lignebc, bool ligneca)
{
    int indexa,indexb,indexc;
    // si la normale est nulle alors on cree une normale avec les 3 points
    QVector3D norm = n.isNull() ? QVector3D::normal(a, b, c) : n;
    // selon le smooth on ajoute les points d'une facon ou d'une autre
    // le initv c'est juste quand on ajoute de maniere smooth, pour savoir jusqu'ou chercher les points pareils
    if (sm == Smooth)
    {
        QVector3D restt;
        restt = geom->appendSmooth(a, norm, initv);
        indexa = (int)restt.x();
        if(restt.y() != 0.0) countv++;
        restt = geom->appendSmooth(b, norm, initv);
        indexb = (int)restt.x();
        if(restt.y() != 0.0) countv++;
        restt = geom->appendSmooth(c, norm, initv);
        indexc = (int)restt.x();
        if(restt.y() != 0.0) countv++;
    }
    else
    {
        indexa = geom->appendFaceted(a, norm);
        indexb = geom->appendFaceted(b, norm);
        indexc = geom->appendFaceted(c, norm);
        countv +=3;
    }
    // et on rajoute 3 points au compteur
    if(ligneab)
    {
        geom->lignes.append(indexa);
        geom->lignes.append(indexb);
        countlignes +=2;
    }
    if(lignebc)
    {
        geom->lignes.append(indexb);
        geom->lignes.append(indexc);
        countlignes +=2;
    }
    if(ligneca)
    {
        geom->lignes.append(indexc);
        geom->lignes.append(indexa);
        countlignes +=2;
    }
    count += 3;
}


// fonction pour ajouter directement un carre, ca decompose direct en deux ajouts de triangles
void Patch::addQuad(const QVector3D &a, const QVector3D &b,  const QVector3D &c, const QVector3D &d, bool ligneab, bool lignebc, bool lignecd, bool ligneda)
{
    int indexa,indexc,indexd;
    //on calcule la normale direct
    QVector3D norm = QVector3D::normal(a, b, c);
    if (sm == Smooth)
    {
        addTri(a, b, c, norm, ligneab, lignebc, false);
        addTri(a, c, d, norm, false, lignecd, ligneda);
    }
    else
    {
        // si on est en faceted alors on partage les points a et c
        addTri(a, b, c, norm, ligneab, lignebc, false);
        int k = geom->vertices.count();
        QVector3D restt;
        restt = geom->appendSmooth(a, norm, k);
        indexa = (int)restt.x();
        if(restt.y() != 0.0) countv++;
        restt = geom->appendSmooth(c, norm, k);
        indexc = (int)restt.x();
        if(restt.y() != 0.0) countv++;
        indexd = geom->appendFaceted(d, norm);
        countv ++;
        if(lignecd)
        {
            geom->lignes.append(indexc);
            geom->lignes.append(indexd);
            countlignes +=2;
        }
        if(ligneda)
        {
            geom->lignes.append(indexd);
            geom->lignes.append(indexa);
            countlignes +=2;
        }
        count += 3;
    }
}

// une fonction qui ressort une matrice de tout nos points extrudes de depth (epaisseur) vers le bas suivant z
QVector<QVector3D> extrude(const QVector<QVector3D> &verts, qreal depth)
{
    QVector<QVector3D> extr = verts;
    for (int v = 0; v < extr.count(); ++v)
        extr[v].setZ(extr[v].z() - depth);
    return extr;
}
