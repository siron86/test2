#include "Bullet.h"
#include"../../ObjectSystem/ObjectSystem.h"

void Bullet::Update()
{
	if (m_isExpired)return;

	//��苗����s�����
	++deleteCount;
	if (deleteCount >= deleteMaxCount)
	{
		//����
		effObj.reset();
		m_isExpired = true;
	}


	//�G�t�F�N�g�̐ݒu
	const KdModelWork::Node* pNode = m_spModel->FindNode("EffectPoint");
	if (pNode)
	{
		effMat = pNode->m_worldTransform * m_mWorld;
		if (!effObj && !m_effFlg)
		{
			//effObj = KdEffekseerManager::GetInstance().Play("O_MissileBoost.efkefc", effMat.Translation(), true);
		}
		//if (!effObj)return;
		//effObj->SetPos(effMat.Translation());
		//m_effFlg = true;
	}

	//�O�ɔ�Ԃ����ړ�
	if (m_homingFlg)m_shotVec = m_targetPos - m_mWorldPoly.Translation();
	Math::Vector3 vMove = m_shotVec;
	vMove.Normalize();

	//vMove *= 0.f;
	vMove *= m_speed;

	Math::Matrix trans = Math::Matrix::CreateTranslation(vMove);

	m_mWorldPoly *= trans;

	//�z�[�~���O�����鎞
	//if (m_homingFlg)
	//{
	//	vMove = m_mWorld.Backward();
	//	vMove.Normalize();

	//	vMove *= m_speed;

	//	trans = Math::Matrix::CreateTranslation(vMove);

	//	m_mWorld *= trans;

	//	//�����ԏ��ɏグ����z�[�~���O
	//	++count;
	//	if (count > maxCount)
	//	{
	//		count = maxCount;
	//		//�z�[�~���O����
	//		m_speed = 2.0f;
	//		m_shotPos = Math::Vector3::Zero;
	//		Math::Vector3 vTarget = (m_weaBase.lock()->GetTarPos() + Math::Vector3(0, 14, 0)) - GetPos();
	//		m_debugWire.AddDebugSphere(m_weaBase.lock()->GetTarPos() + Math::Vector3(0, 14, 0), 10);

	//		UpdateRotateFromTarget(vTarget);
	//	}
	//}

	//m_wpSys.lock()->HitPointTypeSphere(shared_from_this());
	//�����蔻��
	m_wpSys.lock()->HitPointTypeRay(shared_from_this());

}

void Bullet::DrawLit()
{
	if (m_isExpired)return;
	//�Ă񂾑Ώۂ������`���[�̏ꍇ���f����\��
	if (!m_spModel||m_weaBase.lock()->GetWeapomType()!=2)return;
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Bullet::DrawUnLit()
{
	//�e�X�g
	if (m_isExpired)return;
	if (!m_spPoly)return;
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorldPoly);
}

void Bullet::DrawDebug()
{
	m_debugWire.Draw();
}

void Bullet::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Weapon/Bullet/Bullet.gltf"));

	//�e�̒���(�|���S�����͒������邯�ǃ��f�����͂���Ȃ�)
	m_mWorldPoly = Math::Matrix::CreateScale(m_scale) *
		Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(-90)) *
		Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0)) *
		m_mWorld;

	m_mWorld = Math::Matrix::CreateScale(m_scale) *
		m_mWorld;
	//�|���S��
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/bullet.png"));
	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Left_Middle);

	//�����蔻��p
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Bullet", m_spModel, KdCollider::TypeDamage);
}

void Bullet::MissileAction()
{
	Math::Vector3 vMove = m_mWorld.Backward() + m_shotPos;
	vMove.Normalize();

	vMove *= m_speed;
	Math::Matrix trans = Math::Matrix::CreateTranslation(vMove);

	m_mWorld *= trans;

	//����Ɍ�������
	Math::Vector3 vTarget = (m_weaBase.lock()->GetTarPos() + Math::Vector3(0, 18, 0)) - GetPos();
	m_debugWire.AddDebugSphere(m_weaBase.lock()->GetTarPos() + Math::Vector3(0, 18, 0), 10);

	UpdateRotateFromTarget(vTarget);
}

void Bullet::UpdateRotateFromTarget(Math::Vector3& _tarVec)
{
	//���g�̌����Ă�������x�N�g��
	Math::Vector3 nowVec = m_mWorld.Backward();

	//�^�[�Q�b�g�Ɍ����������x�N�g��
	nowVec.Normalize();
	_tarVec.Normalize();

	//��]�p�x�����߂�(����)
	float dot = nowVec.Dot(_tarVec);
	//���~�b�^�[
	dot = std::clamp(dot, -1.0f, 1.0f);

	//���ς���t�]���̒藝�ŊԂ̊p�x�����߂�
	float Ang = acos(dot);
	Ang = DirectX::XMConvertToDegrees(Ang);

	float rotateAng = std::clamp(Ang, -10.0f, 10.0f);

	Math::Vector3 rotAxis;

	nowVec.Cross(_tarVec, rotAxis);
	m_debugWire.AddDebugLine(GetPos(), rotAxis, 5.0f, kRedColor);

	if (rotAxis.LengthSquared() == 0)return;

	//�U����]����ׂ̍s��
	Math::Matrix rotation;
	rotation = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	//���̃��[���h���W���ꎞ�I�ɕۑ�
	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	//��]�s��̍���
	m_mWorld *= rotation;

	//��]����������ɖ߂��i����Ȃ��ƌ��_����̊p�x���c��Ă�������j
	m_mWorld.Translation(pos);
}