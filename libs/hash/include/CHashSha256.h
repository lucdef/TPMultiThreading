#pragma once

#include <iostream>

class CHashSha256
{
private:
	std::string m_lastKnownHash;
	unsigned char m_lastKnownRawHash[ 32 ];

public:
	static std::string getVersion();
	static std::string getBuildDate();

	CHashSha256();
	~CHashSha256();

	void HashFile( const std::string p_fileName );
	void HashBuffer( const unsigned char *p_buffer, const int p_bufferLength );

	std::string GetHash();
	void GetRawHash( unsigned char p_rawHash[ 32 ] );
};
