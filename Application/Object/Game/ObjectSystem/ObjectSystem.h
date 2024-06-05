#pragma once

class ObjectSystem
{
public:
	ObjectSystem() {}
	~ObjectSystem() {}
	//=======================================================================================
	//�����蔻��
	//�������Ăق����^�C�v�Ɠ������������������^�C�v��Ԃ����Ƃœ����蔻�������
	//=======================================================================================
	void HitPointTypeSphere(const std::weak_ptr<KdGameObject>& _owner);


	void HitPointTypeRay(const std::weak_ptr<KdGameObject>& _owner);

	void HitSearch(const std::weak_ptr<KdGameObject>& _owner, const KdCollider::SphereInfo& _sphereInfo);
	void HitSearch(const std::weak_ptr<KdGameObject>& _owner, const KdCollider::RayInfo& _rayInfo);

	//bool TestHitPoint(KdCollider::SphereInfo _ownerInfo ,KdCollider::Type _type);
	//================================================================================================

	//=======================================================================================
	//���b�N�I���@�\(�P�̗p)
	// ���b�N�I���͑Ώۂ̏������炢�A�Ώۂ̍��W��p���đΏۂւ̕����x�N�g�����쐬���A���������킹��
	//=======================================================================================
	//void LockOn(const std::weak_ptr<KdGameObject>& _owner, const std::weak_ptr<KdGameObject>& _target);

	//=======================================================================================
	//���b�N�I���@�\(�����p)
	// ���b�N�I���͑Ώۂ̏������炢�A�Ώۂ̍��W��p���đΏۂւ̕����x�N�g�����쐬���A���������킹��
	// �^�[�Q�b�g���̃��X�g�����炢
	//=======================================================================================
	//void LockOnMulti(const std::weak_ptr<KdGameObject>& _owner, const std::weak_ptr<KdGameObject>& _target) {}

	//�\�[�g�����^�[�Q�b�g����ێ��A�^����
	std::list<std::shared_ptr<KdGameObject>> GetTargetSortList() {}

private:

	//�����蔻�����������������郊�X�g����܂�
	std::list<std::shared_ptr<KdGameObject>> m_hitList;
};