#include "ProtocolParser.hpp"



ProtocolParser::ProtocolParser()
{
}


ProtocolParser::~ProtocolParser()
{
}

int ProtocolParser::Parse(const std::string & request)
{
	if (request == "HELLO-HOW-SHOULD-I-WORK")
	{
		// TODO: stack the response in fifo resp
		//_response = "WORKING-CONTEXT HASH=" + _ordonnanceur->GetHash() + " ALGO=" + _ordonnanceur->GetAlgo() + " ALPHABET=" + _ordonnanceur->GetAlphabet();
	}

	return 0;
}
