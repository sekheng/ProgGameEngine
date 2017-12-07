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
    zeNewAct->m_AnimName = _AnimStateName;
    zeNewAct->autorelease();
    return zeNewAct;
}

AnimTransAct *AnimTransAct::create(const std::string &_AnimStateName, const bool &_WaitForComplete, AnimationHandlerNode *_parentNode)
{
    AnimTransAct *zeNewAct = create(_AnimStateName);
    zeNewAct->m_targetNode = _parentNode;
    switch (_WaitForComplete)
    {
    case true:
        zeNewAct->initWithDuration(_parentNode->getAnimation(_AnimStateName)->getDuration());
        break;
    default:
        break;
    }
    return zeNewAct;
}

AnimTransAct *AnimTransAct::setAnimStateName(const std::string &_AnimStateName)
{
    m_AnimName = m_AnimName;
    return this;
}

void AnimTransAct::startWithTarget(cocos2d::Node *target)
{
    if (!m_targetNode)
    {
        target->stopActionByTag(_tag);
        m_targetNode = static_cast<AnimationHandlerNode*>(target);
    }
    // before that we check whether is WaitForComplete is true!
    m_targetNode->playAnim(m_AnimName);
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
    , m_AnimName("")
{
    _done = false;
    _tag = 69;
}


AnimTransAct::~AnimTransAct()
{
}
