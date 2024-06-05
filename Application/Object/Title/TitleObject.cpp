#include "TitleObject.h"

void TitleObject::DrawLit()
{
	//3D���f��
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void TitleObject::Init()
{
	//���f���i�[�p�̓��ꕨ�쐬
	m_model = std::make_shared<KdModelWork>();

	//m_modeldatas�@���f���S���I
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Title/Title.gltf"));

	//m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);
	//m_rotateMat = Math::Matrix::CreateRotationY(-90);
	//m_transMat = Math::Matrix::CreateTranslation({ 0, 0, 0 });
	m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);
	m_rotateMat = Math::Matrix::CreateRotationY(0);
	m_transMat = Math::Matrix::CreateTranslation({ 0, 0, 0 });

	m_mWorld = m_scaleMat * m_rotateMat * m_transMat;
}