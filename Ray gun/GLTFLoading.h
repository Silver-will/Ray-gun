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
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/parser.hpp>
#include <fastgltf/tools.hpp>

struct Node {
	std::weak_ptr<Node> parent;
	std::vector<std::shared_ptr<Node>> children;
	glm::mat4 localTransform;
	glm::mat4 worldTransform;

	void RefreshTransform(const glm::mat4& parentMatrix);
};

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
	std::vector<Node> nodes;
	BsdfMaterial materialData;
	std::vector <std::shared_ptr<ImageTexture>> images;
	std::vector<GeoSurface> surfaces;
};

std::optional<std::vector<MeshData>>LoadGLTF(const std::string_view filePath);
std::optional<ImageTexture> LoadImageGLTF(fastgltf::Asset& asset, fastgltf::Image& image, const std::string& rootPath);
#endif