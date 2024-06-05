#pragma once
class Rifle;
class Rocket;
class EleGun;

class GameScene;
class GameCamera;

//ウェポン種類
enum WeaponType
{
	Rif = 1,
	Roc = 2,
	Ele = 3,
};

//ウェポン管理クラス
class WeaponBase :public KdGameObject
{
public:
	WeaponBase() {}
	~WeaponBase() {}

	//更新
	void ImGuiUpdate() override;
	void PreUpdate() override;
	void Update()    override;
	void PostUpdate()override;

	//描画
	void DrawLit()   override;
	void DrawLit_SkinMesh() override;
	void DrawUnLit()   override;

	//デバックワイヤー
	void DrawDebug()   override;

	//初期化
	void Init()      override;

	//セッター==========================================================================================
	//カメラ情報のセット
		//ゲームシーンのアドレスをもらう
	void SetScene(GameScene* _scene) { m_scene = _scene; }

	//ターゲットの座標をもらってくる
	void SetTarPos(const Math::Vector3& _tarPos) { m_targetPos = _tarPos; }

	//所有者情報を持ってくる
	void SetOwner(const std::shared_ptr<KdGameObject>& _spOwn) { m_wpOwn = _spOwn; }

	void SetCamera(const std::shared_ptr<GameCamera>& _camera) { m_wpCamera = _camera; }

	virtual void SetRack(const Math::Matrix& _rackMat) { m_rackMat = _rackMat;}

	//ゲッター==========================================================================================

	//ターゲット座標
	Math::Vector3 GetTarPos() { return m_targetPos; }

	//モデル情報
	const std::shared_ptr<const KdModelWork> GetModel() { return m_spModel; }

	//ウェポンリスト取得
	const std::list<std::shared_ptr<WeaponBase>>& GetWeaList() { return m_weaList; }

	//弾の登録
	void AddWeapon(const std::shared_ptr<WeaponBase>& _weapon) { m_weaList.push_back(_weapon); }

	//装備する場所
	const Math::Matrix EquipmentPoint(const std::string_view& _pointName);

	//==================================================================================================

	//選ばれているか
	virtual void SelectFlg() {};

	//起動したのがランチャークラスだったらミサイルを
	//ランチャー以外の場合エフェクトを発射させる
	//エフェクトは武器により変更する
	virtual void ShotFlg() {};

	//武器の銃口情報とカメラのレイ情報を使って画面中心へのベクトルを算出する
	Math::Vector3 CenterShot(const Math::Matrix& _muzzleMat);

	//==================================================================================================
	//エネミーの時起動する関数
	void EnemyBullet(int _maxShotCount = 0, bool _enemyFlg = false)
	{
		m_eneMaxBul = _maxShotCount;
		m_enemyFlg = _enemyFlg;
	}
	bool Enemy() { return m_enemyFlg; }
	int EnemyMaxShot() { return m_eneMaxBul; }
	//==================================================================================================

	//武器ごとの弾情報
	//発射位置、弾のサイズ、スピードの設定
	void ShotBullet(
		const Math::Matrix& _muzzleMat,  //銃口情報
		const Math::Vector3& _size,  //弾の大きさ
		const float& _speed,  //速度
		const int& _damage,  //武器ごとのダメージ
		const Math::Vector3& _shotPos = Math::Vector3::Zero  //ミサイルのみ使用
	);

	virtual Math::Vector3 GetShotPos() { return Math::Vector3::One; }

	//射撃時に起動する関数
	void Active();

	//どれを持っているか
	int GetWeapomType()
	{
		return m_weaType;
	}

	//持っているウェポン
	void SetWeaponType(int _type)
	{
		switch (_type)
		{
		case 1:
			m_weaType = Rif;
			break;
		case 2:
			m_weaType = Roc;
			break;
		case 3:
			m_weaType = Ele;
			break;
		default:
			break;
		}
	}



protected:

	//ウェポンタイプ
	WeaponType m_weaType = Rif;

	//継承先使用変数==================================================================
	//バレット用変数格納
	struct BULLET
	{
		float scale = 0.f;  //大きさ
		float speed = 0.f;  //弾速
	}STR_Bullet;

	//ウェポン用変数格納用
	struct WEAPON
	{
		float scale = 0.f;			  //サイズ
		int bulletMax = 0;			  //最大弾数
		int bulletCount = 0;		  //現在の弾数
		float relTime = 0.f;		  //リロード時間
		float nowRelTime = 0.f;		  //リロード経過時間
		float coolTime = 0.f;		  //クールタイム
		float defaultCoolTime = 5.f;  //デフォルトクールタイム

	}STR_Weapon;

	//ダメージ
	int m_damage = 0;
	//エネミーの時の最大弾数
	int m_eneMaxBul = 0;

	//フラグ============================================================================
	//リロードフラグ
	bool m_relFlg = false;

	//発射フラグ
	bool m_fireFlg = false;

	//選択中ウェポンフラグ
	bool m_selectFlg = false;

	//発射フラグ
	bool m_shotFlg = false;

	//エネミーかどうか
	bool m_enemyFlg = false;

	//ベクター==========================================================================
	//座標
	Math::Vector3 m_pos = Math::Vector3::Zero;

	//角度
	Math::Vector3 m_ang = Math::Vector3::Zero;

	//ターゲットまでのベクトル（オーナーから取得し、各武装へ送る）
	Math::Vector3 m_targetVec = Math::Vector3::Zero;
	//ターゲットの座標
	Math::Vector3 m_targetPos = Math::Vector3::Zero;

	Math::Vector3 m_shotVec = Math::Vector3::Zero;

	//行列==============================================================================
	//拡大行列
	Math::Matrix m_scaleMat = Math::Matrix::Identity;

	//回転行列
	Math::Matrix m_rotateMat = Math::Matrix::Identity;

	//移動行列
	Math::Matrix m_transMat = Math::Matrix::Identity;

	//銃口の行列
	Math::Matrix m_muzzleMat = Math::Matrix::Identity;

	//自機の特定の場所に配置する
	Math::Matrix m_rackMat = Math::Matrix::Identity;

	//情報==============================================================================

	//モデル
	std::shared_ptr<KdModelWork> m_spModel = nullptr;

	//ターゲット情報取得用
	std::weak_ptr<KdGameObject>m_wpTra;

	//所有者の情報
	std::weak_ptr<KdGameObject>m_wpOwn;

	//カメラの情報
	std::weak_ptr<GameCamera> m_wpCamera;

	//ステージの情報（生ポ）
	GameScene* m_scene;

	//エフェクト用====================================================================================
	std::shared_ptr<KdEffekseerObject> effObj;
	Math::Matrix effMat = Math::Matrix::Identity;
	//================================================================================================

	//デバック用======================================================================================
	KdDebugWireFrame m_debugWire;
	//================================================================================================

private:

	//ウェポンリスト=====================================================================
	std::list<std::shared_ptr<WeaponBase>>m_weaList;

	//ウェポン情報=======================================================================
	std::shared_ptr<Rifle>rif;
	std::shared_ptr<Rocket>roc;
	std::shared_ptr<EleGun>ele;
};

//どうやって武器を認識させるか
//