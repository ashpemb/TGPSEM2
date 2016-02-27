#ifndef __Platforms_SCENE_H__
#define __Platforms_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "GameManager.h"
USING_NS_CC;

class Platforms : public cocos2d::Node
{
private:
	Sprite* _movingPlat;
	float OScale;
	float NScale;

public:
	Platforms();
	static Platforms* create();
	~Platforms();

	virtual bool init() override;

	void update(float delta);

	// CONFIRMED FINE
	void setSprite(Sprite* newSprite) { _movingPlat = newSprite; this->addChild(_movingPlat); OScale = _movingPlat->getScale(); };
	Sprite* getSprite() { return _movingPlat; };

	void Selected();
	void UnSelected();

	void MovePlatform(cocos2d::Vec2 T);
	// END CONFIRM

	cocos2d::Sprite* sprite;

	// Touch input
	virtual void onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

};

#endif // __Platforms_SCENE_H__