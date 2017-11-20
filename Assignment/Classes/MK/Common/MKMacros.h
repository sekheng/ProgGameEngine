#ifndef MK_MACROS_H
#define MK_MACROS_H

#define NS_MK_BEGIN namespace MinamiKotori {
#define NS_MK_END }
#define USING_NS_MK using namespace MinamiKotori;
#define NS_MK ::MinamiKotori

#define MK_USE_KEYBOARD true
#define MK_USE_TOUCH true
#define MK_USE_MOUSE true

#define MK_DEBUG true

typedef char mkS8;
typedef short mkS16;
typedef int mkS32;
typedef long long int mkS64;

typedef unsigned char mkU8;
typedef unsigned short mkU16;
typedef unsigned int mkU32;
typedef unsigned long long int mkU64;

typedef float mkF32;
typedef double mkF64;

// This is for when compiling for android.
// std::hash does not work on Enums.
#define MK_INPUTNAME mkU32

#define MK_BESTGIRL "Minami Kotori"
#define MK_BESTGIRLINFO "http://love-live.wikia.com/wiki/Kotori_Minami"

#endif