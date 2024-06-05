#pragma once

class CameraBase : public KdGameObject
{
public:
	CameraBase() {}
	virtual ~CameraBase()	override {}

	void Init()				override;
	void Update()			override;
	void PostUpdate()	    override;
	void PreDraw()			override;
	void DrawDebug()			override;

	void SetTarget(const std::shared_ptr<KdGameObject>& target);

	const std::shared_ptr<const KdCamera>& GetCamera() const
	{
		return m_spCamera;
	}

	const Math::Matrix GetRotationMatrix() const
	{
		return Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z)
		);
	}

	const Math::Matrix GetRotationYMatrix() const
	{
		return Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_degAng.y));
	}

	std::shared_ptr<KdCamera> WorkCamera() const
	{
		return m_spCamera;
	}

protected:
	std::shared_ptr<KdCamera>			m_spCamera = nullptr;
	std::weak_ptr<KdGameObject>			m_wpTarget;

	Math::Matrix						m_LocalPos = Math::Matrix::Identity;
	Math::Matrix						m_rotateMat = Math::Matrix::Identity;
	Math::Matrix						m_transMat = Math::Matrix::Identity;

	//Deb::デバック用
	KdDebugWireFrame m_debugWire;

	//カメラ回転用角度
	Math::Vector3 m_degAng = {};
};