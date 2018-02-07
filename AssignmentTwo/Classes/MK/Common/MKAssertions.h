#ifndef MK_ASSERTIONS_H
#define MK_ASSERTIONS_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MKMacros.h"

#if MK_DEBUG
#define MK_ASSERT(_condition) assert(_condition);
#define MK_ASSERTWITHMSG(_condition, _message) \
	if (!_condition) \
	{ \
        CCLOG(_message); \
		CC_ASSERT(_condition); \
	}
#else
	#define MK_ASSERT(_condition)
	#define MK_ASSERTWITHMSG(_condition, _message)
#endif // MK_DEBUG

#endif // MK_ASSERTIONS_H