/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include <iostream>

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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    shipPtr = Sprite::create("ship.png");

    GameSingleton::getInstance().getState().reset();
   
    
    this->addChild(shipPtr, 2);

    mouseListener = EventListenerMouse::create();

    mouseListener->onMouseMove 
        = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    mouseListener->onMouseDown
        = CC_CALLBACK_1(HelloWorld::onMouseDown, this);

    _eventDispatcher->
        addEventListenerWithSceneGraphPriority(mouseListener, this);

    scheduleUpdate();

    backgroundPtr = Sprite::create("bkg.png");

    this->addChild(backgroundPtr, 1);

    SetAllPositions();

    return true;
}

void HelloWorld::onMouseMove(cocos2d::Event* event)
{
    cocos2d::EventMouse* mouseEvent = (cocos2d::EventMouse*)event;

    GameSingleton::getInstance().getState().shipPos.y = mouseEvent->getCursorY();

    SetAllPositions();
}

void HelloWorld::onMouseDown(cocos2d::Event* event)
{
    GameSingleton::getInstance().getState().reset();
    SetAllPositions();
}

void HelloWorld::update(float delta)
{
    GameSingleton::getInstance().getState().moveBkg(80 * delta);
    GameSingleton::getInstance().getState().moveObjectPos(200 * delta);

    SetAllPositions();

    objectTimer += delta;

    if (objectTimer > 2.0)
    {
        objectTimer = objectTimer - 2.0;

        Vec2 pos = Vec2{ 1400.f, (float)(rand() % 500)};

        // Create asteroid here
        GameSingleton::getInstance()
            .getState()
            .objectPos.push_back(pos);

        Sprite* newObject = Sprite::create("asteroid.png");
        newObject->setPosition(pos);
        addChild(newObject, 3);
        objectPtr.push_back(newObject);
    }
}

void HelloWorld::SetAllPositions()
{
    shipPtr->setPosition(GameSingleton::getInstance().getState().shipPos);
    backgroundPtr->setPosition(GameSingleton::getInstance().getState().bkgPos);

    for (int i = 0; i < objectPtr.size(); i++)
    {
        auto position = GameSingleton::getInstance().getState().objectPos[i];
        objectPtr[i]->setPosition(position);
    }
}

