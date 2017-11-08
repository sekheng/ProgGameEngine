#ifndef MK_MACROS_H
#define MK_MACROS_H

namespace MinamiKotori
{

#define NS_MK_BEGIN namespace MinamiKotori {
#define NS_MK_END }
#define USING_NS_MK using namespace MinamiKotori;
#define NS_MK ::MinamiKotori

#define MK_USE_KEYBOARD true
#define MK_USE_TOUCH true
#define MK_USE_MOUSE true

#define MK_DEBUG true

typedef short mkS16;
typedef int mkS32;
typedef long long int mkS64;

typedef unsigned short mkU16;
typedef unsigned int mkU32;
typedef unsigned long long int mkU64;

// You know why we do this? You know why? Because when fucking compiling for fucking android
// the fucking std::hash does not fucking work on fucking Enums that's fuck why.
typedef mkU32 mkInputNameU32;

typedef float mkF32;
typedef double mkF64;

}

#endif