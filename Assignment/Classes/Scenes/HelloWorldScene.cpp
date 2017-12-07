// Include Cocos
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"
#include "GT/Animation/GTAnimationHandlerNode.h"
#include "GT/Animation/GTAnimTransAct.h"
#include "GT/Audio/GTSimperMusicSys.h"
#include "GT/GameLogic/GTCharacterStatNode.h"

// Include MK
#include "MK/SceneManagement/MKSceneManager.h"
#include "MK/Common/MKMacros.h"

// Include Input Device Handlers
#include "MK/Input/MKKeyboardHandler.h"

// Include Assignment
#include "HelloWorldScene.h"
#include "AudioEngine.h"
#include "external/json/document.h"
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
using namespace GinTama;


HelloWorld::HelloWorld()
{
}

HelloWorld::~HelloWorld()
{
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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Input Testing
	InitialiseInput();

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

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteAnim/assignment_sprite.plist");


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
	GTAnimationHandlerNode *zeTestAnimTrans = GTAnimationHandlerNode::create();
    zeTestAnimTrans->m_SpriteNode = testTransitionSpr;
    testTransitionSpr->addChild(zeTestAnimTrans);
	zeTestAnimTrans->initWithJSON_tag("SpriteAnim/MainCharaData.txt");
    //zeTestAnimTrans->insertAnimSheet("IdleUp", "mainspritecharaidlespritesheet.png", Rect(0, 0, 192, 64), Rect(0, 0, 64, 64), 0.3f, -1);
    //zeTestAnimTrans->insertAnimSheet("IdleDown", "mainspritecharaidlespritesheet.png", Rect(0, 64, 192, 64), Rect(0, 0, 64, 64), 0.3f, -1);
    //zeTestAnimTrans->insertAnimFromCache("walk_right");
    testTransitionSpr->setPosition(Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y));
    // AnimTransAct can be run on AnimationHandlerNode but Sequence will fail regardless what. the forum says that the sequence can only run in Sprite node!
	this->addChild(testTransitionSpr);
    //auto zePhyBody = PhysicsBody::createBox(Size(60.f, 60.f));
    //zePhyBody->setDynamic(false);
    //testTransitionSpr->setPhysicsBody(zePhyBody);

    // mp3 files work even though the documentation said otherwise. May it only works on Lenovo Y50
    //AudioEngine::play2d("Trouble-in-the-Kingdom_Looping.mp3", true, 0.2f);

    // Reading from file. It is a success!
    FILE *fp = fopen("PlaceHolder/TryJson.txt", "rb");
    // Looks like the 256 char array is meant to allocate memory
    char zeBuffer[256];
    FileReadStream zeIS(fp, zeBuffer, sizeof(zeBuffer));
    Document zeD;
    zeD.ParseStream(zeIS);
    fclose(fp);

    int numArr[] = { 5,4,3,2,1 };
    RAPIDJSON_NAMESPACE::Value zeValArr(kArrayType);
    for (auto i : numArr)
    {
        zeValArr.PushBack(i, zeD.GetAllocator());
    }
    if (!zeD.HasMember("lol"))
    {
        zeD.AddMember(StringRef("lol"), "Lol", zeD.GetAllocator());
    }
    if (!zeD.HasMember("tryArr"))
        zeD.AddMember("tryArr", zeValArr, zeD.GetAllocator());
    if (!zeD.HasMember("testingObj"))
    {
        RAPIDJSON_NAMESPACE::Value zeValObj(kObjectType);
        zeValObj.AddMember("LOL", "What", zeD.GetAllocator());
        zeD.AddMember("testingObj", zeValObj, zeD.GetAllocator());
    }

    fp = fopen("PlaceHolder/TryJson.txt", "w");
    // This does not work and i dont know why!
    //char *zeNothing = "Nothing";
    //size_t zeNothingBufferSZ = strlen(zeNothing);
    // Only this work!!
    char zeNothing[256];
    size_t zeNothingBufferSZ = sizeof(zeNothing);
    FileWriteStream zeFWS(fp, zeNothing, zeNothingBufferSZ);
    Writer<FileWriteStream> writer(zeFWS);
    zeD.Accept(writer);
    fclose(fp);

    GinTama::GTSimperMusicSys::GetInstance()->playSound("testbgm");

    return true;
}

