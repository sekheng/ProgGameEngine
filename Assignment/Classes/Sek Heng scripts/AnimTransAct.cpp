#include "AnimTransAct.h"

AnimTransAct* AnimTransAct::create()
{
    AnimTransAct *zeNewAct = new AnimTransAct();
    zeNewAct->autorelease();
    return zeNewAct;
}

AnimTransAct *AnimTransAct::create(const std::string &_AnimStateName)
{
    AnimTransAct *zeNewAct = new AnimTransAct();
    zeNewAct->m_transitStateName = _AnimStateName;
    zeNewAct->autorelease();
    return zeNewAct;
}

AnimTransAct *AnimTransAct::setAnimStateName(const std::string &_AnimStateName)
{
    m_transitStateName = m_transitStateName;
    return this;
}

void AnimTransAct::startWithTarget(cocos2d::Node *target)
{
    m_targetNode = static_cast<AnimationHandlerNode*>(target);
    m_targetNode->TransitState(m_transitStateName);
}

void AnimTransAct::update(float time)
{
    ActionInterval::update(time);
}

void AnimTransAct::stop()
{
    ActionInterval::stop();
}

AnimTransAct::AnimTransAct() : 
    m_targetNode(nullptr)
    , m_transitStateName("")
{
}


AnimTransAct::~AnimTransAct()
{
}
