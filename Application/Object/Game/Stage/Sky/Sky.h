#pragma once
#include"../Stage.h"

class Sky :public Stage
{
public:
	Sky() { Init(); }
	~Sky() {}

	void Update()override;
	void DrawUnLit() override;
	void Init()    override;


private:

};