#ifndef __Platforms_SCENE_H__
#define __Platforms_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
USING_NS_CC;

class Platforms : public cocos2d::Node
{
public:
	Platforms();
	static Platforms* create();
	~Platforms();

	virtual bool init() override;

	void update(float delta);

	// CONFIRMED FINE
	void CreateSprite(std::string tex, int x, int y);

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
private:
	
};

#endif // __Platforms_SCENE_H__