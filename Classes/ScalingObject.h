#ifndef __ScalingObject_SCENE_H__
#define __ScalingObject_SCENE_H__

#include "cocos2d.h"

class ScalingObject : public Node
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void update(float,float totalDiff);
	bool isSelected;
	void SetStartingScale(float startingScale);
	void Collision(cocos2d::Touch* touch);
	void CreateSprite(std::string tex, float scale);
	void Selected();
	void Deselected();
	cocos2d::Sprite* sprite;
	// implement the "static create()" method manually
private:
};

#endif // __ScalingObject_SCENE_H__
