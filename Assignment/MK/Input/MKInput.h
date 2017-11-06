#ifndef MK_INPUT_H
#define MK_INPUT_H

// Include Cocos
//#include "../../../cocos2d/cocos/cocos2d.h"

// Include STL
#include <typeinfo>
#include <string>

// Include MK
#include "MKInputName.h"
#include "../Common/MKMacros.h"

NS_MK_BEGIN

class MKInputBase
{
protected:
	MKInputBase(MKInputName _inputName)
		: m_InputName(_inputName) {}
	virtual ~MKInputBase() {}

public:
	const MKInputName m_InputName;

};

class MKInputAxis : public MKInputBase
{
public:
	const mkF32 m_AxisValue;

	MKInputAxis(MKInputName _inputName, mkF32 _axisValue)
		: MKInputBase(_inputName), m_AxisValue(_axisValue) {}
	virtual ~MKInputAxis() {}
	
	static std::string GetName()
	{
		return typeid(MKInputAxis).name();
	}

};

class MKInputButton : public MKInputBase
{
public:
	enum ButtonState
	{
		PRESS,
		HOLD,
		RELEASE,
	};
	
	const ButtonState m_ButtonState;

	MKInputButton(MKInputName _inputName, ButtonState _buttonState)
		: MKInputBase(_inputName), m_ButtonState(_buttonState) {}
	virtual ~MKInputButton() {}
	
	static std::string GetName()
	{
		return typeid(MKInputButton).name();
	}

};

struct MKCursorPosition
{
public:
	mkU32 m_X, m_Y;

	MKCursorPosition(mkU32 _x = 0, mkU32 _y = 0) { Set(_x, _y); }
	MKCursorPosition(const MKCursorPosition& _other) { Set(_other.m_X, _other.m_Y); }
	~MKCursorPosition() {}

	void Set(mkU32 _x, mkU32 _y) { m_X = _x; m_Y = _y; }
	mkU32 GetX() const { return m_X; }
	mkU32 GetY() const { return m_Y; }
};

class MKInputClick : public MKInputButton
{
public:
	const MKCursorPosition m_CursorPosition;

	MKInputClick(MKInputName _inputName, ButtonState _buttonState, const MKCursorPosition& _cursorPosition)
		: MKInputButton(_inputName, _buttonState), m_CursorPosition(_cursorPosition) {}
	virtual ~MKInputClick() {}
	
	static std::string GetName()
	{
		return typeid(MKInputClick).name();
	}

};

NS_MK_END

#endif