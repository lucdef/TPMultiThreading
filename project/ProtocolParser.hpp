#pragma once

#include <string>

class ProtocolParser
{
public:
	enum Action { HELLO, NEWCHUNK, FOUND };

	ProtocolParser();
	~ProtocolParser();
	static std::string CraftResponse(const std::string request);
	
private:
	static int Parse(const std::string& request);
};
