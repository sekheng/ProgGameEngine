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

class MKKeyboardHandler : public MKSingletonTemplate<MKKeyboardHandler>
{
	friend class MKSingletonTemplate<MKKeyboardHandler>;

private:
	EventListenerKeyboard* m_KeyboardListener = nullptr;
	std::unordered_map<mkU64, std::unordered_set<MK_INPUTNAME> > m_RegisteredButtons;
	mkS32* m_HeldButtons;

	MKKeyboardHandler();
	virtual ~MKKeyboardHandler();

	unordered_set<MK_INPUTNAME> GetValidButtons(mkU64 _mask);
	void SendButtonHeldEvents();
	void ResetHeldButtons();

	// Callbacks
	void OnKeyPressed(cocos2d::EventKeyboard::KeyCode _keyCode, cocos2d::Event* _event);
	void OnKeyReleased(cocos2d::EventKeyboard::KeyCode _keyCode, cocos2d::Event* _event);

public:
	void OnContextChange(MKPasskey<MKInputManager> _key);
	void Update(MKPasskey<MKInputManager> _key);

	void RegisterButton(mkU64 _mask, MKInputName _inputName);
	void UnregisterButton(mkU64 _mask, MKInputName _inputName);

};

NS_MK_END

#endif // MK_USE_KEYBOARD

#endif // MK_KEYBOARD_HANDLER_H