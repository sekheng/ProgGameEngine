#ifndef MK_INPUTCONTROLLERINDEX_H
#define MK_INPUTCONTROLLERINDEX_H

// Include MK
#include "../Common/MKMacros.h"

NS_MK_BEGIN

enum MKControllerIndex
{
	MK_CONTROLLER0 = 0x0001,
	MK_CONTROLLER1 = 0x0002,
	MK_CONTROLLER2 = 0x0004,
	MK_CONTROLLER3 = 0x0008,
	MK_CONTROLLER4 = 0x0010,
	MK_CONTROLLER5 = 0x0020,
	MK_CONTROLLER6 = 0x0040,
	MK_CONTROLLER7 = 0x0080,
	MK_CONTROLLER8 = 0x0100,
	MK_CONTROLLER9 = 0x0200,
	MK_CONTROLLER10 = 0x0400,
	MK_CONTROLLER11 = 0x0800,
	MK_CONTROLLER12 = 0x1000,
	MK_CONTROLLER13 = 0x2000,
	MK_CONTROLLER14 = 0x4000,
	MK_CONTROLLER15 = 0x8000,

	MK_CONTROLLER_ALL = 0xFFFF,
};

NS_MK_END

#endif MK_INPUTCONTROLLERINDEX_H