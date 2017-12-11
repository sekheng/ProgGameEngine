#ifndef GT_REMOVEFROMPARENT_H
#define GT_REMOVEFROMPARENT_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "..\..\GT\Common\GTMacros.h"

USING_NS_CC;

NS_GT_BEGIN

class GTRemoveFromParentAction : public ActionInstant
{
    typedef ActionInstant Super;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(GTRemoveFromParentAction);

public:
    static GTRemoveFromParentAction* Create()
    {
        GTRemoveFromParentAction* newAction = new (std::nothrow) GTRemoveFromParentAction();
        if (newAction)
        {
            newAction->autorelease();
            return newAction;
        }

        CC_SAFE_DELETE(newAction);
        return NULL;
    }

    // SIMI LANJIAO! The original parameter is called _time.
    // _time your head lah _time. Chao Ji Bai. It is the percentage of 
    // the action done lah! Like if the duration is 10 seconds, and 4 seconds has passed,
    // the value is 0.4.
    virtual void update(gtF32 _percentageComplete)
    {
        Super::update(_percentageComplete);
        if (_target) { _target->removeFromParent(); }
    }

CC_CONSTRUCTOR_ACCESS:
    GTRemoveFromParentAction() {}
    virtual ~GTRemoveFromParentAction() {}
};

NS_GT_END

#endif // GT_REMOVEFROMPARENT_H