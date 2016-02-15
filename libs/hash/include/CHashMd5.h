#pragma once

#include <iostream>

class CHashMd5
{
private:
	std::string m_lastKnownHash;
	unsigned char m_lastKnownRawHash[ 20 ];

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CHashMd5();
	~CHashMd5();

	void HashFile( const std::string p_fileName );
	void HashBuffer( const unsigned char *p_buffer, const int p_bufferLength );

	std::string GetHash();
	void GetRawHash( unsigned char p_rawHash[ 20 ] );
};
