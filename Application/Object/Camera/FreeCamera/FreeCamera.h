#pragma once
#include"../CameraBase/CameraBase.h"

class FreeCamera :public CameraBase
{
public:
	FreeCamera() { Init(); }
	~FreeCamera() {}

	void Update()      override;
	void PostUpdate()  override;
	void Init()        override;

	//始まる位置、終わる位置、スピード
	void SetStartPos(Math::Vector3 _startPos)
	{
		m_startPos = _startPos;
	}

	void SetAngle(Math::Vector3 _angle)
	{
		m_startAng = _angle;
	}

	void SetControl(bool _controlFlg)
	{
		m_controlFlg = _controlFlg;
	}

private:

	//コントロールフラグ
	bool m_controlFlg = false;

	Math::Vector3 m_startPos = Math::Vector3::Zero;
	Math::Vector3 m_endPos =   Math::Vector3::Zero;
	Math::Vector3 m_startAng = Math::Vector3::Zero;
	Math::Vector3 m_endAng =   Math::Vector3::Zero;
	Math::Vector3 m_moveVec = {};
};