#include "Ground.h"

void Ground::DrawUnLit()
{
	for (unsigned int i = 0; i < m_vvModel.size(); i++)
	{
		for (unsigned int j = 0; j < m_vvModel[i].size(); j++)
		{
			//3Dモデル
			Math::Matrix mat = Math::Matrix::CreateTranslation(m_vvModel[i][j].pos.x, 0.f, m_vvModel[i][j].pos.y) * m_mWorld;
			KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel , m_mWorld);
		}
	}
}

void Ground::Init()
{
	m_scaleMat = Math::Matrix::CreateScale(20.0f);
	//m_rotatMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90));
	m_transMat = Math::Matrix::CreateTranslation({ 0, 0, 0 });

	m_mWorld = m_scaleMat * m_rotatMat * m_transMat;

	//マップの大きさ
	int mapSize = 12;

	//多重配列の初期化
	m_vvModel.resize(mapSize);
	for (int i = 0; i < mapSize; i++)
	{
		m_vvModel[i].resize(mapSize);
	}

	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Ground/TestStage.gltf"));

	for (unsigned int y = 0; y < m_vvModel.size(); y++)
	{
		for (unsigned int x = 0; x < m_vvModel[y].size(); x++)
		{
			m_vvModel[y][x].dataType = Type::TypeO;
			if (x == 2)m_vvModel[y][x].dataType = Type::TypeU;
			if (x == 3)m_vvModel[y][x].dataType = Type::TypeL;
			if (x == 4)m_vvModel[y][x].dataType = Type::TypeII;
			if (x == 5)m_vvModel[y][x].dataType = Type::TypeI;
			if (x == 6)m_vvModel[y][x].dataType = Type::Error;
			m_vvModel[y][x].pos = Math::Vector2(x * 20.f - 110.f, y * 20.f - 110.f);
		}
	}

	//当たり判定用
	//文字列で参照するための名前付け
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Ground", m_spModel, KdCollider::TypeGround);
}

void Ground::Collider(std::shared_ptr<KdModelWork> _objName)
{
	_objName = std::make_shared<KdModelWork>();
}
