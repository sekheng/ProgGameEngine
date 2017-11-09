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
}

unordered_set<mkInputNameU32> MKTouchHandler::GetValidClicks(mkU64 _mask)
{
	unordered_set<mkInputNameU32> result;
	for (unordered_map<mkU64, unordered_set<mkInputNameU32> >::const_iterator i = m_RegisteredClicks.begin(); i != m_RegisteredClicks.end(); ++i)
	{
		if (!MKInputManager::CompareMask(_mask, i->first))
		{
			continue;
		}
		for (unordered_set<mkInputNameU32>::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			result.insert(*j);
		}
	}

	return result;
}

unordered_set<mkInputNameU32> MKTouchHandler::GetValidAxis(mkU64 _mask)
{
	unordered_set<mkInputNameU32> result;
	for (unordered_map<mkU64, unordered_set<mkInputNameU32> >::const_iterator i = m_RegisteredAxis.begin(); i != m_RegisteredAxis.end(); ++i)
	{
		if (!MKInputManager::CompareMask(_mask, i->first))
		{
			continue;
		}
		for (unordered_set<mkInputNameU32>::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			result.insert(*j);
		}
	}

	return result;
}

void MKTouchHandler::SendClickHeldEvents()
{
	mkU16 currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	for (mkS32 i = 0; i < (mkS32)MKInputName::NUM_INPUTNAME; ++i)
	{
		MK_ASSERTWITHMSG((m_HeldClicks[i] >= 0), "MKTouchHandler::SendClickHeldEvents - Held Buttons counter should never be less than 0.");
		if (m_HeldClicks[i] > 0)
		{
			// Find a way to get cursor position.
			std::unordered_map<mkInputNameU32, MKCursorPosition>::iterator cursorIter = m_CursorPositions.find(static_cast<MKInputName>(i));
			MK_ASSERTWITHMSG((cursorIter != m_CursorPositions.end()), "MKTouchHandler::SendClickHeldEvents - Cursor Position not found!");
			
			MKInputClick* click = new MKInputClick(static_cast<MKInputName>(i), MKInputButton::HOLD, cursorIter->second);
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

void MKTouchHandler::OnContextChange(MKPasskey<MKInputManager> _key)
{
	ResetHeldClicks();
	ResetCursorPositions();
}

void MKTouchHandler::Update(MKPasskey<MKInputManager> _key)
{
	SendClickHeldEvents();
}

void MKTouchHandler::RegisterAxis(mkU64 _mask, MKInputName _inputName)
{
	unordered_map<mkU64, unordered_set<mkInputNameU32> >::iterator mapIter = m_RegisteredAxis.find(_mask);
	if (mapIter == m_RegisteredAxis.end())
	{
		unordered_set<mkInputNameU32> inputNames;
		inputNames.insert(_inputName);
		m_RegisteredAxis.insert(std::pair<mkU64, unordered_set<mkInputNameU32> >(_mask, inputNames));
	}
	else
	{
#if MK_DEBUG
		std::string assertMessage = "MKTouchHandler::RegisterAxis - A InputName was registered twice with the same mask!";
		MK_ASSERTWITHMSG((mapIter->second.find(_inputName) == mapIter->second.end()), assertMessage);
#endif // MK_DEBUG

		mapIter->second.insert(_inputName);
	}
}

void MKTouchHandler::UnregisterAxis(mkU64 _mask, MKInputName _inputName)
{
	unordered_map<mkU64, unordered_set<mkInputNameU32> >::iterator mapIter = m_RegisteredAxis.find(_mask);

#if MK_DEBUG
	{
		std::string assertMessage = "MKTouchHandler::UnregisterAxis - There are no the specified mask is not registered!";
		MK_ASSERTWITHMSG((mapIter == m_RegisteredAxis.end()), assertMessage);
	}
#endif // MK_DEBUG

	unordered_set<mkInputNameU32>::iterator setIter = mapIter->second.find(_inputName);

#if MK_DEBUG
	{
		std::string assertMessage = "MKTouchHandler::UnregisterAxis - There are no InputNames registered using the specified mask!";
		MK_ASSERTWITHMSG((setIter == mapIter->second.end()), assertMessage);
	}
#endif // MK_DEBUG

	mapIter->second.erase(setIter);
	if (mapIter->second.empty())
	{
		m_RegisteredAxis.erase(mapIter);
	}
}

void MKTouchHandler::RegisterClick(mkU64 _mask, MKInputName _inputName)
{
	unordered_map<mkU64, unordered_set<mkInputNameU32> >::iterator mapIter = m_RegisteredClicks.find(_mask);
	if (mapIter == m_RegisteredClicks.end())
	{
		unordered_set<mkInputNameU32> inputNames;
		inputNames.insert(_inputName);
		m_RegisteredClicks.insert(std::pair<mkU64, unordered_set<mkInputNameU32> >(_mask, inputNames));
	}
	else
	{
#if MK_DEBUG
		std::string assertMessage = "MKTouchHandler::RegisterClick - A InputName was registered twice with the same mask!";
		MK_ASSERTWITHMSG((mapIter->second.find(_inputName) == mapIter->second.end()), assertMessage);
#endif // MK_DEBUG

		mapIter->second.insert(_inputName);
	}

	ResetHeldClicks();
}

void MKTouchHandler::UnregisterClick(mkU64 _mask, MKInputName _inputName)
{
	unordered_map<mkU64, unordered_set<mkInputNameU32> >::iterator mapIter = m_RegisteredClicks.find(_mask);

#if MK_DEBUG
	{
		std::string assertMessage = "MKTouchHandler::UnregisterClick - There are no the specified mask is not registered!";
		MK_ASSERTWITHMSG((mapIter == m_RegisteredClicks.end()), assertMessage);
	}
#endif // MK_DEBUG

	unordered_set<mkInputNameU32>::iterator setIter = mapIter->second.find(_inputName);

#if MK_DEBUG
	{
		std::string assertMessage = "MKTouchHandler::UnregisterClick - There are no InputNames registered using the specified mask!";
		MK_ASSERTWITHMSG((setIter == mapIter->second.end()), assertMessage);
	}
#endif // MK_DEBUG

	mapIter->second.erase(setIter);
	if (mapIter->second.empty())
	{
		m_RegisteredClicks.erase(mapIter);
	}

	ResetHeldClicks();
}

// Callbacks
void MKTouchHandler::OnTouchesBegan(const std::vector<Touch*>& _touches, Event* _event)
{
	for (std::vector<Touch*>::const_iterator i = _touches.begin(); i != _touches.end(); ++i)
	{
		Touch* touch = *i;
		Vec2 touchLocation = touch->getLocation();
		MKCursorPosition cursorPosition(touchLocation.x, touchLocation.y);

		mkU16 currentContext = MKInputManager::GetInstance()->GetCurrentContext();
		mkU16 controllerMaskIndexMask = 0x0001 << touch->getID();
		mkU64 mask = MKInputManager::GenerateMask(currentContext, controllerMaskIndexMask, 0);

		std::unordered_set<mkInputNameU32> inputNames = GetValidClicks(mask);
		for (std::unordered_set<mkInputNameU32>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
		{
			++m_HeldClicks[*j];

			MKInputClick* click = new MKInputClick(static_cast<MKInputName>(*j), MKInputButton::PRESS, cursorPosition);
			MKInputManager::GetInstance()->AddInput<MKInputClick>(click);

			std::unordered_map<mkInputNameU32, MKCursorPosition>::iterator cursorIter = m_CursorPositions.find(*j);
			if (cursorIter == m_CursorPositions.end())
			{
				m_CursorPositions.insert(std::pair<mkInputNameU32, MKCursorPosition>(*j, cursorPosition));
			}
			else
			{
				cursorIter->second = cursorPosition;
			}
		}
	}
}

void MKTouchHandler::OnTouchesEnded(const std::vector<Touch*>& _touches, Event* _event)
{
	for (std::vector<Touch*>::const_iterator i = _touches.begin(); i != _touches.end(); ++i)
	{
		Touch* touch = *i;
		Vec2 touchLocation = touch->getLocation();
		MKCursorPosition cursorPosition(touchLocation.x, touchLocation.y);

		mkU16 currentContext = MKInputManager::GetInstance()->GetCurrentContext();
		mkU16 controllerMaskIndexMask = 0x0001 << touch->getID();
		mkU64 mask = MKInputManager::GenerateMask(currentContext, controllerMaskIndexMask, 0);

		std::unordered_set<mkInputNameU32> inputNames = GetValidClicks(mask);
		for (std::unordered_set<mkInputNameU32>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
		{
			mkS32 oldValue = m_HeldClicks[*j];
			m_HeldClicks[*j] = MKMathsHelper::Max<mkS32>(0, m_HeldClicks[*j] - 1);

			if (m_HeldClicks[*j] == 0 && oldValue > 0)
			{
				MKInputClick* click = new MKInputClick(static_cast<MKInputName>(*j), MKInputButton::RELEASE, cursorPosition);
				MKInputManager::GetInstance()->AddInput<MKInputClick>(click);

				std::unordered_map<mkInputNameU32, MKCursorPosition>::iterator cursorIter = m_CursorPositions.find(*j);
				if (cursorIter != m_CursorPositions.end())
				{
					m_CursorPositions.erase(cursorIter);
				}
			}
		}
	}
}

void MKTouchHandler::OnTouchesMoved(const std::vector<Touch*>& _touches, Event* _event)
{
	for (std::vector<Touch*>::const_iterator i = _touches.begin(); i != _touches.end(); ++i)
	{
		Touch* touch = *i;
		Vec2 touchDelta = touch->getDelta();
		mkU16 currentContext = MKInputManager::GetInstance()->GetCurrentContext();
		mkU16 controllerMaskIndexMask = 0x0001 << touch->getID();

		// Horizontal
		{
			float horizontalValue = touchDelta.x;
			mkU64 mask = MKInputManager::GenerateMask(currentContext, controllerMaskIndexMask, MKInputAxis::KeyCode::HORIZONTAL);
			std::unordered_set<mkInputNameU32> inputNames = GetValidAxis(mask);

			for (std::unordered_set<mkInputNameU32>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
			{
				// Horizontal
				MKInputAxis* axis = new MKInputAxis(static_cast<MKInputName>(*j), horizontalValue);
				MKInputManager::GetInstance()->AddInput<MKInputAxis>(axis);
			}
		}
		
		// Vertical
		{
			float verticalValue = touchDelta.y;
			mkU64 mask = MKInputManager::GenerateMask(currentContext, controllerMaskIndexMask, MKInputAxis::KeyCode::VERTICAL);
			std::unordered_set<mkInputNameU32> inputNames = GetValidAxis(mask);

			for (std::unordered_set<mkInputNameU32>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
			{
				// Horizontal
				MKInputAxis* axis = new MKInputAxis(static_cast<MKInputName>(*j), verticalValue);
				MKInputManager::GetInstance()->AddInput<MKInputAxis>(axis);
			}
		}

		// Update CursorPosition
		{
			Vec2 touchLocation = touch->getLocation();
			MKCursorPosition cursorPosition(touchLocation.x, touchLocation.y);
			mkU64 mask = MKInputManager::GenerateMask(currentContext, controllerMaskIndexMask, 0);
			std::unordered_set<mkInputNameU32> inputNames = GetValidClicks(mask);

			for (std::unordered_set<mkInputNameU32>::iterator j = inputNames.begin(); j != inputNames.end(); ++j)
			{
				std::unordered_map<mkInputNameU32, MKCursorPosition>::iterator cursorIter = m_CursorPositions.find(*j);
				if (cursorIter != m_CursorPositions.end())
				{
					cursorIter->second = cursorPosition;
				}
			}
		}
	}
}

NS_MK_END

#endif // MK_USE_TOUCH