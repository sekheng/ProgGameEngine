// Include Cocos
#include "base/ccMacros.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"

// Include STL
#include <typeinfo>

// Include MK
#include "MKTouchHandler.h"
#include "MKInputManager.h"
#include "../Common/MKAssertions.h"
#include "../Common/MKMathsHelper.h"

#if MK_USE_TOUCH

NS_MK_BEGIN

MKTouchHandler::MKTouchHandler()
{
	m_HeldClicks = new mkS32[MKInputName::NUM_INPUTNAME];
	for (mkS32 i = 0; i < (mkS32)MKInputName::NUM_INPUTNAME; ++i)
	{
		m_HeldClicks[i] = 0;
	}

	// Initialise keyboard listener.
	m_TouchListener = EventListenerTouchAllAtOnce::create();
	m_TouchListener->onTouchesBegan = CC_CALLBACK_2(MKTouchHandler::OnTouchesBegan, this);
	m_TouchListener->onTouchesMoved = CC_CALLBACK_2(MKTouchHandler::OnTouchesMoved, this);
	m_TouchListener->onTouchesEnded = CC_CALLBACK_2(MKTouchHandler::OnTouchesEnded, this); // Treat Cancelled as Ended.
	m_TouchListener->onTouchesCancelled = CC_CALLBACK_2(MKTouchHandler::OnTouchesEnded, this); // Treat Cancelled as Ended.
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_TouchListener, 1);
}

MKTouchHandler::~MKTouchHandler()
{
	delete[] m_HeldClicks;
	Director::getInstance()->getEventDispatcher()->removeEventListener(m_TouchListener);
}

void MKTouchHandler::AddHeldTouch(cocos2d::Touch* _touch)
{
	//CCLOG("Adding Held Touch");
	std::map<cocos2d::Touch*, mkU32>::iterator mapIter = m_HeldTouches.find(_touch);
	if (mapIter == m_HeldTouches.end())
	{
		_touch->retain();
		m_HeldTouches.insert(std::pair<cocos2d::Touch*, mkU32>(_touch, 1));
	}
	else
	{
		mapIter->second += 1;
	}
}

void MKTouchHandler::RemoveHeldTouch(cocos2d::Touch* _touch)
{
	//CCLOG("Removing Held Touch");
	std::map<cocos2d::Touch*, mkU32>::iterator mapIter = m_HeldTouches.find(_touch);
	if (mapIter == m_HeldTouches.end()) { return; }
	MK_ASSERTWITHMSG((mapIter != m_HeldTouches.end()), "MKKeyboardHandler::RemoveHeldTouch - Key not found in m_HeldKeys!");
	MK_ASSERTWITHMSG((mapIter->second != 0), "MKKeyboardHandler::RemoveHeldTouch - Invalid value for counter of _touch!");

	mapIter->second -= 1;
	if (mapIter->second == 0)
	{
		mapIter->first->release();
		m_HeldTouches.erase(mapIter);
	}
}

unordered_set<MK_INPUTNAME> MKTouchHandler::GetValidClicks(mkU64 _mask)
{
	unordered_set<MK_INPUTNAME> result;
	for (unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::const_iterator i = m_RegisteredClicks.begin(); i != m_RegisteredClicks.end(); ++i)
	{
		if (!MKInputManager::CompareMask(_mask, i->first))
		{
			continue;
		}
		for (unordered_set<MK_INPUTNAME>::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			result.insert(*j);
		}
	}

	return result;
}

unordered_set<MK_INPUTNAME> MKTouchHandler::GetValidAxis(mkU64 _mask)
{
	unordered_set<MK_INPUTNAME> result;
	for (unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::const_iterator i = m_RegisteredAxis.begin(); i != m_RegisteredAxis.end(); ++i)
	{
		if (!MKInputManager::CompareMask(_mask, i->first))
		{
			continue;
		}
		for (unordered_set<MK_INPUTNAME>::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			result.insert(*j);
		}
	}

	return result;
}

