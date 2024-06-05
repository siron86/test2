#include "SelectObject.h"

void SelectSObject::Update()
{
	//モードを選択し、選択されたモードにあったものを表示


}

void SelectSObject::PostUpdate()
{
	m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);
	m_rotateMat = Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_ang.y),
		DirectX::XMConvertToRadians(m_ang.x),
		DirectX::XMConvertToRadians(m_ang.z)
	);
	m_transMat = Math::Matrix::CreateTranslation({ 0, 0, 0 });

	m_mWorld = m_scaleMat * m_rotateMat * m_transMat;
}

void SelectSObject::DrawLit()
{
	//3Dモデル
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void SelectSObject::Init()
{
	//モデル格納用の入れ物作成
	m_model = std::make_shared<KdModelWork>();

	//m_modeldatas　モデル担当！
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Title/Title.gltf"));
	m_ang.y = -180;
}