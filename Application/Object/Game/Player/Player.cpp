#include "Player.h"
#include"../../Camera/GameCamera/GameCamera.h"
#include"../../Game/Weapon/WeaponBase.h"
#include"../Enemy/EnemyBase.h"

#include"../ObjectSystem/ObjectSystem.h"

void Player::ImGuiUpdate()
{
	//ImGui�p�A�b�v�f�[�g================================
	if (!m_spWeaBase)return;
	m_spWeaBase->ImGuiUpdate();
	//===================================================
}

void Player::PreUpdate()
{
	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->PreUpdate();
	//===================================================
}

void Player::Update()
{
	if (!m_control)return;

	//�J�����̉�]�s��̎擾
	if (!m_wpCamera.expired()) { m_camRotMat = m_wpCamera.lock()->GetRotationYMatrix(); }

	//�����蔻��̋N��
	m_wpSys.lock()->HitPointTypeSphere(shared_from_this());
	m_wpSys.lock()->HitPointTypeRay(shared_from_this());

	//==================================================================================================
	//���@����
	// 
	//�X�e�[�g���@�\���Ă�����X�V
	if (m_nowAction)m_nowAction->Update(*this);

	//����̑I��
	if (GetAsyncKeyState('1') & 0x8000)
	{
		m_spWeaBase->SetWeaponType(m_spWeaBase->damage);
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		m_spWeaBase->SetWeaponType(2);
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		m_spWeaBase->SetWeaponType(3);
	}

	//==================================================================================================


	//==================================================================================================
	//�J��Ԃ�����

	//�����x
	nowAccel = STR_Setting.MoveAccel;
	//�ő呬
	nowSpeedMax = STR_Setting.MoveSpeedMax;
	//���x�ቺ�{��
	nowDownSpeed = STR_Setting.DownSpeed;

	//���������x�𗎂Ƃ�
	m_moveVec *= nowDownSpeed;

	//�u�[�X�g��
	BoostHeal();

	//�I�[�o�[�q�[�g���
	OverHeat();

	//�d�̓e�X�g
	STR_Setting.Gravity -= STR_Setting.GravityAccel;
	m_moveVec.y += STR_Setting.Gravity;


	//���W�ɕϊ����邽�߂̕ϊ�
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_moveVec);

	//�ړ�����
	m_mWorld *= trans;

	//�^�[�Q�b�g��񂪗��Ă�����
	if (!m_wpTraEne.expired())
	{
		//�߂����Ԃɕ��בւ���
		TargetSort();
	}

	//�����͈͊O�Ȃ烍�b�N���O��
	if (m_targetVec.Length() > 250)
	{
		m_lockFlg = false;
	}
	//�^�[�Q�b�g�ւ̃x�N�g���̐��K��
	m_targetVec.Normalize();

	//�J�����̏�񂪗��Ă��邩
	if (m_wpCamera.lock())
	{
		//���b�N�I���\�͈͂��ǂ���
		float dot = m_wpCamera.lock()->GetMatrix().Backward().Dot(m_targetVec);

		//�\�͈͓��Ȃ�
		if (dot >= 0.9f)
		{
			//�m�点��
			m_lockOkFlg = true;
		}
		//���b�N�I�����ɔ͈͊O�ɏo���烍�b�N�I������
		else m_lockOkFlg = false, m_lockFlg = false;
	}

	//���b�N�I��
	//�J�����Ǐ]����^�[�Q�b�g�Ǐ]�Ɉڍs
	if (m_lockFlg)RotateFromTarget(m_mWorld.Backward(), m_targetVec);
	else RotateFromCamera();//�t���O���O�ꂽ�ꍇ�J�����Ǐ]�ɖ߂�
	//==================================================================================================

	//��
	//�\���̕ϐ��ɒl��ۑ����AUi�ɑ���
	m_data->HpData = STR_Setting.Hp;
	m_data->BoostData = STR_Boost.Gauge;

	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->SetCamera(m_wpCamera.lock());
	m_spWeaBase->Update();
	m_weaList = m_spWeaBase->GetWeaList();
	//===================================================
}

