#ifndef TERRY_TESTSCENE_H
#define TERRY_TESTSCENE_H

#include "MK/SceneManagement/MKScene.h"

USING_NS_CC;
USING_NS_MK

class Terry_TestScene : public MKScene
{
	typedef MKScene Super;

protected:
	enum BackgroundLayer
	{
		REAR = 0,
		MIDDLE,
		FRONT,

		NUM_BACKGROUNDLAYER,
	};

private:
	EventListenerCustom* m_ButtonListener = nullptr;
	EventListenerCustom* m_ClickListener = nullptr;
	EventListenerCustom* m_AxisListener = nullptr;
	Label* m_InputLabel = nullptr;

	Sprite** m_Backgrounds = nullptr;
	mkF32 m_BackgroundScrollSpeeds[(mkU32)BackgroundLayer::NUM_BACKGROUNDLAYER];

public:
	Terry_TestScene();
	virtual ~Terry_TestScene();

	virtual bool init();
	virtual void update(float _deltaTime) override;

	void SetBackgroundGLProgram(Sprite* _background);
	void SetBackgroundTextureParameters(Sprite* _background);
	void ScrollBackground(Sprite* _background, float _offset);
	void InitialiseBackground();

	void InitialiseInput();
	void OnButton(EventCustom * _event);
	void OnClick(EventCustom * _event);
	void OnAxis(EventCustom * _event);

	// implement the "static create()" method manually
	CREATE_FUNC(Terry_TestScene);
};


#endif