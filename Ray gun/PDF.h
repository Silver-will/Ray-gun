#ifndef PDF_H
#define PDF_H
#include <glm/glm.hpp>
#include "ONB.h"
#include "ShapeList.h"
struct PDF
{
	virtual ~PDF() {}

	virtual double Value(const glm::vec3& direction) const = 0;
	virtual glm::vec3 Generate() const = 0;
};



struct SpherePDF : public PDF {
	SpherePDF() {}
	double Value(const glm::vec3& direction)const override;
	glm::vec3 Generate() const override;
};

struct CosinePDF : public PDF {
	CosinePDF(const glm::vec3 w);

	double Value(const glm::vec3& direction)const override;
	glm::vec3 Generate() const override;

private:
	ONB uvw;
};

struct HittablePDF : public PDF {
	HittablePDF(const Shape& _objects, const Point& _origin) :
		objects{ _objects },origin{origin} { }
	double Value(const glm::vec3& direction)const override;
	glm::vec3 Generate() const override;
private:
	const Shape& objects;
	Point origin;
};

struct MixturePDF : public PDF {
	MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1);

	double Value(const glm::vec3& direction)const override;
	glm::vec3 Generate() const override;

private:
	std::shared_ptr<PDF> p[2];
};
#endif // !PDF_H

