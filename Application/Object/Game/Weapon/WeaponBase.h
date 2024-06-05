#pragma once
class Rifle;
class Rocket;
class EleGun;

class GameScene;
class GameCamera;

//�E�F�|�����
enum WeaponType
{
	Rif = 1,
	Roc = 2,
	Ele = 3,
};

//�E�F�|���Ǘ��N���X
class WeaponBase :public KdGameObject
{
public:
	WeaponBase() {}
	~WeaponBase() {}

	//�X�V
	void ImGuiUpdate() override;
	void PreUpdate() override;
	void Update()    override;
	void PostUpdate()override;

	//�`��
	void DrawLit()   override;
	void DrawLit_SkinMesh() override;
	void DrawUnLit()   override;

	//�f�o�b�N���C���[
	void DrawDebug()   override;

	//������
	void Init()      override;

	//�Z�b�^�[==========================================================================================
	//�J�������̃Z�b�g
		//�Q�[���V�[���̃A�h���X�����炤
	void SetScene(GameScene* _scene) { m_scene = _scene; }

	//�^�[�Q�b�g�̍��W��������Ă���
	void SetTarPos(const Math::Vector3& _tarPos) { m_targetPos = _tarPos; }

	//���L�ҏ��������Ă���
	void SetOwner(const std::shared_ptr<KdGameObject>& _spOwn) { m_wpOwn = _spOwn; }

	void SetCamera(const std::shared_ptr<GameCamera>& _camera) { m_wpCamera = _camera; }

	virtual void SetRack(const Math::Matrix& _rackMat) { m_rackMat = _rackMat;}

	//�Q�b�^�[==========================================================================================

	//�^�[�Q�b�g���W
	Math::Vector3 GetTarPos() { return m_targetPos; }

	//���f�����
	const std::shared_ptr<const KdModelWork> GetModel() { return m_spModel; }

	//�E�F�|�����X�g�擾
	const std::list<std::shared_ptr<WeaponBase>>& GetWeaList() { return m_weaList; }

	//�e�̓o�^
	void AddWeapon(const std::shared_ptr<WeaponBase>& _weapon) { m_weaList.push_back(_weapon); }

	//��������ꏊ
	const Math::Matrix EquipmentPoint(const std::string_view& _pointName);

	//==================================================================================================

	//�I�΂�Ă��邩
	virtual void SelectFlg() {};

	//�N�������̂������`���[�N���X��������~�T�C����
	//�����`���[�ȊO�̏ꍇ�G�t�F�N�g�𔭎˂�����
	//�G�t�F�N�g�͕���ɂ��ύX����
	virtual void ShotFlg() {};

	//����̏e�����ƃJ�����̃��C�����g���ĉ�ʒ��S�ւ̃x�N�g�����Z�o����
	Math::Vector3 CenterShot(const Math::Matrix& _muzzleMat);

	//==================================================================================================
	//�G�l�~�[�̎��N������֐�
	void EnemyBullet(int _maxShotCount = 0, bool _enemyFlg = false)
	{
		m_eneMaxBul = _maxShotCount;
		m_enemyFlg = _enemyFlg;
	}
	bool Enemy() { return m_enemyFlg; }
	int EnemyMaxShot() { return m_eneMaxBul; }
	//==================================================================================================

	//���킲�Ƃ̒e���
	//���ˈʒu�A�e�̃T�C�Y�A�X�s�[�h�̐ݒ�
	void ShotBullet(
		const Math::Matrix& _muzzleMat,  //�e�����
		const Math::Vector3& _size,  //�e�̑傫��
		const float& _speed,  //���x
		const int& _damage,  //���킲�Ƃ̃_���[�W
		const Math::Vector3& _shotPos = Math::Vector3::Zero  //�~�T�C���̂ݎg�p
	);

	virtual Math::Vector3 GetShotPos() { return Math::Vector3::One; }

	//�ˌ����ɋN������֐�
	void Active();

	//�ǂ�������Ă��邩
	int GetWeapomType()
	{
		return m_weaType;
	}

