#ifndef MK_SCENEMACROS_H
#define MK_SCENEMACROS_H

// Include MK
#include "../Common/MKMacros.h"

USING_NS_MK

NS_MK_BEGIN

#define MK_INITIALISEINPUT(_TYPE_) \
void InitialiseInput() \
{ \
	m_ButtonListener = MKInputManager::GetInstance()->CreateEventListener<MKInputButton>(CC_CALLBACK_1(_TYPE_::OnButton, this)); \
	m_ClickListener = MKInputManager::GetInstance()->CreateEventListener<MKInputClick>(CC_CALLBACK_1(_TYPE_::OnClick, this)); \
	m_AxisListener = MKInputManager::GetInstance()->CreateEventListener<MKInputAxis>(CC_CALLBACK_1(_TYPE_::OnAxis, this)); \
}

NS_MK_END

#endif // MK_SCENEMACROS_H