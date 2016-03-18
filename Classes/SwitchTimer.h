#pragma once
#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/CCComExtensionData.h"
#include <cstdlib>
#include <ctime>
USING_NS_CC;

class SwitchTimer : public Node
{
private:
	cocos2d::ui::CheckBox* _gravSwitchTimer;

	// Gravity Orientation: 0 = Down; 1 = Left; 2 = Up; 3 = Right;
	int _orientation;
	float _timerDefault;
	float _timer;
	int _mil;
	int _sec;
	int _min;
	bool _isTimerStarted;
	bool _isEnabled;
	bool _revertGravity;
public:
	SwitchTimer();
	~SwitchTimer();
	static SwitchTimer* create();

	void update(float delta);

	virtual bool init() override;

	void SetOrientation(int orientation) { _orientation = orientation; };
	int GetOrientation() { return _orientation; };

	void SetRevertGravity(bool revert) { _revertGravity = revert; };
	bool GetRevertGravity();

	int SwitchGravity();

	cocos2d::ui::CheckBox* GetSprite() { return _gravSwitchTimer; };
	void SetSprite(cocos2d::ui::CheckBox* newSprite);

	void ResetTimer();
	bool IsTimerRunning();

	void DisableSwitchTimer();
};

