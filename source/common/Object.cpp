//////////////////////////////////////////////////////////////////////////////
//
//  --- Object.cpp ---
//  Created by Brian Summa
//
//////////////////////////////////////////////////////////////////////////////

#include "common.h"

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Sphere::intersect(vec4 p0, vec4 V)
{
  IntersectionValues result;
  result.t = raySphereIntersection(p0, V);
  result.P = p0 + (result.t * V);
  result.N = normalize(result.P - this->center);
  result.name = this->name;
  return result;
}

/* -------------------------------------------------------------------------- */
/* ------ Ray = p0 + t*V  sphere at origin center and radius radius    : Find t ------- */
double Sphere::raySphereIntersection(vec4 p0, vec4 V)
{
  double t = std::numeric_limits<double>::infinity();
  vec4 centerToRay = p0 - center;

  double a = dot(V, V);
  double b = 2. * dot(centerToRay, V);
  double c = dot(centerToRay, centerToRay) - (this->radius * this->radius);

  double delta = (b * b) - (4 * a * c);

  if (nearlyEqual(delta, 0.0, EPSILON))
  {
    return (-b) / 2 * a;
  }
  if (delta < EPSILON)
  {
    return t;
  }
  double res = (-b - sqrt(delta)) / 2 * a;
  double res2 = (-b + sqrt(delta)) / 2 * a;

  if (res < EPSILON && res2 < EPSILON)
  {
    return t;
  }
  if (res < EPSILON)
  {
    return res2;
  }
  if (res2 < EPSILON || res < res2)
  {
    return res;
  }
  else
  {
    return res2;
  }
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Square::intersect(vec4 p0, vec4 V)
{
  IntersectionValues result;
  result.t = raySquareIntersection(p0, V);
  result.P = p0 + result.t * V;
  result.N = normalize(this->normal);
  result.name = this->name;
  return result;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
double Square::raySquareIntersection(vec4 p0, vec4 V)
{
  double t = std::numeric_limits<double>::infinity();

  vec4 n = vec4(normal.x, normal.y, normal.z, 0.0);

  double D = dot(mesh.vertices[0], n);
  t = (D - (dot(p0, n))) / (dot(V, n));

  if (t >= std::numeric_limits<double>::infinity() || t < EPSILON)
  {
    return std::numeric_limits<double>::infinity();
  }

  vec4 p1, p2, p3, p4;
  p1 = mesh.vertices[0];
  p3 = mesh.vertices[1];
  p4 = mesh.vertices[5];
  p2 = mesh.vertices[2];

  vec4 p1p2, p2p3, p3p4, p4p1;
  p1p2 = p2 - p1;
  p2p3 = p3 - p2;
  p3p4 = p4 - p3;
  p4p1 = p1 - p4;

  vec4 intersection = p0 + (V * t);

  vec4 p1t, p2t, p3t, p4t;
  p1t = intersection - p1;
  p2t = intersection - p2;
  p3t = intersection - p3;
  p4t = intersection - p4;

  vec4 pv14, pv24, pv34, pv44;
  pv14 = cross(p1p2, p1t);
  pv24 = cross(p2p3, p2t);
  pv34 = cross(p3p4, p3t);
  pv44 = cross(p4p1, p4t);

  vec3 pv1 = vec3(pv14.x, pv14.y, pv14.z);
  vec3 pv2 = vec3(pv24.x, pv24.y, pv24.z);
  vec3 pv3 = vec3(pv34.x, pv34.y, pv34.z);
  vec3 pv4 = vec3(pv44.x, pv44.y, pv44.z);

  double n1, n2, n3, n4;
  n1 = dot(pv1, pv2);
  n2 = dot(pv2, pv3);
  n3 = dot(pv3, pv4);
  n4 = dot(pv4, pv1);

  if ((n1 < EPSILON && n2 < EPSILON && n3 < EPSILON && n4 < EPSILON) || (n1 > EPSILON && n2 > EPSILON && n3 > EPSILON && n4 > EPSILON))
  {
    return t;
  }

  return std::numeric_limits<double>::infinity();
}
