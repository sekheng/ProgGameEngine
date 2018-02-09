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
	Director::getInstance()->getEventDispatcher()->removeEventListener(m_KeyboardListener);
}

void MKKeyboardHandler::AddHeldKey(cocos2d::EventKeyboard::KeyCode _keyCode)
{
	//CCLOG("Held Key Added");
	std::map<cocos2d::EventKeyboard::KeyCode, mkU32>::iterator mapIter = m_HeldKeys.find(_keyCode);
	if (mapIter == m_HeldKeys.end())
	{
		m_HeldKeys.insert(std::pair<cocos2d::EventKeyboard::KeyCode, mkU32>(_keyCode, 1));
	}
	else
	{
		mapIter->second += 1;
	}
}

void MKKeyboardHandler::RemoveHeldKey(cocos2d::EventKeyboard::KeyCode _keyCode)
{
	//CCLOG("Held Key Removed");
	std::map<cocos2d::EventKeyboard::KeyCode, mkU32>::iterator mapIter = m_HeldKeys.find(_keyCode);
	if (mapIter == m_HeldKeys.end()) { return; }
	MK_ASSERTWITHMSG((mapIter != m_HeldKeys.end()), "MKKeyboardHandler::RemoveHeldKey - Key not found in m_HeldKeys!");
	MK_ASSERTWITHMSG((mapIter->second != 0), "MKKeyboardHandler::RemoveHeldKey - Invalid value for counter of _keycode!");

	mapIter->second -= 1;
	if (mapIter->second == 0)
	{
		m_HeldKeys.erase(mapIter);
	}
}

void MKKeyboardHandler::HandleOnKeyPressed(cocos2d::EventKeyboard::KeyCode _keyCode)
{
	MKInputContext currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	// We do not support multiple keyboards. All keyboards are keyboard 0.
	mkU64 mask = MKInputManager::GenerateMask(currentContext, MKControllerIndex::MK_CONTROLLER_1, (mkU64)_keyCode);

	std::unordered_set<MK_INPUTNAME> inputNames = GetValidButtons(mask);
	for (std::unordered_set<MK_INPUTNAME>::iterator i = inputNames.begin(); i != inputNames.end(); ++i)
	{
		++m_HeldButtons[*i];

		MKInputButton* press = new MKInputButton(static_cast<MKInputName>(*i), currentContext, MKInputButton::PRESS);
		MKInputManager::GetInstance()->AddInput<MKInputButton>(press);
	}
}

void MKKeyboardHandler::HandleOnKeyReleased(cocos2d::EventKeyboard::KeyCode _keyCode)
{
	MKInputContext currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	// We do not support multiple keyboards. All keyboards are keyboard 0.
	mkU64 mask = MKInputManager::GenerateMask(currentContext, MKControllerIndex::MK_CONTROLLER_1, (mkU64)_keyCode);

	unordered_set<MK_INPUTNAME> inputNames = GetValidButtons(mask);
	for (unordered_set<MK_INPUTNAME>::iterator i = inputNames.begin(); i != inputNames.end(); ++i)
	{
		mkS32 oldValue = m_HeldButtons[*i];
		m_HeldButtons[*i] = MKMathsHelper::Max<mkS32>(0, m_HeldButtons[*i] - 1);

		if (m_HeldButtons[*i] == 0 && oldValue > 0)
		{
			MKInputButton* press = new MKInputButton(static_cast<MKInputName>(*i), currentContext, MKInputButton::RELEASE);
			MKInputManager::GetInstance()->AddInput<MKInputButton>(press);
		}
	}
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
	AddHeldKey(_keyCode);
	HandleOnKeyPressed(_keyCode);	
}

void MKKeyboardHandler::OnKeyReleased(cocos2d::EventKeyboard::KeyCode _keyCode, cocos2d::Event* _event)
{
	RemoveHeldKey(_keyCode);
	HandleOnKeyReleased(_keyCode);	
}

void MKKeyboardHandler::RegisterButton(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName)
{
	PreStateChange();

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
		mkString assertMessage = "MKKeyboardHandler::RegisterButton - A InputName was registered twice with the same mask!";
		MK_ASSERTWITHMSG((mapIter->second.find(_inputName) == mapIter->second.end()), assertMessage.c_str());
#endif // MK_DEBUG

		mapIter->second.insert(_inputName);
	}

	PostStateChange();
}

void MKKeyboardHandler::UnregisterButton(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName)
{
	PreStateChange();

	unordered_map<mkU64, unordered_set<MK_INPUTNAME> >::iterator mapIter = m_RegisteredButtons.find(_mask);

#if MK_DEBUG
	{
		mkString assertMessage = "MKKeyboardHandler::UnregisterButton - There are no the specified mask is not registered!";
		MK_ASSERTWITHMSG((mapIter != m_RegisteredButtons.end()), assertMessage.c_str());
	}
#endif // MK_DEBUG

	unordered_set<MK_INPUTNAME>::iterator setIter = mapIter->second.find(_inputName);

#if MK_DEBUG
	{
		mkString assertMessage = "MKKeyboardHandler::UnregisterButton - There are no InputNames registered using the specified mask!";
		MK_ASSERTWITHMSG((setIter != mapIter->second.end()), assertMessage.c_str());
	}
#endif // MK_DEBUG

	mapIter->second.erase(setIter);
	if (mapIter->second.empty())
	{
		m_RegisteredButtons.erase(mapIter);
	}

	PostStateChange();
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
	MKInputContext currentContext = MKInputManager::GetInstance()->GetCurrentContext();
	for (mkS32 i = 0; i < (mkS32)MKInputName::NUM_INPUTNAME; ++i)
	{
		MK_ASSERTWITHMSG((m_HeldButtons[i] >= 0), "MKKeyboardHandler::SendButtonHeldEvents - Held Buttons counter should never be less than 0.");
		if (m_HeldButtons[i] > 0)
		{
			MKInputButton* hold = new MKInputButton(static_cast<MKInputName>(i), currentContext, MKInputButton::HOLD);
			MKInputManager::GetInstance()->AddInput<MKInputButton>(hold);
		}
	}
}

void MKKeyboardHandler::PreStateChange()
{
	for (std::map<cocos2d::EventKeyboard::KeyCode, mkU32>::iterator i = m_HeldKeys.begin(); i != m_HeldKeys.end(); ++i)
	{
		for (mkU32 j = 0; j < i->second; ++j)
		{
			HandleOnKeyReleased(i->first);
		}
	}
}

void MKKeyboardHandler::PostStateChange()
{
	for (std::map<cocos2d::EventKeyboard::KeyCode, mkU32>::iterator i = m_HeldKeys.begin(); i != m_HeldKeys.end(); ++i)
	{
		for (mkU32 j = 0; j < i->second; ++j)
		{
			HandleOnKeyPressed(i->first);
		}
	}
}

void MKKeyboardHandler::PreContextChange(MKPasskey<MKInputManager> _key)
{
	PreStateChange();
}

void MKKeyboardHandler::PostContextChange(MKPasskey<MKInputManager> _key)
{
	PostStateChange();
}

void MKKeyboardHandler::Update(MKPasskey<MKInputManager> _key)
{
	SendButtonHeldEvents();
}

NS_MK_END

#endif // MK_USE_KEYBOARD