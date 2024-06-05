#pragma once
class GameScene;


class Stage :public KdGameObject
{
public:
	Stage() {}
	~Stage() {}

	void ImGuiUpdate()  override;
	void Update()       override;
	void DrawLit()      override;
	void DrawUnLit()    override;
	void DrawDebug()    override;
	void Init()         override;

	void Collider(const std::shared_ptr<KdModelWork> _objName);

	//当たってほしい場所
	virtual KdCollider::SphereInfo GetHitPointTypeSphere()
	{
		KdCollider::SphereInfo m;
		return m;
	}

	//ゲームシーンのアドレスをもらう
	void SetScene(GameScene* _scene) { m_scene = _scene; }

	void AddStageObject(std::shared_ptr<Stage>& _stageObj) { m_stageList.push_back(_stageObj); }

	virtual bool GetSelect() { return m_selectFlg = false; }

	json11::Json JsonLoad()
	{
		std::ifstream ifs("Asset/Data/StageData.json");
		if (ifs.fail()) { assert(0 && "Jsonファイルのパスが違います"); }

		//文字列として全読み込み
		std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		//文字列のJSONを解析（パース）する
		std::string err;
		json11::Json jsonData = json11::Json::parse(strJson, err);
		if (err.size() > 0) { assert(0 && "読み込んだファイルのJson変換に失敗"); }
		auto& stageData = jsonData["Stage"].object_items();

		return stageData;
	}

protected:

	//デバック用======================================================================================
	KdDebugWireFrame m_debugWire;
	//================================================================================================

	//テスト
	bool m_selectFlg = false;
	float m_scale = 2.f;

	//モデル
	std::shared_ptr<KdModelWork> m_spModel;

	//座標
	Math::Vector3 m_pos = {};
	//回転
	Math::Vector3 m_ang = {};

	//拡縮行列
	Math::Matrix m_scaleMat = Math::Matrix::Identity;
	//回転行列
	Math::Matrix m_rotatMat = Math::Matrix::Identity;
	//移動行列
	Math::Matrix m_transMat = Math::Matrix::Identity;

	//ステージの情報（生ポ）
	GameScene* m_scene;

	//ステージリスト
	std::list<std::shared_ptr<Stage>> m_stageList;

	//当たり判定リスト
	std::list<std::shared_ptr<Stage>> m_hitObjList;
};