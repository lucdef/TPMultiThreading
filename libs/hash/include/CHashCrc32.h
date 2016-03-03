#pragma once

#include <iostream>

class CHashCrc32
{
private:
	std::string m_lastKnownHash;
	unsigned long m_lastKnownRawHash;
	unsigned long m_array_ulCRC32[ 256 ];

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CHashCrc32();
	CHashCrc32( unsigned long p_polynom );
	~CHashCrc32();

	void HashFile(const std::string p_fileName);
	void HashBuffer(const unsigned char *p_buffer, const int p_bufferLength);

	void GetRawHash( unsigned char p_rawHash[ 8 ] );
	std::string GetHash();
};
