#ifndef MK_SINGLETONTEMPLATE_H
#define MK_SINGLETONTEMPLATE_H

#include "MKMacros.h"

NS_MK_BEGIN

template<class T>
class MKSingletonTemplate
{
private:
	static T* m_Instance;

protected:
	MKSingletonTemplate() {}
	virtual ~MKSingletonTemplate() {}

public:
	static T* GetInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new T();
		}

		return m_Instance;
	}

	static bool Destroy()
	{
		if (m_Instance == nullptr)
		{
			return false;
		}

		delete m_Instance;
		m_Instance = nullptr;
		return true;
	}

};

template <class T>
T* MKSingletonTemplate<T>::m_Instance = nullptr;

NS_MK_END

#endif