#include "NormalEnemy.h"
#include"../../../Game/Weapon/WeaponBase.h"
#include"../../../Game/Player/Player.h"
#include"../../../Game/ObjectSystem/ObjectSystem.h"

void NormalEnemy::PreUpdate()
{
	if (m_isExpired)return;

	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->PreUpdate();
	//===================================================
}

void NormalEnemy::Update()
{
	if (m_isExpired)return;

	//当たり判定処理
	std::shared_ptr<ObjectSystem>objSystem = m_wpSys.lock();
	objSystem->HitPointTypeSphere(shared_from_this());
	objSystem->HitPointTypeRay(shared_from_this());

	//攻撃ローテーション
	Attack();

	//死亡時
	DeathAnima();

	//移動
	if (!m_wpPla.expired())
	{
		Math::Vector3 moveVec = m_wpPla.lock()->GetPos() - GetPos();
		if (moveVec.Length() <= 100)moveVec = Math::Vector3::Zero;
		moveVec.Normalize();
		moveVec *= 0.5f;
		//重力テスト
		gravity -= 0.002f;
		moveVec.y += gravity;
		Math::Matrix trans = Math::Matrix::CreateTranslation(moveVec);
		m_mWorld *= trans;
	}

	//移動はこの上に書く
	RotateFromTarget(m_mWorld.Backward(), m_wpPla.lock()->GetPos() - GetPos());

	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	AttackType((int)m_attackType);
	m_spWeaBase->SetTarPos(m_wpPla.lock()->GetHitPointTypeSphere().m_sphere.Center);
	m_spWeaBase->SetScene(m_eScene);
	m_spWeaBase->Update();
	m_weaList = m_spWeaBase->GetWeaList();
	//===================================================
}

void NormalEnemy::PostUpdate()
{
	if (!m_spAnimator)	return;

	// アニメーションの更新
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes(), 3.0f);
	m_spModel->CalcNodeMatrices();

	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->PostUpdate();
	//===================================================
}

void NormalEnemy::DrawLit()
{
	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawLit();
	//===================================================
}

void NormalEnemy::DrawLit_SkinMesh()
{
	if (!m_spModel)return;
	//3Dモデル
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void NormalEnemy::DrawDebug()
{
	m_debugWire.Draw();

	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawDebug();
	//===================================================
}

void NormalEnemy::Init()
{
	//大きさ
	m_scale = 1.f;
	m_mWorld *= Math::Matrix::CreateScale(m_scale);

	std::random_device rnd2;
	std::mt19937 mt2(rnd2());
	std::uniform_int_distribution<int> intRand2(5, 9);

	int randNum2[5] = { 4,3,2,1 };
	int rndNum = intRand2(mt2);

	rndNum = intRand2(mt2);

	for (int j = 0; j < 5; j++)
	{
		rndNum -= randNum2[j];
		m_attackCount = rndNum;
	}

	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/Normal/NormalEnemy.gltf"));

	// 初期のアニメーションをセットする
	m_spAnimator = std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Attack"), false);

	//当たり判定用
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_spModel, KdCollider::TypeEvent);

	//ウェポンベース===============================================================
	m_spWeaBase = std::make_shared<WeaponBase>();
	m_spWeaBase->Init();
	m_spWeaBase->SetWeaponType(1);
	m_spWeaBase->SetOwner(shared_from_this());
	//=============================================================================
}

//攻撃方法
void NormalEnemy::AttackType(int _type)
{
	//攻撃時エネミーという事をウェポンに伝え、一度の攻撃で出る弾数などを教える
	m_spWeaBase->EnemyBullet(m_maxShotCount, true);

	m_spWeaBase->SetWeaponType(_type);
	m_spWeaBase->Active();
}

void NormalEnemy::Attack()
{
	//================================================================================================
	//ローテーション
	//================================================================================================
	if (m_coolTime <= 0)
	{
		++m_attackCount;
		switch (m_attackCount)
		{
		case 1:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			break;
		case 2:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			break;
		case 3:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			break;
		case 4:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			break;
		case 5:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			m_attackCount = 0;
			break;
		}
		m_coolTime = 60.0f * 5.0f;
	}
	--m_coolTime;
	//================================================================================================
}

void NormalEnemy::RotateFromTarget(Math::Vector3 _ownerVec, Math::Vector3 _tarVec)
{
	_ownerVec.Normalize();
	_tarVec.Normalize();

	//回転角度を求める(内積)
	float dot = _ownerVec.Dot(_tarVec);
	//リミッター
	dot = std::clamp(dot, -1.0f, 1.0f);

	//内積から逆余弦の定理で間の角度を求める
	float Ang = acos(dot);
	Ang = DirectX::XMConvertToDegrees(Ang);

	float rotateAng = std::clamp(Ang, -8.0f, 8.0f);

	Math::Vector3 rotAxis;

	_ownerVec.Cross(_tarVec, rotAxis);

	if (rotAxis.LengthSquared() == 0)return;

	//誘導回転する為の行列
	rotAxis = Math::Vector3(0.f, rotAxis.y, 0.f);
	//Error発生元
	m_rotateMat = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	//今のワールド座標を一時的に保存
	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	//回転行列の合成
	m_mWorld *= m_rotateMat;

	//回転をかけた後に戻す（じゃないと原点からの角度が膨れていくから）
	m_mWorld.Translation(pos);
}

void NormalEnemy::DeathAnima()
{
	if (m_hp > 0.f)return;
	//死亡時エフェクト
	KdEffekseerManager::GetInstance().Play("O_patch_stElmo_area_2.efkefc", GetHitPointTypeSphere().m_sphere.Center, false);
	KdEffekseerManager::GetInstance().SetScale("O_patch_stElmo_area_2.efkefc", 3);

	if (!KdEffekseerManager::GetInstance().IsPlaying("O_patch_stElmo_area_2.efkefc")) { m_isExpired = true; }
}