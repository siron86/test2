#pragma once

//前方宣言
class GameCamera;
class EnemyBase;
class WeaponBase;
class ObjectSystem;
class GameScene;

class Player :public KdGameObject
{
public:
	//コンストラクタ
	Player() {}
	//デストラクタ
	~Player() {}

	//テスト
	void SetControl(bool _control) { m_control = _control; }

	//アップデート======================================================================================
	void ImGuiUpdate()  override;
	void PreUpdate()    override;
	void Update()       override;
	void PostUpdate()   override;
	//==================================================================================================


	//シェーダー========================================================================================
	void DrawLit()          override;
	void DrawLit_SkinMesh() override;
	void DrawUnLit()        override;

	//デバックラインの表示用
	void DrawDebug() override;
	//==================================================================================================

	//初期化============================================================================================
	void Init()      override;

	//プレイヤーオリジナル関数==========================================================================
	//視点操作
	void RotateFromCamera();

	//ロックオン
	void RotateFromTarget(Math::Vector3 _ownerVec, Math::Vector3 _tarVec);

	//ソート処理（近いものから）
	void TargetSort();

	//ブースト処理
	void Boost();

	//ブースト回復処理
	void BoostHeal();

	//オーバーヒート時
	void OverHeat();
	//==================================================================================================

	//セッター==========================================================================================
	//カメラ情報のセット
	void SetCamera(const std::shared_ptr<GameCamera>& _camera) { m_wpCamera = _camera; }

	//ターゲット情報
	void SetTarget(const std::shared_ptr<EnemyBase>& _traEne) { m_wpTraEne = _traEne; }

	//JSONから情報をもらう用関数
	void SetData(const json11::Json& _jsonData = nullptr) { m_stageData = _jsonData; }
	//==================================================================================================

	//ゲッター==========================================================================================
	//プレイヤーのモデル情報
	const std::shared_ptr<const KdModelWork> GetModel()override { return m_spModel; }

	//ターゲットの座標
	Math::Vector3 GetTarPos() { return m_targetPos; }
	bool LockOnFlg()override { return m_lockFlg; }

	//Uiに送るデータ（構造体）
	std::shared_ptr<struct PLAYERDATA> GetPlayerData() { return m_data; }
	bool GetLock() { return m_lockOkFlg; }

	//ゲームシーンのアドレスをもらう
	void SetScene(GameScene* _scene) { m_scene = _scene; }

	//当たり判定(球)
	KdCollider::SphereInfo GetHitPointTypeSphere()override
	{
		KdCollider::SphereInfo spherePoint;

		//ノードの名前は固定でヒットポイント
		const KdModelWork::Node* pNode = m_spModel->FindNode("HitPoint");
		Math::Matrix mat;
		mat = pNode->m_worldTransform * m_mWorld;

		if (pNode)
		{
			Math::Vector3 pos = mat.Translation();
			spherePoint.m_sphere.Center = pos;
			m_debugWire.AddDebugSphere(pos, 5.0f);
		}

		spherePoint.m_sphere.Radius = 7.0f;

		spherePoint.m_type = KdCollider::TypeEvent | KdCollider::TypeBump | KdCollider::TypeDamage | KdCollider::TypeGround;

		return spherePoint;
	}

	//当たり判定(レイ)
	KdCollider::RayInfo GetHitPointTypeRay()override
	{
		KdCollider::RayInfo rayPoint;

		//ノードの名前は固定でヒットポイント
		const KdModelWork::Node* pNode = m_spModel->FindNode("HitPoint");
		Math::Matrix mat;
		mat = pNode->m_worldTransform * m_mWorld;

		if (pNode)
		{
			Math::Vector3 pos = mat.Translation();
			rayPoint.m_pos = pos;
		}

		rayPoint.m_dir = m_mWorld.Down();
		rayPoint.m_dir.Normalize();

		rayPoint.m_range = 15.0f;

		m_debugWire.AddDebugLine(rayPoint.m_pos, rayPoint.m_dir, rayPoint.m_range);

		rayPoint.m_type = KdCollider::TypeEvent | KdCollider::TypeBump | KdCollider::TypeGround;

		return rayPoint;
	}

	//自身のタイプ
	KdCollider::Type GetType()override { return KdCollider::TypeEvent; };

	//接触時処理
	void Hit(const int& _type, const Math::Vector3& _hitDir, const Math::Vector3&, const int&)override
	{
		if (_type == KdCollider::TypeBump)
		{
			m_mWorld *= Math::Matrix::CreateTranslation(_hitDir);
			STR_Setting.Gravity = 0.f;
		}

		if (_type == KdCollider::TypeEvent)
		{

		}

		//ダメージをもらうもの
		if (_type == KdCollider::TypeDamage)
		{
			STR_Setting.Hp -= 10;
		}

		//地面に当たったら
		if (_type == KdCollider::TypeGround)
		{
			//少し押す
			m_mWorld *= Math::Matrix::CreateTranslation(_hitDir);
			//重力は初期化
			STR_Setting.Gravity = 0.f;
		}
	}
	//==================================================================================================
private:

	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase() {}

