#ifndef __LightBulb_SCENE_H__
#define __LightBulb_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
USING_NS_CC;

class LightBulb : public cocos2d::Node
{
private:

	// Sprite
	Sprite* _light;

public:
	LightBulb();
	static LightBulb* create(Vec3 lightPos);
	~LightBulb();

	virtual bool init() override;

	void MakeLight(Vec3 lightPos);
	void update(float delta);

	// Sprite
	Sprite* GetSprite() { return _light; };
	void setSprite(Sprite* newSprite) { _light = newSprite; this->addChild(_light); };
};

#endif // __LightBulb_SCENE_H__