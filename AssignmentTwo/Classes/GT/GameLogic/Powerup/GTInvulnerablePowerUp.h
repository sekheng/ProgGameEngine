#ifndef GT_INVULNERABLE_POWERUP_H
#define GT_INVULNERABLE_POWERUP_H

#include "GTPowerUp.h"
#include "../../../MK/Graphics/MKSprite.h"


USING_NS_MK

NS_GT_BEGIN

class GTInvulverablePowerUp : public GTPowerUp
{
protected:
    MKSprite *m_PowerUpSprite;
    static const mkString InvulnerableSpriteFilePath;
    virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
    static GTInvulverablePowerUp *create(MKScene *_scene);

    virtual gtBool init();

    GT_INITIALISECONTACTLISTENER(GTInvulverablePowerUp);
    GT_DEINITIALISECONTACTLISTENER(GTInvulverablePowerUp);

protected:
    GTInvulverablePowerUp(MKScene *_scene);
    virtual ~GTInvulverablePowerUp();
};

NS_GT_END

#endif 
