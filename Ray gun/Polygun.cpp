#include "Polygun.h"

#include "ShapeList.h"
#include "Material.h"

#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Mesh::Mesh(std::string_view model_path, Point pos)
{
    auto model = LoadGLTF(model_path);
    if (model.has_value())
    {
        this->LoadedObject = model.value();
    }
    this->world_position = pos;
}

Mesh::Mesh(const std::vector<MeshData>& _geometry)
{
    this->LoadedObject = _geometry;
}

Point Mesh::GetPos()
{
    return world_position;
}

void Mesh::AddToScene(ShapeList& shapes, std::shared_ptr<Material> mat)
{
    for (int i = 0; i < LoadedObject.size(); i++)
    {
        for (int j = 0; j < LoadedObject[i].indices.size(); j += 3)
        {
            uint32_t i0 = LoadedObject[i].indices[j];
            uint32_t i1 = LoadedObject[i].indices[j + 1];
            uint32_t i2 = LoadedObject[i].indices[j + 2];

            //auto v0 = geometry[i].vertices[i0].position;
            auto v0 = LoadedObject[i].vertices[i0].position;
            auto v1 = LoadedObject[i].vertices[i1].position;
            auto v2 = LoadedObject[i].vertices[i2].position;

            auto n0 = LoadedObject[i].vertices[i0].normal;
            auto n1 = LoadedObject[i].vertices[i1].normal;
            auto n2 = LoadedObject[i].vertices[i2].normal;

            v0 += world_position;
            v1 += world_position;
            v2 += world_position;
            
           shapes.Add(std::make_shared<Triangle>(v0, v1,v2 ,n0, n1, n2, mat));
        }
    }
}

void Mesh::SetWorldPos(const Point& pos)
{
    world_position = pos;
}

bool Triangle::RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)const
{
    static constexpr float kEpsilon = 1e-8;
    
    glm::vec3 e0 = v0 - v2;
    glm::vec3 e1 = v1 - v2;
    glm::vec3 pvec = glm::cross(r.GetDirection(), e1);
    float det = glm::dot(e0, pvec);
    
    if (det < kEpsilon) return false;

    if (fabs(det) < kEpsilon) return false;
    //det = fabs(det);

    float invDet = 1 / det;

    glm::vec3 tvec = r.GetOrigin() - v2;
    auto u = glm::dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return false;

    glm::vec3 qvec = glm::cross(tvec, e0);
    auto v = glm::dot(r.GetDirection(), qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    std::array<glm::vec2, 3>uv{
        glm::vec2(0,0),glm::vec2(1,0),glm::vec2(1,1)
    };
    auto t = glm::dot(e1, qvec) * invDet;

    if (!ray_t.Contains(t))
        return false;
    
    auto uv_sample = u * uv[0] + v * uv[1] + (1 - u - v) * uv[2];
    //hit.u = uv_sample.x;
    //hit.v = uv_sample.y;
    hit.t = t;
    hit.u = u;
    hit.v = v;
    hit.p = r.At(hit.t);
    auto outward_facing_normal = glm::normalize(glm::cross(e0, e1));
    auto interpolated_normal = u * n0 + v * n1 + (1 - u - v) * n2;
    //hit.SetFaceNormal(r, outward_facing_normal);
    //hit.normal = -outward_facing_normal;
    hit.normal = glm::normalize(interpolated_normal);
    //hit.normal = glm::normalize(glm::cross(e0, e1));
    hit.mat = mat;

    return true;
    

    /*glm::vec3 e0 = v2 - v1;
    glm::vec3 e1 = v0 - v2;
    // no need to normalize
    glm::vec3 N = glm::cross(e0, e1);// N
    float denom = glm::dot(N,N);

    // Step 1: finding P

    // check if ray and plane are parallel ?
    float NdotRayDirection = glm::dot(N, r.GetDirection());

    if (fabs(NdotRayDirection) < kEpsilon) // almost 0
       return false; // they are parallel so they don't intersect ! 

    // compute d parameter using equation 2
    float d = glm::dot(-N, v0);

    // compute t (equation 3)
    //t = -(N.dotProduct(orig) + d) / NdotRayDirection;
    t = -(glm::dot(N, r.GetOrigin()) + d) / NdotRayDirection;

    // check if the triangle is in behind the ray
    if (t < 0) return false; // the triangle is behind

    // compute the intersection point using equation 1
    glm::vec3 P = r.GetOrigin()+ t * r.GetDirection();

    // Step 2: inside-outside test
    glm::vec3 C; // vector perpendicular to triangle's plane

    // Calculate u (for triangle BCP)
    glm::vec3 v1p = P - v1;
    C = glm::cross(e0, v1p);
    if ((u = glm::dot(N,C)) < 0) return false; // P is on the right side

    // Calculate v (for triangle CAP)
    glm::vec3 v2p = P - v2;
    C = glm::cross(e1, v2p);
    if ((v = glm::dot(N,C)) < 0) return false; // P is on the right side

    glm::vec3 e2 = v1 - v0;
    glm::vec3 v0p = P - v0;
    C = glm::cross(e2, v0p);
    if (glm::dot(N,C) < 0) return false; // P is on the right side

    u /= denom;
    v /= denom;

    hit.t = t;
    hit.u = u;
    hit.v = v;
    hit.p = r.At(hit.t);
    hit.mat = mat;
    hit.normal = N;
    return true; // this ray hits the triangle
    */

    //Moller trumbore 2
    /*auto e1 = v0 - v1;
    auto e2 = v2 - v0;
    auto n = glm::cross(e1, e2);

    auto c = v0 - r.GetOrigin();
    auto re = glm::cross(r.GetDirection(), c);
    float invDet = 1.0f / glm::dot(n, r.GetDirection());

    float u = glm::dot(re, e2) * invDet;
    float v = glm::dot(re, e1) * invDet;
    float w = 1.0f - u - v;

    // This order of comparisons guarantees that none of u, v, or t, are NaNs:
    // IEEE-754 mandates that they compare to false if the left hand side is a NaN.
    if (u >= 0.0f && v >= 0.0f && u + v <= 1.0f) {
        float t = glm::dot(n, c) * invDet;
        if (t >= 0.0f && t < hit.t) {
            hit.u = u;
            hit.v = v;
            hit.t = t;
            hit.p = r.At(t);
            hit.normal = n;
            // Support two-sided materials by flipping the normal if needed
            if (glm::dot(r.GetDirection(), hit.normal) >= 0.0f)
            {
                hit.normal = -hit.normal;
            }
            return true;
        }
    }
    return false;
    */

}

float Triangle::Area() const
{
    return 0.5f * glm::length(glm::cross(v1 - v0, v2 - v0));
}

double Triangle::PDFValue(const Point& origin, const Point& direction) const
{
    HitRecord rec;
    if (!this->RayHit(Ray(origin, direction), rec, Interval(0.001, infinity)))
        return 0;

    return 1 / Area();
}

glm::vec3 Triangle::Random(const Point& origin)const
{
    return glm::vec3(1, 0, 0);
}
Color Triangle::GetColor()
{
    return color;
}

Point Triangle::GetPos()
{
    return v0;
}

AABB Triangle::GetBoundingBox()const
{
    return BBox;
}