#pragma once
#include "PasswordChunk.h"
static class Agent
{
public:
	
	static void GenerationPassword();
	static CPasswordChunk GetChunk();
	static bool ComparerPassword();
	static std::string HashPassword();
	
};

