#include "AnimationHandlerNode.h"
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"
#include "external/json/document.h"
#include "AnimTransAct.h"

USING_NS_CC;
using namespace GinTama;
using namespace RAPIDJSON_NAMESPACE;

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

bool AnimationHandlerNode::transitState(const std::string &_AnimStateName)
{
    if (m_CurrentAnimTransit != _AnimStateName)
    {
        std::unordered_map<std::string, std::vector<AnimTransCondition*>>::iterator it = m_NameActTransMap.find(_AnimStateName);
        if (it != m_NameActTransMap.end())
        {
            // then we will need to iterate through the vector!
            for (std::vector<AnimTransCondition*>::iterator AnimIt = it->second.begin(), AnimEnd = it->second.end(); AnimIt != AnimEnd; ++AnimIt)
            {
                if ((*AnimIt)->m_TransCondition == "" || (*AnimIt)->m_TransCondition == m_CurrentAnimTransit)
                {
                    // just run the action lol
                    runAction((*AnimIt)->m_ActionPtr);
                    m_CurrentAnimTransit = (*AnimIt)->m_TransName;
                    return true;
                }
            }
        }
    }
    return false;
}

bool AnimationHandlerNode::playAnim(const std::string &_AnimName)
{
    std::unordered_map<std::string, Animation*>::iterator it = m_NameAnimMap.find(_AnimName);
    if (it != m_NameAnimMap.end())
    {
        m_CurrentAnim = it->second;
        m_CurrentAnimName = it->first;
        if (!m_SpriteNode)
        {
            // need to ensure there will be linking
            m_SpriteNode = dynamic_cast<Sprite*>(getParent());
            if (!m_SpriteNode)
            {
                log("Something is wrong with this node as there is no reference to the sprite!");
                return false;
            }
        }
        if (m_CurrentAnim)
        {
            m_SpriteNode->stopAction(m_CurrentAnimate);
            // need to retain the animation!
            m_CurrentAnim->retain();
        }
        m_CurrentAnimate = m_SpriteNode->runAction(Animate::create(m_CurrentAnim));
        m_HistoryOfStates.push_back(_AnimName);
        return true;
    }
    return false;
}

bool AnimationHandlerNode::insertAnimSheet(const std::string &_AnimStateName, cocos2d::Animation *_AnimState)
{
    switch (m_NameAnimMap.count(_AnimStateName))
    {
    case 1:
        // This means there is an animation with that state name exists so do not insert it
        return false;
        break;
    default:
        // making sure it is not a null pointer
        if (_AnimState)
        {
            m_NameAnimMap.insert(std::pair<std::string, Animation*>(_AnimStateName, _AnimState));
            _AnimState->retain();
            if (!m_CurrentAnim)
            {
                // then we shall start the animation in this state since it is not yet initialized
                playAnim(_AnimStateName);
            }
        }
        break;
    }
    return false;
}

bool AnimationHandlerNode::insertAnimSheet(const std::string &_AnimStateName, const std::string &_fileName, const cocos2d::Rect &_totalPixelSize, const cocos2d::Rect &_SpriteInterval, const float &_framePerSec, const int &_loopTimes)
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
            SpriteFrame *zeSpr = SpriteFrame::create(_fileName, Rect(zeMinXSpr + (zeSpriteWidth * j), zeMinYSpr + (i * zeSpriteHeight), zeSpriteWidth, zeSpriteHeight), false, Vec2(0,0), Size(zeSpriteWidth, zeSpriteHeight));
            zeNewAnim->addSpriteFrame(zeSpr);
        }
    }
    return insertAnimSheet(_AnimStateName, zeNewAnim);
}

bool AnimationHandlerNode::insertAnimFromCache(const std::string &_AnimStateName)
{
    auto zeAnim = AnimationCache::getInstance()->getAnimation(_AnimStateName);
    if (zeAnim)
    {
        return insertAnimSheet(_AnimStateName, zeAnim);
    }
    log("Unsuccessful getting the animation cache of this animation: {0}", _AnimStateName);
    return false;
}

bool AnimationHandlerNode::insertAnimFromSPlist(const std::string &_AnimStateName, const float &_framePerSec, const int &_loopTimes, const std::vector<std::string> &_AnimSprRef)
{
    Animation *zeNewAnim = Animation::create();
    auto zeSprCache = SpriteFrameCache::getInstance();
    for (std::vector<std::string>::const_iterator it = _AnimSprRef.begin(), end = _AnimSprRef.end(); it != end; ++it)
    {
        SpriteFrame* zeSprFrame = zeSprCache->getSpriteFrameByName(*it);
        if (zeSprFrame)
        {
            zeNewAnim->addSpriteFrame(zeSprFrame);
        }
        else
        {
            log("Failed to load sprite at insertAnimFromSPlist");
            return false;
        }
    }
    zeNewAnim->setDelayPerUnit(_framePerSec);
    // Then decides what to do with the animation stats
    if (_loopTimes > 0)
    {
        zeNewAnim->setLoops(_loopTimes);
    }
    else
        zeNewAnim->setUnlimitedLoop(true);
    return insertAnimSheet(_AnimStateName, zeNewAnim);
}

