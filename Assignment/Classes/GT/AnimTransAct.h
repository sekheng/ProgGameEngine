#pragma once

#include "cocos2d.h"
#include "GT/AnimationHandlerNode.h"

namespace GinTama
{
    /** To provide a simper transitioning between the actions so that it will be able to make use of other ActionInterval such as DelayTime
    * Should only be used by AnimationHandlerNode! so it will only play the animation
    */
    class AnimTransAct : public cocos2d::ActionInterval
    {
    public:
        static AnimTransAct* create();
        static AnimTransAct* create(const std::string &_AnimStateName);
        static AnimTransAct* create(const std::string &_AnimStateName, const bool &_WaitForComplete, AnimationHandlerNode *_parentNode);

        AnimTransAct* setAnimStateName(const std::string &_AnimStateName);

        virtual void startWithTarget(cocos2d::Node *target);
        /** Wait till i find a purpose with this
        */
        virtual void update(float time);
        virtual void stop();

    CC_CONSTRUCTOR_ACCESS:
        virtual ~AnimTransAct();
    protected:
        AnimTransAct();

        AnimationHandlerNode *m_targetNode;
        std::string m_AnimName;
    };
}
