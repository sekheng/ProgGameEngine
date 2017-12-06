#ifndef GT_OBSTACLEMACROS_H
#define GT_OBSTACLEMACROS_H

#include "GT/Common/GTMacros.h"

NS_GT_BEGIN

#define GT_REGISTERCONTACTCALLBACK(_TYPE_) \
void _TYPE_::RegisterContactCallback(EventListenerPhysicsContact& _eventListener) \
{ \
    _eventListener.onContactBegin = CC_CALLBACK_1(_TYPE_::OnContactBegin, this); \
}

NS_GT_END

#endif // GT_OBSTACLEMACROS_H