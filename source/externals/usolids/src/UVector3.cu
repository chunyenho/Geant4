//
// ********************************************************************
// * This Software is part of the AIDA Unified Solids Library package *
// * See: https://aidasoft.web.cern.ch/USolids                        *
// ********************************************************************
//
// $Id:$
//
// --------------------------------------------------------------------
//
// UVector3
//
// 19.09.12 Marek Gayer
//          Created from original implementation in CLHEP
// --------------------------------------------------------------------

#include "UVector3.hh"
#include "UUtils.hh"

//______________________________________________________________________________
UVector3::UVector3(double theta, double phi)
{
  // Creates a unit vector based on theta and phi angles
  x_ = std::sin(theta) * std::cos(phi);
  y_ = std::sin(theta) * std::sin(phi);
  z_ = std::cos(theta);
}

//______________________________________________________________________________
double UVector3::Angle(const UVector3& q) const
{
  // return the angle w.r.t. another 3-vector
  double ptot2 = Mag2() * q.Mag2();
  if (ptot2 <= 0)
  {
    return 0.0;
  }
  else
  {
    double arg = Dot(q) / std::sqrt(ptot2);
    if (arg >  1.0) arg =  1.0;
    if (arg < -1.0) arg = -1.0;
    return UUtils::ACos(arg);
  }
}

//______________________________________________________________________________
double UVector3::Mag() const
{
  // return the magnitude (rho in spherical coordinate system)

  return std::sqrt(Mag2());
}

//______________________________________________________________________________
double UVector3::Perp() const
{
  //return the transverse component  (R in cylindrical coordinate system)

  return std::sqrt(Perp2());
}

//______________________________________________________________________________
double UVector3::Phi() const
{
  //return the  azimuth angle. returns phi from -pi to pi
  return x_ == 0.0 && y_ == 0.0 ? 0.0 : UUtils::ATan2(y_, x_);
}

//______________________________________________________________________________
double UVector3::Theta() const
{
  //return the polar angle from 0 to pi
  double mag2 = Mag2();
  if (mag2 == 0.0) return 0.0;
  return UUtils::ACos(z_ / std::sqrt(mag2));
}

//______________________________________________________________________________
UVector3 UVector3::Unit() const
{
  // return unit vector parallel to this.
  double  tot = Mag2();
  UVector3 p(x_, y_, z_);
  return tot > 0.0 ? p *= (1.0 / std::sqrt(tot)) : p;
}

//______________________________________________________________________________
double UVector3::Normalize()
{
  // Normalize to unit. Return normalization factor.
  double  mag = Mag2();
  if (mag == 0.0) return mag;;
  mag = std::sqrt(mag);
  x_ /= mag;
  y_ /= mag;
  z_ /= mag;
  return mag;
}

//______________________________________________________________________________
void UVector3::RotateX(double angle)
{
  //rotate vector around X
  double s = std::sin(angle);
  double c = std::cos(angle);
  double yy = y_;
  y_ = c * yy - s * z_;
  z_ = s * yy + c * z_;
}

//______________________________________________________________________________
void UVector3::RotateY(double angle)
{
  //rotate vector around Y
  double s = std::sin(angle);
  double c = std::cos(angle);
  double zz = z_;
  z_ = c * zz - s * x_;
  x_ = s * zz + c * x_;
}

//______________________________________________________________________________
void UVector3::RotateZ(double angle)
{
  //rotate vector around Z
  double s = std::sin(angle);
  double c = std::cos(angle);
  double xx = x_;
  x_ = c * xx - s * y_;
  y_ = s * xx + c * y_;
}
