// This is essentially a copy of MKMacros.

#ifndef GT_MACROS_H
#define GT_MACROS_H

#include "..\..\MK\Common\MKMacros.h"

#define NS_GT_BEGIN namespace GinTama {
#define NS_GT_END }
#define USING_NS_GT using namespace GinTama;
#define NS_GT GinTama::

typedef mkS8 gtS8;
typedef mkS16 gtS16;
typedef mkS32 gtS32;
typedef mkS64 gtS64;

typedef mkU8 gtU8;
typedef mkU16 gtU16;
typedef mkU32 gtU32;
typedef mkU64 gtU64;

typedef mkF32 gtF32;
typedef mkF64 gtF64;

typedef mkBool gtBool;
typedef mkString gtString;

#endif // GT_MACROS_H