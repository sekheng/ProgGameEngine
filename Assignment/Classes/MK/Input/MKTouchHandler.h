#ifndef MK_TOUCHHANDLER_H
#define MK_TOUCHHANDLER_H

// Include Cocos
#include "cocos2d.h"
#include "base/CCEventListenerTouch.h"

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
#include "MKInput.h"

USING_NS_CC;
using namespace std;

#if MK_USE_TOUCH

NS_MK_BEGIN

class MKInputManager;
class MKInputDefinition;

class MKTouchHandler : public MKSingletonTemplate<MKTouchHandler>
{
	friend class MKSingletonTemplate<MKTouchHandler>;

private:
	EventListenerTouchAllAtOnce* m_TouchListener = nullptr;
	mkS32* m_HeldClicks;
	std::unordered_map<mkU64, std::unordered_set<MK_INPUTNAME> > m_RegisteredClicks;
	std::unordered_map<mkU64, std::unordered_set<MK_INPUTNAME> > m_RegisteredAxis;
	std::unordered_map<MK_INPUTNAME, MKCursorPosition> m_CursorPositions;

	// The reason why we have a counter is because if the player is using some macros they might
	// register the key as pressed twice.
	std::map<cocos2d::Touch*, mkU32> m_HeldTouches;

	MKTouchHandler();
	virtual ~MKTouchHandler();

	void AddHeldTouch(cocos2d::Touch* _touch);
	void RemoveHeldTouch(cocos2d::Touch* _touch);

	void HandleTouchBegan(cocos2d::Touch* _touch);
	void HandleTouchMoved(cocos2d::Touch* _touch);
	void HandleTouchEnded(cocos2d::Touch* _touch);

	// Whenever there's a change in state (InputContext, Button Registered/Unregistered etc...) we wanna do this.
	void PreStateChange();
	void PostStateChange();

	unordered_set<MK_INPUTNAME> GetValidClicks(mkU64 _mask);
	unordered_set<MK_INPUTNAME> GetValidAxis(mkU64 _mask);
	void SendClickHeldEvents();
	void ResetHeldClicks();
	void ResetCursorPositions();

	// Callbacks
	void OnTouchesBegan(const std::vector<cocos2d::Touch*>& _touches, Event* _event);
	void OnTouchesMoved(const std::vector<cocos2d::Touch*>& _touches, Event* _event);
	void OnTouchesEnded(const std::vector<cocos2d::Touch*>& _touches, Event* _event);

public:
	void PreContextChange(MKPasskey<MKInputManager> _key);
	void PostContextChange(MKPasskey<MKInputManager> _key);
	void Update(MKPasskey<MKInputManager> _key);

	void RegisterAxis(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName);
	void UnregisterAxis(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName);
	void RegisterClick(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName);
	void UnregisterClick(MKPasskey<MKInputDefinition> _key, mkU64 _mask, MKInputName _inputName);

};

NS_MK_END

#endif // MK_USE_TOUCH

#endif // MK_TOUCHHANDLER_H