		virtual void  Enter(Player& ) {}    //起動時
		virtual void   Exit(Player& ) {}    //終了時
		virtual void Update(Player& ) = 0;  //更新

	};

	//通常時
	class ActionIdle :public ActionStateBase
	{
	public:
		virtual ~ActionIdle() {}

		void  Enter(Player& )override;
		void Update(Player& )override;
	};

	//歩行時
	class ActionWalk :public ActionStateBase
	{
	public:
		virtual ~ActionWalk() {}

		void  Enter(Player& )override;
		void Update(Player& )override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> _nextState);
	std::shared_ptr<ActionStateBase> m_nowAction = nullptr;

	//ステージの情報
	GameScene* m_scene;

	//==================================================================================================
	//発表用（終わったら変更する）
	//==================================================================================================

	//攻撃アニメーションフラグ
	bool m_animeAttackFlg = false;

	//攻撃ディレイ
	float ActiveTime = 0.f;
	float ActivePlayTime = 60.f * 0.5f;
	//================================================================================================

	//デバック用======================================================================================

	KdDebugWireFrame m_debugWire;
	//================================================================================================

	//フラグ==========================================================================================
	bool m_control = true;

	//ロックオン関係
	bool m_lockFlg = false;
	bool m_lockOkFlg = false;

	//ブースト関係
	bool m_boostFlg = false;
	bool m_overheatFlg = false;

	//テスト
	float nowAccel = 0.f;
	float nowDownSpeed = 0.f;
	float nowSpeedMax = 0.f;

	//設定用変数
	struct SETTING
	{
		int   Hp = 0;              //体力
		float Gravity = 0.f;       //重力
		float GravityAccel = 0.f;  //重力加速度
		float MoveSpeed = 0.f;     //移動速度
		float MoveSpeedMax = 0.f;  //移動速度最大値
		float MoveAccel = 0.f;     //移動加速度
		float DownSpeed = 0.f;     //移動速度低下値
		float Ang = 0.f;           //通常回転速度
		float LookAng = 0.f;       //ロック時回転速度
	}STR_Setting;

	//ブースト関係
	struct BOOST
	{
		float Gauge = 0.f;         //ゲージ
		float GaugeMax = 0.f;      //ゲージ最大値
		float Speed = 0.f;         //速度
		float MaxSpeed = 0.f;      //速度最大値
		float MinSpeed = 0.f;      //速度最低値
		float Accel = 0.f;         //加速度
		float DownSpeed = 0.f;     //速度低下率
		float Heal = 0.f;          //回復量
		float HealTime = 0.f;      //回復時間
		float HealElapTime = 0.f;  //回復経過時間
		float HealSpeed = 0.f;     //回復速度
		float Cost = 0.f;          //使用時消費量
	}STR_Boost;

	//Uiに送るデータ群
	std::shared_ptr<struct PLAYERDATA>m_data;


	//モデル情報========================================================================================

	std::shared_ptr<KdModelWork> m_spModel;

	//ベクター==========================================================================================

	//カメラからみた方向ベクトル
	Math::Vector3 m_camLookVec = Math::Vector3::Zero;

	//ターゲットの座標
	Math::Vector3 m_targetPos = Math::Vector3::Zero;

	//ターゲットへのベクトル
	Math::Vector3 m_targetVec = Math::Vector3::Zero;

	//最終移動方向ベクトル
	Math::Vector3 m_moveVec = Math::Vector3::Zero;

	//行列==============================================================================================
	//カメラの回転行列情報
	Math::Matrix m_camRotMat = Math::Matrix::Identity;

	//回転行列
	Math::Matrix m_rotateMat = Math::Matrix::Identity;

	//アドレス===========================================================================================
	//カメラの情報
	std::weak_ptr<GameCamera> m_wpCamera;

	//システム
	std::weak_ptr<ObjectSystem> m_wpSys;

	//ターゲット情報
	std::weak_ptr<EnemyBase> m_wpTraEne;

	//アニメーション
	std::shared_ptr <KdAnimator> m_spAnimator = nullptr;

	//ウェポン
	std::shared_ptr<WeaponBase> m_spWeaBase;

	//プレイヤーの持っているウェポンリスト
	std::list<std::shared_ptr<WeaponBase>> m_weaList;

	//ロックオン対象リスト
	std::list<std::shared_ptr<EnemyBase>> m_lookTarList;

	//Jsonデータ
	json11::Json m_stageData;

	//エフェクト用====================================================================================
	std::shared_ptr<KdEffekseerObject> effObj;
	Math::Matrix effMat = Math::Matrix::Identity;
	//================================================================================================
};


//Uiに情報を渡す用
struct PLAYERDATA
{
	//Uiに送る情報群
	int HpData = 0;    //
	float BoostData = 0.f;  //
};