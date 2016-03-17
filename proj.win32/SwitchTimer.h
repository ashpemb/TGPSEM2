#pragma once
#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <cstdlib>
#include <ctime>
#include "SwitchTimer.h"
	USING_NS_CC;

	class SwitchTimer : public Node
	{
	private:
		cocos2d::ui::CheckBox* _gravSwitchTimer;

		// Gravity Orientation: 0 = Down; 1 = Left; 2 = Up; 3 = Right;
		int _orientation;
		int _timerLength;
		bool _enabled;
		bool _playerNearBy;
	public:
		SwitchTimer();
		~SwitchTimer();
		static SwitchTimer* create();

		virtual bool init() override;

		void SetOrientation(int orientation) { _orientation = orientation; };
		int GetOrientation() { return _orientation; };
		void CheckNear(cocos2d::Sprite* player);

		void UpdateTimer(float dt);
		void ReverseOrientation(int _orientation);
		cocos2d::ui::CheckBox* GetSprite() { return _gravSwitchTimer; };
		void SetSprite(cocos2d::ui::CheckBox* newSprite);
	};

