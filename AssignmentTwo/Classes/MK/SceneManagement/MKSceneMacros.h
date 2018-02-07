#ifndef MK_SCENEMACROS_H
#define MK_SCENEMACROS_H

// Include MK
#include "../Common/MKMacros.h"

USING_NS_MK

NS_MK_BEGIN

#define MK_INITIALISEINPUT(_TYPE_) \
void InitialiseInput() \
{ \
    if (m_ButtonListener == NULL) \
    { \
        m_ButtonListener = MKInputManager::GetInstance()->CreateEventListener<MKInputButton>(CC_CALLBACK_1(_TYPE_::OnButton, this)); \
    } \
\
    if (m_ClickListener == NULL) \
    { \
        m_ClickListener = MKInputManager::GetInstance()->CreateEventListener<MKInputClick>(CC_CALLBACK_1(_TYPE_::OnClick, this)); \
    } \
\
    if (m_AxisListener == NULL) \
    { \
        m_AxisListener = MKInputManager::GetInstance()->CreateEventListener<MKInputAxis>(CC_CALLBACK_1(_TYPE_::OnAxis, this)); \
    } \
}

#define MK_DEINITIALISEINPUT(_TYPE_) \
void DeinitialiseInput() \
{ \
    if (m_ButtonListener != NULL) \
    { \
        MKInputManager::GetInstance()->RemoveEventListener(m_ButtonListener); \
        m_ButtonListener = NULL; \
    } \
\
    if (m_ClickListener != NULL) \
    { \
        MKInputManager::GetInstance()->RemoveEventListener(m_ClickListener); \
        m_ClickListener = NULL; \
    } \
\
    if (m_AxisListener != NULL) \
    { \
        MKInputManager::GetInstance()->RemoveEventListener(m_AxisListener); \
        m_AxisListener = NULL; \
	} \
}

NS_MK_END

#endif // MK_SCENEMACROS_H