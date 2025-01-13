#ifndef GLTF_LOADER
#define GLTF_LOADER
#include <filesystem>
#include <optional>
#include <vector>
#include <memory>
#include <iostream>
#include <vector>
#include "Typedefs.h"
#include "Texture.h"
#include <optional>

struct Vertex {
	glm::vec3 position;
	float uv_x;
	glm::vec3 normal;
	float uv_y;
};
struct GeoSurface {
	uint32_t startIndex;
	uint32_t count;
};

struct BsdfMaterial {
	std::optional<ImageTexture> albedo;
	std::optional<ImageTexture> normal;
	std::optional<ImageTexture> metalRoughAo;
};

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	BsdfMaterial materialData;
	std::vector<ImageTexture> images;
	std::vector<GeoSurface> surfaces;
};

std::optional<std::vector<MeshData>>LoadGLTF(const std::string_view filePath);
#endif