#include "GameCamera.h"

#include "../../../Scene/GameScene/GameScene.h"

void GameCamera::Update()
{
	//フリーカメラ仮操作用
	Math::Vector3 m_moveVec = {};

	if (GetAsyncKeyState('W') & 0x8000)       m_moveVec += Math::Vector3(0, 0, 1);
	if (GetAsyncKeyState('A') & 0x8000)       m_moveVec += Math::Vector3(-1, 0, 0);
	if (GetAsyncKeyState('S') & 0x8000)       m_moveVec += Math::Vector3(0, 0, -1);
	if (GetAsyncKeyState('D') & 0x8000)       m_moveVec += Math::Vector3(1, 0, 0);
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)  m_moveVec += Math::Vector3(0, 1, 0);
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)m_moveVec += Math::Vector3(0, -1, 0);
	m_moveVec.Normalize();
	m_pos += m_moveVec.Transform(m_moveVec, m_rotateMat) * 2.0f;

	Math::Vector3 m_moveAng = {};

	if (GetAsyncKeyState(VK_UP) & 0x8000)     m_moveAng += Math::Vector3(1, 0, 0);
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)   m_moveAng += Math::Vector3(0, -1, 0);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)   m_moveAng += Math::Vector3(-1, 0, 0);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)  m_moveAng += Math::Vector3(0, 1, 0);
	m_moveAng.Normalize();
	m_degAng += m_moveAng * 2.0f;

	//カーソルフリーボタン
	if (GetAsyncKeyState('I') & 0x8000)
	{
		controlFlg = false;
		ShowCursor(true);
	}
	if (GetAsyncKeyState('U') & 0x8000)
	{
		controlFlg = true;
		ShowCursor(false);
	}
	CameraBase::Update();
}

void GameCamera::PostUpdate()
{
	Math::Matrix targetMat = Math::Matrix::Identity;

	if (!m_wpTarget.expired() && !m_freeFlg)
	{
		const std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();

		if (spTarget)
		{
			targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos());
		}

		//カメラ移動テスト
		//現在の座標と行きたい場所の座標を引いて差分を出し差分の分移動させる

		if(controlFlg)UpdateRotateByMouse();

		m_rotateMat = GetRotationMatrix();
		m_delayMat[0] = m_LocalPos * m_rotateMat * targetMat;

		for (int i = m_delayFrame - 1; i > 0; i--)
		{
			m_delayMat[i] = m_delayMat[i - 1];
		}
		m_mWorld = m_delayMat[m_delayFrame - 1];
	}
	else
	{
		//フリーカメラ用
		m_rotateMat = Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z)
		);
		m_transMat = Math::Matrix::CreateTranslation(m_pos);
		m_mWorld = m_rotateMat * m_transMat;
	}

	UpdateCollision();

	CameraBase::PostUpdate();
}

void GameCamera::Init()
{
	//基準点（ターゲット）
	m_LocalPos = Math::Matrix::CreateTranslation(15.f, 18.0f, -60.0f);

	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
	CameraBase::Init();
}

//マウスカメラ操作
void GameCamera::UpdateRotateByMouse()
{
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;

	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	m_degAng.x += mouseMove.y * 0.15f;
	m_degAng.y += mouseMove.x * 0.15f;

	m_degAng.x = std::clamp(m_degAng.x, -90.0f, 90.0f);
}

//===========================
//めり込み防止
//===========================
void GameCamera::UpdateCollision()
{
	const std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
	if (!spTarget)return;

	KdCollider::RayInfo rayInfor;

	rayInfor.m_pos = spTarget->GetPos() + Math::Vector3(0.f, 10.f, 0.f);

	rayInfor.m_pos.y += 1.0f;

	rayInfor.m_dir = GetPos() - rayInfor.m_pos;
	rayInfor.m_dir.Normalize();

	rayInfor.m_range = (GetPos() - rayInfor.m_pos).Length();

	rayInfor.m_type = KdCollider::TypeBump | KdCollider::TypeGround;

	std::list<std::shared_ptr<KdGameObject>> list = m_scene->GetHitObjList();

	if (!list.empty())
	{
		std::list<KdCollider::CollisionResult> retRayList;

		for (auto& obj : list)
		{
			obj->Intersects(rayInfor, &retRayList);

			float maxOverLap = 0;
			Math::Vector3 hitPos = Math::Vector3::Zero;
			bool hit = false;

			for (auto& ret : retRayList)
			{
				if (maxOverLap < ret.m_overlapDistance)
				{
					maxOverLap = ret.m_overlapDistance;
					hitPos = ret.m_hitPos;
					hit = true;
				}
			}
			if (hit)
			{
				SetPos(hitPos);
			}
		}
	}
}