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

AnimTransAct::AnimTransAct() : 
    m_targetNode(nullptr)
{
}


AnimTransAct::~AnimTransAct()
{
}
