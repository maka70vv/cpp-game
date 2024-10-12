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
using namespace cocos2d;

class GameState
{
    public:
        Vec2 shipPos;
        Vec2 bkgPos;

    void reset()
    {
        shipPos = Vec2(300, 300);
        bkgPos = Vec2(1060, 350);
    }

    void moveBkg(float x)
    {
        bkgPos.x = bkgPos.x - x;
        if (bkgPos.x < 0)
        {
            bkgPos.x += 1060;
        }
    }
};

class GameSingleton
{
    protected:
        GameSingleton() {};
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

class HelloWorld : public Scene
{
public:
    static Scene* createScene();

    virtual bool init();

    Sprite* shipPtr;
    Sprite* backgroundPtr;
    
    // a selector callback
    // void menuCloseCallback(cocos2d::Ref* pSender);

    EventListenerMouse* mouseListener;

    void onMouseMove(Event* event);
    void onMouseDown(Event* event);

    void update(float delta) override;
    void SetAllPositions();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
