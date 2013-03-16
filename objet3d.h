#ifndef OBJET3D_H
#define OBJET3D_H

/*
  ça c'est une librairie vraiment spécifique au traçage en 3D, qui définit la géométrie dans laquelle est stockée
  l'objet, qui définit les fonctions openGL à envoyer pour le dessiner
*/


void qSetColor(float colorVec[], QColor c);

//! [0]
// creation d'une structure, c'est a dire un assemblage de variables de type differents
struct Geometry
{
    // QVector est un tableau dynamique de type ce qui il y a entre <>
    // glushort ca veut dire que c'est un type GL ushort -> unsigned short : entier de 0 a 65535
    // le tableau face c'est un tableau qui pointe vers les bons sommets les uns apres les autres pour la declaration en
    // GL_TRIANGLES
    QVector<GLushort> faces;
    // tableau qui contient les lignes pour le dessin en filaire
    QVector<GLushort> lignes;
    // Qvector3D c'est un type de vecteurs (ou de points) 3d avec x, y et z
    // les vertices ca va etre les sommets de notre objet
    QVector<QVector3D> vertices;
    // les normales a chaque sommet, qui permettent l'affichage avec l'eclairage
    QVector<QVector3D> normals;
    //rajoute un sommet de maniere smooth, en lissant les normales
    QVector3D appendSmooth(const QVector3D &a, const QVector3D &n, int from);
    //rajoute un sommet de maniere brutale, avec arete france
    int appendFaceted(const QVector3D &a, const QVector3D &n);
    //norme les vecteurs normaux
    void finalize();
    //charge les sommets et leurs vecteurs normaux
    void loadArrays() const;
};
//! [0]

// cree une classe patch
//! [1]
class Patch
{
public:
    //cree une enumeration de noms smoothing avec 2 valeurs possibles
    enum Smoothing { Faceted, Smooth };

    Patch(Geometry *);
    //une fonction qui definit la valeur de smoothing
    void setSmoothing(Smoothing s) { sm = s; }

    void translate(const QVector3D &t);
    void rotate(qreal deg, QVector3D axis);
    void rotate(QQuaternion quater);
    void rotateRapportVecteurEtCentre(qreal deg, QVector3D axis, QVector3D centre);
    void draw(bool p_filaire) const;
    void addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n, bool ligneab, bool lignebc, bool ligneca);
    void addQuad(const QVector3D &a, const QVector3D &b,  const QVector3D &c, const QVector3D &d, bool ligneab, bool lignebc, bool lignecd, bool ligneda);

    GLushort start;
    GLushort count;
    GLushort initv;
    GLushort startlignes;
    GLushort countlignes;
    GLushort countv;

    GLfloat faceColor[4];
    //matrice de transformation
    QMatrix4x4 mat;
    Smoothing sm;
    // cree une geometrie
    Geometry *geom;
};
//! [1]

QVector<QVector3D> extrude(const QVector<QVector3D> &verts, qreal depth);



#endif // OBJET3D_H
