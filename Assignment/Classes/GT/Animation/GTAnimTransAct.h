#pragma once

#include "cocos2d.h"
#include "GTAnimationHandlerNode.h"

namespace GinTama
{
    /** To provide a simper transitioning between the actions so that it will be able to make use of other ActionInterval such as DelayTime
    * Should only be used by AnimationHandlerNode! so it will only play the animation
    */
    class GTAnimTransAct : public cocos2d::ActionInterval
    {
    public:
        static GTAnimTransAct* create();
        static GTAnimTransAct* create(const std::string &_AnimStateName);
        static GTAnimTransAct* create(const std::string &_AnimStateName, const bool &_WaitForComplete, GTAnimationHandlerNode *_parentNode);

        GTAnimTransAct* setAnimStateName(const std::string &_AnimStateName);

        virtual void startWithTarget(cocos2d::Node *target);
        /** Wait till i find a purpose with this
        */
        virtual void update(float time);
        virtual void stop();

    CC_CONSTRUCTOR_ACCESS:
        virtual ~GTAnimTransAct();
    protected:
        GTAnimTransAct();

        GTAnimationHandlerNode *m_targetNode;
        std::string m_AnimName;
    };
}
