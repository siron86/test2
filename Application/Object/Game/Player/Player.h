#pragma once

//�O���錾
class GameCamera;
class EnemyBase;
class WeaponBase;
class ObjectSystem;
class GameScene;

class Player :public KdGameObject
{
public:
	//�R���X�g���N�^
	Player() {}
	//�f�X�g���N�^
	~Player() {}

	//�e�X�g
	void SetControl(bool _control) { m_control = _control; }

	//�A�b�v�f�[�g======================================================================================
	void ImGuiUpdate()  override;
	void PreUpdate()    override;
	void Update()       override;
	void PostUpdate()   override;
	//==================================================================================================


	//�V�F�[�_�[========================================================================================
	void DrawLit()          override;
	void DrawLit_SkinMesh() override;
	void DrawUnLit()        override;

	//�f�o�b�N���C���̕\���p
	void DrawDebug() override;
	//==================================================================================================

	//������============================================================================================
	void Init()      override;

	//�v���C���[�I���W�i���֐�==========================================================================
	//���_����
	void RotateFromCamera();

	//���b�N�I��
	void RotateFromTarget(Math::Vector3 _ownerVec, Math::Vector3 _tarVec);

	//�\�[�g�����i�߂����̂���j
	void TargetSort();

	//�u�[�X�g����
	void Boost();

	//�u�[�X�g�񕜏���
	void BoostHeal();

	//�I�[�o�[�q�[�g��
	void OverHeat();
	//==================================================================================================

	//�Z�b�^�[==========================================================================================
	//�J�������̃Z�b�g
	void SetCamera(const std::shared_ptr<GameCamera>& _camera) { m_wpCamera = _camera; }

	//�^�[�Q�b�g���
	void SetTarget(const std::shared_ptr<EnemyBase>& _traEne) { m_wpTraEne = _traEne; }

	//JSON����������炤�p�֐�
	void SetData(const json11::Json& _jsonData = nullptr) { m_stageData = _jsonData; }
	//==================================================================================================

	//�Q�b�^�[==========================================================================================
	//�v���C���[�̃��f�����
	const std::shared_ptr<const KdModelWork> GetModel()override { return m_spModel; }

	//�^�[�Q�b�g�̍��W
	Math::Vector3 GetTarPos() { return m_targetPos; }
	bool LockOnFlg()override { return m_lockFlg; }

	//Ui�ɑ���f�[�^�i�\���́j
	std::shared_ptr<struct PLAYERDATA> GetPlayerData() { return m_data; }
	bool GetLock() { return m_lockOkFlg; }

	//�Q�[���V�[���̃A�h���X�����炤
	void SetScene(GameScene* _scene) { m_scene = _scene; }

	//�����蔻��(��)
	KdCollider::SphereInfo GetHitPointTypeSphere()override
	{
		KdCollider::SphereInfo spherePoint;

		//�m�[�h�̖��O�͌Œ�Ńq�b�g�|�C���g
		const KdModelWork::Node* pNode = m_spModel->FindNode("HitPoint");
		Math::Matrix mat;
		mat = pNode->m_worldTransform * m_mWorld;

		if (pNode)
		{
			Math::Vector3 pos = mat.Translation();
			spherePoint.m_sphere.Center = pos;
			m_debugWire.AddDebugSphere(pos, 5.0f);
		}

		spherePoint.m_sphere.Radius = 7.0f;

		spherePoint.m_type = KdCollider::TypeEvent | KdCollider::TypeBump | KdCollider::TypeDamage | KdCollider::TypeGround;

		return spherePoint;
	}

	//�����蔻��(���C)
	KdCollider::RayInfo GetHitPointTypeRay()override
	{
		KdCollider::RayInfo rayPoint;

		//�m�[�h�̖��O�͌Œ�Ńq�b�g�|�C���g
		const KdModelWork::Node* pNode = m_spModel->FindNode("HitPoint");
		Math::Matrix mat;
		mat = pNode->m_worldTransform * m_mWorld;

		if (pNode)
		{
			Math::Vector3 pos = mat.Translation();
			rayPoint.m_pos = pos;
		}

		rayPoint.m_dir = m_mWorld.Down();
		rayPoint.m_dir.Normalize();

		rayPoint.m_range = 15.0f;

		m_debugWire.AddDebugLine(rayPoint.m_pos, rayPoint.m_dir, rayPoint.m_range);

		rayPoint.m_type = KdCollider::TypeEvent | KdCollider::TypeBump | KdCollider::TypeGround;

		return rayPoint;
	}

	//���g�̃^�C�v
	KdCollider::Type GetType()override { return KdCollider::TypeEvent; };

	//�ڐG������
	void Hit(const int& _type, const Math::Vector3& _hitDir, const Math::Vector3&, const int&)override
	{
		if (_type == KdCollider::TypeBump)
		{
			m_mWorld *= Math::Matrix::CreateTranslation(_hitDir);
			STR_Setting.Gravity = 0.f;
		}

		if (_type == KdCollider::TypeEvent)
		{

		}

		//�_���[�W�����炤����
		if (_type == KdCollider::TypeDamage)
		{
			STR_Setting.Hp -= 10;
		}

		//�n�ʂɓ���������
		if (_type == KdCollider::TypeGround)
		{
			//��������
			m_mWorld *= Math::Matrix::CreateTranslation(_hitDir);
			//�d�͂͏�����
			STR_Setting.Gravity = 0.f;
		}
	}
	//==================================================================================================
private:

	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase() {}

