// Include Cocos
#include "base/ccMacros.h"

// Include MK
#include "MKInputManager.h"

// Include Input Device Handlers
#if MK_USE_KEYBOARD
#include "MKKeyboardHandler.h"
#endif // MK_USE_KEYBOARD

NS_MK_BEGIN

void MKInputManager::InitializeDefinitions()
{
	MKKeyboardHandler* keyboardHandler = MKKeyboardHandler::GetInstance();

	// Add Input Definitions here.
	{
		mkU64 jumpMask = MKInputManager::GenerateMask(MK_CONTEXT_ALL, MK_CONTROLLER_ALL, (mkU32)EventKeyboard::KeyCode::KEY_UP_ARROW);
		m_InputDefinitions[MKInputName::JUMP]->Register1(
			CC_CALLBACK_2(MKKeyboardHandler::RegisterButton, keyboardHandler),
			CC_CALLBACK_2(MKKeyboardHandler::UnregisterButton, keyboardHandler),
			jumpMask);
	}
}

MKInputManager::MKInputManager()
{
	m_CurrentContext = MK_CONTEXT_NONE;

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

	InitializeDefinitions();
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

#if MK_USE_KEYBOARD
		MKKeyboardHandler::GetInstance()->OnContextChange({});
#endif // MK_USE_KEYBOARD

	m_CurrentContext = _currentContext;
	FlushBuffer();
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

	SendAllInputEvents();
}

NS_MK_END