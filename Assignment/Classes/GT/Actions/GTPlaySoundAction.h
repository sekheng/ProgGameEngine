#ifndef GT_PLAYSOUNDACTION_H
#define GT_PLAYSOUNDACTION_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "GT/Common/GTMacros.h"
#include "GT/Audio/GTSimperMusicSys.h"

USING_NS_CC;

NS_GT_BEGIN

class GTPlaySoundAction : public ActionInstant
{
    typedef ActionInstant Super;

private:
    const gtString m_SoundName;

    CC_DISALLOW_COPY_AND_ASSIGN(GTPlaySoundAction);

public:
    static GTPlaySoundAction* Create(const gtString& _soundName)
    {
        GTPlaySoundAction* newAction = new (std::nothrow) GTPlaySoundAction(_soundName);
        if (newAction)
        {
            newAction->autorelease();
            return newAction;
        }

        CC_SAFE_DELETE(newAction);
        return NULL;
    }

    // SIMI LANJIAO! The original parameter is called _time.
    // _time your head lah _time. Chao Ji Bai. It is the percentage of 
    // the action done lah! Like if the duration is 10 seconds, and 4 seconds has passed,
    // the value is 0.4.
    virtual void update(gtF32 _percentageComplete)
    {
        Super::update(_percentageComplete);
        GTSimperMusicSys::GetInstance()->playSound(m_SoundName);
    }

CC_CONSTRUCTOR_ACCESS:
    GTPlaySoundAction(const gtString& _soundName) : m_SoundName(_soundName) {}
    virtual ~GTPlaySoundAction() {}
};

NS_GT_END

#endif // GT_PLAYSOUNDACTION_H