#ifndef MK_ASSERTIONS_H
#define MK_ASSERTIONS_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MKMacros.h"

#if MK_DEBUG

/*
    Important Note:
    When using MK_ASSERT, add an extra pair of brackets if the condition is a
    function that needs to be ran! Otherwise, if MK_DEBUG is false, the function
    will not be ran.

    E.g. MK_ASSERT((playerData->SaveData(playerData->GetWritablePath()))); <- Correct
    E.g. MK_ASSERTWITHMSG((playerData->SaveData(playerData->GetWritablePath())), "This is a message."); <- Correct
    E.g. MK_ASSERT(playerData->SaveData(playerData->GetWritablePath())); <- Wrong
    E.g. MK_ASSERTWITHMSG(playerData->SaveData(playerData->GetWritablePath()), "This is a message."); <- Wrong

    Reason:
    When MK_DEBUG is false, the definition for this macro is empty.
    That means that the _condition in MK_ASSERT is ignoreed.
    So if _condition is a function, that function will not be called.
    In this case, playerData will not run the SaveData() function.
    However, when we add an extra pair of brackets, what happens is that brackets
    are evaluated first. So this means that BEFORE entering the macro, the function is ran FIRST.
    After the function runs, the function returns a boolean. That boolean is THEN
    used in the macro, which is then ignored.

    There are exceptions:

    E.g. MK_ASSERT(coins->value.IsUint64()); <- Works with or without brackets.
    E.g. MK_ASSERTWITHMSG(coins->value.IsUint64(), "This is a message."); <- Works with or without brackets.

    Reason:
    coins->value.IsUint64() is just to check if coins is an unsigned 64 bit int.
    coins is not modified at all in the IsUint64() function.
    Therefore, when there are no brackets present, the IsUint64() is not ran,
    but it does not matter since it does not change anything whether it ran or not.

    TLDR (Or you don't know what the fuck I'm talking about.):
    If unsure, just always add an extra pair of brackets when using MK_ASSERT.

    E.g. MK_ASSERT((a == b)); <- Do this.
    E.g. MK_ASSERT((variable.Function())); <- Do this.
    E.g. MK_ASSERT(a == b); <- Don't do this.
    E.g. MK_ASSERT(variable.Function()); <- Don't do this.

    E.g. MK_ASSERTWITHMSG((a == b), "This is a message"); <- Do this.
    E.g. MK_ASSERTWITHMSG((variable.Function()), "This is a message"); <- Do this.
    E.g. MK_ASSERTWITHMSG(a == b, "This is a message"); <- Don't do this.
    E.g. MK_ASSERTWITHMSG(variable.Function(), "This is a message"); <- Don't do this.
*/
#define MK_ASSERT(_condition) assert(_condition);
#define MK_ASSERTWITHMSG(_condition, _message) \
	if (!_condition) \
	{ \
        CCLOG(_message); \
		CC_ASSERT(_condition); \
	}
#else
	#define MK_ASSERT(_condition);
	#define MK_ASSERTWITHMSG(_condition, _message);
#endif // MK_DEBUG

#endif // MK_ASSERTIONS_H