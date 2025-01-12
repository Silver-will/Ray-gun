#ifndef QUAD_H
#define QUAD_H
#include "ShapeList.h"
#include "Typedefs.h"
#include <glm/glm.hpp>
struct Quad : public Shape
{
	Quad(const Point& _Q, const glm::vec3& _u, const glm::vec3& _v, std::shared_ptr<Material> m);
	virtual void SetBoundingBox();
	AABB GetBoundingBox()const override;
	bool RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)const override;
	virtual bool IsInterior(double a, double b, HitRecord& rec) const;
    double PDFValue(const Point& origin, const Point& direction) const override;
    glm::vec3 Random(const Point& origin)const override;

private:
	Point Q;
	glm::vec3 u, v;
	std::shared_ptr<Material> mat;
	AABB BBox;
	glm::vec3 normal;
	double d;
    double area;
	glm::vec3 w;
};

inline std::shared_ptr<ShapeList> Box(const Point& a, const Point& b, std::shared_ptr<Material> mat)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = std::make_shared<ShapeList>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = Point(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    auto max = Point(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    auto dx = glm::vec3(max.x - min.x, 0, 0);
    auto dy = glm::vec3(0, max.y - min.y, 0);
    auto dz = glm::vec3(0, 0, max.z - min.z);

    sides->Add(make_shared<Quad>(Point(min.x, min.y, max.z), dx, dy, mat)); // front
    sides->Add(make_shared<Quad>(Point(max.x, min.y, max.z), -dz, dy, mat)); // right
    sides->Add(make_shared<Quad>(Point(max.x, min.y, min.z), -dx, dy, mat)); // back
    sides->Add(make_shared<Quad>(Point(min.x, min.y, min.z), dz, dy, mat)); // left
    sides->Add(make_shared<Quad>(Point(min.x, max.y, max.z), dx, -dz, mat)); // top
    sides->Add(make_shared<Quad>(Point(min.x, min.y, min.z), dx, dz, mat)); // bottom

    return sides;
}
#endif

