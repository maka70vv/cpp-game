
#ifndef HELLOWORLD_SCENE_H
#define HELLOWORLD_SCENE_H

#include <iostream>

#include "cocos2d.h"
#include <vector>
using namespace cocos2d;

struct ObjectCreationData
{
    double scale = 1.0;
    cocos2d::Vec2 pos;
    int type = 0;

};

class Strategy
{
public:
    virtual void changePosition(cocos2d::Vec2& pos, float x)
    {
        pos.x = pos.x - x;
    }
};
class ZigzagStrategy : public Strategy
{
public:
    float shift = 0;
    float dir = -1;
    void changePosition(cocos2d::Vec2& pos, float x)
    {
        pos.x = pos.x - x;
        pos.y = pos.y + dir * x;
        shift = shift + dir * x;
        if (shift > 100)
        {
            dir = -1;
        }
        if (shift < -100)
        {
            dir = 1;
        }

    }

};

struct ObjectPosAndStrategyData
{
    cocos2d::Vec2 pos;
    int strategy;
    float scale;
    int objectType;
};

struct ObjectPosAndStrategy
{
    cocos2d::Vec2 pos;
    std::shared_ptr<Strategy> strategy;
    int strategyType;
    float scale;
    int objectType;
    void move(float x)
    {
        strategy -> changePosition(pos,x);
    }
    ObjectPosAndStrategyData SaveData()
    {
        ObjectPosAndStrategyData result;
        result.pos = pos;
        result.strategy = strategyType;
        result.scale = scale;
        result.objectType = objectType;
        return result;

    }
    void RestoreFromData(ObjectPosAndStrategyData data)
    {
        pos = data.pos;
        strategyType = data.strategy;
        scale = data.scale;
        objectType = data.objectType;
        if (strategyType == 0)
        {
            strategy = std::make_shared<Strategy>();
        }
        else
        {
            strategy = std::make_shared<ZigzagStrategy>();
        }
    }
};



struct  Memento
{
    double objectTimer = 0;
    cocos2d::Vec2 shipPos;
    cocos2d::Vec2 bkgPos;
    std::vector<ObjectPosAndStrategyData> objectPosAndStrategyData;
    std::vector<ObjectCreationData> objectCreationData;
};


class GameState
{
protected:

public:
    double objectTimer = 0;
    cocos2d::Vec2 shipPos;
    cocos2d::Vec2 bkgPos;
    std::vector<ObjectPosAndStrategy> objectPos;
    std::vector<ObjectCreationData> objectCraetionData;

    Memento SaveData()
    {
        Memento result;
        result.objectTimer = objectTimer;
        result.shipPos = shipPos;
        result.bkgPos = bkgPos;
        result.objectCreationData = objectCraetionData;

        for (auto& object : objectPos)
        {
            result.objectPosAndStrategyData.push_back(object.SaveData());
        }
        return result;
    }
    void RestoreFromData(Memento data)
    {
        objectTimer = data.objectTimer;
        shipPos = data.shipPos;
        bkgPos = data.bkgPos;
        objectCraetionData = data.objectCreationData;
        objectPos.clear();
        for (auto& objectAndStrategy : data.objectPosAndStrategyData)
        {
            ObjectPosAndStrategy obj;
            obj.RestoreFromData(objectAndStrategy);
            objectPos.push_back(obj);
        }
    }
    void reset()
    {
        shipPos = cocos2d::Vec2(300, 300);
        bkgPos = cocos2d::Vec2(1325, 400);

        objectCraetionData.push_back({ 0.2,{1400,300}, 0 });
        objectCraetionData.push_back({ 0.2,{1400,300}, 0 });
        objectCraetionData.push_back({ 0.2,{1400,300}, 0 });
        objectCraetionData.push_back({ 0.2,{1400,300}, 0 });
        objectCraetionData.push_back({ 0.2,{1400,300}, 0 });


    }
    void moveBkg(float x)
    {
        bkgPos.x = bkgPos.x - x;
        if (bkgPos.x < -0)
        {
            bkgPos.x += 1325;
        }

    }
    void moveObkectPos(float x)
    {
        for (auto& obj : objectPos)
        {
            obj.move(x);
        }
    }

};
class GameSingleton
{
protected:
    GameSingleton(){}
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

struct ObjectData
{
    cocos2d::Vec2 pos;
    cocos2d::Sprite* sprite;
    int type;
    ObjectData& setPos(cocos2d::Vec2 newpos)
    {
        pos = newpos;
        return *this;

    }
    ObjectData& setScale(float s)
    {
        sprite->setScale(s);
        return *this;
    }
};

class ObjectSourceInterface
{
public:
    virtual ObjectData createObject() = 0;
};
class ObjectFactory : public ObjectSourceInterface
{
public:
    double scale = 0.1;
    ObjectData createObject();
};
class ObjectProxy : public ObjectSourceInterface
{
public:
    ObjectData createObject();
    ObjectFactory* factory = nullptr;

};
struct SolidObject;
struct CloudObject;

struct Visitor
{
    virtual void visitObject(SolidObject& s)
    {
        std::cout << "Solid Object" << std::endl;
    }
    virtual void visitObject(CloudObject& s)
    {
        std::cout << "Cloud Object" << std::endl;
    }
};




struct FlyingObject
{
    cocos2d::Sprite* sprite;
    FlyingObject(cocos2d::Sprite* s)
        :sprite{s}
    {}
    virtual void accept(Visitor& v) = 0;


};

struct SolidObject : public FlyingObject
{

    SolidObject(cocos2d::Sprite* s)
        :FlyingObject{ s }
    {}
    virtual void accept(Visitor& v)
    {
        v.visitObject(*this);
    }
};

struct CloudObject : public FlyingObject
{
    CloudObject(cocos2d::Sprite* s)
        :FlyingObject{ s }
    {}
    virtual void accept(Visitor& v)
    {
        v.visitObject(*this);
    }
};

class SpriteDecorator
{
public:
    //cocos2d::Sprite* sprite;
    std::unique_ptr<FlyingObject> sprite;
public:
    SpriteDecorator(cocos2d::Sprite* s, int type)
    {
        if (type == 0)
        {
            sprite = std::unique_ptr<FlyingObject>{ new SolidObject{s} };
        }
        else
        {
            sprite = std::unique_ptr<FlyingObject>{ new CloudObject{s} };
        }
    }
    virtual void setPosition(cocos2d::Vec2 p)
    {
        sprite->sprite->setPosition(p);
    }
    virtual cocos2d::Vec2 getPosition()

    {
        return sprite->sprite->getPosition();

    }
    virtual float getScale()
    {
        return sprite->sprite->getScale();
    }
};
class ShakingSpriteDecorator : public SpriteDecorator
{
public:
    ShakingSpriteDecorator(cocos2d::Sprite* s, int type)
        :SpriteDecorator(s, type)
    {

    }
    void setPosition(cocos2d::Vec2 p)override
    {
        p = p + cocos2d::Vec2{ rand() % 10 - 5.f,rand() % 10 - 5.f };
        SpriteDecorator::setPosition(p);
    }
};


class HelloWorld : public cocos2d::Scene
    ,public Visitor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    cocos2d::Sprite* shipPtr;
    cocos2d::Sprite* backgroundPtr;

    std::vector<std::unique_ptr<SpriteDecorator>> objectPtr;
    std::vector<Memento> saves;
    ObjectFactory factory;
    ObjectProxy proxy;
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    cocos2d::EventListenerMouse* mouseListener;
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void update(float delta) override;
    void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    void SetALLPositions();
    void reload();
};

#endif // HELLOWORLD_SCENE_H