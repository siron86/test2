#pragma once
#include"../BaseScene/BaseScene.h"
class TitleUi;

class TitleScene :public BaseScene
{
public:

	TitleScene() { Init(); };
	~TitleScene() {};

	//�^�C�g���pJson������������ė���p
	json11::Json StageData()
	{
		//std::ifstream ifs("Asset/Data/Stage.json");
		//if (ifs.fail()) { assert(0 && "Json�t�@�C���̃p�X���Ⴂ�܂�"); }

		////������Ƃ��đS�ǂݍ���
		//std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		////�������JSON����́i�p�[�X�j����
		//std::string err;
		//json11::Json jsonData = json11::Json::parse(strJson, err);
		//if (err.size() > 0) { assert(0 && "�ǂݍ��񂾃t�@�C����Json�ϊ��Ɏ��s"); }
		//auto& stageMode = jsonData[].object_items();
		//return stageMode;
	}


private:
	void Init()override;
	void Event()override;
	std::shared_ptr<TitleUi>tUi;
};