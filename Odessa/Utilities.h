#pragma once

#include "pch.h"

inline std::vector<char> Read(std::string File)
{
	std::ifstream read(File.c_str(), std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = read.tellg();

	std::vector<char> buff(pos);
	read.seekg(0, std::ios::beg);
	read.read(buff.data(), pos);

	return buff;
}
