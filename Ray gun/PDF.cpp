#include "PDF.h"
#include "Common.h"

double SpherePDF::Value(const glm::vec3& direction)const
{
	return 1 / (4 * Common::pi);
}

glm::vec3 SpherePDF::Generate() const {
	return RandomUnitVector();
}

CosinePDF::CosinePDF(const glm::vec3 w)
{
	uvw.build_from_w(w);
}

double CosinePDF::Value(const glm::vec3& direction)const
{
	auto cosine_theta = glm::dot(glm::normalize(direction), uvw.w());
	return fmax(0, cosine_theta / Common::pi);
}

glm::vec3 CosinePDF::Generate() const
{
	return uvw.transform(RandomCosineDirection());
}

double HittablePDF::Value(const glm::vec3& direction)const
{
	return objects.PDFValue(origin, direction);
}

glm::vec3 HittablePDF::Generate() const
{
	return objects.Random(origin);
}

MixturePDF::MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1) {
	p[0] = p0;
	p[1] = p1;
}

double MixturePDF::Value(const glm::vec3& direction) const {
	return 0.5 * p[0]->Value(direction) + 0.5 * p[1]->Value(direction);
}

glm::vec3 MixturePDF::Generate() const{
	if (random_double() < 0.5)
		return p[0]->Generate();
	else
		return p[1]->Generate();
}