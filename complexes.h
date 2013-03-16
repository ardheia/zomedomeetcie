#ifndef COMPLEXES_H
#define COMPLEXES_H

class complexes
{
public:
    complexes();
    complexes(complexes &nbcomp);
    complexes(double ree,double ima);
    complexes(const complexes &nbcomp);
    ~complexes();
    void miseAJourRho();
    void miseAJourTheta();
    void setRe(double ree);
    void setIm(double ima);
    double re() const;
    double im() const;
    complexes conjuque();
    complexes racineCarre();
    complexes racineCubique();
    double module();

    //surcharge operateur
    complexes& operator+=(const complexes &a);
    complexes& operator+=(const double &a);
    complexes& operator-=(const complexes &a);
    complexes& operator-=(const double &a);
    complexes& operator*=(const complexes &a);
    complexes& operator*=(const double &a);

private:

    double reel, imag;
    double rho, theta;
};

complexes operator+(complexes const& a, complexes const& b);
complexes operator-(complexes const& a, complexes const& b);
complexes operator+(double const& a, complexes const& b);
complexes operator-(double const& a, complexes const& b);
complexes operator-(complexes const& a, double const& b);
complexes operator-(complexes const& a);
complexes operator*(complexes const& a, complexes const& b);
complexes operator*(double const& a, complexes const& b);
bool operator==(complexes const& a, complexes const& b);
bool operator!=(complexes const& a, complexes const& b);

#endif // COMPLEXES_H