void Player::PostUpdate()
{
	if (!m_spAnimator)	return;

	// �A�j���[�V�����̍X�V
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes(), 1.0f);
	m_spModel->CalcNodeMatrices();

	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->PostUpdate();
	//===================================================
}

void Player::DrawLit()
{
	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawLit();
	//===================================================
}

void Player::DrawLit_SkinMesh()
{
	//3D���f��
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawLit_SkinMesh();
	//===================================================
}

void Player::DrawUnLit()
{
	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawUnLit();
	//===================================================
}

void Player::DrawDebug()
{
	m_debugWire.Draw();

	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawDebug();
	//===================================================
}

void Player::Init()
{
	m_data = std::make_shared<PLAYERDATA>();
	//===================================================================
	// Json������
	//===================================================================
	std::ifstream ifs("Asset/Data/Object.json");
	if (ifs.fail()) { assert(0 && "Json�t�@�C���̃p�X���Ⴂ�܂�"); }

	//������Ƃ��đS�ǂݍ���
	std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	//�������JSON����́i�p�[�X�j����
	std::string err;
	json11::Json jsonObj = json11::Json::parse(strJson, err);
	if (err.size() > 0) { assert(0 && "�ǂݍ��񂾃t�@�C����Json�ϊ��Ɏ��s"); }
	auto& player = jsonObj["Player"].object_items();

	//===================================================================
	//�v���C���[���
	//�傫��
	float scale = (float)player["Scale"].number_value();
	//�̗�
	STR_Setting.Hp = player["Hp"].int_value();
	//�d��
	STR_Setting.Gravity = (float)player["Gravity"].number_value();
	//�d�͉����x
	STR_Setting.GravityAccel = (float)player["GravityAccel"].number_value();
	//�ړ������x
	STR_Setting.MoveAccel = (float)player["MoveAccel"].number_value();
	//�ړ����x�ቺ��
	STR_Setting.DownSpeed = (float)player["DownSpeed"].number_value();
	//�ړ����x�ő�
	STR_Setting.MoveSpeedMax = (float)player["MoveSpeedMax"].number_value();
	//��]���x
	STR_Setting.Ang = (float)player["Ang"].number_value();
	//���b�N�I������]���x
	STR_Setting.LookAng = (float)player["LookAng"].number_value();
	//===================================================================

	//===================================================================
	STR_Boost.GaugeMax = (float)player["GaugeMax"].number_value();
	STR_Boost.Gauge = STR_Boost.GaugeMax;

	STR_Boost.Speed = (float)player["Speed"].number_value();

	STR_Boost.DownSpeed = (float)player["BoostDownSpeed"].number_value();
	STR_Boost.Accel = (float)player["BoostAccel"].number_value();

	STR_Boost.Heal = (float)player["Heal"].number_value();

	STR_Boost.HealTime = (float)player["HealTime"].number_value();
	STR_Boost.HealElapTime = STR_Boost.HealTime;

	STR_Boost.HealSpeed = (float)player["HealSpeed"].number_value();

	STR_Boost.Cost = (float)player["Cost"].number_value();

	//===================================================================

	//�����o�����W�ݒ�
	auto& sPlayer = m_stageData["Player"].object_items();
	//���W
	auto& pos = sPlayer["Pos"].array_items();
	Math::Vector3 nowPos = {
		(float)pos[0].number_value(),
		(float)pos[1].number_value(),
		(float)pos[2].number_value()
	};

	//������=============================================================================================
	//���f���̓��ꕨ
	m_spModel = std::make_shared<KdModelWork>();
	m_mWorld = Math::Matrix::CreateScale(scale) * Math::Matrix::CreateTranslation(nowPos);

	//���f�����
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Player/Test.gltf"));

	//�A�j���[�V�����̃Z�b�g
	m_spAnimator = std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Walk"));

	//�����X�e�[�g�̎w��
	ChangeActionState(std::make_shared<ActionIdle>());

	//�����蔻��p
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player", m_spModel, KdCollider::TypeEvent);

	//�E�F�|���x�[�X===============================================================
	m_spWeaBase = std::make_shared<WeaponBase>();
	m_spWeaBase->SetScene(m_scene);
	m_spWeaBase->Init();
	m_spWeaBase->SetWeaponType(1);
	m_spWeaBase->SetOwner(shared_from_this());
	//=============================================================================
}

