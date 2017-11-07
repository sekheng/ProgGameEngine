// Include Cocos
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"
#include "Sek Heng scripts/AnimationHandlerNode.h"
#include "Sek Heng scripts/AnimTransAct.h"
#include "SimpleAudioEngine.h"

// Include Input Device Handlers
#include "Common/MKMacros.h"
#include "Input/MKKeyboardHandler.h"

// Include Assignment
#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	// Input Testing
	MKInputManager::GetInstance()->SetCurrentContext(MK_CONTEXT3);
	m_ButtonListener = MKInputManager::GetInstance()->CreateEventListener<MKInputButton>(CC_CALLBACK_1(HelloWorld::OnButtonInput, this));

	// Adding Inputs during runtime test.
	{
		MKKeyboardHandler* keyboardHandler = MKKeyboardHandler::GetInstance();
		mkU64 jumpMask = MKInputManager::GenerateMask(MK_CONTEXT0, 0x0001, (mkU32)EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
		MKInputManager::GetInstance()->GetInputDefinition(MKInputName::SMASH)->Register1(
			CC_CALLBACK_2(MKKeyboardHandler::RegisterButton, keyboardHandler),
			CC_CALLBACK_2(MKKeyboardHandler::UnregisterButton, keyboardHandler),
			jumpMask);
	}

    // Trying to figure out the Lua stuff

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    //// add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");
    //auto sprite = Sprite::create("mainspritecharaidlespritesheet.png", Rect(64, 0, 64, 64));
    //if (sprite == nullptr)
    //{
    //    problemLoading("'HelloWorld.png'");
    //}
    //else
    //{
    //    // position the sprite on the center of the screen
    //    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //    // add the sprite as a child to this layer
    //    this->addChild(sprite, 0);
    //}

    LuaEngine *luaEngine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(luaEngine);
    lua_State* L = luaEngine->getLuaStack()->getLuaState();
    lua_module_register(L);

    FileUtils::getInstance()->addSearchPath("Resources");
    FileUtils::getInstance()->addSearchPath("PlaceHolder");
	luaEngine->executeScriptFile("DataDriven.lua");
	scheduleUpdate();

    //Animation *zeHengAnim = Animation::create();
    //zeHengAnim->addSpriteFrame(sprite->getSpriteFrame());
    //zeHengAnim->addSpriteFrameWithFile("CloseNormal.png");
    //zeHengAnim->addSpriteFrameWithFile("CloseSelected.png");
    //zeHengAnim->init();
    //zeHengAnim->setDelayPerUnit(1.f);
    //zeHengAnim->setUnlimitedLoop(true);
    //Sprite *zeHengSprite = Sprite::create();
    //zeHengSprite->runAction(Animate::create(zeHengAnim));
    //this->addChild(zeHengSprite);
    //zeHengSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    Sprite *testTransitionSpr = Sprite::create();
	testTransitionSpr->setAnchorPoint(Vec2(0.5f, 0.5f));
	AnimationHandlerNode *zeTestAnimTrans = AnimationHandlerNode::create();
    zeTestAnimTrans->setTag(69);
    zeTestAnimTrans->m_SpriteNode = testTransitionSpr;
    testTransitionSpr->addChild(zeTestAnimTrans);
    zeTestAnimTrans->insertAnimSheet("IdleUp", "mainspritecharaidlespritesheet.png", Rect(0, 0, 192, 64), Rect(0, 0, 64, 64), 0.3f, -1);
    zeTestAnimTrans->insertAnimSheet("IdleDown", "mainspritecharaidlespritesheet.png", Rect(0, 64, 192, 64), Rect(0, 0, 64, 64), 0.3f, -1);
    testTransitionSpr->setPosition(Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y));
    DelayTime *zeDelay = DelayTime::create(1.5f);
    AnimTransAct *zeDown = AnimTransAct::create("IdleDown");
    AnimTransAct *zeUp = AnimTransAct::create("IdleUp");
    Sequence  *zeSeq = Sequence::create(zeDown, zeDelay, zeUp, zeDelay, zeDown, nullptr);
    zeTestAnimTrans->runAction(zeDown);
    // AnimTransAct can be run on AnimationHandlerNode but Sequence will fail regardless what. the forum says that the sequence can only run in Sprite node!
    zeTestAnimTrans->runAction(zeSeq);
	this->addChild(testTransitionSpr);

    return true;
}

void HelloWorld::OnButtonInput(EventCustom* _event)
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
	CCLOG(logMessage.c_str());
}

void HelloWorld::update(float _deltaTime)
{
	MKInputManager::GetInstance()->Update();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
