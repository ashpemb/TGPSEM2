#ifndef __Exit_H__
#define __Exit_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameWinScene.h"
USING_NS_CC;

class Exit : public Node
{
private:
	cocos2d::ui::CheckBox* _exit;
public:
	Exit(cocos2d::ui::CheckBox* checkBox);
	~Exit();
	static Exit* create(cocos2d::ui::CheckBox* checkBox);

	virtual bool init() override;

	void CheckNear(cocos2d::Sprite* player);
};

#endif