void MKTouchHandler::SendClickHeldEvents()
{
	MKInputContext currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	for (mkS32 i = 0; i < (mkS32)MKInputName::NUM_INPUTNAME; ++i)
	{
		MK_ASSERTWITHMSG((m_HeldClicks[i] >= 0), "MKTouchHandler::SendClickHeldEvents - Held Buttons counter should never be less than 0.");
		if (m_HeldClicks[i] > 0)
		{
			// Find a way to get cursor position.
			std::unordered_map<MK_INPUTNAME, MKCursorPosition>::iterator cursorIter = m_CursorPositions.find(static_cast<MKInputName>(i));
			MK_ASSERTWITHMSG((cursorIter != m_CursorPositions.end()), "MKTouchHandler::SendClickHeldEvents - Cursor Position not found!");
			
			MKInputClick* click = new MKInputClick(static_cast<MKInputName>(i), currentContext, MKInputButton::HOLD, cursorIter->second);
			MKInputManager::GetInstance()->AddInput<MKInputClick>(click);
		}
	}
}

void MKTouchHandler::ResetHeldClicks()
{
	for (mkS32 i = 0; i < (mkS32)MKInputName::NUM_INPUTNAME; ++i)
	{
		m_HeldClicks[i] = 0;
	}
}

void MKTouchHandler::ResetCursorPositions()
{
	m_CursorPositions.clear();
}

void MKTouchHandler::PreStateChange()
{
	//CCLOG("PreStateChange");
	for (std::map<cocos2d::Touch*, mkU32>::iterator i = m_HeldTouches.begin(); i != m_HeldTouches.end(); ++i)
	{
		for (mkU32 j = 0; j < i->second; ++j)
		{
			HandleTouchEnded(i->first);
		}
	}
}

void MKTouchHandler::PostStateChange()
{
	//CCLOG("PostStateChange");
	for (std::map<cocos2d::Touch*, mkU32>::iterator i = m_HeldTouches.begin(); i != m_HeldTouches.end(); ++i)
	{
		for (mkU32 j = 0; j < i->second; ++j)
		{
			HandleTouchBegan(i->first);
		}
	}
}

void MKTouchHandler::PreContextChange(MKPasskey<MKInputManager> _key)
{
	PreStateChange();
}

void MKTouchHandler::PostContextChange(MKPasskey<MKInputManager> _key)
{
	PostStateChange();
}

void MKTouchHandler::Update(MKPasskey<MKInputManager> _key)
{
	SendClickHeldEvents();
}

void MKTouchHandler::RegisterAxis(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName)
{
	unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::iterator mapIter = m_RegisteredAxis.find(_mask);
	if (mapIter == m_RegisteredAxis.end())
	{
		unordered_set<MK_INPUTNAME> inputNames;
		inputNames.insert(_inputName);
		m_RegisteredAxis.insert(std::pair<mkU64, unordered_set<MK_INPUTNAME> >(_mask, inputNames));
	}
	else
	{
#if MK_DEBUG
		mkString assertMessage = "MKTouchHandler::RegisterAxis - A InputName was registered twice with the same mask!";
		MK_ASSERTWITHMSG((mapIter->second.find(_inputName) == mapIter->second.end()), assertMessage.c_str());
#endif // MK_DEBUG

		mapIter->second.insert(_inputName);
	}
}

void MKTouchHandler::UnregisterAxis(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName)
{
	unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::iterator mapIter = m_RegisteredAxis.find(_mask);

#if MK_DEBUG
	{
		mkString assertMessage = "MKTouchHandler::UnregisterAxis - There are no the specified mask is not registered!";
		MK_ASSERTWITHMSG((mapIter != m_RegisteredAxis.end()), assertMessage.c_str());
	}
#endif // MK_DEBUG

	unordered_set<MK_INPUTNAME>::iterator setIter = mapIter->second.find(_inputName);

#if MK_DEBUG
	{
		mkString assertMessage = "MKTouchHandler::UnregisterAxis - There are no InputNames registered using the specified mask!";
		MK_ASSERTWITHMSG((setIter != mapIter->second.end()), assertMessage.c_str());
	}
#endif // MK_DEBUG

	mapIter->second.erase(setIter);
	if (mapIter->second.empty())
	{
		m_RegisteredAxis.erase(mapIter);
	}
}

