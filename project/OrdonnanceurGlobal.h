#pragma once
#include "ASingleton.h"
class OrdonnanceurGlobal :public ASingleton<OrdonnanceurGlobal>
{

	friend class ASingleton<OrdonnanceurGlobal>;
public:
	bool GenererChunck(int sizeChunck);
private:
	OrdonnanceurGlobal();
	~OrdonnanceurGlobal();
};

