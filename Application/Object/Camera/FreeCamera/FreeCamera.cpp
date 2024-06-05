#include "FreeCamera.h"

void FreeCamera::Update()
{
	if (!m_controlFlg)return;
	//呼ばれたクラスから情報をもらいカメラを移動させたい

	//=====================================================================================
	//移動、回転
	//=====================================================================================
		//カメラの回転
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