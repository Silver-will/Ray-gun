#include "Quad.h"
#include<iostream>
Quad::Quad(const Point& _Q, const glm::vec3& _u, const glm::vec3& _v, std::shared_ptr<Material> m)
	:Q(_Q),u(_u),v(_v), mat{m}
{
    auto n = cross(u, v);
    normal = glm::normalize(n);
    d = dot(normal, Q);
    w = n / dot(n, n);

    area = glm::length(n);
    SetBoundingBox();
}

double Quad::PDFValue(const Point& origin, const Point& direction) const
{
    HitRecord rec;
    if (!this->RayHit(Ray(origin, direction), rec, Interval(0.001, Common::infinity)))
        return 0;


    auto distance_squared = rec.t * rec.t * glm::dot(direction, direction);
    auto cosine = fabs(glm::dot(direction, rec.normal)) / glm::length(direction);

    return distance_squared / (cosine * area);
}

glm::vec3 Quad::Random(const Point& origin)const
{
    auto p = Q + ((float)random_double() * u) + ((float)random_double() * v);
    return p - origin;
}

void Quad::SetBoundingBox()
{
    BBox = AABB(Q, Q + u + v).Pad();

}

AABB Quad::GetBoundingBox() const
{
    return BBox;
}

bool Quad::RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t) const
{
    auto denom = glm::dot(normal, r.GetDirection());
    // No hit if the ray is parallel to the plane.
    if (fabs(denom) < 1e-8)
        return false;

    // Return false if the hit point parameter t is outside the ray interval.
    auto t = (d - dot(normal, r.GetOrigin())) / denom;
    if (!ray_t.Contains(t))
        return false;

    // Determine the hit point lies within the planar shape using its plane coordinates.
    auto intersection = r.At(t);
    glm::vec3 planar_hitpt_vector = intersection - Q;
    auto alpha = dot(w, cross(planar_hitpt_vector, v));
    auto beta = dot(w, cross(u, planar_hitpt_vector));

    if (!IsInterior(alpha, beta, hit))
        return false;

    // Ray hits the 2D shape; set the rest of the hit record and return true.
    hit.t = t;
    hit.p = intersection;
    hit.mat = mat;
    hit.SetFaceNormal(r, normal);
    return true;

}

bool Quad::IsInterior(double a, double b, HitRecord& rec) const
{
    if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
        return false;

    rec.u = a;
    rec.v = b;
    return true;
}