//================================================================
// �J�����Ǐ]
// �J�����̐��ʕ�����Ώۂɓ��ςƊO�ς�p���ĉ�]����p�x�A���������߂Ă���֐�
//================================================================
void Player::RotateFromCamera()
{
	Math::Vector3 nowVec = m_mWorld.Backward();

	Math::Vector3 tarVec = m_camRotMat.Backward();

	if (nowVec == tarVec)return;
	nowVec.Normalize();
	tarVec.Normalize();

	//���ρi�h�b�g�ρj
	float dot = nowVec.Dot(tarVec);
	dot = std::clamp(dot, -1.f, 1.f);

	//�p�x�֕ϊ�
	float Ang = acos(dot);
	Ang = DirectX::XMConvertToDegrees(Ang);

	float rotateAng = std::clamp(Ang, 0.f, 8.f);

	Math::Vector3 rotAxis;

	//�O��
	nowVec.Cross(tarVec, rotAxis);

	//�����Ă����������������΋A��
	if (rotAxis.LengthSquared() == 0)return;

	//��]�s��쐬
	m_rotateMat = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3::Zero);

	m_mWorld *= m_rotateMat;

	m_mWorld.Translation(pos);
}

//================================================================
// �G�Ǐ]
// �G�̍��W��Ώۂɓ��ςƊO�ς�p���ĉ�]����p�x�A���������߂Ă���֐�
//================================================================
void Player::RotateFromTarget(Math::Vector3 _ownerVec, Math::Vector3 _tarVec)
{
	//Null�`�F�b�N
	if (_ownerVec.Length() == 0.f || _tarVec.Length() == 0.f)return;
	//���K��
	_ownerVec.Normalize();
	_tarVec.Normalize();

	//�f�o�b�N�p
	m_debugWire.AddDebugLine(GetPos(), _ownerVec, 5.0f, kBlueColor);
	m_debugWire.AddDebugLine(GetPos(), _tarVec, 5.0f, kGreenColor);

	//����
	float dot = _ownerVec.Dot(_tarVec);
	dot = std::clamp(dot, -1.0f, 1.0f);

	//�p�x�֕ϊ�
	float Ang = acos(dot);
	Ang = DirectX::XMConvertToDegrees(Ang);

	float rotateAng = std::clamp(Ang, -8.0f, 8.0f);

	Math::Vector3 rotAxis;

	//�O��
	_ownerVec.Cross(_tarVec, rotAxis);
	m_debugWire.AddDebugLine(GetPos(), rotAxis, 5.0f, kRedColor);

	//�����Ă����������������΋A��
	if (rotAxis.LengthSquared() == 0)return;

	//��]�s��쐬
	m_rotateMat = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3::Zero);

	//��]
	m_mWorld *= m_rotateMat;

	m_mWorld.Translation(pos);
}

void Player::TargetSort()
{
	//Null�`�F�b�N
	if (m_wpTraEne.lock()->GetEneList().size() == 0) {
		m_lockFlg = false;
		return;
	}

	std::list<std::shared_ptr<EnemyBase>> enemList = m_wpTraEne.lock()->GetEneList();
	std::list<EnemySorter> targetSortList;

	//�߂����ɕ��ёւ�
	for (auto& target : enemList) {
		targetSortList.push_back(EnemySorter(target, (target->GetPos() - GetPos()).Length()));
	}
	targetSortList.sort();

	//��ԋ߂����̂����b�N
	m_targetVec = targetSortList.front().m_enem->GetPos() - GetPos();

	//���b�N�I�����E�F�|���ɓn���p
	if (m_lockFlg) {
		m_spWeaBase->SetTarPos(targetSortList.front().m_enem->GetHitPointTypeSphere().m_sphere.Center);
	}
}

