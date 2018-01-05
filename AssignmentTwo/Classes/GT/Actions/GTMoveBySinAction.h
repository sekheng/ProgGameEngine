#ifndef GT_MOVEBYSINACTTION_H
#define GT_MOVEBYSINACTTION_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "../../GT/Common/GTMacros.h"

// Include MK
#include "../../MK/Common/MKMathsHelper.h"

// Include STL
#include <cmath>

NS_GT_BEGIN

class GTMoveBySinAction : public cocos2d::ActionInterval
{
    typedef cocos2d::ActionInterval Super;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(GTMoveBySinAction);

protected:
    gtF32 m_PhaseShift = 0.0f;
    gtF32 m_Amplitude = 1.0f;
    gtF32 m_Frequency = 1.0f;
    cocos2d::Vec3 m_Direction;
    gtF32 m_PreviousDisplacementMagnitude = 0.0f;

public:
    static GTMoveBySinAction* Create(gtF32 _duration = 1.0f, gtF32 _phaseShift = 0.0f, gtF32 _amplitude = 1.0f, gtF32 _frequency = 1.0f, const cocos2d::Vec3& _direction = cocos2d::Vec3(0.0f, 1.0f, 0.0f))
    {
        GTMoveBySinAction* newAction = new (std::nothrow) GTMoveBySinAction();
        if (newAction && newAction->initWithDuration(_duration, _phaseShift, _amplitude, _frequency, _direction))
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

        gtF32 displacementMagnitude = sin((_percentageComplete * MKMathsHelper::TWO_PI * m_Frequency) + m_PhaseShift) * m_Amplitude;
        cocos2d::Vec3 displacement = m_Direction * (displacementMagnitude - m_PreviousDisplacementMagnitude);
        m_PreviousDisplacementMagnitude = displacementMagnitude;

        cocos2d::Vec3 newPosition = displacement + _target->getPosition3D();
        _target->setPosition3D(newPosition);
    }

    virtual void startWithTarget(Node* _targetNode) override
    {
        Super::startWithTarget(_targetNode);
        m_PreviousDisplacementMagnitude = 0.0f;
    }

CC_CONSTRUCTOR_ACCESS:
    GTMoveBySinAction() {}
    virtual ~GTMoveBySinAction() {}

    gtBool initWithDuration(gtF32 _duration, gtF32 _phaseShift, gtF32 _amplitude, gtF32 _frequency, const cocos2d::Vec3& _direction)
    {
        if (!Super::initWithDuration(_duration)) { return false; }

        CC_ASSERT(_duration > 0.0f);

        m_PhaseShift = _phaseShift;
        m_Amplitude = _amplitude;
        m_Frequency = _frequency;
        m_Direction = _direction.getNormalized();
        m_PreviousDisplacementMagnitude = 0.0f;

        return true;
    }
};

NS_GT_END

#endif // GT_MOVEBYSINACTTION_H