bool HelloWorld::initWithPhysics()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::initWithPhysics())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Input Testing
    InitialiseInput();
    scheduleUpdate();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteAnim/assignment_sprite.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteAnim/assignment_sprite2.plist");
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    getPhysicsWorld()->setGravity(Vec2(0, -98.f));
    getPhysicsWorld()->setAutoStep(true);
    getPhysicsWorld()->setUpdateRate(2);
    //getPhysicsWorld()->setSpeed(0.1f);
    Sprite *testTransitionSpr = Sprite::create();
    testTransitionSpr->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Run (1).png"));
    testTransitionSpr->setAnchorPoint(Vec2(0.5f, 0.5f));
    GTAnimationHandlerNode *zeTestAnimTrans = GTAnimationHandlerNode::create();
    zeTestAnimTrans->m_SpriteNode = testTransitionSpr;
    testTransitionSpr->addChild(zeTestAnimTrans);
    zeTestAnimTrans->initWithJSON_tag("SpriteAnim/MainCharaData.json");
    testTransitionSpr->setPosition(Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y));
    // AnimTransAct can be run on AnimationHandlerNode but Sequence will fail regardless what. the forum says that the sequence can only run in Sprite node!
    this->addChild(testTransitionSpr);
    auto zePhyBody = PhysicsBody::createBox(testTransitionSpr->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    zePhyBody->setMass(1.f);
    zePhyBody->setDynamic(true);
    zePhyBody->setGravityEnable(true);
    //zePhyBody->setVelocityLimit(0.001f);
    //zePhyBody->setVelocity(Vec2(0, 1));
    //zePhyBody->setAngularVelocityLimit(0);
    testTransitionSpr->setPhysicsBody(zePhyBody);

    //Sprite *zeTestFloor = Sprite::create("Environment/Ground.png");
    //zeTestFloor->setPosition(Vec2(visibleSize.width * 0.5f + origin.x, 0));
    //zeTestFloor->setScaleX(20.0f);
    //PhysicsBody *zeTestPhy = PhysicsBody::createBox(Size(10000, 10));
    //zeTestFloor->setPhysicsBody(zeTestPhy);
    //zeTestPhy->setDynamic(false);
    //zeTestPhy->setGravityEnable(false);
    //this->addChild(zeTestFloor);

    PhysicsBody *zeEdgePhy = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    zeEdgePhy->setDynamic(true);
    zeEdgePhy->setGravityEnable(false);
    auto zeEdgeNode = Node::create();
    zeEdgeNode->setPosition(Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y));
    zeEdgeNode->setPhysicsBody(zeEdgePhy);
    this->addChild(zeEdgeNode);

    GTCharacterStatNode *zeCharStat = GTCharacterStatNode::create(zeEdgePhy);
    //zeCharStat->setSpeedX(5);
    zeCharStat->scheduleUpdate();
    testTransitionSpr->addChild(zeCharStat);
    m_CharSpr = testTransitionSpr;

    auto cam = getDefaultCamera();
    //cam->runAction(MoveTo::create(3.f, Vec3(cam->getPositionX(), cam->getPositionY(), cam->getPositionZ() + 300.f)));
    cam->setPositionZ(cam->getPositionZ() + 300.0f);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContact, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    GinTama::GTSimperMusicSys::GetInstance()->playSound("testbgm");
    return true;
}

bool HelloWorld::onContact(PhysicsContact &_contact)
{
    CCLOG("They are contacting with 1 another\n");
    return true;
}

void HelloWorld::InitialiseInput()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_InputLabel = Label::createWithTTF("Input Debug Label", "fonts/Marker Felt.ttf", 24);
	// position the label on the center of the screen
	m_InputLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - m_InputLabel->getContentSize().height - (visibleSize.height / 4)));
	this->addChild(m_InputLabel, 1);

	MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_1);
	m_ButtonListener = MKInputManager::GetInstance()->CreateEventListener<MKInputButton>(CC_CALLBACK_1(HelloWorld::OnButton, this));
	m_ClickListener = MKInputManager::GetInstance()->CreateEventListener<MKInputClick>(CC_CALLBACK_1(HelloWorld::OnClick, this));
	m_AxisListener = MKInputManager::GetInstance()->CreateEventListener<MKInputAxis>(CC_CALLBACK_1(HelloWorld::OnAxis, this));
}

void HelloWorld::OnButton(EventCustom* _event)
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
        switch (buttonEvent->m_InputName)
        {
        case MinamiKotori::MKInputName::JUMP:
            GinTama::GTSimperMusicSys::GetInstance()->pauseSound("testbgm");
            inputName = "Jump";
            break;
        case MinamiKotori::MKInputName::SLIDE:
            GinTama::GTSimperMusicSys::GetInstance()->resumeSound("testbgm");
            inputName = "Slide";
            break;
        case MinamiKotori::MKInputName::SMASH:
            GinTama::GTSimperMusicSys::GetInstance()->stopSound("testbgm");
            inputName = "Smash";
            break;
        default:
            inputName = "Unknown InputName";
            break;
        }
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
	//CCLOG(logMessage.c_str());
	m_InputLabel->setString(logMessage);
}

void HelloWorld::OnClick(EventCustom* _event)
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
	//CCLOG(logMessage.c_str());
	m_InputLabel->setString(logMessage);
}

void HelloWorld::OnAxis(EventCustom* _event)
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
	//CCLOG(logMessage.c_str());
	m_InputLabel->setString(logMessage);
}

void HelloWorld::update(float _deltaTime)
{
	m_SceneChangeCounter -= _deltaTime;
	if (m_SceneChangeCounter < 0.0f)
	{
		//MKSceneManager::GetInstance()->ReplaceScene("MKSceneDerived");
		//MKSceneManager::GetInstance()->PushScene("MKSceneDerived");
		m_SceneChangeCounter = 3.0f;
	}
    //m_CharSpr->getChildByTag<CharacterStatNode*>(1)->adjustSpeedX(_deltaTime * 5.0f);
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