#ifndef MK_INPUT_H
#define MK_INPUT_H

// Include Cocos

// Include STL
#include <typeinfo>
#include <string>
#include <string>

// Include MK
#include "MKInputName.h"
#include "MKInputContext.h"
#include "../Common/MKMacros.h"

NS_MK_BEGIN

class MKInputBase
{
protected:
	MKInputBase(MKInputName _inputName, MKInputContext _contextDuringInput)
		: m_InputName(_inputName), m_ContextDuringInput(_contextDuringInput) {}
	virtual ~MKInputBase() {}

public:
	const MKInputName m_InputName;
	const MKInputContext m_ContextDuringInput; // The current context when this Input was created.

};

class MKInputAxis : public MKInputBase
{
public:
	// Axis does not have a defined KeyCode, so we will define it here.
	enum KeyCode
	{
		HORIZONTAL = 0x00000001,
		VERTICAL = 0x00000002,
	};

	const mkF32 m_AxisValue;

	MKInputAxis(MKInputName _inputName, MKInputContext _contextDuringInput, mkF32 _axisValue)
		: MKInputBase(_inputName, _contextDuringInput), m_AxisValue(_axisValue) {}
	virtual ~MKInputAxis() {}
	
	static std::string GetName()
	{
		return typeid(MKInputAxis).name();
	}

};

class MKInputButton : public MKInputBase
{
public:
	// Cocos already has define KeyCodes for keyboard, so we will be using those.
	// The ButtonState is NOT the KeyCode.
	enum ButtonState
	{
		PRESS = 0,
		HOLD = 1,
		RELEASE = 2,
	};
	
	const ButtonState m_ButtonState;

	MKInputButton(MKInputName _inputName, MKInputContext _contextDuringInput, ButtonState _buttonState)
		: MKInputBase(_inputName, _contextDuringInput), m_ButtonState(_buttonState) {}
	virtual ~MKInputButton() {}
	
	static std::string GetName()
	{
		return typeid(MKInputButton).name();
	}

};

struct MKCursorPosition
{
private:
    mkF32 m_X, m_Y;

public:
	MKCursorPosition(mkF32 _x = 0, mkF32 _y = 0) { Set(_x, _y); }
	MKCursorPosition(const MKCursorPosition& _other) { Set(_other.m_X, _other.m_Y); }
	~MKCursorPosition() {}

	void Set(mkF32 _x, mkF32 _y) { m_X = _x; m_Y = _y; }
	mkF32 GetX() const { return m_X; }
	mkF32 GetY() const { return m_Y; }

	MKCursorPosition& operator=(const MKCursorPosition& _other) { Set(_other.m_X, _other.m_Y); return *this; }
};

class MKInputClick : public MKInputButton
{
public:
	const MKCursorPosition m_CursorPosition;

	MKInputClick(MKInputName _inputName, MKInputContext _contextDuringInput, ButtonState _buttonState, const MKCursorPosition& _cursorPosition)
		: MKInputButton(_inputName, _contextDuringInput, _buttonState), m_CursorPosition(_cursorPosition) {}
	virtual ~MKInputClick() {}
	
	static std::string GetName()
	{
		return typeid(MKInputClick).name();
	}

};

NS_MK_END

#endif