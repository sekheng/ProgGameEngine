#ifndef MK_SCENEMACROS_H
#define MK_SCENEMACROS_H

// Include MK
#include "../Common/MKMacros.h"

USING_NS_MK

NS_MK_BEGIN

#define MK_INITIALISEINPUT(_TYPE_) \
void _TYPE_::InitialiseInput() \
{ \
	m_ButtonListener = MKInputManager::GetInstance()->CreateEventListener<MKInputButton>(CC_CALLBACK_1(_TYPE_::OnButton, this)); \
	m_ClickListener = MKInputManager::GetInstance()->CreateEventListener<MKInputClick>(CC_CALLBACK_1(_TYPE_::OnClick, this)); \
	m_AxisListener = MKInputManager::GetInstance()->CreateEventListener<MKInputAxis>(CC_CALLBACK_1(_TYPE_::OnAxis, this)); \
}

#define MK_DEINITIALISEINPUT(_TYPE_) \
void _TYPE_::DeinitialiseInput() \
{ \
	MKInputManager::GetInstance()->RemoveEventListener(m_ButtonListener); \
	MKInputManager::GetInstance()->RemoveEventListener(m_ClickListener); \
	MKInputManager::GetInstance()->RemoveEventListener(m_AxisListener); \
}

NS_MK_END

#endif // MK_SCENEMACROS_H