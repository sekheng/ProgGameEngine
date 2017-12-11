#ifndef MK_PASSKEY_H
#define MK_PASSKEY_H

#include "MKMacros.h"

NS_MK_BEGIN

template<typename T>
struct MKPasskey
{
	friend T;

private:
	MKPasskey() {}

public:
	~MKPasskey() {}
};

template<typename T0, typename T1>
struct MKPasskey2
{
	friend T0;
	friend T1;

private:
	MKPasskey2() {}

public:
	~MKPasskey2() {}
};

template<typename T0, typename T1, typename T2>
struct MKPasskey3
{
	friend T0;
	friend T1;
	friend T2;

private:
	MKPasskey3() {}

public:
	~MKPasskey3() {}
};

NS_MK_END

#endif