#include "ephemeris.hh"
#include "minivec.hh"
/* |            t0e tow     |  - > tow - t0e, <3.5 days, so ok

   | t0e                tow |   -> tow - t0e > 3.5 days, so 
                                   7*86400 - tow + t0e

   |         tow t0e        |   -> 7*86400 - tow + t0e > 3.5 days, so
                                  tow - t0e (negative age)

   | tow               t0e  |   -> 7*86400 - tow + t0e < 3.5 days, ok
*/

// positive age = t0e in the past
int ephAge(int tow, int t0e)
{
  int diff = tow - t0e;
  if(diff > 3.5*86400)
    diff -= 604800;
  if(diff < -3.5*86400)
    diff += 604800;
  return diff;
}

// all axes start at earth center of gravity
// x-axis is on equator, 0 longitude
// y-axis is on equator, 90 longitude
// z-axis is straight up to the north pole
// https://en.wikipedia.org/wiki/ECEF#/media/File:Ecef.png
std::pair<double,double> getLongLat(double x, double y, double z)
{
  Point core{0,0,0};
  Point LatLon0{1,0,0};

  Point pos{x, y, z};
  
  Point proj{x, y, 0}; // in equatorial plane now
  Vector flat(core, proj);
  Vector toLatLon0{core, LatLon0};
  double longitude = acos( toLatLon0.inner(flat) / (toLatLon0.length() * flat.length()));
  if(y < 0)
    longitude *= -1;
  
  Vector toUs{core, pos};
  double latitude =  acos( flat.inner(toUs) / (toUs.length() * flat.length()));
  if(z < 0)
    latitude *= -1;

  return std::make_pair(longitude, latitude);
}