//================================================================
//�u�[�X�g����
// �g�p�ɂ͈��̃R�X�g���|���茻�݂̑��x�ɉ��Z����`
//================================================================
void Player::Boost()
{
	//Null�`�F�b�N
	if (m_overheatFlg)return;
	
	if (!m_boostFlg)
	{
		//�A�j���[�V�����̏������Ɛݒ�
		m_spAnimator = std::make_shared<KdAnimator>();
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Boost"));
		m_boostFlg = true;
	}

	nowAccel = STR_Boost.Accel;
	nowSpeedMax = STR_Boost.Speed;
	nowDownSpeed = STR_Boost.DownSpeed;
	STR_Boost.HealElapTime = STR_Boost.HealTime;

	//�g�p��������
	STR_Boost.HealElapTime = STR_Boost.HealTime;
	STR_Setting.Gravity = 0.f;

	//�u�[�X�g��
	--STR_Boost.Gauge;
}

//================================================================
//�u�[�X�g�񕜏���
//���Ԍo�߂ŉ񕜂������čő�l�܂ŉ�
//�I�[�o�[�q�[�g�ƈႢ�񕜒��ł��u�[�X�g�͎g�p�\
//================================================================
void Player::BoostHeal()
{
	if (m_overheatFlg || STR_Boost.Gauge >= STR_Boost.GaugeMax)return;
	--STR_Boost.HealElapTime;
	if (STR_Boost.HealElapTime <= 0.f)
	{
		STR_Boost.Gauge += STR_Boost.Heal;

		//
		if (STR_Boost.Gauge >= STR_Boost.GaugeMax)STR_Boost.HealElapTime = STR_Boost.HealTime;
	}
}

//================================================================
//�I�[�o�[�q�[�g����
// �ʏ�񕜂�1.5�{�̑��x�ŉ񕜂���
// �ő�񕜂���܂Ńu�[�X�g�͎g�p�s���
//================================================================
void Player::OverHeat()
{
	if (!m_overheatFlg)return;

	m_boostFlg = false;

	--STR_Boost.HealElapTime;
	if (STR_Boost.HealElapTime <= 0.f)
	{
		STR_Boost.Gauge += STR_Boost.Heal * STR_Boost.HealSpeed;

		if (STR_Boost.Gauge >= STR_Boost.GaugeMax)STR_Boost.HealElapTime = STR_Boost.HealTime, m_overheatFlg = false;
	}
}

//===========================================================================
// ��Ԃ�ς��鏈��
// ���݂̃L�����̏�Ԃ�j�����V������Ԃ֕ύX���邽�߂̊֐�
//===========================================================================
void Player::ChangeActionState(std::shared_ptr<ActionStateBase> _nextState)
{
	//�֐����Ă΂ꂽ�Ƃ����݂̏�Ԃ��I���i�j���j����
	if (m_nowAction) m_nowAction->Exit(*this);

	//���̏�Ԃ�����������
	m_nowAction = _nextState;

	//�n�߂�
	m_nowAction->Enter(*this);
}

void Player::ActionIdle::Enter(Player& _owner)
{
	//�A�j���[�V�����̏������Ɛݒ�
	_owner.m_spAnimator = std::make_shared<KdAnimator>();
	_owner.m_spAnimator->SetAnimation(_owner.m_spModel->GetData()->GetAnimation("Idle"));
}

