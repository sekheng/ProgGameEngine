#ifndef MK_KEYBOARD_HANDLER_H
#define MK_KEYBOARD_HANDLER_H

// Include Cocos
#include "cocos2d.h"
#include "base/CCEventListenerKeyboard.h"

// Include STL
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKSingletonTemplate.h"
#include "../Common/MKPasskey.h"
#include "MKInputName.h"

USING_NS_CC;
using namespace std;

#if MK_USE_KEYBOARD

NS_MK_BEGIN

class MKInputManager;
class MKInputDefinition;

class MKKeyboardHandler : public MKSingletonTemplate<MKKeyboardHandler>
{
	friend class MKSingletonTemplate<MKKeyboardHandler>;

private:
	EventListenerKeyboard* m_KeyboardListener = nullptr;
	std::unordered_map<mkU64, std::unordered_set<MK_INPUTNAME> > m_RegisteredButtons;
	mkS32* m_HeldButtons;

	// The reason why we have a counter is because if the player is using 2 keyboards(?) or some macros they might
	// register the key as pressed twice.
	std::map<cocos2d::EventKeyboard::KeyCode, mkU32> m_HeldKeys;

	MKKeyboardHandler();
	virtual ~MKKeyboardHandler();

	void AddHeldKey(cocos2d::EventKeyboard::KeyCode _keyCode);
	void RemoveHeldKey(cocos2d::EventKeyboard::KeyCode _keyCode);
	void HandleOnKeyPressed(cocos2d::EventKeyboard::KeyCode _keyCode);
	void HandleOnKeyReleased(cocos2d::EventKeyboard::KeyCode _keyCode);

	// Whenever there's a change in state (InputContext, Button Registered/Unregistered etc...) we wanna do this.
	void PreStateChange();
	void PostStateChange();

	unordered_set<MK_INPUTNAME> GetValidButtons(mkU64 _mask);
	void SendButtonHeldEvents();
	void ResetHeldButtons();

	// Callbacks
	void OnKeyPressed(cocos2d::EventKeyboard::KeyCode _keyCode, cocos2d::Event* _event);
	void OnKeyReleased(cocos2d::EventKeyboard::KeyCode _keyCode, cocos2d::Event* _event);

public:
	void PreContextChange(MKPasskey<MKInputManager> _key);
	void PostContextChange(MKPasskey<MKInputManager> _key);

	void Update(MKPasskey<MKInputManager> _key);

	void RegisterButton(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName);
	void UnregisterButton(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName);

};

NS_MK_END

#endif // MK_USE_KEYBOARD

#endif // MK_KEYBOARD_HANDLER_H