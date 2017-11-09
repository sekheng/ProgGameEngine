#pragma once

#include "2d/CCNode.h"
#include <unordered_map>
#include <string>
#include "cocos2d.h"

/** To handle the transition between the animations that are being created.
* And to be attached to the sprite as a node so that it can handle the transition anytime
* otherwise make sure it has reference to the Sprite class.
* You must also have knowledge regarding how Cocos2dx Animation class works
*/
class AnimationHandlerNode : public cocos2d::Node
{
public:
    static AnimationHandlerNode *create();

    /** To check the current animation state name
    * 
    * @return The current animation name
    */
    std::string getCurrAnimName();

    /** To access the animation
    *
    * @param _AnimStateName to access the hashset animation
    * @return If the hashset contains the animation state name. Otherwise a nullptr
    */
    cocos2d::Animation *getAnimation(const std::string &_AnimStateName);

    /** To transit to other states when referenced by other scripts
    *
    * @param _AnimStateName of the Animation state to transit to
    * @return true if successful. And vice versa
    */
    bool transitState(const std::string &_AnimStateName);

    /** To Insert an animation state based on the name provided the information is filled up at Animation class
    *
    * @param _AnimStateName which is the name you want to give to this animation
    * @param _AnimState which is the animation state
    * @return true if there is no such animation in this node and _AnimState is not null
    */
    bool insertAnimSheet(const std::string &_AnimStateName, cocos2d::Animation *_AnimState);
    /** Does the same as other function but it will be in the event that there will be no animation pointer being passed here. To deal with spritesheet.
    * 
    * @param _AnimStateName the name which you want to give to this animation state
    * @param _totalPixelSize which will be the size of pixel which you want to extract from the sprite sheet.
    * @param _SpriteInterval will be the size of the sprite to start the interval with
    * @param _framePerSec which will be the animation time
    * @param _loopTimes which is the number of times it should be playing. Setting it beyond negative will set the Animation to be infinite times
    * @return true if successful otherwise false
    */
    bool insertAnimSheet(const std::string &_AnimStateName, const std::string &_fileName, const cocos2d::Rect &_totalPixelSize, const cocos2d::Rect &_SpriteInterval, const float &_framePerSec, const int &_loopTimes);
    
    /** This should only be used when the animation and sprite is inside the SpriteFrameCache and AnimationCache
    *
    * @param _AnimStateName is the name of the animation in the animation cache
    * @return true if successful in inserting the animation. false if there is no such animation.
    */
    bool insertAnimFromCache(const std::string &_AnimStateName);

    /** To be used only by SpriteFrameCache as it only takes in Sprite frame references straight from plist
    *
    * @param _AnimStateName is the name of this animation
    * @param _framePerSec is the animation frame in seconds
    * @param _looptimes number of times this should loop. putting it in 0 or below will make it loop infinitely
    * @param _AnimSprRef will be the sprite image reference from plist
    */
    bool insertAnimFromSPlist(const std::string &_AnimStateName, const float &_framePerSec, const int &_loopTimes, const std::vector<std::string> &_AnimSprRef);

    cocos2d::Animation *m_CurrentAnim;
    cocos2d::Sprite *m_SpriteNode;

protected:
    std::unordered_map<std::string, cocos2d::Animation*> m_NameAnimMap;
    std::string m_CurrentAnimName;
    std::vector<std::string> m_HistoryOfStates;
	cocos2d::Action *m_CurrentAnimate;

CC_CONSTRUCTOR_ACCESS:
    AnimationHandlerNode();
    virtual ~AnimationHandlerNode();
};
