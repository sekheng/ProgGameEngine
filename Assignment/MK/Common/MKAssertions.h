#ifndef MK_ASSERTIONS_H
#define MK_ASSERTIONS_H

#include <assert.h>
#include <iostream>

#define MK_ASSERT(_condition) assert(_condition);

#define MK_ASSERT(_condition, _message) \
	if (!_condition) \
	{ \
		std::cout << _message << std::endl; \
		assert(_condition); \
	}

#endif