#pragma once
#include"../BaseScene/BaseScene.h"
class SelectUi;

class SelectScene :public BaseScene
{
public:

	SelectScene() { Init(); };
	~SelectScene() {};


private:
	void Init()override;
	void Event()override;

	std::shared_ptr<SelectUi> sUi;
};