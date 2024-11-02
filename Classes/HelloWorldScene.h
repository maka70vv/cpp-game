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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

using namespace cocos2d;

class GameState
{
public:
    cocos2d::Vec2 shipPos;
    cocos2d::Vec2 bkgPos;
    std::vector<cocos2d::Vec2> objectPos;


    void reset()
    {
        shipPos = cocos2d::Vec2(300, 300);
        bkgPos = cocos2d::Vec2(1325, 400);
    }

    void moveBkg(float x)
    {
        bkgPos.x = bkgPos.x - x;
        if (bkgPos.x < 0)
        {
            bkgPos.x += 1325;
        }
    }

    void moveObjectPos(float x)
    {
        for (auto& obj : objectPos)
        {
            obj.x = obj.x - x;
        }
    }
};

class GameSingleton
{
protected:
    GameSingleton() {}

    GameState currentState;
public:
    static GameSingleton& getInstance()
    {
        static GameSingleton instance;

        return instance;
    }

    GameState& getState()
    {
        return currentState;
    }
};


class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    cocos2d::Sprite* shipPtr;
    cocos2d::Sprite* backgroundPtr;
    std::vector<cocos2d::Sprite*> objectPtr;

    double objectTimer = 0;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    cocos2d::EventListenerMouse* mouseListener;

    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);

    void update(float delta) override;

    void SetAllPositions();
};

#endif // __HELLOWORLD_SCENE_H__
