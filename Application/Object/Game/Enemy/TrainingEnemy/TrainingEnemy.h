#pragma once
#include"../EnemyBase.h"

class ObjectSystem;

class TrainingEnemy :public EnemyBase
{
public:
	TrainingEnemy() {}
	~TrainingEnemy() {}

	void Update()override;
	void PostUpdate()override;

	void DrawLit()override;
	void DrawSprite()override;
	void DrawDebug()override;

	void Init()override;

	Math::Color  m_color = kWhiteColor;

	//�����蔻��
	KdCollider::SphereInfo GetHitPointTypeSphere()override
	{
		//�����蔻��͋�
		KdCollider::SphereInfo spherePoint;

		//�e�X�g
		const KdModelWork::Node* pNode = m_spModel->FindNode("HitPoint");
		Math::Matrix mat;
		mat = pNode->m_worldTransform * m_mWorld;

		if (pNode)
		{
			Math::Vector3 pos = mat.Translation();
			//�ʒu�̎w��
			spherePoint.m_sphere.Center = pos;
			m_debugWire.AddDebugSphere(pos, 5.0f);
		}

		//���a
		spherePoint.m_sphere.Radius = 7.0f;

		//�����蔻����������^�C�v
		spherePoint.m_type =
			KdCollider::TypeBump |
			KdCollider::TypeDamage |
			KdCollider::TypeDamageLine |
			KdCollider::TypeEvent;

		return spherePoint;
	}

	//�J�������̃Z�b�g
	void SetCamera(const std::shared_ptr<GameCamera>& _camera)
	{
		m_wpCamera = _camera;
	}

	void Hit(const int& _type, const Math::Vector3&, const Math::Vector3&, const int&)override
	{
		//�m�F�p
		if (_type == KdCollider::TypeBump)
		{
			m_color = kGreenColor;
		}
		if (_type == KdCollider::TypeDamage)
		{
			m_color = kRedColor;
		}
		if (_type == KdCollider::TypeDamageLine)
		{
			m_color = { 1.f,0.f,1.f,1.f };//�p�[�v��
		}
		if (_type == KdCollider::TypeEvent)
		{
			m_color = kBlueColor;
		}
		m_hitFlg = true;
	}

	//���g�̃^�C�v
	KdCollider::Type GetType()override { return KdCollider::TypeEvent; };

private:

	bool m_reverseFlg = false;

	std::shared_ptr<KdTexture> m_spTex;

	//�J�����̏��
	std::weak_ptr<GameCamera> m_wpCamera;
};