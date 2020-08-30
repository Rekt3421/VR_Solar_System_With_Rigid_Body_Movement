#pragma once

#include <string>
#include "LoadMesh.h"
#include "LoadShader.h"
#include "LoadTexture.h"

namespace LoadAssets
{
   const std::string AssetPath = "assets/";
   const std::string MeshPath = "assets/meshes/";
   const std::string AudioPath = "assets/audio/";
   const std::string ShaderPath = "assets/shaders/";
   const std::string TexturePath = "assets/textures/";

   GLuint LoadShader(const char* vertexShaderFile, const char* fragmentShaderFile);
   GLuint LoadShader(const char* vertexShaderFile, const char* geometryShaderFile, const char* fragmentShaderFile);
   GLuint LoadTexture(const char* fname);
   MeshData LoadMesh(const std::string& pFile);
};
