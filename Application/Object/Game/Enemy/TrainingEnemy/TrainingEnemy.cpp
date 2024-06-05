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
	//3D���f��
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld, m_color);
}

void TrainingEnemy::DrawSprite()
{
	//�v�Z���ĕԂ��Ă����l
	Math::Vector3 returnPos;

	//�ǂ��ɕ\�����邩�v�Z
	KdDirect3D::Instance().ConverWorldToScreenDatail(
		*m_wpCamera.lock()->WorkCamera(), //�J�����̏��
		GetHitPointTypeSphere().m_sphere.Center,    //�G�l�~�[�̍��W
		returnPos);                       //�\������ʒu
	
	//��̌v�Z�ŏo�����W�����炢�`��
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
	//�傫��
	m_scale = 1.0f;

	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/Training/Target.gltf"));

	m_spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Enemy/LockOk.png");

	m_pos.x = 100.f;
	m_mWorld = Math::Matrix::CreateScale(m_scale) * Math::Matrix::CreateTranslation(m_pos);

	//�����蔻��p
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_spModel, KdCollider::TypeEvent);
}