#pragma once
#include"../WeaponBase.h"

class ObjectSystem;

class Bullet :public WeaponBase
{
public:
	Bullet() {}
	~Bullet() {}

	void Update()override;
	void PostUpdate()override {}

	void DrawLit()override;
	void DrawUnLit()override;

	void DrawDebug()override;

	void Init()override;

	//ミサイル弾用挙動
	void MissileAction();

	//ホーミングする場合
	void Homing(Math::Vector3 _targetPos)
	{
		m_homingFlg = true;
		m_targetPos = _targetPos;
	}

	void UpdateRotateFromTarget(Math::Vector3& _tarVec);

	//武器によって設定を変える
	void SetBulletType(Math::Vector3 _scale, float _speed)
	{
		m_scale = _scale;
		m_speed = _speed;
	}

	//発射方向
	void SetShotVec(const Math::Vector3& _shotVec) { m_shotVec = _shotVec; }

	void GetWeaBase(const std::shared_ptr<KdGameObject>& _weaBase)
	{
		//ダウンキャスト
		m_weaBase = std::dynamic_pointer_cast<WeaponBase>(_weaBase);
	}

	void Hit(const int&, const Math::Vector3&, const Math::Vector3&,const int&)override
	{
		m_isExpired = true;
	}

	KdCollider::RayInfo GetHitPointTypeRay()override
	{
		//当たり判定は球
		KdCollider::RayInfo rayPoint;

		//位置の指定
		rayPoint.m_pos = m_mWorldPoly.Translation();
		
		//もし方向が来てなかったら仮指定
		if (m_shotVec.z == 0.0f)m_shotVec = Math::Vector3(0.f, 0.f, 1.f);
		//飛ぶ方向に当たり判定用レイ飛ばす
		Math::Vector3 targetVec = m_shotVec;
		targetVec.Normalize();//正規化
		//方向の確定
		rayPoint.m_dir = targetVec;

		//半径
		rayPoint.m_range = 5.0f;

		//当たり判定をしたいタイプ
		m_debugWire.AddDebugLine(m_mWorldPoly.Translation(), targetVec, rayPoint.m_range, kRedColor);
		rayPoint.m_type = KdCollider::TypeEvent | KdCollider::TypeBump | KdCollider::TypeGround;

		return rayPoint;
	}

	//自身のタイプ
	KdCollider::Type GetType()override { return KdCollider::TypeDamage; };

private:

	float count = 0.f;
	float deleteCount = 0.f;
	float maxCount = 60.0f;
	float deleteMaxCount = 60.0f * 1.0f;

	float m_activeCount = 0.0f;

	//発射速度
	float m_speed = 0.0f;

	//射程
	static const int m_range = 1000;

	//ミサイルフラグ
	bool m_homingFlg = false;
	//エフェクトフラグ
	bool m_effFlg = false;

	Math::Vector3 m_shotPos = {};

	//弾のサイズ
	Math::Vector3 m_scale = Math::Vector3::Zero;

	//ポリゴン用行列
	Math::Matrix m_mWorldPoly;

	//オブジェクト情報
	std::shared_ptr<KdModelWork> m_spModel;

	std::shared_ptr<KdSquarePolygon> m_spPoly;

	std::weak_ptr<WeaponBase>m_weaBase;

	//システム
	std::weak_ptr<ObjectSystem> m_wpSys;

	//エフェクト用====================================================================================
	std::shared_ptr<KdEffekseerObject> effObj;
	Math::Matrix effMat = Math::Matrix::Identity;
	//================================================================================================
};