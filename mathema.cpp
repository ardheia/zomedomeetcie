#include <QVector2D>
#include <QVector3D>
//#include <cmath>
#include <qmath.h>
#include <QMatrix4x4>
#include <QQuaternion>
#include "complexes.h"

#include "mathema.h"



//fonctions angles
double radians(double angledeg)
{
    double anglerad(angledeg*pi/180);
    return anglerad;
}

double degres(double anglerad)
{
    double angledeg(anglerad*180/pi);
    return angledeg;
}

double absol(double nombre)
{
    if(nombre<0)
    {return -nombre;}
    else
    {return nombre;}
}

int absol(int nombre2)
{
    if(nombre2<0)
    {return -nombre2;}
    else
    {return nombre2;}
}

int compte(QVector<short> tab, int i)
{
    int result;
    result = 0;
    for(int j=0;j<=i;i++)
    {
        result += tab[j];
    }
    return result;
}

bool pair(int h)
{
    int k = (int)h/2.0;
    if(h-2.0*k==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int signe(double h)
{
    if(h<0)
        return -1;
    else
        return 1;
}

double determinantMatrice3par3(QMatrix3x3 matr)
{
    double result;
    result = (matr(0,0)*matr(1,1)*matr(2,2)+matr(0,1)*matr(1,2)*matr(2,0)+matr(0,2)*matr(1,0)*matr(2,1))-(matr(0,2)*matr(1,1)*matr(2,0)+matr(0,0)*matr(2,1)*matr(1,2)+matr(2,2)*matr(1,0)*matr(0,1));
    return result;
}

QVector3D milieu2Points3D(QVector3D a, QVector3D b)
{
    QVector3D result;

    result.setX((a.x()+b.x())/2);
    result.setY((a.y()+b.y())/2);
    result.setZ((a.z()+b.z())/2);

    return result;
}

QQuaternion convertirMatriceRotation(QMatrix4x4 matr)
{
    QQuaternion result;
    double S,trace,max;
    trace = matr(0,0)+matr(1,1)+matr(2,2)+1;
    max = qMax(absol(matr(0,0)),absol(matr(1,1)));
    max = qMax(absol(max),absol(matr(2,2)));
    if(trace>0)
    {
        S = 1/(2*qSqrt(trace));
        result.setX((matr(2,1)-matr(1,2))*S);
        result.setY((matr(0,2)-matr(2,0))*S);
        result.setZ((matr(1,0)-matr(0,1))*S);
        result.setScalar(1/(4*S));
    }
    else if(max == absol(matr(0,0)))
    {
        S = qSqrt(1+matr(0,0)-matr(1,1)-matr(2,2))*2.0;
        result.setX(1/(2*S));
        result.setY((matr(0,1)-matr(1,0))/S);
        result.setZ((matr(0,2)-matr(2,0))/S);
        result.setScalar((matr(1,2)-matr(2,1))/S);
    }
    else if(max == absol(matr(1,1)))
    {
        S = qSqrt(1-matr(0,0)+matr(1,1)-matr(2,2))*2.0;
        result.setX((matr(0,1)-matr(1,0))/S);
        result.setY(1/(2*S));
        result.setZ((matr(1,2)-matr(2,1))/S);
        result.setScalar((matr(0,2)-matr(2,0))/S);
    }
    else if(max == absol(matr(2,2)))
    {
        S = qSqrt(1-matr(0,0)-matr(1,1)+matr(2,2))*2.0;
        result.setX((matr(0,2)-matr(2,0))/S);
        result.setY((matr(1,2)-matr(2,1))/S);
        result.setZ(1/(2*S));
        result.setScalar((matr(0,1)-matr(1,0))/S);
    }

    return result;
}

QVector3D rotationSelonQuaternion(QVector3D Vinit, QQuaternion quat)
{
    QVector3D Vfinal;
    double a,b,c,d,t2,t3,t4,t5,t6,t7,t8,t9,t10,v1,v2,v3;
    v1 = Vinit.x();
    v2 = Vinit.y();
    v3 = Vinit.z();
    b = quat.x();
    c = quat.y();
    d = quat.z();
    a = quat.scalar();
    t2 =   a*b;
    t3 =   a*c;
    t4 =   a*d;
    t5 =  -b*b;
    t6 =   b*c;
    t7 =   b*d;
    t8 =  -c*c;
    t9 =   c*d;
    t10 = -d*d;
    Vfinal.setX(2.0*( (t8 + t10)*v1 + (t6 -  t4)*v2 + (t3 + t7)*v3 ) + v1);
    Vfinal.setY(2.0*( (t4 +  t6)*v1 + (t5 + t10)*v2 + (t9 - t2)*v3 ) + v2);
    Vfinal.setZ(2.0*( (t7 -  t3)*v1 + (t2 +  t9)*v2 + (t5 + t8)*v3 ) + v3);
    return Vfinal;

}

QVector3D centreCercle3Points3D(QVector3D b, QVector3D a, QVector3D c)
{
    QVector3D M1, M2, centre, ab,ac,Vn;
    float D,E,F,G,H,I,J,K,L,Dprime,Eprime,Fprime,Gprime;

    M1 = milieu2Points3D(a,b);
    M2 = milieu2Points3D(a,c);
    ab = b-a;
    ac = c-a;
    Vn = QVector3D::normal(ab,ac);

    D = -ab.y()/ab.x();
    E = -ab.z()/ab.x();
    F = QVector3D::dotProduct(M1,ab)/ab.x();
    G = ac.y()/ac.x();
    H = ac.z()/ac.x();
    I = QVector3D::dotProduct(M2,ac)/ac.x();
    J = Vn.y()/Vn.x();
    K = Vn.z()/Vn.x();
    L = QVector3D::dotProduct(a,Vn)/Vn.x();
    Dprime = -(H+E)/(D+G);
    Eprime = (I-F)/(D+G);
    Fprime = (K+E)/(J+D);
    Gprime = (L-F)/(J+D);

    centre.setZ((Gprime-Eprime)/(Fprime+Dprime));
    centre.setY(Dprime*centre.z()+Eprime);
    centre.setX(D*centre.y()+E*centre.z()+F);

    return centre;
}

double rayonCercle3Points3D(QVector3D a, QVector3D b, QVector3D c)
{
    QVector3D centre, acentre;
    double rayon;

    centre = centreCercle3Points3D(a,b,c);

    acentre = centre - a;
    rayon = acentre.length();

    return rayon;
}

//fonction qui retourne un vecteur norme normal au vecteurs ab et ac
QVector3D vecteurNormalNorme3Points3D(QVector3D a, QVector3D b, QVector3D c)
{
    QVector3D vn,v1,v2;
    v1 = b-a;
    v2 = c-a;
    vn = QVector3D::crossProduct(v1,v2);
    vn = vn.normalized();
    return vn;
}

QVector3D intersection2Plans1Sphere3D(QVector3D pointPlan1, QVector3D VecteurNormalPlan1, QVector3D pointPlan2, QVector3D VecteurNormalPlan2, QVector3D centreSphere, double rayonSphere)
{
    double A,B,C,D,E,G,H,Aprime,Bprime,Cprime,Delta;
    QVector3D inter1, inter2, result;

    if(VecteurNormalPlan1.x()==0){VecteurNormalPlan1.setX(0.00001);}

    A = QVector3D::dotProduct(pointPlan1,VecteurNormalPlan1);
    B = QVector3D::dotProduct(pointPlan2,VecteurNormalPlan2);
    C = VecteurNormalPlan2.y()-VecteurNormalPlan2.x()*VecteurNormalPlan1.y()/VecteurNormalPlan1.x();
    D = VecteurNormalPlan2.z()-VecteurNormalPlan2.x()*VecteurNormalPlan1.z()/VecteurNormalPlan1.x();
    E = B-A*VecteurNormalPlan2.x()/VecteurNormalPlan1.x();
    G = A/VecteurNormalPlan1.x()-E*VecteurNormalPlan1.y()/(C*VecteurNormalPlan1.x());
    H = D*VecteurNormalPlan1.y()/(C*VecteurNormalPlan1.x())-VecteurNormalPlan1.z()/VecteurNormalPlan1.x();
    Aprime = qPow(H,2.0)+qPow(D,2.0)/qPow(C,2.0)+1;
    Bprime = 2*H*(G-centreSphere.x())-2*D*(E/C-centreSphere.y())/C-2*centreSphere.z();
    double aa,bb,cc,dd;
    aa = qPow((G-centreSphere.x()),2.0);
    bb = qPow((E/C-centreSphere.y()),2.0);
    cc = qPow(centreSphere.z(),2.0);
    dd = qPow(rayonSphere,2.0);
    Cprime = qPow((G-centreSphere.x()),2.0)+qPow((E/C-centreSphere.y()),2.0)+qPow(centreSphere.z(),2.0)-qPow(rayonSphere,2.0);
    Delta = qPow(Bprime,2.0)-4*Aprime*Cprime;
//probleme de delta negatif
    inter1.setZ((-Bprime+qSqrt(Delta))/(2*Aprime));
    inter2.setZ((-Bprime-qSqrt(Delta))/(2*Aprime));
    inter1.setY((E-D*inter1.z())/C);
    inter2.setY((E-D*inter2.z())/C);
    inter1.setX((A-inter1.y()*VecteurNormalPlan1.y()-inter1.z()*VecteurNormalPlan1.z())/VecteurNormalPlan1.x());
    inter2.setX((A-inter2.y()*VecteurNormalPlan1.y()-inter2.z()*VecteurNormalPlan1.z())/VecteurNormalPlan1.x());

    //deux solutions, on choisit la plus proche des chevrons

    if((inter1-pointPlan1).length()<(inter2-pointPlan1).length())
    {
        result=inter1;
    }
    else
    {
        result=inter2;
    }

    return result;
}

QVector3D intersection1Plan1CercleDansPlanOxz3D(QVector3D pointPlan, QVector3D VecteurNormalPlan, QVector3D centreCercle, double rayonCercle)
{
    double A,C,D;
    double eqa,eqb,eqc,eqdelta;
    QVector3D sol1,sol2;

    D = QVector3D::dotProduct(pointPlan,VecteurNormalPlan);
    A = D/VecteurNormalPlan.x();
    C = -VecteurNormalPlan.z()/VecteurNormalPlan.x();

    eqa = qPow(C,2.0)+1;
    eqb = 2.0*C*(A-centreCercle.x())-2.0*centreCercle.z();
    eqc = qPow((A-centreCercle.x()),2.0)+qPow(centreCercle.z(),2.0)-qPow(rayonCercle,2.0);
    eqdelta = qPow(eqb,2.0)-4*eqa*eqc;

    sol1.setZ((-eqb+qSqrt(eqdelta))/(2.0*eqa));
    sol2.setZ((-eqb-qSqrt(eqdelta))/(2.0*eqa));
    sol1.setY(0);
    sol2.setY(0);
    sol1.setX(A+C*sol1.z());
    sol2.setX(A+C*sol2.z());

    if(sol1.x()>sol2.x())
    {
        return sol1;
    }
    else
    {
        return sol2;
    }

}

double angleEntre2Vecteurs3D(QVector3D a, QVector3D b)
{
    double angle;
    angle = qAcos(QVector3D::dotProduct(a,b)/(a.length()*b.length()));
    if(angle>pi)
    {angle = 2.0*pi-angle;}
    return angle;
}

double anglePhiHorizontalEntre2PointsEt1Centre3D(QVector3D centre, QVector3D point1, QVector3D point2)
{
    point1.setZ(centre.z());
    point2.setZ(centre.z());
    QVector3D a, b;
    a = point1-centre;
    b = point2-centre;
    double ang;
    ang = angleEntre2Vecteurs3D(a,b);
    return ang;
}

QVector3D rotationAutourDeZ(QVector3D a, double phi)
{
    QVector3D re;
    re.setZ(a.z());
    re.setX(a.x()*qCos(phi)-a.y()*qSin(phi));
    re.setY(a.x()*qSin(phi)+a.y()*qCos(phi));
    return re;
}

QVector3D rotationAutourDeZetCentre(QVector3D point, double ang, QVector3D centre)
{
    point = point - centre;
    point = rotationAutourDeZ(point,ang);
    point = point + centre;
    return point;
}

QVector<complexes> resolutionDegre3Cardan(double a,double b,double c,double d)
{
    QVector<complexes> solu;
    solu.clear();

    complexes j,jcarre;
    j.setRe(-0.5);
    j.setIm(qSqrt(3.0)/2.0);
    jcarre.setRe(-0.5);
    jcarre.setIm(-qSqrt(3.0)/2.0);

    double cardan_p, cardan_q, cardan_delta;
    cardan_p = -qPow(b,2.0)/(3.0*qPow(a,2.0))+c/a;
    cardan_q = 2.0*qPow(b,3.0)/qPow(3.0*a,3.0)-b*c/(3.0*qPow(a,2.0))+d/a;
    cardan_delta = qPow(cardan_q,2.0)+4.0*qPow(cardan_p,3.0)/27.0;
    if(cardan_delta>0)
    {
        double u,v;
        u = signe(-cardan_q+qSqrt(cardan_delta))*qPow(qAbs(-cardan_q+qSqrt(cardan_delta))/2.0,1.0/3.0);
        v = signe(-cardan_q-qSqrt(cardan_delta))*qPow(qAbs(-cardan_q-qSqrt(cardan_delta))/2.0,1.0/3.0);
        solu.append(complexes(u+v,0.0));
        solu.append(complexes(-0.5*u-0.5*v,qSqrt(3.0)*u/2.0-qSqrt(3.0)*v/2.0));
        solu.append(complexes(-0.5*u-0.5*v,-qSqrt(3.0)*u/2.0+qSqrt(3.0)*v/2.0));
    }
    else if (cardan_delta<0)
    {
        solu.append(complexes(2.0*qSqrt(-cardan_p/3.0)*qCos((1.0/3.0)*qAcos(-cardan_q*qSqrt(27.0/-qPow(cardan_p,3.0))/2.0)+2.0*0.0*pi/3.0),0.0));
        solu.append(complexes(2.0*qSqrt(-cardan_p/3.0)*qCos((1.0/3.0)*qAcos(-cardan_q*qSqrt(27.0/-qPow(cardan_p,3.0))/2.0)+2.0*1.0*pi/3.0),0.0));
        solu.append(complexes(2.0*qSqrt(-cardan_p/3.0)*qCos((1.0/3.0)*qAcos(-cardan_q*qSqrt(27.0/-qPow(cardan_p,3.0))/2.0)+2.0*2.0*pi/3.0),0.0));
    }
    else
    {
        if((cardan_p==0)||(cardan_q==0))
        {
            solu.append(complexes(0.0,0.0));
            solu.append(complexes(0.0,0.0));
            solu.append(complexes(0.0,0.0));
        }
        else
        {
            solu.append(complexes(3.0*cardan_q/cardan_p,0.0));
            solu.append(complexes(-3.0*cardan_q/(2.0*cardan_p),0.0));
            solu.append(complexes(-3.0*cardan_q/(2.0*cardan_p),0.0));
        }
    }
    for(int i=0;i<3;i++)
        solu[i] = solu[i]-b/(3.0*a);
    return solu;
}

//fonction obsolete plus utilisee nulle part
QVector<double> resolutionDegre4Ferrari(double a,double b,double c,double d,double e)
{
    //resolution equation du 4 eme degre az4+bz3+cz2+dz+e=0 d'apres ferrari
    double ferrari_p, ferrari_q, ferrari_r, ferrari_a, ferrari_b, ferrari_c, ferrari_d, ferrari_y0;
    ferrari_p = -3.0*qPow(b,2.0)/(8.0*qPow(a,2.0))+c/a;
    ferrari_q = qPow(b/2.0,3.0)/qPow(a,3.0)-0.5*b*c/qPow(a,2.0)+d/a;
    ferrari_r = -3.0*qPow(b/(4*a),4.0)+c*(qPow(b/4.0,2.0)/qPow(a,3.0))-0.25*b*d/qPow(a,2.0)+e/a;
    ferrari_a = 8.0;
    ferrari_b = -4.0*ferrari_p;
    ferrari_c = -8.0*ferrari_r;
    ferrari_d = 4.0*ferrari_r*ferrari_p-qPow(ferrari_q,2.0);

    //un peu de methode de cardan pour le 3eme degre ay3+by2+cy+d=0
    QVector<complexes> soll;
    soll = resolutionDegre3Cardan(ferrari_a,ferrari_b,ferrari_c,ferrari_d);
    double cardan_z0 = soll[0].re();

    //et on reprend ferrari
    ferrari_y0 = cardan_z0-ferrari_b/(3.0*ferrari_a);
    double ferrari_a0carre, ferrari_b0, ferrari_delta12, ferrari_delta34;
    QVector<double> ferrari_x;
    ferrari_a0carre = -ferrari_p+2.0*ferrari_y0;
    if(2.0*ferrari_y0-ferrari_p==0)
    {
        ferrari_b0 = qSqrt(qPow(ferrari_y0,2.0)-ferrari_r);
    }
    else
    {
        ferrari_b0 = -ferrari_q/(2.0*qSqrt(ferrari_a0carre));
    }
    ferrari_delta12 = ferrari_a0carre-4.0*(ferrari_y0-ferrari_b0);
    ferrari_delta34 = ferrari_a0carre-4.0*(ferrari_y0+ferrari_b0);
    ferrari_x.clear();
    ferrari_x.append((qSqrt(ferrari_a0carre)+qSqrt(ferrari_delta12))/2.0);
    ferrari_x.append((qSqrt(ferrari_a0carre)-qSqrt(ferrari_delta12))/2.0);
    ferrari_x.append((-qSqrt(ferrari_a0carre)+qSqrt(ferrari_delta34))/2.0);
    ferrari_x.append((-qSqrt(ferrari_a0carre)-qSqrt(ferrari_delta34))/2.0);

    //et la on a 4 solutions
    QVector<double> sol_x;
    sol_x.clear();
    for(int i=0;i<4;i++)
    {
        sol_x.append(ferrari_x[i]-b/(4.0*a));
    }
    return sol_x;
}

QVector3D intersection1CylindreSelonOzEtUnCercleDansUnPlan(double rayonCylindre, double rayonCercle, QVector3D centreCercle, QVector3D pointPlan, QVector3D VNplan)
{
    double m_A,m_B,m_C,m_D,m_E,m_F,a,b,c,d,e;
    m_A = QVector3D::dotProduct(pointPlan,VNplan);
    m_B = qPow(centreCercle.x(),2.0)+qPow(centreCercle.y(),2.0)+qPow(centreCercle.z(),2.0)+qPow(rayonCylindre,2.0)-qPow(rayonCercle,2.0)-2.0*centreCercle.z()*m_A/VNplan.z()+qPow(rayonCylindre,2.0)*qPow(VNplan.y(),2.0)/qPow(VNplan.z(),2.0)+qPow(m_A,2.0)/qPow(VNplan.z(),2.0);
    m_C = -2.0*centreCercle.x()+2.0*centreCercle.z()*VNplan.x()/VNplan.z()-2.0*m_A*VNplan.x()/qPow(VNplan.z(),2.0);
    m_D = qPow(VNplan.x(),2.0)/qPow(VNplan.z(),2.0)-qPow(VNplan.y(),2.0)/qPow(VNplan.z(),2.0);
    m_E = -2.0*centreCercle.y()+2.0*centreCercle.z()*VNplan.y()/VNplan.z()-2.0*m_A*VNplan.y()/qPow(VNplan.z(),2.0);
    m_F = 2.0*VNplan.x()*VNplan.y()/qPow(VNplan.z(),2.0);
    a = qPow(m_D,2.0)+qPow(m_F,2.0);
    b = 2.0*m_C*m_D+2.0*m_E*m_F;
    c = qPow(m_C,2.0)+2*m_B*m_D-qPow(rayonCylindre,2.0)*qPow(m_F,2.0)+qPow(m_E,2.0);
    d = 2.0*m_B*m_C-qPow(rayonCylindre,2.0)*2.0*m_E*m_F;
    e = qPow(m_B,2.0)-qPow(rayonCylindre,2.0)*qPow(m_E,2.0);
    //resolution equation du 4 eme degre az4+bz3+cz2+dz+e=0 d'apres ferrari
    //QVector<double> sol_x;
    //sol_x = resolutionDegre4Ferrari(a,b,c,d,e);
    QVector<complexes> sol_x;
    sol_x = resolutionDegre4Lagrange(a,b,c,d,e);

    //et la on a 4 solutions
    QVector<QVector3D> sol;

    QVector<double> dist;
    sol.clear();
    sol.resize(8);
    dist.clear();
    dist.resize(8);
    double distmin = 90000.0;
    QVector3D sol_approche = intersection2Plans1Sphere3D(pointPlan,VNplan,QVector3D(rayonCylindre,0.0,0.0),QVector3D(1.0,0.0,0.0),centreCercle,rayonCercle);

    for(int i=0;i<4;i++)
    {
        sol[i*2].setX(qAbs(sol_x[i].re()));
        sol[i*2].setY(-qSqrt(qPow(rayonCylindre,2.0)-qPow(sol[i*2].x(),2.0)));
        sol[i*2].setZ((m_A-sol[i*2].x()*VNplan.x()-sol[i*2].y()*VNplan.y())/VNplan.z());
        dist[i*2] = (sol[i*2]-sol_approche).length();
        if((dist[i*2]<distmin)&&(rayonCylindre>=sol[i*2].x())&&(sol_x[i].im()==0.0))
        {
            distmin = dist[i*2];
        }
        sol[i*2+1].setX(qAbs(sol_x[i].re()));
        sol[i*2+1].setY(qSqrt(qPow(rayonCylindre,2.0)-qPow(sol[i*2+1].x(),2.0)));
        sol[i*2+1].setZ((m_A-sol[i*2+1].x()*VNplan.x()-sol[i*2+1].y()*VNplan.y())/VNplan.z());
        dist[i*2+1] = (sol[i*2+1]-sol_approche).length();
        if((dist[i*2+1]<distmin)&&(rayonCylindre>=sol[i*2+1].x())&&(sol_x[i].im()==0.0))
        {
            distmin = dist[i*2+1];
        }
    }
    QVector3D resuc;
    for(int i=0;i<8;i++)
    {
        if(qAbs(dist[i]-distmin)<0.01)
        {
            resuc = sol[i];
        }
    }
    return resuc;
}

QVector<complexes> resolutionDegre4Lagrange(double a, double b, double c, double d, double e)
{

    //resolution equation du 4 eme degre ax4+bx3+cx2+dx+e=0 d'apres lagrange
    double p, q, r;
    p = -3.0*qPow(b,2.0)/(8.0*qPow(a,2.0))+c/a;
    q = qPow(b/2.0,3.0)/qPow(a,3.0)-0.5*b*c/qPow(a,2.0)+d/a;
    r = -3.0*qPow(b/(4*a),4.0)+c*(qPow(b/4.0,2.0)/qPow(a,3.0))-0.25*b*d/qPow(a,2.0)+e/a;
    double card_a,card_b,card_c,card_d;
    card_a = 1;
    card_b = 2.0*p;
    card_c = qPow(p,2.0)-4.0*r;
    card_d = -qPow(q,2.0);
    QVector<complexes> cardan;
    cardan = resolutionDegre3Cardan(card_a,card_b,card_c,card_d);
    cardan.append(cardan[0].racineCarre());
    cardan.append(cardan[1].racineCarre());
    cardan.append(cardan[2].racineCarre());
    complexes qcomplexe(-q,0.0);
    if((cardan[3]*cardan[4]*cardan[5])!=qcomplexe)
        cardan[5]=-cardan[5];
    QVector<complexes> sol_z, sol_x;
    sol_z.clear();
    sol_z.append(0.5*(cardan[3]+cardan[4]+cardan[5]));
    sol_z.append(0.5*(cardan[3]-cardan[4]-cardan[5]));
    sol_z.append(0.5*(-cardan[3]+cardan[4]-cardan[5]));
    sol_z.append(0.5*(-cardan[3]-cardan[4]+cardan[5]));
    sol_x.clear();
    for(int i=0;i<4;i++)
        sol_x.append(sol_z[i]-b/(4.0*a));
    return sol_x;
}

QVector3D intersectionDroite1Cercle2D(QVector3D premierPointDroite, QVector3D deuxiemePointDroite, QVector3D centreCercle, double rayonCercle)
{
    QVector3D result,inter1,inter2;
    double a,b,A,B,C,delta;
    a = (deuxiemePointDroite.y()-premierPointDroite.y())/(deuxiemePointDroite.x()-premierPointDroite.x());
    b = premierPointDroite.y()-a*premierPointDroite.x();
    A = 1+qPow(a,2.0);
    B = 2*a*(b-centreCercle.y())-2*centreCercle.x();
    C = qPow(b,2.0)-2*b*centreCercle.y()+qPow(centreCercle.y(),2.0)-qPow(rayonCercle,2.0)+qPow(centreCercle.x(),2.0);
    delta = qPow(B,2.0)-4*A*C;

    inter1.setX((-B+qSqrt(delta))/(2*A));inter1.setY(a*inter1.x()+b);
    inter2.setX((-B-qSqrt(delta))/(2*A));inter2.setY(a*inter2.x()+b);

    if(inter1.y()>inter2.y())
    {result = inter1;}
    else
    {result = inter2;}

    result.setZ(premierPointDroite.z());

    return result;
}

QVector3D intersection2DroitesPlanOxy(QVector3D point1droite1, QVector3D point2droite1, QVector3D point1droite2, QVector3D point2droite2)
{
    double a1,b1,a2,b2;
    QVector3D sol;
    a1 = (point2droite1.y()-point1droite1.y())/(point2droite1.x()-point1droite1.x());
    b1 = point1droite1.y()-a1*point1droite1.x();
    a2 = (point2droite2.y()-point1droite2.y())/(point2droite2.x()-point1droite2.x());
    b2 = point1droite2.y()-a2*point1droite2.x();
    sol.setZ(point1droite1.z());
    sol.setX((b2-b1)/(a1-a2));
    sol.setY(a1*sol.x()+b1);
    return sol;

}

QVector3D intersection2Cercles2D(QVector3D centrePremierCercle, double rayonPremierCercle, QVector3D centreDeuxiemeCercle, double rayonDeuxiemeCercle, int positionGaucheouDroiteRapportCentrePremierCercle)
{
    double N,A,B,C,Delta,sol1,sol2;
    QVector3D result;
    // si positionGaucheouDroiteRapportCentrePremierCercle = 0 alors solution de gauche, si = 1 alors solution de droite

    N = (qPow(rayonDeuxiemeCercle,2.0)-qPow(rayonPremierCercle,2.0)-qPow(centreDeuxiemeCercle.x(),2.0)+qPow(centrePremierCercle.x(),2.0)-qPow(centreDeuxiemeCercle.y(),2.0)+qPow(centrePremierCercle.y(),2.0))/(2*(centrePremierCercle.y()-centreDeuxiemeCercle.y()));
    A = qPow((centrePremierCercle.x()-centreDeuxiemeCercle.x())/(centrePremierCercle.y()-centreDeuxiemeCercle.y()),2.0)+1;
    B = 2*centrePremierCercle.y()*(centrePremierCercle.x()-centreDeuxiemeCercle.x())/(centrePremierCercle.y()-centreDeuxiemeCercle.y())-2*N*(centrePremierCercle.x()-centreDeuxiemeCercle.x())/(centrePremierCercle.y()-centreDeuxiemeCercle.y())-2*centrePremierCercle.x();
    C = qPow(centrePremierCercle.x(),2.0)+qPow(centrePremierCercle.y(),2.0)+qPow(N,2.0)-qPow(rayonPremierCercle,2.0)-2*centrePremierCercle.y()*N;
    Delta = qPow(B,2.0)-4*A*C;
    if(Delta>=0)
    {
        sol1 = (-B+qSqrt(Delta))/(2.0*A);
        sol2 = (-B-qSqrt(Delta))/(2.0*A);
        if(positionGaucheouDroiteRapportCentrePremierCercle==0)
        {
            if(sol1<sol2)
            {
                result.setX(sol1);
            }
            else
            {
                result.setX(sol2);
            }
        }
        else
        {
            if(sol1>sol2)
            {
                result.setX(sol1);
            }
            else
            {
                result.setX(sol2);
            }
        }
        result.setZ(centrePremierCercle.z());
        result.setY(N-result.x()*(centrePremierCercle.x()-centreDeuxiemeCercle.x())/(centrePremierCercle.y()-centreDeuxiemeCercle.y()));
    }
    else
    {
        result.setX(0);result.setY(0);result.setZ(0);
    }

    return result;
}

double valeurLaPlusAGauche(QVector3D a, QVector3D b, QVector3D c, QVector3D d)
{
    double result;
    if(a.x()<b.x())
    {
        result = a.x();
    }
    else
    {
        result = b.x();
    }
    if(c.x()<result)
    {
        result = c.x();
    }
    if(d.x()<result)
    {
        result = d.x();
    }
    return result;
}

double valeurLaPlusADroite(QVector3D a, QVector3D b, QVector3D c, QVector3D d)
{
    double result;
    if(a.x()>b.x())
    {
        result = a.x();
    }
    else
    {
        result = b.x();
    }
    if(c.x()>result)
    {
        result = c.x();
    }
    if(d.x()>result)
    {
        result = d.x();
    }
    return result;
}

double valeurLaPlusEnBas(QVector3D a, QVector3D b, QVector3D c, QVector3D d)
{
    double result;
    if(a.y()<b.y())
    {
        result = a.y();
    }
    else
    {
        result = b.y();
    }
    if(c.y()<result)
    {
        result = c.y();
    }
    if(d.y()<result)
    {
        result = d.y();
    }
    return result;
}

QVector2D milieu2Points2D(QVector2D a, QVector2D b)
{
    QVector2D result;

    result.setX((a.x()+b.x())/2);
    result.setY((a.y()+b.y())/2);

    return result;
}

QVector2D centreCercle2Points2DavecRayon1(QVector2D a, QVector2D b, double rayon)
{
    QVector2D milieu, centre;
    double A,B,E,F,G,delta;

    milieu.setX((a.x()+b.x())/2);
    milieu.setY((a.y()+b.y())/2);

    A = (b.x()-a.x())/(a.y()-b.y());
    B = (a.y()+b.y())/2-((a.x()+b.x())/2)*((b.x()-a.x())/(a.y()-b.y()));
    E = 1+pow(A,2);
    F = 2*A*B-2*a.x()-2*A*a.y();
    G = -(pow(rayon,2)-pow(a.y(),2)-pow(a.x(),2)-pow(B,2)+2*a.y()*B);
    delta = pow(F,2)-4*E*G;

    centre.setX((-F+sqrt(delta))/(2*E));
    centre.setY(A*centre.x()+B);

    return centre;

}

QVector2D centreCercle2Points2DavecRayon2(QVector2D a, QVector2D b, double rayon)
{
    QVector2D milieu, centre;
    double A,B,E,F,G,delta;

    milieu.setX((a.x()+b.x())/2);
    milieu.setY((a.y()+b.y())/2);

    A = (b.x()-a.x())/(a.y()-b.y());
    B = (a.y()+b.y())/2-((a.x()+b.x())/2)*((b.x()-a.x())/(a.y()-b.y()));
    E = 1+pow(A,2);
    F = 2*A*B-2*a.x()-2*A*a.y();
    G = -(pow(rayon,2)-pow(a.y(),2)-pow(a.x(),2)-pow(B,2)+2*a.y()*B);
    delta = pow(F,2)-4*E*G;

    centre.setX((-F-sqrt(delta))/(2*E));
    centre.setY(A*centre.x()+B);

    return centre;
}

QVector2D centreCercle3Points2D(QVector2D a, QVector2D b, QVector2D c)
{
    QVector2D M1, M2, centre;
    float A,B,C,D;

    M1 = milieu2Points2D(a,b);
    M2 = milieu2Points2D(a,c);

    if((a.y()-b.y())==0)
    {
        A = (b.x()-a.x())/0.0001;
    }
    else
    {
        A = (b.x()-a.x())/(a.y()-b.y());
    }
    B = M1.y()-A*M1.x();
    if((a.y()-c.y())==0)
    {
        C = (c.x()-a.x())/0.0001;
    }
    else
    {
        C = (c.x()-a.x())/(a.y()-c.y());
    }
    D = M2.y()-C*M2.x();

    centre.setX((D-B)/(A-C));
    centre.setY(A*centre.x()+B);

    return centre;
}

double rayonCercle3Points2D(QVector2D a, QVector2D b, QVector2D c)
{
    QVector2D centre, acentre;
    double rayon;

    centre = centreCercle3Points2D(a,b,c);

    acentre = centre - a;
    rayon = acentre.length();

    return rayon;
}

double angleHorizontalCentrePoint3D(QVector3D centre, QVector3D point)
{
    double ang;
    if(point.x()==centre.x())
    {
        ang = qAtan((point.z()-centre.z())/0.0001);
    }
    else if(centre.x()<point.x())
    {
        ang = qAtan((point.z()-centre.z())/(point.x()-centre.x()));
    }
    else if(centre.x()>point.x())
    {
        ang = pi-qAbs(qAtan((point.z()-centre.z())/(point.x()-centre.x())));
    }
    return ang;
}

double angleHorizontalCentrePoint2D(QVector2D centre, QVector2D point)
{
    double ang;
    if(point.x()==centre.x())
    {
        ang = qAtan((point.y()-centre.y())/0.0001);
    }
    else if(centre.x()<point.x())
    {
        ang = qAtan((point.y()-centre.y())/(point.x()-centre.x()));
    }
    else if(centre.x()>point.x())
    {
        ang = pi-qAbs(qAtan((point.y()-centre.y())/(point.x()-centre.x())));
    }
    return ang;
}

double angleHorizontalCentrePoint2D_2(QVector2D centre, QVector2D point)
{
    double ang;
    if(point.x()==centre.x())
    {
        ang = pi/2.0;
    }
    else if(centre.x()<point.x())
    {
        ang = qAtan(qAbs(point.y()-centre.y())/(point.x()-centre.x()));
    }
    else if(centre.x()>point.x())
    {
        ang = pi-(qAtan(qAbs(point.y()-centre.y())/qAbs(point.x()-centre.x())));
    }
    if(point.y()<centre.y())
        ang =2.0*pi-ang;
    return ang;
}

QVector3D intersectionDroiteAvecDroiteHorizontale(QVector3D premierPointDroite, QVector3D deuxiemePointDroite, double YdeLaDroiteHorizontale)
{
    double a,b;
    QVector3D sol;
    a = (deuxiemePointDroite.y()-premierPointDroite.y())/(deuxiemePointDroite.x()-premierPointDroite.x());
    b = premierPointDroite.y()-a*premierPointDroite.x();
    sol = QVector3D((YdeLaDroiteHorizontale-b)/a,YdeLaDroiteHorizontale,premierPointDroite.z());
    return sol;
}

QVector2D rotation2DautourOrigine(QVector2D a, double angle)
{
    QVector2D b = QVector2D(a.x()*qCos(angle)+a.y()*qSin(angle),-a.x()*qSin(angle)+a.y()*qCos(angle));
    return b;
}

double angleDeDifferenceDuPremierVersLeDeuxieme(double premierAngle, double secondAngle, bool horaire)
{
    double res;
    if(horaire)
    {
        while(secondAngle>premierAngle)
        {
            secondAngle -= 2.0*pi;
        }
        res = premierAngle-secondAngle;
    }
    else
    {
        while(secondAngle<premierAngle)
        {
            secondAngle += 2.0*pi;
        }
        res = secondAngle-premierAngle;
    }
    return res;

}
