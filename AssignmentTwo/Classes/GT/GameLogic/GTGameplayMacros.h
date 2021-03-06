#ifndef GT_GAMEPLAYMACROS_H
#define GT_GAMEPLAYMACROS_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "../../GT/Common/GTMacros.h"

NS_GT_BEGIN

#define GT_INITIALISECONTACTLISTENER(_TYPE_) \
void InitialiseContactListener() \
{ \
    if (m_ContactListener == NULL) \
    { \
        m_ContactListener = EventListenerPhysicsContact::create(); \
        m_ContactListener->onContactBegin = CC_CALLBACK_1(_TYPE_::OnContactBegin, this); \
        m_ContactListener->retain(); \
        GetScene()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_ContactListener, GetScene()); \
    } \
}

#define GT_DEINITIALISECONTACTLISTENER(_TYPE_) \
void DeinitialiseContactListener() \
{ \
    if (m_ContactListener != NULL) \
    { \
        GetScene()->getEventDispatcher()->removeEventListener(m_ContactListener); \
        m_ContactListener->onContactBegin = NULL; \
        m_ContactListener->release(); \
        m_ContactListener = NULL; \
    } \
}

NS_GT_END

#endif // GT_GAMEPLAYMACROS_H