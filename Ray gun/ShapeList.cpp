#include "ShapeList.h"

ShapeList::ShapeList(std::shared_ptr<Shape> object)
{
	Add(object);
}

void ShapeList::Add(std::shared_ptr<Shape> object)
{
	objects.push_back(object);
	Bbox = AABB(Bbox, object->GetBoundingBox());
}

bool ShapeList::RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)const
{
    HitRecord temp_rec;
    auto hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto& object : objects) {
        if (object->RayHit(r, temp_rec, Interval(ray_t.min, closest_so_far))) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            hit = temp_rec;
        }
    }

    return hit_anything;
}

void ShapeList::Clear()
{
    objects.clear();
}

AABB ShapeList::GetBoundingBox() const
{
    return Bbox;
}

double ShapeList::PDFValue(const Point& origin, const Point& direction) const
{
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;
    for (const auto& object : objects)
        sum += weight * object->PDFValue(origin, direction);
    return sum;

}

glm::vec3 ShapeList::Random(const Point& origin)const
{
    auto int_size = static_cast<int>(objects.size());
    return objects[random_int(0, int_size - 1)]->Random(origin);
}