void Player::ActionIdle::Update(Player& _owner)
{
	Math::Vector3 moveDir{};

	//�O�ړ�
	if (GetAsyncKeyState('W') & 0x8000) { moveDir += Math::Vector3::Backward; }

	//���ړ�
	if (GetAsyncKeyState('A') & 0x8000) { moveDir += Math::Vector3::Left; }

	//���ړ�
	if (GetAsyncKeyState('S') & 0x8000) { moveDir += Math::Vector3::Forward; }

	//�E�ړ�
	if (GetAsyncKeyState('D') & 0x8000) { moveDir += Math::Vector3::Right; }

	//�㏸
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) { moveDir += Math::Vector3::Up; }

	//���b�N�I��
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		//����͈͓��Ȃ烍�b�N�I���\
		if (_owner.m_lockOkFlg)
		{
			//���b�N�I�����̓J�����ւ̒Ǐ]��؂�
			_owner.m_lockFlg = true;
			_owner.m_lockOkFlg = false;
		}
	}

	//�ˌ�
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		//�����ꂽ��

		//m_weaBase��Active�֐����N������
		//2�b��N��
		++_owner.ActiveTime;
		if (_owner.ActiveTime >= _owner.ActivePlayTime)
		{
			_owner.m_spWeaBase->Active();
		}
	}
	else _owner.ActiveTime = 0;

	//�ړ����͂���Ă�����
	//�ړ��X�e�[�g�Ɉڍs
	if (moveDir.LengthSquared() > 0.f) { _owner.ChangeActionState(std::make_shared<ActionWalk>()); }
}

void Player::ActionWalk::Enter(Player& _owner)
{
	//�A�j���[�V�����̏������Ɛݒ�
	_owner.m_spAnimator = std::make_shared<KdAnimator>();
	_owner.m_spAnimator->SetAnimation(_owner.m_spModel->GetData()->GetAnimation("Walk"));
}

void Player::ActionWalk::Update(Player& _owner)
{
	Math::Vector3 moveDir{};

	//�O�ړ�
	if (GetAsyncKeyState('W') & 0x8000) { moveDir += Math::Vector3::Backward; }

	//���ړ�
	if (GetAsyncKeyState('S') & 0x8000) { moveDir += Math::Vector3::Forward; }

	//���ړ�
	if (GetAsyncKeyState('A') & 0x8000) { moveDir += Math::Vector3::Left; }

	//�E�ړ�
	if (GetAsyncKeyState('D') & 0x8000) { moveDir += Math::Vector3::Right; }

	//�㏸
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		moveDir += Math::Vector3::Up;
		_owner.STR_Setting.Gravity = 0.f;
	}

	//�u�[�X�g
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) 
	{ 
		if (_owner.STR_Boost.Gauge < 0.f)_owner.ChangeActionState(std::make_shared<ActionIdle>());
		//�u�[�X�g�֐��N��
		_owner.Boost();
	}

	if (moveDir.LengthSquared() == 0)
	{
		//�������͂���Ă��Ȃ��̂őҋ@��Ԃֈڍs
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	//�ړ�����
	if (moveDir != Math::Vector3::Zero)
	{
		moveDir.Normalize();
		moveDir = Math::Vector3::TransformNormal(moveDir, _owner.m_camRotMat);
	}

	//�����x
	float acc = _owner.nowAccel;
	//�ő呬
	float speedMax = _owner.nowSpeedMax;
	//���x�ቺ�{��
	float downSpeed = 0.f;
	downSpeed = _owner.nowDownSpeed;

	//�ړ�
	//�ړ��x�N�g�����ő呬�x�ȉ��Ȃ����
	if (_owner.m_moveVec.Length() < speedMax)
	{
		//����
		_owner.m_moveVec += moveDir * acc;
		//�ő呬�x�ȏ�ɂȂ�Ȃ�����
		if (_owner.m_moveVec.Length() >= speedMax)
		{
			//��x���K������
			_owner.m_moveVec.Normalize();
			//�ő呬�x�������đ��x�����Ɂi�K�^�K�^���Ȃ��悤�Ɂj
			_owner.m_moveVec *= speedMax;
		}
	}
}