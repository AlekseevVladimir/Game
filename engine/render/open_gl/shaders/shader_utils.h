#pragma once
#include <filesystem>
#include <fstream>

std::pair<bool, unsigned int> createShader(const std::string& shaderSrcFileName, int shaderType, unsigned int programID);