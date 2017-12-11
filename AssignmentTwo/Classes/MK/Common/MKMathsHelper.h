#ifndef MK_MATHSHELPER_H
#define MK_MATHSHELPER_H

// Include MK
#include "MKMacros.h"

NS_MK_BEGIN

class MKMathsHelper
{
public:
    static const float HALF_PI;
    static const float PI;
    static const float TWO_PI;
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

    template<typename T>
    static T Clamp(T _value, T _min, T _max)
    {
        if (_value < _min) { return _min; }
        if (_value > _max) { return _max; }
        return value;
    }

    template<typename T>
    static T Abs(T _a)
    {
        return (_a > static_cast<T>(0)) ? _a : -_a;
    }

    template<typename T>
    static T CompareBitmasks(T _a, T _b)
    {
        return 0 != (_a & _b);
    }

    // Checks if _b contains _a.
    template<typename T>
    static T ContainsBitmask(T _a, T _b)
    {
        return _a == (_a & _b);
    }

};

NS_MK_END

#endif