	//�����Ă���E�F�|��
	void SetWeaponType(int _type)
	{
		switch (_type)
		{
		case 1:
			m_weaType = Rif;
			break;
		case 2:
			m_weaType = Roc;
			break;
		case 3:
			m_weaType = Ele;
			break;
		default:
			break;
		}
	}



protected:

	//�E�F�|���^�C�v
	WeaponType m_weaType = Rif;

	//�p����g�p�ϐ�==================================================================
	//�o���b�g�p�ϐ��i�[
	struct BULLET
	{
		float scale = 0.f;  //�傫��
		float speed = 0.f;  //�e��
	}STR_Bullet;

	//�E�F�|���p�ϐ��i�[�p
	struct WEAPON
	{
		float scale = 0.f;			  //�T�C�Y
		int bulletMax = 0;			  //�ő�e��
		int bulletCount = 0;		  //���݂̒e��
		float relTime = 0.f;		  //�����[�h����
		float nowRelTime = 0.f;		  //�����[�h�o�ߎ���
		float coolTime = 0.f;		  //�N�[���^�C��
		float defaultCoolTime = 5.f;  //�f�t�H���g�N�[���^�C��

	}STR_Weapon;

	//�_���[�W
	int m_damage = 0;
	//�G�l�~�[�̎��̍ő�e��
	int m_eneMaxBul = 0;

	//�t���O============================================================================
	//�����[�h�t���O
	bool m_relFlg = false;

	//���˃t���O
	bool m_fireFlg = false;

	//�I�𒆃E�F�|���t���O
	bool m_selectFlg = false;

	//���˃t���O
	bool m_shotFlg = false;

	//�G�l�~�[���ǂ���
	bool m_enemyFlg = false;

	//�x�N�^�[==========================================================================
	//���W
	Math::Vector3 m_pos = Math::Vector3::Zero;

	//�p�x
	Math::Vector3 m_ang = Math::Vector3::Zero;

	//�^�[�Q�b�g�܂ł̃x�N�g���i�I�[�i�[����擾���A�e�����֑���j
	Math::Vector3 m_targetVec = Math::Vector3::Zero;
	//�^�[�Q�b�g�̍��W
	Math::Vector3 m_targetPos = Math::Vector3::Zero;

	Math::Vector3 m_shotVec = Math::Vector3::Zero;

	//�s��==============================================================================
	//�g��s��
	Math::Matrix m_scaleMat = Math::Matrix::Identity;

	//��]�s��
	Math::Matrix m_rotateMat = Math::Matrix::Identity;

	//�ړ��s��
	Math::Matrix m_transMat = Math::Matrix::Identity;

	//�e���̍s��
	Math::Matrix m_muzzleMat = Math::Matrix::Identity;

	//���@�̓���̏ꏊ�ɔz�u����
	Math::Matrix m_rackMat = Math::Matrix::Identity;

	//���==============================================================================

	//���f��
	std::shared_ptr<KdModelWork> m_spModel = nullptr;

	//�^�[�Q�b�g���擾�p
	std::weak_ptr<KdGameObject>m_wpTra;

	//���L�҂̏��
	std::weak_ptr<KdGameObject>m_wpOwn;

	//�J�����̏��
	std::weak_ptr<GameCamera> m_wpCamera;

	//�X�e�[�W�̏��i���|�j
	GameScene* m_scene;

	//�G�t�F�N�g�p====================================================================================
	std::shared_ptr<KdEffekseerObject> effObj;
	Math::Matrix effMat = Math::Matrix::Identity;
	//================================================================================================

	//�f�o�b�N�p======================================================================================
	KdDebugWireFrame m_debugWire;
	//================================================================================================

private:

	//�E�F�|�����X�g=====================================================================
	std::list<std::shared_ptr<WeaponBase>>m_weaList;

	//�E�F�|�����=======================================================================
	std::shared_ptr<Rifle>rif;
	std::shared_ptr<Rocket>roc;
	std::shared_ptr<EleGun>ele;
};

//�ǂ�����ĕ����F�������邩
//