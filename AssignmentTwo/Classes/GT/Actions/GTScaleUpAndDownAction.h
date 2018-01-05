#ifndef GT_SCALEUPANDDOWN_H
#define GT_SCALEUPANDDOWN_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "../../GT/Common/GTMacros.h"

// Include MK
#include "../../MK/Common/MKMathsHelper.h"

// Include STL
#include <cmath>

USING_NS_CC;

NS_GT_BEGIN

class GTScaleUpAndDownAction : public cocos2d::ActionInterval
{
    typedef cocos2d::ActionInterval Super;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(GTScaleUpAndDownAction);

protected:
    gtF32 m_MinScale = -1.0f;
    gtF32 m_MaxScale = -1.0f;

    // 1 Speed means that it scales and shrinks 1 time per second.
    gtF32 m_StartSpeed = -1.0f;
    gtF32 m_EndSpeed = -1.0f;

public:
    static GTScaleUpAndDownAction* Create(gtF32 _duration, gtF32 _minScale, gtF32 _maxScale, gtF32 _startSpeed, gtF32 _endSpeed)
    {
        GTScaleUpAndDownAction* newAction = new (std::nothrow) GTScaleUpAndDownAction();
        if (newAction && newAction->initWithDuration(_duration, _minScale, _maxScale, _startSpeed, _endSpeed))
        {
            newAction->autorelease();
            return newAction;
        }

        CC_SAFE_DELETE(newAction);
        return NULL;
    }

    // SIMI LANJIAO! The original parameter is called time.
    // time your head lah time. Chao Ji Bai. It is the percentage of 
    // the action done lah! Like if the duration is 10 seconds, and 4 seconds has passed,
    // the value is 0.4.
    virtual void update(gtF32 _percentageComplete) override
    {
        Super::update(_percentageComplete);
        if (_target == NULL) { return; }

        gtF32 midScale = (m_MaxScale + m_MinScale) * 0.5f;
        gtF32 halfScaleDifference = 0.5f * (m_MaxScale - m_MinScale);
        gtF32 currentSpeed = m_StartSpeed + (m_EndSpeed - m_StartSpeed) * _percentageComplete;
        gtF32 timePassed = _percentageComplete * getDuration();

        _target->setScale(midScale + halfScaleDifference * std::sin(timePassed * MKMathsHelper::TWO_PI * currentSpeed));
    }

CC_CONSTRUCTOR_ACCESS:
    GTScaleUpAndDownAction() {}
    virtual ~GTScaleUpAndDownAction() {}

    gtBool initWithDuration(gtF32 _duration, gtF32 _minScale, gtF32 _maxScale, gtF32 _startSpeed, gtF32 _endSpeed)
    {
        if (!Super::initWithDuration(_duration)) { return false; }

        CC_ASSERT(_duration > 0.0f);
        CC_ASSERT(_minScale <= _maxScale);

        m_MinScale = _minScale;
        m_MaxScale = _maxScale;
        m_StartSpeed = _startSpeed;
        m_EndSpeed = _endSpeed;

        return true;
    }
};

NS_GT_END

#endif // GT_SCALEUPANDDOWN_H