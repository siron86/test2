#include "Sky.h"

void Sky::Update()
{
	//m_ang.y += 0.001f;
	//if (m_ang.y >= 360)m_ang.y = 0;

	m_scaleMat = Math::Matrix::CreateScale(3000.f, 3000.f, 3000.f);
	m_rotatMat = Math::Matrix::CreateRotationY(m_ang.y);
	m_transMat = Math::Matrix::CreateTranslation({ m_pos });

	m_mWorld = m_scaleMat * m_rotatMat * m_transMat;
}

void Sky::DrawUnLit()
{
	//3D���f��
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Sky::Init()
{
	//z-1200�܂łȂ�`��\

	//���f���i�[�p�̓��ꕨ�쐬
	m_spModel = std::make_shared<KdModelWork>();

	//m_modeldatas�@���f���S���I
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Sky/Sky.gltf"));
}