bool AnimationHandlerNode::initWithJSON_tag(const std::string &_JsonTag)
{
	// Will be initialized with the PList filetype
	FILE *zefp = fopen(_JsonTag.c_str(), "r");
	// this means there can only be an array of 65536 characters!
	char zeBuffer[65536];
	FileReadStream zeIS(zefp, zeBuffer, sizeof(zeBuffer));
	RAPIDJSON_NAMESPACE::Document zeAnimDoc;
	zeAnimDoc.ParseStream(zeIS);
	fclose(zefp);
    auto zeAnimLoadingArr = zeAnimDoc.FindMember("AnimationLoading");
	// So we will need to iterate through the member and get the data so that it will work and make sure it is an array
	if (zeAnimLoadingArr->value.IsArray())
	{
		for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator it = zeAnimLoadingArr->value.Begin(), end = zeAnimLoadingArr->value.End(); it != end; ++it)
		{
			// And then we get the hardcoded string members from it!
            if (it->IsObject())
            {
                std::string zeStateName = it->FindMember("StateName")->value.GetString();
                float zeFramesPerSec = it->FindMember("FPS")->value.GetFloat();
                int zeNumLoop = it->FindMember("loop")->value.GetInt();
                auto zeSpriteRefArr = it->FindMember("SpriteReference")->value.GetArray();
                std::vector<std::string> zeVectorOfSprFile;
                // for an array, we will need the ConstValueIterator
                for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator sprRefIt = zeSpriteRefArr.Begin(), sprRefEnd = zeSpriteRefArr.End(); sprRefIt != sprRefEnd; ++sprRefIt)
                {
                    zeVectorOfSprFile.push_back(sprRefIt->GetString());
                }
                // then we call the function when all of the pieces are in place!
                insertAnimFromSPlist(zeStateName, zeFramesPerSec, zeNumLoop, zeVectorOfSprFile);
            }
		}
	}
    // To load the animation transition between the states
    auto zeAnimTransLoad = zeAnimDoc.FindMember("AnimationTransition");
    if (zeAnimTransLoad->value.IsArray())
    {
        for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator it = zeAnimTransLoad->value.Begin(), end = zeAnimTransLoad->value.End(); it != end; ++it)
        {
            // we get the name and transit condition
            std::string zeTransName = it->FindMember("TransitName")->value.GetString();
            std::string zeTransCond = it->FindMember("TransitCondition")->value.GetString();
            auto zeActSeqJSON = it->FindMember("ActionSequence");
            cocos2d::Vector<cocos2d::FiniteTimeAction*> zeActionVector;
            std::string zeActName;
            // then we iterate through the array in this object!
            for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator AnimTransIt = zeActSeqJSON->value.Begin(), AnimTransEnd = zeActSeqJSON->value.End(); AnimTransIt != AnimTransEnd; ++AnimTransIt)
            {
                // then we iterate though the member
                for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator ActionTransIt = AnimTransIt->MemberBegin(), ActionTransEnd = AnimTransIt->MemberEnd(); ActionTransIt != ActionTransEnd; ++ActionTransIt)
                {
                    if (ActionTransIt->name == "ActionName")
                        zeActName = ActionTransIt->value.GetString();
                    else if (ActionTransIt->name == "ActionValue")
                    {
                        // Then check what is the action name then extract the variable!
                        if (zeActName == "AnimTrans")
                        {
                            zeActionVector.pushBack(AnimTransAct::create(ActionTransIt->value.GetString(), true, this));
                        }
                        else if (zeActName == "Delay")
                        {
                            zeActionVector.pushBack(DelayTime::create(ActionTransIt->value.GetFloat()));
                        }
                    }
                }
            }
            insertAnimTransSeq(zeTransName, zeActionVector, zeTransCond);
        }
    }
    return true;
}

bool AnimationHandlerNode::insertAnimTransSeq(const std::string &_AnimTransName, const cocos2d::Vector<cocos2d::FiniteTimeAction *> &_sequenceOfAct, const std::string &_conditionStr)
{
    Sequence *zeSeqAct = Sequence::create(_sequenceOfAct);
    return insertAnimTransSeq(_AnimTransName, zeSeqAct, _conditionStr);
}

bool AnimationHandlerNode::insertAnimTransSeq(const std::string &_AnimTransName, cocos2d::Sequence* _sequenceAct, const std::string &_conditionStr)
{
    _sequenceAct->retain();
    AnimTransCondition *zeTransCondition = new AnimTransCondition(_AnimTransName, _conditionStr, _sequenceAct);
    if (!m_NameActTransMap.count(_AnimTransName))
    {
        m_NameActTransMap.insert(std::pair<std::string, std::vector<AnimTransCondition*>>(_AnimTransName, { zeTransCondition }));
    }
    else
    {
        m_NameActTransMap[_AnimTransName].push_back(zeTransCondition);
    }
    return true;
}

AnimationHandlerNode::AnimationHandlerNode() :
    m_CurrentAnim(nullptr)
    , m_SpriteNode(nullptr)
    , m_CurrentAnimate(nullptr)
{
	setTag(69);
}


AnimationHandlerNode::~AnimationHandlerNode()
{
    for (std::unordered_map<std::string, cocos2d::Animation*>::iterator it = m_NameAnimMap.begin(), end = m_NameAnimMap.end(); it != end; ++it)
    {
        // just in case the animation is not released
        it->second->release();
        it->second->release();
        it->second->release();
        it->second->release();
    }
    m_NameAnimMap.clear();
    m_CurrentAnim = nullptr;
    for (std::unordered_map<std::string, std::vector<AnimTransCondition*>>::iterator it = m_NameActTransMap.begin(), end = m_NameActTransMap.end(); it != end; ++it)
    {
        for (std::vector<AnimTransCondition*>::iterator AnimIt = it->second.begin(), AnimEnd = it->second.end(); AnimIt != AnimEnd; ++AnimIt)
        {
            (*AnimIt)->m_ActionPtr->release();
            //(*AnimIt)->m_ActionPtr->release();
        }
    }
    m_NameActTransMap.clear();
}

AnimationHandlerNode *AnimationHandlerNode::create()
{
    AnimationHandlerNode *zeNewNode = new AnimationHandlerNode();
    zeNewNode->init();
    zeNewNode->autorelease();
    return zeNewNode;
}

