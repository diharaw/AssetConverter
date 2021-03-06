#pragma once

#include <common/image.h>
#include <common/mesh.h>
#include <common/material.h>
#include <common/scene.h>

namespace ast
{
bool load_image(const std::string& path, Image& image);
bool load_mesh(const std::string& path, Mesh& mesh);
bool load_material(const std::string& path, Material& material);
bool load_scene(const std::string& path, Scene& scene);
} // namespace ast
