// Include Cocos
#include "base/ccMacros.h"
#include "base/CCScheduler.h"
#include "base/CCDirector.h"

// Include MK
#include "MKInputManager.h"

// Include Input Device Handlers
#if MK_USE_KEYBOARD
#include "MKKeyboardHandler.h"
#endif // MK_USE_KEYBOARD

#if MK_USE_TOUCH
#include "MKTouchHandler.h"
#endif // MK_USE_TOUCH

NS_MK_BEGIN

void MKInputManager::InitializeDefinitions()
{
	MKKeyboardHandler* keyboardHandler = MKKeyboardHandler::GetInstance();
	MKTouchHandler* touchHandler = MKTouchHandler::GetInstance();

	// Add Input Definitions here.

    // Start Game (Touch/Mouse)
    {
        mkU64 inputMask = MKInputManager::GenerateMask(MK_INPUT_CONTEXT_STARTSCENE, MK_CONTROLLER_ALL, 0);
        m_InputDefinitions[MKInputName::START_GAME]->Register1(
            CC_CALLBACK_3(MKTouchHandler::RegisterClick, touchHandler),
            CC_CALLBACK_3(MKTouchHandler::UnregisterClick, touchHandler),
            inputMask);
    }
    // Jumping (Keyboard)
	{
		mkU64 inputMask = MKInputManager::GenerateMask(MK_INPUT_CONTEXT_GAMESCENE, MK_CONTROLLER_1, (mkU32)EventKeyboard::KeyCode::KEY_UP_ARROW);
		m_InputDefinitions[MKInputName::JUMP]->Register1(
			CC_CALLBACK_3(MKKeyboardHandler::RegisterButton, keyboardHandler),
			CC_CALLBACK_3(MKKeyboardHandler::UnregisterButton, keyboardHandler),
            inputMask);
	}
    // Jumping (Touch/Mouse)
	{
		mkU64 inputMask = MKInputManager::GenerateMask(MK_INPUT_CONTEXT_GAMESCENE, MK_CONTROLLER_1, 0);
		m_InputDefinitions[MKInputName::JUMP]->Register2(
			CC_CALLBACK_3(MKTouchHandler::RegisterClick, touchHandler),
			CC_CALLBACK_3(MKTouchHandler::UnregisterClick, touchHandler),
            inputMask);
	}
    // Sliding (Keyboard)
    {
        mkU64 inputMask = MKInputManager::GenerateMask(MK_INPUT_CONTEXT_GAMESCENE, MK_CONTROLLER_1, (mkU32)EventKeyboard::KeyCode::KEY_DOWN_ARROW);
        m_InputDefinitions[MKInputName::SLIDE]->Register1(
            CC_CALLBACK_3(MKKeyboardHandler::RegisterButton, keyboardHandler),
            CC_CALLBACK_3(MKKeyboardHandler::UnregisterButton, keyboardHandler),
            inputMask);
    }
    // Sliding (Touch/Mouse)
    {
        mkU64 inputMask = MKInputManager::GenerateMask(MK_INPUT_CONTEXT_GAMESCENE, MK_CONTROLLER_1, 0);
        m_InputDefinitions[MKInputName::SLIDE]->Register2(
            CC_CALLBACK_3(MKTouchHandler::RegisterClick, touchHandler),
            CC_CALLBACK_3(MKTouchHandler::UnregisterClick, touchHandler),
            inputMask);
    }
}

MKInputManager::MKInputManager()
{
	m_CurrentContext = MKInputContext::MK_INPUT_CONTEXT_DEFAULT;

	// Initialise Event Dispatcher.
#if MK_USE_DIRECTOR_DISPATCHER
    // Do nothing.
#else
	m_EventDispatcher = new (std::nothrow) EventDispatcher();
	m_EventDispatcher->setEnabled(true);
#endif

	// Input Definitions
	m_InputDefinitions = new MKInputDefinition*[MKInputName::NUM_INPUTNAME];
	for (mkS32 i = 0; i < MKInputName::NUM_INPUTNAME; ++i)
	{
		MKInputName inputName = static_cast<MKInputName>(i);
		m_InputDefinitions[i] = new MKInputDefinition(inputName);
	}

	// Ensure that the Input Device Handlers get created.
#if MK_USE_KEYBOARD
	MKKeyboardHandler::GetInstance();
#endif // MK_USE_KEYBOARD

#if MK_USE_TOUCH
	MKTouchHandler::GetInstance();
#endif // MK_USE_TOUCH

	InitializeDefinitions();

	// Schedule Update
	Director::getInstance()->getScheduler()->scheduleUpdate(this, 1, false);
}

