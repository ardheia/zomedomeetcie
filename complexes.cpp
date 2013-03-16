#include "complexes.h"
#include <qmath.h>
#include "mathema.h"

complexes::complexes()
{
    reel = 0;
    imag = 0;
}

complexes::complexes(complexes &nbcomp)
{
    reel = nbcomp.re();
    imag = nbcomp.im();
}

complexes::~complexes()
{

}
complexes::complexes(const complexes &nbcomp)
{
    reel = nbcomp.re();
    imag = nbcomp.im();
}

void complexes::miseAJourRho()
{
    rho = qSqrt(qPow(reel,2.0)+qPow(imag,2.0));
}

void complexes::miseAJourTheta()
{
    if(reel>0)
        theta = qAtan(imag/reel);
    else
        theta = qAtan(imag/reel) + pi;
}

complexes complexes::racineCubique()
{
    miseAJourRho();
    miseAJourTheta();
    complexes solu;
    solu.setRe(qPow(rho,1.0/3.0)*qCos(theta/3.0+0.0/3.0));
    solu.setIm(qPow(rho,1.0/3.0)*qSin(theta/3.0+0.0/3.0));
    return solu;
}

complexes::complexes(double ree,double ima):reel(ree),imag(ima)
{
}

void complexes::setIm(double ima)
{
    imag = ima;
}

void complexes::setRe(double ree)
{
    reel = ree;
}

double complexes::im() const
{
    return imag;
}

double complexes::re() const
{
    return reel;
}

complexes& complexes::operator+=(const complexes &a)
{
    reel += a.re();
    imag += a.im();
    return *this;
}

complexes& complexes::operator-=(const complexes &a)
{
    reel -= a.re();
    imag -= a.im();
    return *this;
}

complexes& complexes::operator+=(const double &a)
{
    reel += a;
    return *this;
}

complexes& complexes::operator-=(const double &a)
{
    reel -= a;
    return *this;
}

complexes& complexes::operator*=(const complexes &a)
{
    double r,i;
    r = reel*a.re()-imag*a.im();
    i = reel*a.im()+imag*a.re();
    reel = r;
    imag = i;
    return *this;
}

complexes& complexes::operator*=(const double &a)
{
    reel *= a;
    imag *= a;
    return *this;
}

complexes operator+(complexes const& a, complexes const& b)
{
    complexes res(a);
    res += b;
    return res;
}

complexes operator-(complexes const& a, complexes const& b)
{
    complexes res(a);
    res -= b;
    return res;
}

complexes operator+(double const& a, complexes const& b)
{
    complexes res(b);
    res.setRe(b.re()+a);
    return res;
}

complexes operator-(double const& a, complexes const& b)
{
    complexes res;
    res.setRe(a-b.re());
    res.setIm(-b.im());
    return res;
}

complexes operator-(complexes const& a, double const& b)
{
    complexes res(a);
    res.setRe(a.re()-b);
    return res;
}

complexes operator-(complexes const& a)
{
    complexes res;
    res.setRe(-a.re());
    res.setIm(-a.im());
    return res;
}

complexes operator*(complexes const& a, complexes const& b)
{
    complexes res(a);
    res *= b;
    return res;
}

complexes operator*(double const& a, complexes const& b)
{
    complexes res(b);
    res *= a;
    return res;
}

complexes complexes::conjuque()
{
    complexes res;
    res.setIm(-imag);
    res.setRe(reel);
}

double complexes::module()
{
    double res;
    res = qSqrt(qPow(reel,2.0)+qPow(imag,2.0));
    return res;
}

complexes complexes::racineCarre()
{
    complexes res;
    res.setRe(qSqrt((this->module()+reel)/2.0));
    res.setIm(signe(imag)*qSqrt((this->module()-reel)/2.0));
    return res;
}

bool operator==(complexes const& a, complexes const& b)
{
    if((qAbs(a.re()-b.re())*10000.0<1.0)&&(qAbs(a.im()-b.im())*10000.0<1.0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool operator!=(complexes const& a, complexes const& b)
{
    if((qAbs(a.re()-b.re())*10000.0<1.0)&&(qAbs(a.im()-b.im())*10000.0<1.0))
    {
        return false;
    }
    else
    {
        return true;
    }
}
