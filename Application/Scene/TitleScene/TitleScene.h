#pragma once
#include"../BaseScene/BaseScene.h"
class TitleUi;

class TitleScene :public BaseScene
{
public:

	TitleScene() { Init(); };
	~TitleScene() {};

	//タイトル用Jsonから情報を持って来る用
	json11::Json StageData()
	{
		//std::ifstream ifs("Asset/Data/Stage.json");
		//if (ifs.fail()) { assert(0 && "Jsonファイルのパスが違います"); }

		////文字列として全読み込み
		//std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		////文字列のJSONを解析（パース）する
		//std::string err;
		//json11::Json jsonData = json11::Json::parse(strJson, err);
		//if (err.size() > 0) { assert(0 && "読み込んだファイルのJson変換に失敗"); }
		//auto& stageMode = jsonData[].object_items();
		//return stageMode;
	}


private:
	void Init()override;
	void Event()override;
	std::shared_ptr<TitleUi>tUi;
};