// Include STL
#include <iostream>

// Include MK
#include "Terry_TestScene.h"

Terry_TestScene::Terry_TestScene()
{
}

Terry_TestScene::~Terry_TestScene()
{
}

bool Terry_TestScene::init()
{
	if (!Super::init())
	{
		return false;
	}

	InitialiseInput();
	InitialiseBackground();
	scheduleUpdate();

	return true;
}

void Terry_TestScene::SetBackgroundGLProgram(Sprite* _background)
{
	auto glProgram = new GLProgram();
	glProgram->initWithFilenames("Shaders/TextureOffset.vsh", "Shaders/TextureOffset.fsh");
	glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	glProgram->link();
	glProgram->updateUniforms();

	GLProgramState* programState = GLProgramState::getOrCreateWithGLProgram(glProgram);
	_background->setGLProgram(glProgram);
	_background->setGLProgramState(programState);

	SetBackgroundTextureParameters(_background);
}

void Terry_TestScene::SetBackgroundTextureParameters(Sprite* _background)
{
	Texture2D::TexParams textureParameters;
	textureParameters.minFilter = GL_NEAREST;
	textureParameters.magFilter = GL_NEAREST;
	textureParameters.wrapS = GL_REPEAT;
	textureParameters.wrapT = GL_REPEAT;
	_background->getTexture()->setTexParameters(textureParameters);
}

void Terry_TestScene::ScrollBackground(Sprite* _background, float _offset)
{
	Rect scrollRect = _background->getTextureRect();
	scrollRect.origin.x += _offset;
	_background->setTextureRect(scrollRect);
}

void Terry_TestScene::InitialiseBackground()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_Backgrounds = new Sprite*[(mkU32)BackgroundLayer::NUM_BACKGROUNDLAYER];
	for (mkU32 i = 0; i < NUM_BACKGROUNDLAYER; ++i)
	{
		m_Backgrounds[i] = nullptr;
	}

	m_Backgrounds[REAR] = Sprite::create("Backgrounds/Background_Rear.jpg");

	// Set the background texture parameters so that the texture tiling is repeat instead of stretch.
	for (mkU32 i = 0; i < NUM_BACKGROUNDLAYER; ++i)
	{
		if (m_Backgrounds[i] == nullptr) { continue; }

		m_Backgrounds[i]->setPosition(0.0f, 0.0f);
		m_Backgrounds[i]->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f));
		SetBackgroundTextureParameters(m_Backgrounds[i]);
		this->addChild(m_Backgrounds[i]);
	}

	// Scroll Speeds
	m_BackgroundScrollSpeeds[REAR] = 500.0f;
	m_BackgroundScrollSpeeds[MIDDLE] = 500.0f;
	m_BackgroundScrollSpeeds[FRONT] = 500.0f;
}

void Terry_TestScene::InitialiseInput()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_InputLabel = Label::createWithTTF("Input Debug Label", "fonts/Marker Felt.ttf", 24);
	// position the label on the center of the screen
	m_InputLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - m_InputLabel->getContentSize().height - (visibleSize.height / 4)));
	this->addChild(m_InputLabel, 1);

	MKInputManager::GetInstance()->SetCurrentContext(MK_CONTEXT1);
	m_ButtonListener = MKInputManager::GetInstance()->CreateEventListener<MKInputButton>(CC_CALLBACK_1(Terry_TestScene::OnButton, this));
	m_ClickListener = MKInputManager::GetInstance()->CreateEventListener<MKInputClick>(CC_CALLBACK_1(Terry_TestScene::OnClick, this));
	m_AxisListener = MKInputManager::GetInstance()->CreateEventListener<MKInputAxis>(CC_CALLBACK_1(Terry_TestScene::OnAxis, this));
}

void Terry_TestScene::OnButton(EventCustom* _event)
{
	MKInputButton* buttonEvent = static_cast<MKInputButton*>(_event->getUserData());

	std::string inputName;
	switch (buttonEvent->m_InputName)
	{
	case MinamiKotori::MKInputName::JUMP:
		inputName = "Jump";
		break;
	case MinamiKotori::MKInputName::SLIDE:
		inputName = "Slide";
		break;
	case MinamiKotori::MKInputName::SMASH:
		inputName = "Smash";
		break;
	default:
		inputName = "Unknown InputName";
		break;
	}

	std::string buttonState;
	switch (buttonEvent->m_ButtonState)
	{
	case MinamiKotori::MKInputButton::ButtonState::PRESS:
		buttonState = "Pressed";
		break;
	case MinamiKotori::MKInputButton::ButtonState::HOLD:
		buttonState = "Held";
		break;
	case MinamiKotori::MKInputButton::ButtonState::RELEASE:
		buttonState = "Released";
		break;
	default:
		buttonState = "Unknown ButtonState";
		break;
	}

	std::string logMessage = inputName + " " + buttonState;
	m_InputLabel->setString(logMessage);

	CCLOG(logMessage.c_str());
}

void Terry_TestScene::OnClick(EventCustom* _event)
{
	MKInputClick* clickEvent = static_cast<MKInputClick*>(_event->getUserData());

	std::string inputName;
	switch (clickEvent->m_InputName)
	{
	case MinamiKotori::MKInputName::JUMP:
		inputName = "Jump";
		break;
	case MinamiKotori::MKInputName::SLIDE:
		inputName = "Slide";
		break;
	case MinamiKotori::MKInputName::SMASH:
		inputName = "Smash";
		break;
	default:
		inputName = "Unknown InputName";
		break;
	}

	std::string buttonState;
	switch (clickEvent->m_ButtonState)
	{
	case MinamiKotori::MKInputButton::ButtonState::PRESS:
		buttonState = "Pressed";
		break;
	case MinamiKotori::MKInputButton::ButtonState::HOLD:
		buttonState = "Held";
		break;
	case MinamiKotori::MKInputButton::ButtonState::RELEASE:
		buttonState = "Released";
		break;
	default:
		buttonState = "Unknown ButtonState";
		break;
	}

	std::string logMessage = inputName + " " + buttonState;
	m_InputLabel->setString(logMessage);

	CCLOG(logMessage.c_str());
}

void Terry_TestScene::OnAxis(EventCustom* _event)
{
	MKInputAxis* axisEvent = static_cast<MKInputAxis*>(_event->getUserData());

	std::string inputName;
	switch (axisEvent->m_InputName)
	{
	case MinamiKotori::MKInputName::JUMP:
		inputName = "Jump";
		break;
	case MinamiKotori::MKInputName::SLIDE:
		inputName = "Slide";
		break;
	case MinamiKotori::MKInputName::SMASH:
		inputName = "Smash";
		break;
	default:
		inputName = "Unknown InputName";
		break;
	}

	std::string axisValue;
	axisValue += axisEvent->m_AxisValue;

	std::string logMessage = inputName + " " + axisValue;
	m_InputLabel->setString(logMessage);
}

void Terry_TestScene::update(float _deltaTime)
{
	for (mkU32 i = 0; i < NUM_BACKGROUNDLAYER; ++i)
	{
		//ScrollBackground(m_Backgrounds[i], m_BackgroundScrollSpeeds[i]);
	}

	ScrollBackground(m_Backgrounds[REAR], m_BackgroundScrollSpeeds[REAR] * _deltaTime);
}