MKInputManager::~MKInputManager()
{
	// Clean up Event Dispatcher.
#if MK_USE_DIRECTOR_DISPATCHER
#else
	m_EventDispatcher->removeAllEventListeners();
	CC_SAFE_RELEASE(m_EventDispatcher);
#endif

	// Input Definitions
	for (mkS32 i = 0; i < MKInputName::NUM_INPUTNAME; ++i)
	{
		delete m_InputDefinitions[i];
	}
	delete[] m_InputDefinitions;

	FlushBuffer();

	Director::getInstance()->getScheduler()->unscheduleUpdate(this);

#if MK_USE_KEYBOARD
	MKKeyboardHandler::GetInstance()->Destroy();
#endif // MK_USE_KEYBOARD

#if MK_USE_TOUCH
	MKTouchHandler::GetInstance()->Destroy();
#endif // MK_USE_TOUCH
}

void MKInputManager::SetCurrentContext(MKInputContext _currentContext)
{
	if (_currentContext == m_CurrentContext)
	{
		return;
	}

    CCLOG("MKInputManager::SetCurrentContext - PreContextChange Start");

    // Pre-Context Change
#if MK_USE_KEYBOARD
	MKKeyboardHandler::GetInstance()->PreContextChange({});
#endif // MK_USE_KEYBOARD

#if MK_USE_TOUCH
	MKTouchHandler::GetInstance()->PreContextChange({});
#endif // MK_USE_TOUCH

    SendAllInputEvents();

    CCLOG("MKInputManager::SetCurrentContext - PreContextChange End");

    CCLOG("MKInputManager::SetCurrentContext - ContextChange Start");
	// Context Change
	m_CurrentContext = _currentContext;
    CCLOG("MKInputManager::SetCurrentContext - ContextChange End");

    CCLOG("MKInputManager::SetCurrentContext - PostContextChange Start");

	// Post Context Change
#if MK_USE_KEYBOARD
	MKKeyboardHandler::GetInstance()->PostContextChange({});
#endif // MK_USE_KEYBOARD

#if MK_USE_TOUCH
	MKTouchHandler::GetInstance()->PostContextChange({});
#endif // MK_USE_TOUCH

    SendAllInputEvents();

    CCLOG("MKInputManager::SetCurrentContext - PostContextChange End");
}

MKInputContext MKInputManager::GetCurrentContext() const
{
	return m_CurrentContext;
}

void MKInputManager::SendAllInputEvents()
{
    CCLOG("MKInputManager::SendAllInputEvents Start");

    while (m_InputEventQueue.empty() == false)
    {
        EventCustom* inputEvent = m_InputEventQueue.front();
        m_InputEventQueue.pop();

#if MK_USE_DIRECTOR_DISPATCHER
        Director::getInstance()->getEventDispatcher()->dispatchEvent(inputEvent);
#else
        m_EventDispatcher->dispatchEvent(inputEvent);
#endif // MK_USE_DIRECTOR_DISPATCHER
    }

    CCLOG("MKInputManager::SendAllInputEvents End");
}

void MKInputManager::FlushBuffer()
{
    while (m_InputEventQueue.empty() == false)
    {
        m_InputEventQueue.pop();
    }
}

void MKInputManager::Update()
{
    CCLOG("MKInputManager::Update Start");

#if MK_USE_KEYBOARD
	MKKeyboardHandler::GetInstance()->Update({});
#endif // MK_USE_KEYBOARD

#if MK_USE_TOUCH
	MKTouchHandler::GetInstance()->Update({});
#endif // MK_USE_TOUCH

	SendAllInputEvents();

    CCLOG("MKInputManager::Update End");
}

NS_MK_END