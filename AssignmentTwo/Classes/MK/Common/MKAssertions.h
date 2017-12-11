#ifndef MK_ASSERTIONS_H
#define MK_ASSERTIONS_H

// Include STL
#include <assert.h>
#include <iostream>

// Include MK
#include "MKMacros.h"

#if MK_DEBUG
#define MK_ASSERT(_condition) assert(_condition);
#define MK_ASSERTWITHMSG(_condition, _message) \
	if (!_condition) \
	{ \
		std::cout << _message << std::endl; \
		assert(_condition); \
	}
#else
	#define MK_ASSERT(_condition)
	#define MK_ASSERTWITHMSG(_condition, _message)
#endif // MK_DEBUG

#endif // MK_ASSERTIONS_H