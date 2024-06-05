#pragma once
class GameScene;
class WeaponBase;

class ObjectSystem;
class GameCamera;
class Player;

class EnemyBase :public KdGameObject
{
public:
	EnemyBase() {}
	~EnemyBase() {}

	//アップデート====================================================================================
	void PreUpdate()override;
	//更新
	void Update()override;
	//最終更新
	void PostUpdate()override;
	//================================================================================================

	//描画============================================================================================
	//陰影あり描画
	void DrawLit()override;
	void DrawLit_SkinMesh() override;
	//2D描画
	void DrawSprite()override;
	void DrawDebug()override;
	//================================================================================================

	//初期化==========================================================================================
	void Init()override;
	//================================================================================================

	//ゲッター========================================================================================
	Math::Vector3 GetTargetPos() { return m_targetPos; }

	//当たってほしい場所
	virtual KdCollider::SphereInfo GetHitPointTypeSphere()
	{
		KdCollider::SphereInfo m;
		return m;
	}

	std::list<std::shared_ptr<EnemyBase>>GetEneList() { return m_eneList; }
	//=================================================================================================

	//セッター=========================================================================================
	void SetPlayer(std::weak_ptr<Player> _pla) { m_wpPla = _pla; }

	//カメラ情報のセット
	void SetCamera(const std::shared_ptr<GameCamera>& _camera) { m_wpECamera = _camera; }

	void SetControl(bool _controlFlg) { m_controlFlg = _controlFlg; }

	//JSONから情報をもらう用関数
	void SetData(json11::Json _jsonData = nullptr) { m_stageData = _jsonData; }

	//ゲームシーンのアドレスをもらう
	void SetScene(GameScene* _scene) { m_scene = _scene; }

	//=================================================================================================

	//ゲッター==========================================================================================
	//モデル情報
	const std::shared_ptr<const KdModelWork> GetModel()override { return m_spModel; }

	bool EndFlg() { return endFlg; }

	//=================================================================================================

protected:

	//デバック用======================================================================================
	KdDebugWireFrame m_debugWire;
	//================================================================================================

	//フラグ==========================================================================================
	//当たったか
	bool m_hitFlg = false;
	bool m_controlFlg = true;
	bool endFlg = false;

	//int=============================================================================================
	//テスト
	int m_waveCount = 0;
	int m_endWave = 0;
	char* test;

	//float===========================================================================================

	float m_alpha = 1.0f;
	float deg = 0.0f;
	float m_scale = 0.0f;

	//ベクター========================================================================================
	Math::Vector3 m_pos = {};

	//対象エネミーの座標
	Math::Vector3 m_targetPos = {};
	//================================================================================================

	//行列============================================================================================

	//拡縮行列
	Math::Matrix m_scaleMat = Math::Matrix::Identity;

	//回転行列
	Math::Matrix m_rotateMat = Math::Matrix::Identity;

	//移動行列
	Math::Matrix m_transMat = Math::Matrix::Identity;
	//================================================================================================

	//アドレス========================================================================================

	//モデル情報
	std::shared_ptr<KdModelWork>m_spModel;

	//ウェポン
	std::shared_ptr<WeaponBase> m_spWeaBase;

	//ステージの情報（生ポ）
	GameScene* m_scene;

	//カメラの情報
	std::weak_ptr<GameCamera> m_wpECamera;

	//プレイヤーの情報
	std::weak_ptr<Player> m_wpPla;

	//システム
	std::weak_ptr<ObjectSystem> m_wpSys;

	//Jsonデータ
	json11::Json m_stageData;

	//エネミーの持っているウェポンリスト
	std::list<std::shared_ptr<WeaponBase>> m_weaList;

	//エネミーリスト
	std::list<std::shared_ptr<EnemyBase>> m_eneList;
	//================================================================================================

private:
};

typedef struct EnemySorter
{
	EnemySorter(std::shared_ptr<EnemyBase>_e, float _dist) :m_enem(_e), m_dist(_dist) {}
	std::shared_ptr<EnemyBase>m_enem;
	float m_dist;

	//降順
	bool operator<(const EnemySorter& _b)
	{
		return m_dist < _b.m_dist;
	}

	//昇順
	//bool operator>(const EnemySorter& _b)
	//{
	//	return m_dist > _b.m_dist;
	//}
}test;