		virtual void  Enter(Player& ) {}    //�N����
		virtual void   Exit(Player& ) {}    //�I����
		virtual void Update(Player& ) = 0;  //�X�V

	};

	//�ʏ펞
	class ActionIdle :public ActionStateBase
	{
	public:
		virtual ~ActionIdle() {}

		void  Enter(Player& )override;
		void Update(Player& )override;
	};

	//���s��
	class ActionWalk :public ActionStateBase
	{
	public:
		virtual ~ActionWalk() {}

		void  Enter(Player& )override;
		void Update(Player& )override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> _nextState);
	std::shared_ptr<ActionStateBase> m_nowAction = nullptr;

	//�X�e�[�W�̏��
	GameScene* m_scene;

	//==================================================================================================
	//���\�p�i�I�������ύX����j
	//==================================================================================================

	//�U���A�j���[�V�����t���O
	bool m_animeAttackFlg = false;

	//�U���f�B���C
	float ActiveTime = 0.f;
	float ActivePlayTime = 60.f * 0.5f;
	//================================================================================================

	//�f�o�b�N�p======================================================================================

	KdDebugWireFrame m_debugWire;
	//================================================================================================

	//�t���O==========================================================================================
	bool m_control = true;

	//���b�N�I���֌W
	bool m_lockFlg = false;
	bool m_lockOkFlg = false;

	//�u�[�X�g�֌W
	bool m_boostFlg = false;
	bool m_overheatFlg = false;

	//�e�X�g
	float nowAccel = 0.f;
	float nowDownSpeed = 0.f;
	float nowSpeedMax = 0.f;

	//�ݒ�p�ϐ�
	struct SETTING
	{
		int   Hp = 0;              //�̗�
		float Gravity = 0.f;       //�d��
		float GravityAccel = 0.f;  //�d�͉����x
		float MoveSpeed = 0.f;     //�ړ����x
		float MoveSpeedMax = 0.f;  //�ړ����x�ő�l
		float MoveAccel = 0.f;     //�ړ������x
		float DownSpeed = 0.f;     //�ړ����x�ቺ�l
		float Ang = 0.f;           //�ʏ��]���x
		float LookAng = 0.f;       //���b�N����]���x
	}STR_Setting;

	//�u�[�X�g�֌W
	struct BOOST
	{
		float Gauge = 0.f;         //�Q�[�W
		float GaugeMax = 0.f;      //�Q�[�W�ő�l
		float Speed = 0.f;         //���x
		float MaxSpeed = 0.f;      //���x�ő�l
		float MinSpeed = 0.f;      //���x�Œ�l
		float Accel = 0.f;         //�����x
		float DownSpeed = 0.f;     //���x�ቺ��
		float Heal = 0.f;          //�񕜗�
		float HealTime = 0.f;      //�񕜎���
		float HealElapTime = 0.f;  //�񕜌o�ߎ���
		float HealSpeed = 0.f;     //�񕜑��x
		float Cost = 0.f;          //�g�p�������
	}STR_Boost;

	//Ui�ɑ���f�[�^�Q
	std::shared_ptr<struct PLAYERDATA>m_data;


	//���f�����========================================================================================

	std::shared_ptr<KdModelWork> m_spModel;

	//�x�N�^�[==========================================================================================

	//�J��������݂������x�N�g��
	Math::Vector3 m_camLookVec = Math::Vector3::Zero;

	//�^�[�Q�b�g�̍��W
	Math::Vector3 m_targetPos = Math::Vector3::Zero;

	//�^�[�Q�b�g�ւ̃x�N�g��
	Math::Vector3 m_targetVec = Math::Vector3::Zero;

	//�ŏI�ړ������x�N�g��
	Math::Vector3 m_moveVec = Math::Vector3::Zero;

	//�s��==============================================================================================
	//�J�����̉�]�s����
	Math::Matrix m_camRotMat = Math::Matrix::Identity;

	//��]�s��
	Math::Matrix m_rotateMat = Math::Matrix::Identity;

	//�A�h���X===========================================================================================
	//�J�����̏��
	std::weak_ptr<GameCamera> m_wpCamera;

	//�V�X�e��
	std::weak_ptr<ObjectSystem> m_wpSys;

	//�^�[�Q�b�g���
	std::weak_ptr<EnemyBase> m_wpTraEne;

	//�A�j���[�V����
	std::shared_ptr <KdAnimator> m_spAnimator = nullptr;

	//�E�F�|��
	std::shared_ptr<WeaponBase> m_spWeaBase;

	//�v���C���[�̎����Ă���E�F�|�����X�g
	std::list<std::shared_ptr<WeaponBase>> m_weaList;

	//���b�N�I���Ώۃ��X�g
	std::list<std::shared_ptr<EnemyBase>> m_lookTarList;

	//Json�f�[�^
	json11::Json m_stageData;

	//�G�t�F�N�g�p====================================================================================
	std::shared_ptr<KdEffekseerObject> effObj;
	Math::Matrix effMat = Math::Matrix::Identity;
	//================================================================================================
};


//Ui�ɏ���n���p
struct PLAYERDATA
{
	//Ui�ɑ�����Q
	int HpData = 0;    //
	float BoostData = 0.f;  //
};