void MKTouchHandler::RegisterClick(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName)
{
	PreStateChange();

	unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::iterator mapIter = m_RegisteredClicks.find(_mask);
	if (mapIter == m_RegisteredClicks.end())
	{
		unordered_set<MK_INPUTNAME> inputNames;
		inputNames.insert(_inputName);
		m_RegisteredClicks.insert(std::pair<mkU64, unordered_set<MK_INPUTNAME> >(_mask, inputNames));
	}
	else
	{
#if MK_DEBUG
		mkString assertMessage = "MKTouchHandler::RegisterClick - A InputName was registered twice with the same mask!";
		MK_ASSERTWITHMSG((mapIter->second.find(_inputName) == mapIter->second.end()), assertMessage.c_str());
#endif // MK_DEBUG

		mapIter->second.insert(_inputName);
	}

	PostStateChange();
}

void MKTouchHandler::UnregisterClick(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName)
{
	PreStateChange();

	unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::iterator mapIter = m_RegisteredClicks.find(_mask);

#if MK_DEBUG
	{
		mkString assertMessage = "MKTouchHandler::UnregisterClick - There are no the specified mask is not registered!";
		MK_ASSERTWITHMSG((mapIter != m_RegisteredClicks.end()), assertMessage.c_str());
	}
#endif // MK_DEBUG

	unordered_set<MK_INPUTNAME>::iterator setIter = mapIter->second.find(_inputName);

#if MK_DEBUG
	{
		mkString assertMessage = "MKTouchHandler::UnregisterClick - There are no InputNames registered using the specified mask!";
		MK_ASSERTWITHMSG((setIter != mapIter->second.end()), assertMessage.c_str());
	}
#endif // MK_DEBUG

	mapIter->second.erase(setIter);
	if (mapIter->second.empty())
	{
		m_RegisteredClicks.erase(mapIter);
	}

	PostStateChange();
}

