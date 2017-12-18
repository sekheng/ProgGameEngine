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
    if (!m_intentedRemoveNode)
        m_intentedRemoveNode = target;
    if (m_intentedRemoveNode->getReferenceCount() > 0)
    {
        m_intentedRemoveNode->getParent()->removeChild(m_intentedRemoveNode);
        // TODO: for some reasons, this will cause a crash on IOS. Will need further investigation with this!
        //m_intentedRemoveNode->release();
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
