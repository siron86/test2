#pragma once
#include"../Stage.h"

class StageObject :public Stage
{
public:
	StageObject() {}
	~StageObject() {}

	void ImGuiUpdate()  override;
	void Update()       override;
	void DrawLit()      override;
	void DrawDebug()    override;
	void Init()         override;

	void Collider(const std::shared_ptr<KdModelWork> _objName);

	bool GetSelect()override
	{
		return m_selectFlg;
	}

	void BuildingLoad(const json11::Json& _buildingData)
	{
		m_objectData = _buildingData;
	}

	KdCollider::SphereInfo GetHitPointTypeSphere()override
	{
		//�����蔻��͋�
		KdCollider::SphereInfo spherePoint;

		//�ʒu�̎w��
		spherePoint.m_sphere.Center = GetPos();

		//���a
		spherePoint.m_sphere.Radius = 7.0f;
		m_debugWire.AddDebugSphere(GetPos(), 10.0f);

		//�����蔻����������^�C�v
		spherePoint.m_type = KdCollider::TypeEvent;

		return spherePoint;
	}

	enum class ModelType
	{
		Building,
		Apartment,
	};

private:

	json11::Json m_objectData;
};