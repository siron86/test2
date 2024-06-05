#pragma once
#include"../Stage.h"

class Ground :public Stage
{
public:
	Ground() { Init(); }
	~Ground() {}

	void DrawUnLit() override;
	void Init()    override;

	void Collider(const std::shared_ptr<KdModelWork> _objName);

	

	//
	enum class  Type
	{
		TypeO,
		TypeU,
		TypeL,
		TypeII,
		TypeI,
		Error,
	};

	void SetGroundData(const Type& _type, const Math::Vector2&_pos)
	{
		m_groundData.dataType=_type;
		m_groundData.pos = _pos;
	}

	const std::shared_ptr<KdModelWork>& SelectModel(Type& _type)
	{
		m_spModel = std::make_shared<KdModelWork>();
		switch (_type)
		{
		case Ground::Type::TypeO:
			m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Ground/StageBlock_O.gltf"));
			break;
		case Ground::Type::TypeU:
			m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Ground/StageBlock_U.gltf"));
			break;
		case Ground::Type::TypeL:
			m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Ground/StageBlock_L.gltf"));
			break;
		case Ground::Type::TypeII:
			m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Ground/StageBlock_II.gltf"));
			break;
		case Ground::Type::TypeI:
			m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Ground/StageBlock_I.gltf"));
			break;
		case Ground::Type::Error:
			//error—p‚Ì”’‚¢’n–Ê‚ð•\Ž¦
			//m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Ground/StageBlock_O.gltf"));
			break;
		}
		return m_spModel;
	}

private:

	struct ModelData
	{
		ModelData() {
			dataType = Type::Error;
		}
		Math::Vector2 pos;
		Type dataType;
	};


	std::vector<std::vector<ModelData>> m_vvModel;
	ModelData m_groundData;
};