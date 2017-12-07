#ifndef GT_OBSTACLEMACROS_H
#define GT_OBSTACLEMACROS_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "GT/Common/GTMacros.h"

NS_GT_BEGIN

#define GT_INITIALISECONTACTLISTENER(_TYPE_) \
void _TYPE_::InitialiseContactListener() \
{ \
    m_ContactListener = EventListenerPhysicsContact::create(); \
    m_ContactListener->onContactBegin = CC_CALLBACK_1(_TYPE_::OnContactBegin, this); \
    m_ContactListener->retain(); \
    GetScene()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_ContactListener, GetScene()); \
}

#define GT_DEINITIALISECONTACTLISTENER(_TYPE_) \
void _TYPE_::UninitialiseContactListener() \
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

#endif // GT_OBSTACLEMACROS_H