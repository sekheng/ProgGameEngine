#include "GTAnimTransAct.h"
using namespace GinTama;

GTAnimTransAct* GTAnimTransAct::create()
{
    GTAnimTransAct *zeNewAct = new (std::nothrow) GTAnimTransAct();
    zeNewAct->autorelease();
    return zeNewAct;
}

GTAnimTransAct *GTAnimTransAct::create(const std::string &_AnimStateName)
{
    GTAnimTransAct *zeNewAct = new (std::nothrow) GTAnimTransAct();
    zeNewAct->m_AnimName = _AnimStateName;
    zeNewAct->autorelease();
    return zeNewAct;
}

GTAnimTransAct *GTAnimTransAct::create(const std::string &_AnimStateName, const bool &_WaitForComplete, GTAnimationHandlerNode *_parentNode)
{
    GTAnimTransAct *zeNewAct = create(_AnimStateName);
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

GTAnimTransAct *GTAnimTransAct::setAnimStateName(const std::string &_AnimStateName)
{
    m_AnimName = m_AnimName;
    return this;
}

void GTAnimTransAct::startWithTarget(cocos2d::Node *target)
{
    if (!m_targetNode)
    {
        target->stopActionByTag(_tag);
        m_targetNode = static_cast<GTAnimationHandlerNode*>(target);
    }
    // before that we check whether is WaitForComplete is true!
    m_targetNode->playAnim(m_AnimName);
}

void GTAnimTransAct::update(float time)
{
    // stop this action since it is pointless
    m_targetNode->stopAction(this);
}

void GTAnimTransAct::stop()
{
    ActionInterval::stop();
    _done = true;
}

GTAnimTransAct::GTAnimTransAct() : 
    m_targetNode(nullptr)
    , m_AnimName("")
{
    _done = false;
    _tag = 69;
}


GTAnimTransAct::~GTAnimTransAct()
{
}
