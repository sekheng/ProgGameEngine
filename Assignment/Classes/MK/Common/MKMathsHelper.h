#ifndef MK_MATHSHELPER_H
#define MK_MATHSHELPER_H

// Include MK
#include "MKMacros.h"

NS_MK_BEGIN

class MKMathsHelper
{
public:
    static const float PI;
    static const float Deg2Rad;
    static const float Rad2Deg;

	template<typename T>
	static T Max(T _a, T _b)
	{
		return (_a > _b) ? _a : _b;
	}

	template<typename T>
	static T Min(T _a, T _b)
	{
		return (_a < _b) ? _a : _b;
	}

};

NS_MK_END

#endif