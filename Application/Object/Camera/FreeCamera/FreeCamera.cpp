#include "FreeCamera.h"

void FreeCamera::Update()
{
	if (!m_controlFlg)return;
	//�Ă΂ꂽ�N���X����������炢�J�������ړ���������

	//=====================================================================================
	//�ړ��A��]
	//=====================================================================================
		//�J�����̉�]
	m_rotateMat = Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_startAng.y),
		DirectX::XMConvertToRadians(m_startAng.x),
		DirectX::XMConvertToRadians(m_startAng.z));
	m_transMat = Math::Matrix::CreateTranslation(m_startPos);
	m_mWorld = m_rotateMat * m_transMat;

	//=====================================================================================
	CameraBase::Update();
}

void FreeCamera::PostUpdate()
{
	if (!m_controlFlg)return;



	CameraBase::PostUpdate();
}

void FreeCamera::Init()
{
	m_controlFlg = true;

	CameraBase::Init();
}