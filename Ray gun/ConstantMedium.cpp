#include "ConstantMedium.h"
#include "Material.h"

ConstantMedium::ConstantMedium(std::shared_ptr<Shape> b, double d, std::shared_ptr<Texture> a)
	: boundary{b},negInvDensity{-1/d},phaseFunction{std::make_shared<Isotropic>(a)}
{

}

ConstantMedium::ConstantMedium(std::shared_ptr<Shape> b, double d, Color c)
	: boundary{ b }, negInvDensity{ -1 / d }, phaseFunction{ std::make_shared<Isotropic>(c) }
{

}

AABB ConstantMedium::GetBoundingBox() const
{
    return boundary->GetBoundingBox();
}

bool ConstantMedium::RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)const
{
    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001;

    HitRecord rec1, rec2;

    if (!boundary->RayHit(r, rec1,Interval::universe))
        return false;

    if (!boundary->RayHit(r, rec2, Interval(rec1.t + 0.0001, infinity)))
        return false;

    if (debugging) std::clog << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

    if (rec1.t < ray_t.min) rec1.t = ray_t.min;
    if (rec2.t > ray_t.max) rec2.t = ray_t.max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    auto ray_length = r.GetDirection().length();
    auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    auto hit_distance = negInvDensity * log(random_double());

    if (hit_distance > distance_inside_boundary)
        return false;

    hit.t = rec1.t + hit_distance / ray_length;
    hit.p = r.At(hit.t);

    if (debugging) {
        std::clog << "hit_distance = " << hit_distance << '\n'
            << "rec.t = " << hit.t << '\n'
            << "rec.p = " << hit.p.x << " "<<hit.p.y<< " "<<hit.p.z<<'\n';
    }

    hit.normal = glm::vec3(1, 0, 0);  // arbitrary
    hit.frontFace = true;     // also arbitrary
    hit.mat = phaseFunction;

    return true;
}