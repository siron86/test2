#include "Bullet.h"
#include"../../ObjectSystem/ObjectSystem.h"

void Bullet::Update()
{
	if (m_isExpired)return;

	//一定距離飛行後消滅
	++deleteCount;
	if (deleteCount >= deleteMaxCount)
	{
		//消滅
		effObj.reset();
		m_isExpired = true;
	}


	//エフェクトの設置
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

	//前に飛ぶだけ移動
	if (m_homingFlg)m_shotVec = m_targetPos - m_mWorldPoly.Translation();
	Math::Vector3 vMove = m_shotVec;
	vMove.Normalize();

	//vMove *= 0.f;
	vMove *= m_speed;

	Math::Matrix trans = Math::Matrix::CreateTranslation(vMove);

	m_mWorldPoly *= trans;

	//ホーミングさせる時
	//if (m_homingFlg)
	//{
	//	vMove = m_mWorld.Backward();
	//	vMove.Normalize();

	//	vMove *= m_speed;

	//	trans = Math::Matrix::CreateTranslation(vMove);

	//	m_mWorld *= trans;

	//	//一定期間上空に上げた後ホーミング
	//	++count;
	//	if (count > maxCount)
	//	{
	//		count = maxCount;
	//		//ホーミング処理
	//		m_speed = 2.0f;
	//		m_shotPos = Math::Vector3::Zero;
	//		Math::Vector3 vTarget = (m_weaBase.lock()->GetTarPos() + Math::Vector3(0, 14, 0)) - GetPos();
	//		m_debugWire.AddDebugSphere(m_weaBase.lock()->GetTarPos() + Math::Vector3(0, 14, 0), 10);

	//		UpdateRotateFromTarget(vTarget);
	//	}
	//}

	//m_wpSys.lock()->HitPointTypeSphere(shared_from_this());
	//当たり判定
	m_wpSys.lock()->HitPointTypeRay(shared_from_this());

}

void Bullet::DrawLit()
{
	if (m_isExpired)return;
	//呼んだ対象がランチャーの場合モデルを表示
	if (!m_spModel||m_weaBase.lock()->GetWeapomType()!=2)return;
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Bullet::DrawUnLit()
{
	//テスト
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

	//弾の調整(ポリゴン側は調整するけどモデル側はいらない)
	m_mWorldPoly = Math::Matrix::CreateScale(m_scale) *
		Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(-90)) *
		Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0)) *
		m_mWorld;

	m_mWorld = Math::Matrix::CreateScale(m_scale) *
		m_mWorld;
	//ポリゴン
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/bullet.png"));
	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Left_Middle);

	//当たり判定用
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

	//相手に向く処理
	Math::Vector3 vTarget = (m_weaBase.lock()->GetTarPos() + Math::Vector3(0, 18, 0)) - GetPos();
	m_debugWire.AddDebugSphere(m_weaBase.lock()->GetTarPos() + Math::Vector3(0, 18, 0), 10);

	UpdateRotateFromTarget(vTarget);
}

void Bullet::UpdateRotateFromTarget(Math::Vector3& _tarVec)
{
	//自身の向いている方向ベクトル
	Math::Vector3 nowVec = m_mWorld.Backward();

	//ターゲットに向かう方向ベクトル
	nowVec.Normalize();
	_tarVec.Normalize();

	//回転角度を求める(内積)
	float dot = nowVec.Dot(_tarVec);
	//リミッター
	dot = std::clamp(dot, -1.0f, 1.0f);

	//内積から逆余弦の定理で間の角度を求める
	float Ang = acos(dot);
	Ang = DirectX::XMConvertToDegrees(Ang);

	float rotateAng = std::clamp(Ang, -10.0f, 10.0f);

	Math::Vector3 rotAxis;

	nowVec.Cross(_tarVec, rotAxis);
	m_debugWire.AddDebugLine(GetPos(), rotAxis, 5.0f, kRedColor);

	if (rotAxis.LengthSquared() == 0)return;

	//誘導回転する為の行列
	Math::Matrix rotation;
	rotation = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	//今のワールド座標を一時的に保存
	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	//回転行列の合成
	m_mWorld *= rotation;

	//回転をかけた後に戻す（じゃないと原点からの角度が膨れていくから）
	m_mWorld.Translation(pos);
}