#ifndef MK_SCENE_DERIVED_H
#define MK_SCENE_DERIVED_H

// Include MK
#include "MKScene.h"

NS_MK_BEGIN

class MKSceneManager;

class MKSceneDerived : public MKScene
{
	typedef MKScene Super;

private:
	float m_SceneChangeCounter = 3.0f;

public:
	MKSceneDerived() {}
	virtual ~MKSceneDerived() {}

	virtual bool init();
	virtual void update(float _deltaTime) override;

	// implement the "static create()" method manually
	CREATE_FUNC(MKSceneDerived);
};

NS_MK_END

#endif