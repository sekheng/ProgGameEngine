#include "AnimationHandlerNode.h"

USING_NS_CC;

std::string AnimationHandlerNode::getCurrAnimName()
{
    return m_CurrentAnimName;
}

Animation *AnimationHandlerNode::getAnimation(const std::string &_AnimStateName)
{
    std::unordered_map<std::string, Animation*>::iterator it = m_NameAnimMap.find(_AnimStateName);
    // If the unordered map can't find anything, it will be the end of m_NameAnimMap
    if (it != m_NameAnimMap.end())
    {
        return it->second;
    }
    return nullptr;
}

bool AnimationHandlerNode::TransitState(const std::string &_AnimStateName)
{
    std::unordered_map<std::string, Animation*>::iterator it = m_NameAnimMap.find(_AnimStateName);
    if (it != m_NameAnimMap.end())
    {
        m_CurrentAnim = it->second;
        m_CurrentAnimName = it->first;
        if (!m_SpriteNode)
        {
            // need to ensure there will be linking
            m_SpriteNode = dynamic_cast<Sprite*>(getParent());
            if (!m_SpriteNode)
                throw;
        }
        switch (m_CurrentAnim->isUnlimitedLoop())
        {
        case true:
            m_SpriteNode->runAction(RepeatForever::create(Animate::create(m_CurrentAnim)));
            break;
        default:
            m_SpriteNode->runAction(Animate::create(m_CurrentAnim));
            break;
        }
        m_HistoryOfStates.push_back(_AnimStateName);
        return true;
    }
    return false;
}

bool AnimationHandlerNode::InsertAnimSheet(const std::string &_AnimStateName, cocos2d::Animation *_AnimState)
{
    switch (m_NameAnimMap.count(_AnimStateName))
    {
    case 1:
        // This means there is an animation with that state name exists so do not insert it
        return false;
        break;
    default:
        // making sure it is not a nullpointer
        if (_AnimState)
        {
            m_NameAnimMap.insert(std::pair<std::string, Animation*>(_AnimStateName, _AnimState));
            if (!m_CurrentAnim)
            {
                // then we shall start the animation in this state since it is not yet initialized
                TransitState(_AnimStateName);
            }
        }
        break;
    }
    return false;
}

bool AnimationHandlerNode::InsertAnimSheet(const std::string &_AnimStateName, const std::string &_fileName, const cocos2d::Rect &_totalPixelSize, const cocos2d::Rect &_SpriteInterval, const float &_framePerSec, const int &_loopTimes)
{
    // we will make use of this and start create the Animation class then pass it to the other overloaded function
    Animation *zeNewAnim = Animation::create();
    zeNewAnim->setDelayPerUnit(_framePerSec);
    // set the number of times it loop
    if (_loopTimes < 0)
    { 
        zeNewAnim->setUnlimitedLoop(true);
    }
    else
    {
        zeNewAnim->setLoops(_loopTimes);
    }
    // The complex algorithm to calculate the sprite in that certain sprite sheet texture position
    float zeMaxYSpr = _totalPixelSize.getMaxY();
    float zeMinYSpr = _totalPixelSize.getMinY();
    float zeMaxXSpr = _totalPixelSize.getMaxX();
    float zeMinXSpr = _totalPixelSize.getMinX();
    float zeSpriteWidth = _SpriteInterval.getMaxX();
    float zeSpriteHeight = _SpriteInterval.getMaxY();
    int zeRowOfSpr = (zeMaxYSpr - zeMinYSpr) / zeSpriteHeight;
    int zeColOfSpr = (zeMaxXSpr - zeMinXSpr) / zeSpriteWidth;
    // from there we try to get the interval size
    for (int i = 0; i < zeRowOfSpr; ++i)
    {
        for (int j = 0; j < zeColOfSpr; ++j)
        {
            SpriteFrame *zeSpr = SpriteFrame::create(_fileName, Rect(zeMinXSpr + (zeSpriteWidth * j), zeMinYSpr + (i * zeSpriteHeight), zeSpriteWidth, zeSpriteHeight));
            zeNewAnim->addSpriteFrame(zeSpr);
        }
    }
    return InsertAnimSheet(_AnimStateName, zeNewAnim);
}

AnimationHandlerNode::AnimationHandlerNode() :
    m_CurrentAnim(nullptr)
    , m_SpriteNode(nullptr)
{
}


AnimationHandlerNode::~AnimationHandlerNode()
{
    m_NameAnimMap.clear();
    m_CurrentAnim = nullptr;
}

AnimationHandlerNode *AnimationHandlerNode::create()
{
    AnimationHandlerNode *zeNewNode = new AnimationHandlerNode();
    zeNewNode->init();
    zeNewNode->autorelease();
    return zeNewNode;
}

