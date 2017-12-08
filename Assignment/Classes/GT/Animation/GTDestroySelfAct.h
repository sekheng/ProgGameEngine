#pragma once

#include "cocos2d.h"
#include "GT/Common/GTMacros.h"

NS_GT_BEGIN
/** What it will does is to tell the node remove itself from the parent
*/
class GTDestroySelfAct : public cocos2d::ActionInterval
{
public:
    static GTDestroySelfAct* create();
    static GTDestroySelfAct* create(cocos2d::Node* _target);

    virtual void startWithTarget(cocos2d::Node *target);
    virtual void update(float time);

protected:
    cocos2d::Node* m_intentedRemoveNode;

    GTDestroySelfAct();
    virtual ~GTDestroySelfAct();
};
NS_GT_END