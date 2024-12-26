#include "HelloWorldScene.h"
#include <iostream>

#include "CCDirector.h"
#include "CCEventListenerMouse.h"
#include "CCPlatformMacros.h"
#include "2d/CCSprite.h"

namespace cocos2d
{
    class Scene;
}

USING_NS_CC;

auto HelloWorld::createScene() -> cocos2d::Scene*
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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    shipPtr = Sprite::create("ship.png");
    GameSingleton::getInstance().getState().shipPos = Vec2(300, 300);
    GameSingleton::getInstance().getState().bkgPos = Vec2(1325, 400);


    shipPtr->setPosition(300, 300);
    this->addChild(shipPtr,2);
    mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove
        = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    mouseListener->onMouseDown
        = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    _eventDispatcher->
        addEventListenerWithSceneGraphPriority(mouseListener, this);

    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
    scheduleUpdate();

    backgroundPtr = Sprite::create("bkg.png");
       backgroundPtr ->setPosition(0, 0);
    this->addChild(backgroundPtr,1);

    scheduleUpdate();

    GameSingleton::getInstance().getState().reset();
    proxy.factory = &factory;
    return true;
}
void HelloWorld::onMouseMove(cocos2d::Event* event)
{
    cocos2d::EventMouse* mouseEvent = (cocos2d::EventMouse*)event;
    GameSingleton::getInstance().getState().shipPos.y = mouseEvent->getCursorY();
        SetALLPositions();


}
void HelloWorld::onMouseDown(cocos2d::Event* event)
{
    GameSingleton::getInstance().getState().reset();
    SetALLPositions();
}
void HelloWorld::SetALLPositions()
{
    shipPtr->setPosition(GameSingleton::getInstance().getState().shipPos);
    backgroundPtr->setPosition(GameSingleton::getInstance().getState().bkgPos);
    for (int i = 0; i < objectPtr.size(); i++)
    {
        auto position = GameSingleton::getInstance().getState().objectPos[i];
        objectPtr[i]->setPosition(position.pos);
    }
}

ObjectData  ObjectFactory::createObject()
{
    ObjectData result;
    Vec2 pos = Vec2{ 1400.f, (float)(rand() % 500) };
    int type = rand() % 2;
    std::string name;
    if (type == 0)
    {
        name = "asteroid.png";
    }
    else
    {
        name = "cloud.png";
    }
    result.pos = pos;
    Sprite* newObject = Sprite::create(name);
    newObject->setScale(scale);
    newObject->setPosition(pos);
    result.sprite = newObject;
    scale = scale * 1.15;
    result.type = type;
    return result;



}
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    if (code == EventKeyboard::KeyCode::KEY_S)
    {
       // MessageBoxA(0, "Key S pressed", "Key S presd", 0);
        Memento save = GameSingleton::getInstance().getState().SaveData();
        saves.push_back(save);

    }
    else if (code == EventKeyboard::KeyCode::KEY_L)
    {
        //MessageBoxA(0, "Key L pressed", "Key L presd", 0);
        if (saves.size() > 0)
        {
            Memento save = saves.back();
            GameSingleton::getInstance().getState().RestoreFromData(save);
            reload();
        }

    }
}
void HelloWorld::update(float delta)
{
    GameSingleton::getInstance().getState().moveBkg(80*delta);
    GameSingleton::getInstance().getState().moveObkectPos(200 * delta);
    SetALLPositions();
    GameSingleton::getInstance().getState().objectTimer += delta;
    if (GameSingleton::getInstance().getState().objectTimer > 2.0)
    {
        GameSingleton::getInstance().getState().objectTimer = GameSingleton::getInstance().getState().objectTimer - 2.0;

        auto newobject = proxy.createObject();

        std::shared_ptr<Strategy> strategy;
        int type = rand() % 2;
        if (type == 0)
        {
            strategy = std::make_shared<Strategy>();

        }
        else
        {
            strategy = std::make_shared<ZigzagStrategy>();
        }
        GameSingleton::getInstance()
            .getState()
            .objectPos.push_back({ newobject.pos,strategy, type, newobject.sprite->getScale(), newobject.type

                });

        addChild(newobject.sprite, 3);
        objectPtr.push_back(std::make_unique<ShakingSpriteDecorator>(newobject.sprite,newobject.type ));
    }
    for (auto& object : objectPtr)
    {
        float x = object->getPosition().x;
        float y = object->getPosition().y;
        float R = object->getScale() * 300;
        float w = 150;
        float h = 150;
        float px = shipPtr->getPosition().x;
        float py = shipPtr->getPosition().y;

        if ((abs(y - (py - h / 2)) < R
            || abs(y + (py - h / 2)) < R)
            && (x > px - w / 2)
            && (x < px + w / 2))
        {
            auto& flyingObject = object->sprite;
            flyingObject->accept(*this);
        }


   }


}

void HelloWorld::reload()
{
    for (auto& object : objectPtr)
    {
        object->sprite->sprite->removeFromParent();
    }
    objectPtr.clear();
    for (auto& ObjectPosAndStrategy : GameSingleton::getInstance().getState().objectPos)
    {
        ObjectData newObject;
        newObject.pos = ObjectPosAndStrategy.pos;
        if (ObjectPosAndStrategy.objectType == 0) {
            newObject.sprite = Sprite::create("asteroid.png");
        }
        else {
            newObject.sprite = Sprite::create("cloud.png");

        }
        newObject.type = ObjectPosAndStrategy.objectType;
        newObject.sprite->setPosition(ObjectPosAndStrategy.pos);
        newObject.sprite->setScale(ObjectPosAndStrategy.scale);

        addChild(newObject.sprite, 3);
        objectPtr
            .push_back(
                std::make_unique<ShakingSpriteDecorator>(newObject.sprite, newObject.type)
            );

    }
}


ObjectData ObjectProxy::createObject()
{
    auto& objectCreationData = GameSingleton::getInstance().getState().objectCraetionData;

    if (objectCreationData.size() > 0)
    {
        ObjectCreationData d = objectCreationData[objectCreationData.size() - 1];
        objectCreationData.pop_back();
        ObjectData result;
        result.pos = d.pos;
        if (d.type == 0)
        {
            result.sprite = Sprite::create("asteroid.png");
        }
        else
        {

            result.sprite = Sprite::create("cloud.png");
        }
        result.type = d.type;
        result.sprite->setPosition(d.pos);
        result.sprite->setScale(d.scale);
        return result;
    }
    else
    {
        return factory->createObject();
    }


}