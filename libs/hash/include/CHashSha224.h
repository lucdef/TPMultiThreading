#pragma once

#include <iostream>

class CHashSha224
{
private:
	std::string m_lastKnownHash;
	unsigned char m_lastKnownRawHash[ 28 ];

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CHashSha224();
	~CHashSha224();

	void HashFile( const std::string p_fileName );
	void HashBuffer( const unsigned char *p_buffer, const int p_bufferLength );

	std::string GetHash();
	void GetRawHash( unsigned char p_rawHash[ 28 ] );
};
