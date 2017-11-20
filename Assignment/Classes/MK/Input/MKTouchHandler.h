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

class MKTouchHandler : public MKSingletonTemplate<MKTouchHandler>
{
	friend class MKSingletonTemplate<MKTouchHandler>;

private:
	EventListenerTouchAllAtOnce* m_TouchListener = nullptr;
	mkS32* m_HeldClicks;
	std::unordered_map<mkU64, std::unordered_set<MK_INPUTNAME> > m_RegisteredClicks;
	std::unordered_map<mkU64, std::unordered_set<MK_INPUTNAME> > m_RegisteredAxis;
	std::unordered_map<MK_INPUTNAME, MKCursorPosition> m_CursorPositions;

	MKTouchHandler();
	virtual ~MKTouchHandler();

	unordered_set<MK_INPUTNAME> GetValidClicks(mkU64 _mask);
	unordered_set<MK_INPUTNAME> GetValidAxis(mkU64 _mask);
	void SendClickHeldEvents();
	void ResetHeldClicks();
	void ResetCursorPositions();

	// Callbacks
	void OnTouchesBegan(const std::vector<Touch*>& _touches, Event* _event);
	void OnTouchesMoved(const std::vector<Touch*>& _touches, Event* _event);
	void OnTouchesEnded(const std::vector<Touch*>& _touches, Event* _event);

public:
	void PreContextChange(MKPasskey<MKInputManager> _key);
	void PostContextChange(MKPasskey<MKInputManager> _key);
	void Update(MKPasskey<MKInputManager> _key);

	void RegisterAxis(mkU64 _mask, MKInputName _inputName);
	void UnregisterAxis(mkU64 _mask, MKInputName _inputName);
	void RegisterClick(mkU64 _mask, MKInputName _inputName);
	void UnregisterClick(mkU64 _mask, MKInputName _inputName);

};

NS_MK_END

#endif // MK_USE_TOUCH

#endif // MK_TOUCHHANDLER_H