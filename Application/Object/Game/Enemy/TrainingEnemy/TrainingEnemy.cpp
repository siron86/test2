#include "TrainingEnemy.h"
#include"../../../Camera/GameCamera/GameCamera.h"

#include"../../ObjectSystem/ObjectSystem.h"

void TrainingEnemy::Update()
{
	if (m_hitFlg)
	{
		m_hitFlg = false;
	}
	else
	{
		m_color = kWhiteColor;
	}

	m_wpSys.lock()->HitPointTypeSphere(shared_from_this());
}

void TrainingEnemy::PostUpdate()
{
}

void TrainingEnemy::DrawLit()
{
	if (!m_spModel)return;
	//3Dモデル
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld, m_color);
}

void TrainingEnemy::DrawSprite()
{
	//計算して返ってきた値
	Math::Vector3 returnPos;

	//どこに表示するか計算
	KdDirect3D::Instance().ConverWorldToScreenDatail(
		*m_wpCamera.lock()->WorkCamera(), //カメラの情報
		GetHitPointTypeSphere().m_sphere.Center,    //エネミーの座標
		returnPos);                       //表示する位置
	
	//上の計算で出た座標をもらい描画
	KdShaderManager::Instance().m_spriteShader.DrawTex(
		m_spTex.get(),
		(int)returnPos.x,
		(int)returnPos.y,
		50,
		50
	);

}

void TrainingEnemy::DrawDebug()
{
	m_debugWire.Draw();
}

void TrainingEnemy::Init()
{
	//大きさ
	m_scale = 1.0f;

	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/Training/Target.gltf"));

	m_spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Enemy/LockOk.png");

	m_pos.x = 100.f;
	m_mWorld = Math::Matrix::CreateScale(m_scale) * Math::Matrix::CreateTranslation(m_pos);

	//当たり判定用
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_spModel, KdCollider::TypeEvent);
}