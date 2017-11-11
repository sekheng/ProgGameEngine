// Include Cocos
#include "base/ccMacros.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"

// Include STL
#include <typeinfo>

// Include MK
#include "MKKeyboardHandler.h"
#include "MKInputManager.h"
#include "MKInput.h"
#include "../Common/MKAssertions.h"
#include "../Common/MKMathsHelper.h"

#if MK_USE_KEYBOARD

NS_MK_BEGIN

MKKeyboardHandler::MKKeyboardHandler()
{
	m_HeldButtons = new mkS32[MKInputName::NUM_INPUTNAME];
	for (mkS32 i = 0; i < (mkS32)MKInputName::NUM_INPUTNAME; ++i)
	{
		m_HeldButtons[i] = 0;
	}

	// Initialise keyboard listener.
	m_KeyboardListener = EventListenerKeyboard::create();
	m_KeyboardListener->onKeyPressed = CC_CALLBACK_2(MKKeyboardHandler::OnKeyPressed, this);
	m_KeyboardListener->onKeyReleased = CC_CALLBACK_2(MKKeyboardHandler::OnKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_KeyboardListener, 1);
}

MKKeyboardHandler::~MKKeyboardHandler()
{
	delete[] m_HeldButtons;
}

unordered_set<MK_INPUTNAME> MKKeyboardHandler::GetValidButtons(mkU64 _mask)
{
	unordered_set<MK_INPUTNAME> result;
	for (unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::const_iterator i = m_RegisteredButtons.begin(); i != m_RegisteredButtons.end(); ++i)
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

// Callbacks
void MKKeyboardHandler::OnKeyPressed(cocos2d::EventKeyboard::KeyCode _keyCode, cocos2d::Event* _event)
{
	mkU16 currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	// We do not support multiple keyboards. All keyboards are keyboard 0.
	mkU64 mask = MKInputManager::GenerateMask(currentContext, MKControllerIndex::MK_CONTROLLER0, (mkU64)_keyCode);
	
	std::unordered_set<MK_INPUTNAME> inputNames = GetValidButtons(mask);
	for (std::unordered_set<MK_INPUTNAME>::iterator i = inputNames.begin(); i != inputNames.end(); ++i)
	{
		++m_HeldButtons[*i];

		MKInputButton* press = new MKInputButton(static_cast<MKInputName>(*i), MKInputButton::PRESS);
		MKInputManager::GetInstance()->AddInput<MKInputButton>(press);
	}
}

void MKKeyboardHandler::OnKeyReleased(cocos2d::EventKeyboard::KeyCode _keyCode, cocos2d::Event* _event)
{
	mkU16 currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	// We do not support multiple keyboards. All keyboards are keyboard 0.
	mkU64 mask = MKInputManager::GenerateMask(currentContext, MKControllerIndex::MK_CONTROLLER0, (mkU64)_keyCode);
	
	unordered_set<MK_INPUTNAME> inputNames = GetValidButtons(mask);
	for (unordered_set<MK_INPUTNAME>::iterator i = inputNames.begin(); i != inputNames.end(); ++i)
	{
		mkS32 oldValue = m_HeldButtons[*i];
		m_HeldButtons[*i] = MKMathsHelper::Max<mkS32>(0, m_HeldButtons[*i] - 1);

		if (m_HeldButtons[*i] == 0 && oldValue > 0)
		{
			MKInputButton* press = new MKInputButton(static_cast<MKInputName>(*i), MKInputButton::RELEASE);
			MKInputManager::GetInstance()->AddInput<MKInputButton>(press);
		}
	}
}

void MKKeyboardHandler::RegisterButton(mkU64 _mask, MKInputName _inputName)
{
	unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::iterator mapIter = m_RegisteredButtons.find(_mask);
	if (mapIter == m_RegisteredButtons.end())
	{
		unordered_set<MK_INPUTNAME> inputNames;
		inputNames.insert(_inputName);
		m_RegisteredButtons.insert(std::pair<mkU64, unordered_set<MK_INPUTNAME> >(_mask, inputNames));
	}
	else
	{
#if MK_DEBUG
		std::string assertMessage = "MKKeyboardHandler::RegisterButton - A InputName was registered twice with the same mask!";
		MK_ASSERTWITHMSG((mapIter->second.find(_inputName) == mapIter->second.end()), assertMessage);
#endif // MK_DEBUG

		mapIter->second.insert(_inputName);
	}

	ResetHeldButtons();
}

void MKKeyboardHandler::UnregisterButton(mkU64 _mask, MKInputName _inputName)
{
	unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::iterator mapIter = m_RegisteredButtons.find(_mask);

#if MK_DEBUG
	{
		std::string assertMessage = "MKKeyboardHandler::UnregisterButton - There are no the specified mask is not registered!";
		MK_ASSERTWITHMSG((mapIter != m_RegisteredButtons.end()), assertMessage);
	}
#endif // MK_DEBUG

	unordered_set<MK_INPUTNAME>::iterator setIter = mapIter->second.find(_inputName);

#if MK_DEBUG
	{
		std::string assertMessage = "MKKeyboardHandler::UnregisterButton - There are no InputNames registered using the specified mask!";
		MK_ASSERTWITHMSG((setIter != mapIter->second.end()), assertMessage);
	}
#endif // MK_DEBUG

	mapIter->second.erase(setIter);
	if (mapIter->second.empty())
	{
		m_RegisteredButtons.erase(mapIter);
	}

	ResetHeldButtons();
}

void MKKeyboardHandler::ResetHeldButtons()
{
	for (mkS32 i = 0; i < (mkS32)MKInputName::NUM_INPUTNAME; ++i)
	{
		m_HeldButtons[i] = 0;
	}
}

void MKKeyboardHandler::SendButtonHeldEvents()
{
	// Send out events for the buttons that are being held.
	mkU16 currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	for (mkS32 i = 0; i < (mkS32)MKInputName::NUM_INPUTNAME; ++i)
	{
		MK_ASSERTWITHMSG((m_HeldButtons[i] >= 0), "MKKeyboardHandler::SendButtonHeldEvents - Held Buttons counter should never be less than 0.");
		if (m_HeldButtons[i] > 0)
		{
			MKInputButton* hold = new MKInputButton(static_cast<MKInputName>(i), MKInputButton::HOLD);
			MKInputManager::GetInstance()->AddInput<MKInputButton>(hold);
		}
	}
}

void MKKeyboardHandler::OnContextChange(MKPasskey<MKInputManager> _key)
{
	ResetHeldButtons();
}

void MKKeyboardHandler::Update(MKPasskey<MKInputManager> _key)
{
	SendButtonHeldEvents();
}

NS_MK_END

#endif // MK_USE_KEYBOARD