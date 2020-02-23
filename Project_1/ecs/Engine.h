#ifndef _HG_Engine_
#define _HG_Engine_

#include <string>
#include <vector>
#include <map>

#include "System.h"

namespace Engine
{
	int Initialize(void);
	int Destroy(void);

	int Run(void);

	int AddSystem(System* system);

}

#endif
