#pragma once
class GameScene;
class WeaponBase;

class ObjectSystem;
class GameCamera;
class Player;

class EnemyBase :public KdGameObject
{
public:
	EnemyBase() {}
	~EnemyBase() {}

	//�A�b�v�f�[�g====================================================================================
	void PreUpdate()override;
	//�X�V
	void Update()override;
	//�ŏI�X�V
	void PostUpdate()override;
	//================================================================================================

	//�`��============================================================================================
	//�A�e����`��
	void DrawLit()override;
	void DrawLit_SkinMesh() override;
	//2D�`��
	void DrawSprite()override;
	void DrawDebug()override;
	//================================================================================================

	//������==========================================================================================
	void Init()override;
	//================================================================================================

	//�Q�b�^�[========================================================================================
	Math::Vector3 GetTargetPos() { return m_targetPos; }

	//�������Ăق����ꏊ
	virtual KdCollider::SphereInfo GetHitPointTypeSphere()
	{
		KdCollider::SphereInfo m;
		return m;
	}

	std::list<std::shared_ptr<EnemyBase>>GetEneList() { return m_eneList; }
	//=================================================================================================

	//�Z�b�^�[=========================================================================================
	void SetPlayer(std::weak_ptr<Player> _pla) { m_wpPla = _pla; }

	//�J�������̃Z�b�g
	void SetCamera(const std::shared_ptr<GameCamera>& _camera) { m_wpECamera = _camera; }

	void SetControl(bool _controlFlg) { m_controlFlg = _controlFlg; }

	//JSON����������炤�p�֐�
	void SetData(json11::Json _jsonData = nullptr) { m_stageData = _jsonData; }

	//�Q�[���V�[���̃A�h���X�����炤
	void SetScene(GameScene* _scene) { m_scene = _scene; }

	//=================================================================================================

	//�Q�b�^�[==========================================================================================
	//���f�����
	const std::shared_ptr<const KdModelWork> GetModel()override { return m_spModel; }

	bool EndFlg() { return endFlg; }

	//=================================================================================================

protected:

	//�f�o�b�N�p======================================================================================
	KdDebugWireFrame m_debugWire;
	//================================================================================================

	//�t���O==========================================================================================
	//����������
	bool m_hitFlg = false;
	bool m_controlFlg = true;
	bool endFlg = false;

	//int=============================================================================================
	//�e�X�g
	int m_waveCount = 0;
	int m_endWave = 0;
	char* test;

	//float===========================================================================================

	float m_alpha = 1.0f;
	float deg = 0.0f;
	float m_scale = 0.0f;

	//�x�N�^�[========================================================================================
	Math::Vector3 m_pos = {};

	//�ΏۃG�l�~�[�̍��W
	Math::Vector3 m_targetPos = {};
	//================================================================================================

	//�s��============================================================================================

	//�g�k�s��
	Math::Matrix m_scaleMat = Math::Matrix::Identity;

	//��]�s��
	Math::Matrix m_rotateMat = Math::Matrix::Identity;

	//�ړ��s��
	Math::Matrix m_transMat = Math::Matrix::Identity;
	//================================================================================================

	//�A�h���X========================================================================================

	//���f�����
	std::shared_ptr<KdModelWork>m_spModel;

	//�E�F�|��
	std::shared_ptr<WeaponBase> m_spWeaBase;

	//�X�e�[�W�̏��i���|�j
	GameScene* m_scene;

	//�J�����̏��
	std::weak_ptr<GameCamera> m_wpECamera;

	//�v���C���[�̏��
	std::weak_ptr<Player> m_wpPla;

	//�V�X�e��
	std::weak_ptr<ObjectSystem> m_wpSys;

	//Json�f�[�^
	json11::Json m_stageData;

	//�G�l�~�[�̎����Ă���E�F�|�����X�g
	std::list<std::shared_ptr<WeaponBase>> m_weaList;

	//�G�l�~�[���X�g
	std::list<std::shared_ptr<EnemyBase>> m_eneList;
	//================================================================================================

private:
};

typedef struct EnemySorter
{
	EnemySorter(std::shared_ptr<EnemyBase>_e, float _dist) :m_enem(_e), m_dist(_dist) {}
	std::shared_ptr<EnemyBase>m_enem;
	float m_dist;

	//�~��
	bool operator<(const EnemySorter& _b)
	{
		return m_dist < _b.m_dist;
	}

	//����
	//bool operator>(const EnemySorter& _b)
	//{
	//	return m_dist > _b.m_dist;
	//}
}test;

