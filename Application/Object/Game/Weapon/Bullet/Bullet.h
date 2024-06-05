#pragma once
#include"../WeaponBase.h"

class ObjectSystem;

class Bullet :public WeaponBase
{
public:
	Bullet() {}
	~Bullet() {}

	void Update()override;
	void PostUpdate()override {}

	void DrawLit()override;
	void DrawUnLit()override;

	void DrawDebug()override;

	void Init()override;

	//�~�T�C���e�p����
	void MissileAction();

	//�z�[�~���O����ꍇ
	void Homing(Math::Vector3 _targetPos)
	{
		m_homingFlg = true;
		m_targetPos = _targetPos;
	}

	void UpdateRotateFromTarget(Math::Vector3& _tarVec);

	//����ɂ���Đݒ��ς���
	void SetBulletType(Math::Vector3 _scale, float _speed)
	{
		m_scale = _scale;
		m_speed = _speed;
	}

	//���˕���
	void SetShotVec(const Math::Vector3& _shotVec) { m_shotVec = _shotVec; }

	void GetWeaBase(const std::shared_ptr<KdGameObject>& _weaBase)
	{
		//�_�E���L���X�g
		m_weaBase = std::dynamic_pointer_cast<WeaponBase>(_weaBase);
	}

	void Hit(const int&, const Math::Vector3&, const Math::Vector3&,const int&)override
	{
		m_isExpired = true;
	}

	KdCollider::RayInfo GetHitPointTypeRay()override
	{
		//�����蔻��͋�
		KdCollider::RayInfo rayPoint;

		//�ʒu�̎w��
		rayPoint.m_pos = m_mWorldPoly.Translation();
		
		//�������������ĂȂ������牼�w��
		if (m_shotVec.z == 0.0f)m_shotVec = Math::Vector3(0.f, 0.f, 1.f);
		//��ԕ����ɓ����蔻��p���C��΂�
		Math::Vector3 targetVec = m_shotVec;
		targetVec.Normalize();//���K��
		//�����̊m��
		rayPoint.m_dir = targetVec;

		//���a
		rayPoint.m_range = 5.0f;

		//�����蔻����������^�C�v
		m_debugWire.AddDebugLine(m_mWorldPoly.Translation(), targetVec, rayPoint.m_range, kRedColor);
		rayPoint.m_type = KdCollider::TypeEvent | KdCollider::TypeBump | KdCollider::TypeGround;

		return rayPoint;
	}

	//���g�̃^�C�v
	KdCollider::Type GetType()override { return KdCollider::TypeDamage; };

private:

	float count = 0.f;
	float deleteCount = 0.f;
	float maxCount = 60.0f;
	float deleteMaxCount = 60.0f * 1.0f;

	float m_activeCount = 0.0f;

	//���ˑ��x
	float m_speed = 0.0f;

	//�˒�
	static const int m_range = 1000;

	//�~�T�C���t���O
	bool m_homingFlg = false;
	//�G�t�F�N�g�t���O
	bool m_effFlg = false;

	Math::Vector3 m_shotPos = {};

	//�e�̃T�C�Y
	Math::Vector3 m_scale = Math::Vector3::Zero;

	//�|���S���p�s��
	Math::Matrix m_mWorldPoly;

	//�I�u�W�F�N�g���
	std::shared_ptr<KdModelWork> m_spModel;

	std::shared_ptr<KdSquarePolygon> m_spPoly;

	std::weak_ptr<WeaponBase>m_weaBase;

	//�V�X�e��
	std::weak_ptr<ObjectSystem> m_wpSys;

	//�G�t�F�N�g�p====================================================================================
	std::shared_ptr<KdEffekseerObject> effObj;
	Math::Matrix effMat = Math::Matrix::Identity;
	//================================================================================================
};