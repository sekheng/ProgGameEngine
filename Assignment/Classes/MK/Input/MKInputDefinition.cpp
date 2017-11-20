// Include MK
#include "MKInputDefinition.h"

NS_MK_BEGIN

void MKInputDefinition::Register(const HandlerRegisterFunction& _registerFunction, const HandlerUnregisterFunction& _unregisterFunction, mkU64 _mask, mkU32 _index)
{
	Unregister(_index);

	m_InputRegisterData[_index] = new InputRegisterData();
	m_InputRegisterData[_index]->m_RegisterFunction = _registerFunction;
	m_InputRegisterData[_index]->m_UnregisterFunction = _unregisterFunction;
	m_InputRegisterData[_index]->m_Mask = _mask;

	_registerFunction({}, _mask, m_InputName);
}

void MKInputDefinition::Unregister(mkU32 _index)
{
	if (m_InputRegisterData[_index] == nullptr)
	{
		return;
	}

	mkU64 mask = m_InputRegisterData[_index]->m_Mask;
	m_InputRegisterData[_index]->m_UnregisterFunction({}, mask, m_InputName);

	delete m_InputRegisterData[_index];
	m_InputRegisterData[_index] = nullptr;
}

NS_MK_END