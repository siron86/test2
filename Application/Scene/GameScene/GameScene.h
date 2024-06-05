#pragma once
#include"../BaseScene/BaseScene.h"

class GameCamera;
class Player;
class EnemyBase;
class UiBase;
class ResultObject;

class GameScene :public BaseScene
{
public:

	GameScene() { Init(); };
	~GameScene() {};

	//どのJsonを読み込むのかをここで判断し、読み込んだJson情報を各々のオブジェクトに与える
	//SelectSceneからモード情報をもらう
	void SelectMode(std::string_view _modeName)
	{
		m_nowMode = _modeName;
	}

	//ステージ情報を各オブジェクトへ伝達
	//名前のみSelectSceneからもらって来ることで、どのモードかを判断する
	json11::Json StageData(std::string_view _modeName = "")
	{
		std::ifstream ifs("Asset/Data/Stage.json");
		if (ifs.fail()) { assert(0 && "Jsonファイルのパスが違います"); }

		//文字列として全読み込み
		std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		//文字列のJSONを解析（パース）する
		std::string err;
		json11::Json jsonData = json11::Json::parse(strJson, err);
		if (err.size() > 0) { assert(0 && "読み込んだファイルのJson変換に失敗"); }
		auto& stageMode = jsonData[_modeName.data()].object_items();
		return stageMode;
	}

	//当たり判定をするものをゲームシーンでリストに入れるための関数
	void AddHitObject(const std::shared_ptr<KdGameObject>& _hitObj)
	{
		m_hitObjList.push_back(_hitObj);
	}

	const std::list<std::shared_ptr<KdGameObject>>& GetList()
	{
		return m_hitObjList;
	}

private:
	void Init()override;
	void Event()override;

	//テスト
	std::string_view m_nowMode = "Traninng";

	//テスト
	std::weak_ptr<GameCamera> m_wpTpsCam;
	std::weak_ptr<ResultObject> m_wpReOb;
	std::weak_ptr<Player> m_wpPla;
	std::weak_ptr<EnemyBase> m_wpEne;
	std::weak_ptr<UiBase> m_wpUi;
};