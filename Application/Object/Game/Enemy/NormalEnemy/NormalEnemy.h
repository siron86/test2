#pragma once
#include"../EnemyBase.h"
class EnemyUi;
class GameScene;
class ObjectSystem;

class NormalEnemy :public EnemyBase
{
public:

	NormalEnemy() {}
	~NormalEnemy() {}

	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;

	void DrawLit()override;
	void DrawLit_SkinMesh()override;
	void DrawDebug() override;

	void Init()override;

	enum class AttackWeapon
	{
		Zero,
		Rifle,
		Missile,
		EleGun
	};

	//攻撃方法
	void AttackType(int _type);
	void Attack();

	//ロックオン
	void RotateFromTarget(Math::Vector3 _ownerVec, Math::Vector3 _tarVec);
	bool LockOnFlg()override { return m_lockFlg; }

	//死亡時アニメーション
	void DeathAnima();
	void SetNPlayer(std::weak_ptr<Player> _pla) { m_wpPla = _pla; }

	//ゲームシーンのアドレスをもらう
	void SetEScene(GameScene* _scene) { m_eScene = _scene; }

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
		}

		spherePoint.m_sphere.Radius = 7.0f;

		spherePoint.m_type = KdCollider::TypeEvent | KdCollider::TypeBump | KdCollider::TypeDamage | KdCollider::TypeGround;
		m_debugWire.AddDebugSphere(spherePoint.m_sphere.Center, spherePoint.m_sphere.Radius);
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

	void Hit(const int& _type, const Math::Vector3& _hitDir, const Math::Vector3&, const int&)override
	{
		if (_type == KdCollider::TypeGround)
		{
			m_mWorld *= Math::Matrix::CreateTranslation(_hitDir);
			gravity = 0.f;
		}

		if (_type == KdCollider::TypeBump)
		{
			m_mWorld *= Math::Matrix::CreateTranslation(_hitDir);
			gravity = 0.f;
		}
		if (_type == KdCollider::TypeDamage)
		{
			m_hp = 0.f;
		}

		if (_type == KdCollider::TypeEvent)
		{
		}
	}

private:

	int hp = 10;
	int m_attackCount = 0;
	int m_shotCount = 0;
	int m_maxShotCount = 0;

	float m_hp = 450.f;
	//重力
	float gravity = 0.f;
	float m_coolTime = 0;

	bool m_lockFlg = true;
	bool m_reverseFlg = false;

	//アニメーション
	std::shared_ptr <KdAnimator> m_spAnimator = nullptr;

	std::shared_ptr<EnemyUi>m_eneUi;
	
	//システム
	std::weak_ptr<ObjectSystem> m_wpSys;

	//ステージの情報（生ポ）
	GameScene* m_eScene;

	AttackWeapon m_attackType = AttackWeapon::Rifle;
};