#include "LoadAssets.h"

GLuint LoadAssets::LoadShader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
   return ::LoadShader(std::string(ShaderPath+vertexShaderFile).c_str(), std::string(ShaderPath+fragmentShaderFile).c_str());
}

GLuint LoadAssets::LoadShader(const char* vertexShaderFile, const char* geometryShaderFile, const char* fragmentShaderFile)
{
   return ::LoadShader(std::string(ShaderPath + vertexShaderFile).c_str(), std::string(ShaderPath + geometryShaderFile).c_str(), std::string(ShaderPath + fragmentShaderFile).c_str());
}

GLuint LoadAssets::LoadTexture(const char* fname)
{
   return ::LoadTexture(std::string(TexturePath+fname).c_str());
}

MeshData LoadAssets::LoadMesh(const std::string& pFile)
{
   return ::LoadMesh(MeshPath+pFile);
}