void MKTouchHandler::HandleTouchBegan(cocos2d::Touch* _touch)
{
	Vec2 touchLocation = _touch->getLocation();
	MKCursorPosition cursorPosition(touchLocation.x, touchLocation.y);

	MKInputContext currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	mkU16 controllerMaskIndexMask = 0x0001 << _touch->getID();
	mkU64 mask = MKInputManager::GenerateMask((mkU16)currentContext, controllerMaskIndexMask, 0);

	std::unordered_set<MK_INPUTNAME> inputNames = GetValidClicks(mask);
	for (std::unordered_set<MK_INPUTNAME>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
	{
		++m_HeldClicks[*j];

		MKInputClick* click = new MKInputClick(static_cast<MKInputName>(*j), currentContext, MKInputButton::PRESS, cursorPosition);
		MKInputManager::GetInstance()->AddInput<MKInputClick>(click);

		std::unordered_map<MK_INPUTNAME, MKCursorPosition>::iterator cursorIter = m_CursorPositions.find(*j);
		if (cursorIter == m_CursorPositions.end())
		{
			m_CursorPositions.insert(std::pair<MK_INPUTNAME, MKCursorPosition>(*j, cursorPosition));
		}
		else
		{
			cursorIter->second = cursorPosition;
		}
	}
}

void MKTouchHandler::HandleTouchMoved(cocos2d::Touch* _touch)
{
	Vec2 touchDelta = _touch->getDelta();
	MKInputContext currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	mkU16 controllerMaskIndexMask = 0x0001 << _touch->getID();
    mkF32 screenHeight = Director::getInstance()->getVisibleSize().height;

	// Horizontal
	{
		float horizontalValue = touchDelta.x / screenHeight;
		mkU64 mask = MKInputManager::GenerateMask((mkU16)currentContext, controllerMaskIndexMask, MKInputAxis::KeyCode::HORIZONTAL);
		std::unordered_set<MK_INPUTNAME> inputNames = GetValidAxis(mask);

		for (std::unordered_set<MK_INPUTNAME>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
		{
			// Horizontal
			MKInputAxis* axis = new MKInputAxis(static_cast<MKInputName>(*j), currentContext, horizontalValue);
			MKInputManager::GetInstance()->AddInput<MKInputAxis>(axis);
		}
	}

	// Vertical
	{
		float verticalValue = touchDelta.y / screenHeight;
		mkU64 mask = MKInputManager::GenerateMask((mkU16)currentContext, controllerMaskIndexMask, MKInputAxis::KeyCode::VERTICAL);
		std::unordered_set<MK_INPUTNAME> inputNames = GetValidAxis(mask);

		for (std::unordered_set<MK_INPUTNAME>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
		{
			// Horizontal
			MKInputAxis* axis = new MKInputAxis(static_cast<MKInputName>(*j), currentContext, verticalValue);
			MKInputManager::GetInstance()->AddInput<MKInputAxis>(axis);
		}
	}

	// Update CursorPosition
	{
		Vec2 touchLocation = _touch->getLocation();
		MKCursorPosition cursorPosition(touchLocation.x, touchLocation.y);
		mkU64 mask = MKInputManager::GenerateMask(currentContext, controllerMaskIndexMask, 0);
		std::unordered_set<MK_INPUTNAME> inputNames = GetValidClicks(mask);

		for (std::unordered_set<MK_INPUTNAME>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
		{
			std::unordered_map<MK_INPUTNAME, MKCursorPosition>::iterator cursorIter = m_CursorPositions.find(*j);
			if (cursorIter != m_CursorPositions.end())
			{
				cursorIter->second = cursorPosition;
			}
		}
	}
}

void MKTouchHandler::HandleTouchEnded(cocos2d::Touch* _touch)
{
	Vec2 touchLocation = _touch->getLocation();
	MKCursorPosition cursorPosition(touchLocation.x, touchLocation.y);

	MKInputContext currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	mkU16 controllerMaskIndexMask = (0x0001 << _touch->getID());
	mkU64 mask = MKInputManager::GenerateMask((mkU16)currentContext, controllerMaskIndexMask, 0);

	std::unordered_set<MK_INPUTNAME> inputNames = GetValidClicks(mask);
	for (std::unordered_set<MK_INPUTNAME>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
	{
		mkS32 oldValue = m_HeldClicks[*j];
		m_HeldClicks[*j] = MKMathsHelper::Max<mkS32>(0, m_HeldClicks[*j] - 1);

		if (m_HeldClicks[*j] == 0 && oldValue > 0)
		{
			MKInputClick* click = new MKInputClick(static_cast<MKInputName>(*j), currentContext, MKInputButton::RELEASE, cursorPosition);
			MKInputManager::GetInstance()->AddInput<MKInputClick>(click);

			std::unordered_map<MK_INPUTNAME, MKCursorPosition>::iterator cursorIter = m_CursorPositions.find(*j);
			if (cursorIter != m_CursorPositions.end())
			{
				m_CursorPositions.erase(cursorIter);
			}
		}
	}
}

// Callbacks
void MKTouchHandler::OnTouchesBegan(const std::vector<Touch*>& _touches, Event* _event)
{
	//CCLOG("On Touch Began");
	for (std::vector<Touch*>::const_iterator i = _touches.begin(); i != _touches.end(); ++i)
	{
		int id = (*i)->getID();
		//CCLOG(std::to_string(id).c_str());
		AddHeldTouch(*i);
		HandleTouchBegan(*i);
	}
}

void MKTouchHandler::OnTouchesEnded(const std::vector<Touch*>& _touches, Event* _event)
{
	//CCLOG("On Touch Ended");
	for (std::vector<Touch*>::const_iterator i = _touches.begin(); i != _touches.end(); ++i)
	{
		int id = (*i)->getID();
		//CCLOG(std::to_string(id).c_str());
		RemoveHeldTouch(*i);
		HandleTouchEnded(*i);
	}
}

void MKTouchHandler::OnTouchesMoved(const std::vector<Touch*>& _touches, Event* _event)
{
	for (std::vector<Touch*>::const_iterator i = _touches.begin(); i != _touches.end(); ++i)
	{
		HandleTouchMoved(*i);
	}
}

NS_MK_END

#endif // MK_USE_TOUCH