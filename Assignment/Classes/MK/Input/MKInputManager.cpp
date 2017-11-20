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
	{
		mkU64 jumpMask = MKInputManager::GenerateMask(MK_CONTEXT_ALL | MK_CONTEXT1, MK_CONTROLLER1 | MK_CONTROLLER1, (mkU32)EventKeyboard::KeyCode::KEY_UP_ARROW);
		m_InputDefinitions[MKInputName::JUMP]->Register1(
			CC_CALLBACK_2(MKKeyboardHandler::RegisterButton, keyboardHandler),
			CC_CALLBACK_2(MKKeyboardHandler::UnregisterButton, keyboardHandler),
			jumpMask);
	}

	{
		mkU64 jumpMask = MKInputManager::GenerateMask(MK_CONTEXT_ALL, MK_CONTROLLER1, MKInputAxis::KeyCode::VERTICAL);
		m_InputDefinitions[MKInputName::JUMP]->Register2(
			CC_CALLBACK_2(MKTouchHandler::RegisterAxis, touchHandler),
			CC_CALLBACK_2(MKTouchHandler::UnregisterAxis, touchHandler),
			jumpMask);
	}
}

MKInputManager::MKInputManager()
{
	m_CurrentContext = MKInputContext::MK_CONTEXT_DEFAULT;

	// Initialise Event Dispatcher.
	m_EventDispatcher.setEnabled(true);

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
	m_EventDispatcher.removeAllEventListeners();

	// Input Definitions
	for (mkS32 i = 0; i < MKInputName::NUM_INPUTNAME; ++i)
	{
		delete m_InputDefinitions[i];
	}
	delete[] m_InputDefinitions;

	FlushBuffer();
}

void MKInputManager::SetCurrentContext(MKInputContext _currentContext)
{
	if (_currentContext == m_CurrentContext)
	{
		return;
	}

	// Pre-Context Change
#if MK_USE_KEYBOARD
	MKKeyboardHandler::GetInstance()->PreContextChange({});
#endif // MK_USE_KEYBOARD

#if MK_USE_TOUCH
	MKTouchHandler::GetInstance()->PreContextChange({});
#endif // MK_USE_TOUCH

	SendAllInputEvents();

	// Context Change
	m_CurrentContext = _currentContext;

	// Post Context Change
#if MK_USE_KEYBOARD
	MKKeyboardHandler::GetInstance()->PostContextChange({});
#endif // MK_USE_KEYBOARD

#if MK_USE_TOUCH
	MKTouchHandler::GetInstance()->PostContextChange({});
#endif // MK_USE_TOUCH
}

MKInputContext MKInputManager::GetCurrentContext() const
{
	return m_CurrentContext;
}

void MKInputManager::SendAllInputEvents()
{
	while (m_InputEventQueue.empty() == false)
	{
		m_EventDispatcher.dispatchEvent(m_InputEventQueue.front());
		m_InputEventQueue.pop();
	}
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
#if MK_USE_KEYBOARD
	MKKeyboardHandler::GetInstance()->Update({});
#endif // MK_USE_KEYBOARD

#if MK_USE_TOUCH
	MKTouchHandler::GetInstance()->Update({});
#endif // MK_USE_TOUCH

	SendAllInputEvents();
}

NS_MK_END