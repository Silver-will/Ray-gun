#include "Polygun.h"
#include "Material.h"
#include "GLTFLoading.h"
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Polygun::Polygun(std::string_view model_path, Point pos)
{
    auto model = LoadGLTF(model_path);
    if (model.has_value())
    {
        this->geometry = model.value();
    }
    this->worldPos = pos;
}

Polygun::Polygun(const std::vector<MeshData>& _geometry)
{
    this->geometry = _geometry;
}

Point Polygun::GetPos()
{
    return worldPos;
}

void Polygun::AddToScene(ShapeList& shapes)
{
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));

    for (int i = 0; i < geometry.size(); i++)
    {
        for (int j = 0; j < geometry[i].indices.size(); j+=3)
        {
            uint32_t i0 = geometry[i].indices[j];
            uint32_t i1 = geometry[i].indices[j + 1];
            uint32_t i2 = geometry[i].indices[j + 2];

            auto v0 = geometry[i].vertices[i0].position;
            auto v1 = geometry[i].vertices[i1].position;
            auto v2 = geometry[i].vertices[i2].position;


            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -10.0f));

            glm::vec4 e0 = transform * glm::vec4(v0, 1.0f);
            glm::vec4 e1 = transform * glm::vec4(v1, 1.0f);
            glm::vec4 e2 = transform * glm::vec4(v2, 1.0f);

            //v0 = glm::vec3(e0);
            //v1 = glm::vec3(e1);
            //v2 = glm::vec3(e2);

            //std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(v0.position, v1.position, v2.position, green);
            shapes.Add(std::make_shared<Triangle>(v0, v1, v2, green));

            //auto offset = Point(265, 0, 295);
            //auto offset2 = offset * 1.5f;
            //shapes.Add(triangle);
        }
    }
    auto stuff = shapes.objects.size();
    stuff += 2;
}

Triangle::Triangle(const Triangle& tri)
{
    this->v0 = tri.v0;
    this->v1 = tri.v1;
    this->v2 = tri.v2;

    this->BBox = tri.BBox;
    this->mat = tri.mat;
}

void Triangle::Translate(const glm::vec3& direction)
{
    v0 -= direction;
    v1 -= direction;
    v2 -= direction;
}

void Triangle::Scale(const glm::vec3& scale)
{
    v0 = Point(v0[0] * scale[0], v0[1] * scale[1], v0[2] * scale[2]);
}
bool Triangle::RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)
{
    /*static constexpr float kEpsilon = 1e-8;
    glm::vec3 e0 = v0 - v2;
    glm::vec3 e1 = v1 - v2;
    glm::vec3 pvec = glm::cross(r.GetDirection(), e1);
    float det = glm::dot(e0,pvec);

    
    Culling
     if (det < kEpsilon) return false;
    
    if (fabs(det) < kEpsilon) return false;

    float invDet = 1 / det;

    glm::vec3 tvec = r.GetOrigin() - v2;
    u = glm::dot(tvec,pvec) * invDet;
    if (u < 0 || u > 1) return false;

    glm::vec3 qvec = glm::cross(tvec, e0);
    v = glm::dot(r.GetDirection(), qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    std::array<glm::vec2, 3>uv{
        glm::vec2(0,0),glm::vec2(1,0),glm::vec2(1,1)
    };
    t = glm::dot(e1, qvec) * invDet;
    hit.t = t;
    auto uv_sample = u * uv[0] + v * uv[1] + (1-u-v) * uv[2];
    //hit.u = uv_sample.x;
    //hit.v = uv_sample.y;
    hit.u = u;
    hit.v = v;
    //auto outward_facing_normal = glm::cross();
    hit.p = r.At(hit.t);
    hit.normal = glm::normalize(glm::cross(v1-v0,v2-v0));
    hit.mat = mat;

    return true;
    */

    static constexpr float kEpsilon = 1e-8;
    glm::vec3 e0 = v1 - v0;
    glm::vec3 e1 = v2 - v0;
    glm::vec3 pvec = glm::cross(r.GetDirection(), e1);
    float det = glm::dot(e0, pvec);

    /*
    Culling
     if (det < kEpsilon) return false;
    */
    if (fabs(det) < kEpsilon) return false;

    float invDet = 1 / det;

    glm::vec3 tvec = r.GetOrigin() - v0;
    u = glm::dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return false;

    glm::vec3 qvec = glm::cross(tvec, e0);
    v = glm::dot(r.GetDirection(), qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    std::array<glm::vec2, 3>uv{
        glm::vec2(0,0),glm::vec2(1,0),glm::vec2(1,1)
    };
    t = glm::dot(e1, qvec) * invDet;
    
    auto uv_sample = u * uv[0] + v * uv[1] + (1 - u - v) * uv[2];
    //hit.u = uv_sample.x;
    //hit.v = uv_sample.y;
    hit.t = t;
    hit.u = u;
    hit.v = v;
    //auto outward_facing_normal = glm::cross();
    hit.p = r.At(hit.t);
    auto outward_facing_normal = glm::cross(e0, e1);
    hit.SetFaceNormal(r, outward_facing_normal);
    //hit.normal = glm::normalize(glm::cross(e0, e1));
    hit.mat = mat;

    return true;


    /*glm::vec3 e0 = v2 - v1;
    glm::vec3 e1 = v0 - v2;
    // no need to normalize
    glm::vec3 N = glm::cross(e0, e1);// N
    float denom = glm::dot(N, N);

    // Step 1: finding P

    // check if ray and plane are parallel ?
    float NdotRayDirection = glm::dot(N, r.GetDirection());

    if (fabs(NdotRayDirection) < kEpsilon) // almost 0
        return false; // they are parallel so they don't intersect ! 

    // compute d parameter using equation 2
    float d = -glm::dot(N, v0);

    // compute t (equation 3)
    t = -(N.dotProduct(orig) + d) / NdotRayDirection;

    // check if the triangle is in behind the ray
    if (t < 0) return false; // the triangle is behind

    // compute the intersection point using equation 1
    glm::vec3 P = orig + t * dir;

    // Step 2: inside-outside test
    glm::vec3 C; // vector perpendicular to triangle's plane

    // Calculate u (for triangle BCP)
    glm::vec3 v1p = P - v1;
    C = e0.crossProduct(v1p);
    if ((u = N.dotProduct(C)) < 0) return false; // P is on the right side

    // Calculate v (for triangle CAP)
    glm::vec3 v2p = P - v2;
    C = e1.crossProduct(v2p);
    if ((v = N.dotProduct(C)) < 0) return false; // P is on the right side

    glm::vec3 e2 = v1 - v0;
    glm::vec3 v0p = P - v0;
    C = e2.crossProduct(v0p);
    if (N.dotProduct(C) < 0) return false; // P is on the right side

    u /= denom;
    v /= denom;

    return true; // this ray hits the triangle
    */

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