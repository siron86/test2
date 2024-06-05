#include "CameraBase.h"

void CameraBase::Update()
{
}

void CameraBase::PostUpdate()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetCameraMatrix(m_mWorld);
}

void CameraBase::PreDraw()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetToShader();
}

void CameraBase::DrawDebug()
{
	m_debugWire.Draw();
}

void CameraBase::Init()
{
	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<KdCamera>();
	}

	m_degAng = Math::Vector3::Zero;
}

void CameraBase::SetTarget(const std::shared_ptr<KdGameObject>& target)
{
	if (!target) { return; }

	m_wpTarget = target;
}
