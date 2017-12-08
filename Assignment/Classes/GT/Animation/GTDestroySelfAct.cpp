#include "GTDestroySelfAct.h"

USING_NS_GT

GTDestroySelfAct *GTDestroySelfAct::create()
{
    GTDestroySelfAct *zeNewNode = new (std::nothrow)GTDestroySelfAct();
    zeNewNode->autorelease();
    return zeNewNode;
}

GTDestroySelfAct *GTDestroySelfAct::create(cocos2d::Node* _target)
{
    GTDestroySelfAct *zeNewNode = create();
    zeNewNode->m_intentedRemoveNode = _target;
    return zeNewNode;
}

void GTDestroySelfAct::startWithTarget(cocos2d::Node *target)
{
    if (m_intentedRemoveNode)
    {
        m_intentedRemoveNode->getParent()->removeChild(m_intentedRemoveNode);
        m_intentedRemoveNode->release();
    }
    else
    {
        target->getParent()->removeChild(target, true);
        target->release();
    }
}

void GTDestroySelfAct::update(float time)
{

}

GTDestroySelfAct::GTDestroySelfAct() :
    m_intentedRemoveNode(nullptr)
{

}

GTDestroySelfAct::~GTDestroySelfAct()
{

}