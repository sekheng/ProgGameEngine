#ifndef MK_INPUTDEFINITION_H
#define MK_INPUTDEFINITION_H

// Include Cocos

// Include STL
#include <functional>

// Include MK
#include "../Common/MKMacros.h"
#include "MKInputName.h"
#include "MKInput.h"
#include "../Common/MKPasskey.h"

NS_MK_BEGIN

class MKInputDefinition
{
public:
	typedef std::function<void(MKPasskey<MKInputDefinition>, mkU64 _mask, MKInputName _inputName)> HandlerRegisterFunction;
	typedef std::function<void(MKPasskey<MKInputDefinition>, mkU64 _mask, MKInputName _inputName)> HandlerUnregisterFunction;

    static const mkU32 MAX_INPUTS_PER_DEFINITION = 3;

private:
	struct InputRegisterData
	{
		mkU64 m_Mask = 0;
		HandlerRegisterFunction m_RegisterFunction;
		HandlerUnregisterFunction m_UnregisterFunction;
	};

	const MKInputName m_InputName;
	InputRegisterData* m_InputRegisterData[MAX_INPUTS_PER_DEFINITION] = { nullptr, nullptr, nullptr };

	void Register(const HandlerRegisterFunction& _registerFunction, const HandlerUnregisterFunction& _unregisterFunction, mkU64 _mask, mkU32 _index);
	void Unregister(mkU32 _index);

public:
	MKInputDefinition(MKInputName _inputName)
		: m_InputName(_inputName) {}
	virtual ~MKInputDefinition()
	{
		Unregister1();
		Unregister2();
	}

	MKInputName GetInputName() const { return m_InputName; }

	void Register1(const HandlerRegisterFunction& _registerFunction, const HandlerUnregisterFunction& _unregisterFunction, mkU64 _mask) { Register(_registerFunction, _unregisterFunction, _mask, 0); }
	void Register2(const HandlerRegisterFunction& _registerFunction, const HandlerUnregisterFunction& _unregisterFunction, mkU64 _mask) { Register(_registerFunction, _unregisterFunction, _mask, 1); }
    void Register3(const HandlerRegisterFunction& _registerFunction, const HandlerUnregisterFunction& _unregisterFunction, mkU64 _mask) { Register(_registerFunction, _unregisterFunction, _mask, 2); }
	void Unregister1() { Unregister(0); }
	void Unregister2() { Unregister(1); }
    void Unregister3() { Unregister(2); }

};

NS_MK_END

#endif // MK_INPUTDEFINITION_H