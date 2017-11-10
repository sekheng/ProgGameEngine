#include "AnimTransAct.h"
using namespace GinTama;

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
    target->stopActionByTag(_tag);
    m_targetNode = static_cast<AnimationHandlerNode*>(target);
    m_targetNode->transitState(m_transitStateName);
    stop();
}

void AnimTransAct::update(float time)
{
    // stop this action since it is pointless
    m_targetNode->stopAction(this);
}

void AnimTransAct::stop()
{
    ActionInterval::stop();
    _done = true;
}

AnimTransAct::AnimTransAct() : 
    m_targetNode(nullptr)
    , m_transitStateName("")
{
    _done = false;
    _tag = 69;
}


AnimTransAct::~AnimTransAct()
{
}
