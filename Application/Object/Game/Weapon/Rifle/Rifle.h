#pragma once
#include"../WeaponBase.h"

class Rifle :public WeaponBase
{
public:
	Rifle() { Init(); }
	~Rifle() {}

	//�X�V
	void Update()override;
	void PostUpdate()override;

	//�`��
	void DrawLit_SkinMesh() override;
	void DrawDebug()override;
	
	//������
	void Init()override;

	//�@�\�֐�

	//�����[�h����
	void Reload();

	//�U���i�ˌ��j����
	void Shot();

	//�U���Ԋu�̐ݒ�
	void CoolTime();

	void SetRack(const Math::Matrix& _rackMat)override { m_rackMat = _rackMat; }

	void SelectFlg()override { m_selectFlg = true; }

	void ShotFlg()override { m_shotFlg = true; }

	void SetWeaBase(const std::shared_ptr<KdGameObject>& _weaBase)
	{
		//�_�E���L���X�g
		m_weaBase = std::dynamic_pointer_cast<WeaponBase>(_weaBase);
	}

private:

	std::weak_ptr <WeaponBase>m_weaBase;

	//�G�t�F�N�g�p====================================================================================
	std::shared_ptr<KdEffekseerObject> effObj;
	Math::Matrix effMat = Math::Matrix::Identity;
	std::list<std::shared_ptr<WeaponBase>> m_weaList;
};