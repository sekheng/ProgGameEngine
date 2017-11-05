#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"
#include "Sek Heng scripts/AnimationHandlerNode.h"
#include "Sek Heng scripts/AnimTransAct.h"

USING_NS_CC;

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

    LuaEngine *zeLuaEngine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(zeLuaEngine);
    lua_State* L = zeLuaEngine->getLuaStack()->getLuaState();
    lua_module_register(L);

    FileUtils::getInstance()->addSearchPath("Resources");
    FileUtils::getInstance()->addSearchPath("PlaceHolder");
    zeLuaEngine->executeScriptFile("DataDriven.lua");

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
    AnimationHandlerNode *zeTestAnimTrans = AnimationHandlerNode::create();
    zeTestAnimTrans->m_SpriteNode = testTransitionSpr;
    zeTestAnimTrans->InsertAnimSheet("IdleUp", "mainspritecharaidlespritesheet.png", Rect(0, 0, 192, 64), Rect(0, 0, 64, 64), 0.3f, -1);
    zeTestAnimTrans->InsertAnimSheet("IdleDown", "mainspritecharaidlespritesheet.png", Rect(0, 64, 192, 64), Rect(0, 0, 64, 64), 0.3f, -1);
    testTransitionSpr->setPosition(Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y));
    DelayTime *zeDelay = DelayTime::create(0.5f);
    AnimTransAct *zeDown = AnimTransAct::create("IdleDown");
    AnimTransAct *zeUp = AnimTransAct::create("IdleUp");
    Sequence  *zeSeq = Sequence::create(zeUp, zeDelay, zeDown, nullptr);
    zeTestAnimTrans->stopAllActions();
    zeTestAnimTrans->runAction(zeSeq);
    this->addChild(testTransitionSpr);

    return true;
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
