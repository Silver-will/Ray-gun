#include "Perlin.h"
#include "Common.h"
Perlin::Perlin()
{
    ranVec = new glm::vec3[pointCount];
    for (int i = 0; i < pointCount; ++i) {
        ranVec[i] = glm::normalize(RandomVector(-1, 1));
    }

    permX = PerlinGeneratePerm();
    permY = PerlinGeneratePerm();
    permZ = PerlinGeneratePerm();
}

Perlin::~Perlin()
{
	delete[] ranVec;
	delete[] permX;
	delete[] permY;
	delete[] permZ;
}

double Perlin::Noise(const Point& p) const
{
    auto u = p.x - floor(p.x);
    auto v = p.y - floor(p.y);
    auto w = p.z - floor(p.z);
    auto i = static_cast<int>(floor(p.x));
    auto j = static_cast<int>(floor(p.y));
    auto k = static_cast<int>(floor(p.z));
    glm::vec3 c[2][2][2];

    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] = ranVec[
                    permX[(i + di) & 255] ^
                        permY[(j + dj) & 255] ^
                        permZ[(k + dk) & 255]
                ];

    return PerlinInterp(c, u, v, w);
}

double Perlin::Turb(const Point& p, int depth)const
{
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++) {
        accum += weight * Noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }

    return fabs(accum);
}

int* Perlin::PerlinGeneratePerm()
{
    auto p = new int[pointCount];

    for (int i = 0; i < pointCount; i++)
        p[i] = i;

    Permute(p, pointCount);

    return p;
}

void Perlin::Permute(int* p, int n)
{
    for (int i = n - 1; i > 0; i--) 
    {
        int target = random_int(0, i);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

double Perlin::PerlinInterp(Point c[2][2][2], double u, double v, double w)
{
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) {
                glm::vec3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                    (j * vv + (1 - j) * (1 - vv)) *
                    (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
            }

    return accum;

}