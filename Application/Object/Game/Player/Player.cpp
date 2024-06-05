#include "Player.h"
#include"../../Camera/GameCamera/GameCamera.h"
#include"../../Game/Weapon/WeaponBase.h"
#include"../Enemy/EnemyBase.h"

#include"../ObjectSystem/ObjectSystem.h"

void Player::ImGuiUpdate()
{
	//ImGui用アップデート================================
	if (!m_spWeaBase)return;
	m_spWeaBase->ImGuiUpdate();
	//===================================================
}

void Player::PreUpdate()
{
	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->PreUpdate();
	//===================================================
}

void Player::Update()
{
	if (!m_control)return;

	//カメラの回転行列の取得
	if (!m_wpCamera.expired()) { m_camRotMat = m_wpCamera.lock()->GetRotationYMatrix(); }

	//当たり判定の起動
	m_wpSys.lock()->HitPointTypeSphere(shared_from_this());
	m_wpSys.lock()->HitPointTypeRay(shared_from_this());

	//==================================================================================================
	//自機操作
	// 
	//ステートが機能していたら更新
	if (m_nowAction)m_nowAction->Update(*this);

	//武器の選択
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
	//繰り返し処理

	//加速度
	nowAccel = STR_Setting.MoveAccel;
	//最大速
	nowSpeedMax = STR_Setting.MoveSpeedMax;
	//速度低下倍率
	nowDownSpeed = STR_Setting.DownSpeed;

	//少しずつ速度を落とす
	m_moveVec *= nowDownSpeed;

	//ブースト回復
	BoostHeal();

	//オーバーヒート状態
	OverHeat();

	//重力テスト
	STR_Setting.Gravity -= STR_Setting.GravityAccel;
	m_moveVec.y += STR_Setting.Gravity;


	//座標に変換するための変換
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_moveVec);

	//移動完了
	m_mWorld *= trans;

	//ターゲット情報が来ていたら
	if (!m_wpTraEne.expired())
	{
		//近い順番に並べ替える
		TargetSort();
	}

	//距離範囲外ならロックを外す
	if (m_targetVec.Length() > 250)
	{
		m_lockFlg = false;
	}
	//ターゲットへのベクトルの正規化
	m_targetVec.Normalize();

	//カメラの情報が来ているか
	if (m_wpCamera.lock())
	{
		//ロックオン可能範囲かどうか
		float dot = m_wpCamera.lock()->GetMatrix().Backward().Dot(m_targetVec);

		//可能範囲内なら
		if (dot >= 0.9f)
		{
			//知らせる
			m_lockOkFlg = true;
		}
		//ロックオン中に範囲外に出たらロックオン解除
		else m_lockOkFlg = false, m_lockFlg = false;
	}

	//ロックオン
	//カメラ追従からターゲット追従に移行
	if (m_lockFlg)RotateFromTarget(m_mWorld.Backward(), m_targetVec);
	else RotateFromCamera();//フラグが外れた場合カメラ追従に戻る
	//==================================================================================================

	//変
	//構造体変数に値を保存し、Uiに送る
	m_data->HpData = STR_Setting.Hp;
	m_data->BoostData = STR_Boost.Gauge;

	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->SetCamera(m_wpCamera.lock());
	m_spWeaBase->Update();
	m_weaList = m_spWeaBase->GetWeaList();
	//===================================================
}

void Player::PostUpdate()
{
	if (!m_spAnimator)	return;

	// アニメーションの更新
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes(), 1.0f);
	m_spModel->CalcNodeMatrices();

	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->PostUpdate();
	//===================================================
}

void Player::DrawLit()
{
	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawLit();
	//===================================================
}

void Player::DrawLit_SkinMesh()
{
	//3Dモデル
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawLit_SkinMesh();
	//===================================================
}

void Player::DrawUnLit()
{
	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawUnLit();
	//===================================================
}

void Player::DrawDebug()
{
	m_debugWire.Draw();

	//ウェポンベース=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawDebug();
	//===================================================
}

void Player::Init()
{
	m_data = std::make_shared<PLAYERDATA>();
	//===================================================================
	// Json初期化
	//===================================================================
	std::ifstream ifs("Asset/Data/Object.json");
	if (ifs.fail()) { assert(0 && "Jsonファイルのパスが違います"); }

	//文字列として全読み込み
	std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	//文字列のJSONを解析（パース）する
	std::string err;
	json11::Json jsonObj = json11::Json::parse(strJson, err);
	if (err.size() > 0) { assert(0 && "読み込んだファイルのJson変換に失敗"); }
	auto& player = jsonObj["Player"].object_items();

	//===================================================================
	//プレイヤー情報
	//大きさ
	float scale = (float)player["Scale"].number_value();
	//体力
	STR_Setting.Hp = player["Hp"].int_value();
	//重力
	STR_Setting.Gravity = (float)player["Gravity"].number_value();
	//重力加速度
	STR_Setting.GravityAccel = (float)player["GravityAccel"].number_value();
	//移動加速度
	STR_Setting.MoveAccel = (float)player["MoveAccel"].number_value();
	//移動速度低下率
	STR_Setting.DownSpeed = (float)player["DownSpeed"].number_value();
	//移動速度最大
	STR_Setting.MoveSpeedMax = (float)player["MoveSpeedMax"].number_value();
	//回転速度
	STR_Setting.Ang = (float)player["Ang"].number_value();
	//ロックオン時回転速度
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

	//初期出現座標設定
	auto& sPlayer = m_stageData["Player"].object_items();
	//座標
	auto& pos = sPlayer["Pos"].array_items();
	Math::Vector3 nowPos = {
		(float)pos[0].number_value(),
		(float)pos[1].number_value(),
		(float)pos[2].number_value()
	};

	//初期化=============================================================================================
	//モデルの入れ物
	m_spModel = std::make_shared<KdModelWork>();
	m_mWorld = Math::Matrix::CreateScale(scale) * Math::Matrix::CreateTranslation(nowPos);

	//モデル情報
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Player/Test.gltf"));

	//アニメーションのセット
	m_spAnimator = std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Walk"));

	//初期ステートの指定
	ChangeActionState(std::make_shared<ActionIdle>());

	//当たり判定用
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player", m_spModel, KdCollider::TypeEvent);

	//ウェポンベース===============================================================
	m_spWeaBase = std::make_shared<WeaponBase>();
	m_spWeaBase->SetScene(m_scene);
	m_spWeaBase->Init();
	m_spWeaBase->SetWeaponType(1);
	m_spWeaBase->SetOwner(shared_from_this());
	//=============================================================================
}

//================================================================
// カメラ追従
// カメラの正面方向を対象に内積と外積を用いて回転する角度、方向を求めている関数
//================================================================
void Player::RotateFromCamera()
{
	Math::Vector3 nowVec = m_mWorld.Backward();

	Math::Vector3 tarVec = m_camRotMat.Backward();

	if (nowVec == tarVec)return;
	nowVec.Normalize();
	tarVec.Normalize();

	//内積（ドット積）
	float dot = nowVec.Dot(tarVec);
	dot = std::clamp(dot, -1.f, 1.f);

	//角度へ変換
	float Ang = acos(dot);
	Ang = DirectX::XMConvertToDegrees(Ang);

	float rotateAng = std::clamp(Ang, 0.f, 8.f);

	Math::Vector3 rotAxis;

	//外積
	nowVec.Cross(tarVec, rotAxis);

	//向いている方向が等しければ帰る
	if (rotAxis.LengthSquared() == 0)return;

	//回転行列作成
	m_rotateMat = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3::Zero);

	m_mWorld *= m_rotateMat;

	m_mWorld.Translation(pos);
}

//================================================================
// 敵追従
// 敵の座標を対象に内積と外積を用いて回転する角度、方向を求めている関数
//================================================================
void Player::RotateFromTarget(Math::Vector3 _ownerVec, Math::Vector3 _tarVec)
{
	//Nullチェック
	if (_ownerVec.Length() == 0.f || _tarVec.Length() == 0.f)return;
	//正規化
	_ownerVec.Normalize();
	_tarVec.Normalize();

	//デバック用
	m_debugWire.AddDebugLine(GetPos(), _ownerVec, 5.0f, kBlueColor);
	m_debugWire.AddDebugLine(GetPos(), _tarVec, 5.0f, kGreenColor);

	//内積
	float dot = _ownerVec.Dot(_tarVec);
	dot = std::clamp(dot, -1.0f, 1.0f);

	//角度へ変換
	float Ang = acos(dot);
	Ang = DirectX::XMConvertToDegrees(Ang);

	float rotateAng = std::clamp(Ang, -8.0f, 8.0f);

	Math::Vector3 rotAxis;

	//外積
	_ownerVec.Cross(_tarVec, rotAxis);
	m_debugWire.AddDebugLine(GetPos(), rotAxis, 5.0f, kRedColor);

	//向いている方向が等しければ帰る
	if (rotAxis.LengthSquared() == 0)return;

	//回転行列作成
	m_rotateMat = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3::Zero);

	//回転
	m_mWorld *= m_rotateMat;

	m_mWorld.Translation(pos);
}

void Player::TargetSort()
{
	//Nullチェック
	if (m_wpTraEne.lock()->GetEneList().size() == 0) {
		m_lockFlg = false;
		return;
	}

	std::list<std::shared_ptr<EnemyBase>> enemList = m_wpTraEne.lock()->GetEneList();
	std::list<EnemySorter> targetSortList;

	//近い順に並び替え
	for (auto& target : enemList) {
		targetSortList.push_back(EnemySorter(target, (target->GetPos() - GetPos()).Length()));
	}
	targetSortList.sort();

	//一番近いものをロック
	m_targetVec = targetSortList.front().m_enem->GetPos() - GetPos();

	//ロックオン中ウェポンに渡す用
	if (m_lockFlg) {
		m_spWeaBase->SetTarPos(targetSortList.front().m_enem->GetHitPointTypeSphere().m_sphere.Center);
	}
}

//================================================================
//ブースト処理
// 使用には一定のコストが掛かり現在の速度に加算する形
//================================================================
void Player::Boost()
{
	//Nullチェック
	if (m_overheatFlg)return;
	
	if (!m_boostFlg)
	{
		//アニメーションの初期化と設定
		m_spAnimator = std::make_shared<KdAnimator>();
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Boost"));
		m_boostFlg = true;
	}

	nowAccel = STR_Boost.Accel;
	nowSpeedMax = STR_Boost.Speed;
	nowDownSpeed = STR_Boost.DownSpeed;
	STR_Boost.HealElapTime = STR_Boost.HealTime;

	//使用時初期化
	STR_Boost.HealElapTime = STR_Boost.HealTime;
	STR_Setting.Gravity = 0.f;

	//ブースト時
	--STR_Boost.Gauge;
}

//================================================================
//ブースト回復処理
//時間経過で回復しだして最大値まで回復
//オーバーヒートと違い回復中でもブーストは使用可能
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
//オーバーヒート処理
// 通常回復の1.5倍の速度で回復する
// 最大回復するまでブーストは使用不可状態
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
// 状態を変える処理
// 現在のキャラの状態を破棄し新しい状態へ変更するための関数
//===========================================================================
void Player::ChangeActionState(std::shared_ptr<ActionStateBase> _nextState)
{
	//関数が呼ばれたとき現在の状態を終了（破棄）する
	if (m_nowAction) m_nowAction->Exit(*this);

	//次の状態が何か教える
	m_nowAction = _nextState;

	//始める
	m_nowAction->Enter(*this);
}

void Player::ActionIdle::Enter(Player& _owner)
{
	//アニメーションの初期化と設定
	_owner.m_spAnimator = std::make_shared<KdAnimator>();
	_owner.m_spAnimator->SetAnimation(_owner.m_spModel->GetData()->GetAnimation("Idle"));
}

void Player::ActionIdle::Update(Player& _owner)
{
	Math::Vector3 moveDir{};

	//前移動
	if (GetAsyncKeyState('W') & 0x8000) { moveDir += Math::Vector3::Backward; }

	//左移動
	if (GetAsyncKeyState('A') & 0x8000) { moveDir += Math::Vector3::Left; }

	//後ろ移動
	if (GetAsyncKeyState('S') & 0x8000) { moveDir += Math::Vector3::Forward; }

	//右移動
	if (GetAsyncKeyState('D') & 0x8000) { moveDir += Math::Vector3::Right; }

	//上昇
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) { moveDir += Math::Vector3::Up; }

	//ロックオン
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		//視野範囲内ならロックオン可能
		if (_owner.m_lockOkFlg)
		{
			//ロックオン時はカメラへの追従を切る
			_owner.m_lockFlg = true;
			_owner.m_lockOkFlg = false;
		}
	}

	//射撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		//押されたら

		//m_weaBaseのActive関数を起動する
		//2秒後起動
		++_owner.ActiveTime;
		if (_owner.ActiveTime >= _owner.ActivePlayTime)
		{
			_owner.m_spWeaBase->Active();
		}
	}
	else _owner.ActiveTime = 0;

	//移動入力されていたら
	//移動ステートに移行
	if (moveDir.LengthSquared() > 0.f) { _owner.ChangeActionState(std::make_shared<ActionWalk>()); }
}

void Player::ActionWalk::Enter(Player& _owner)
{
	//アニメーションの初期化と設定
	_owner.m_spAnimator = std::make_shared<KdAnimator>();
	_owner.m_spAnimator->SetAnimation(_owner.m_spModel->GetData()->GetAnimation("Walk"));
}

void Player::ActionWalk::Update(Player& _owner)
{
	Math::Vector3 moveDir{};

	//前移動
	if (GetAsyncKeyState('W') & 0x8000) { moveDir += Math::Vector3::Backward; }

	//後ろ移動
	if (GetAsyncKeyState('S') & 0x8000) { moveDir += Math::Vector3::Forward; }

	//左移動
	if (GetAsyncKeyState('A') & 0x8000) { moveDir += Math::Vector3::Left; }

	//右移動
	if (GetAsyncKeyState('D') & 0x8000) { moveDir += Math::Vector3::Right; }

	//上昇
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		moveDir += Math::Vector3::Up;
		_owner.STR_Setting.Gravity = 0.f;
	}

	//ブースト
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) 
	{ 
		if (_owner.STR_Boost.Gauge < 0.f)_owner.ChangeActionState(std::make_shared<ActionIdle>());
		//ブースト関数起動
		_owner.Boost();
	}

	if (moveDir.LengthSquared() == 0)
	{
		//何も入力されていないので待機状態へ移行
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	//移動調整
	if (moveDir != Math::Vector3::Zero)
	{
		moveDir.Normalize();
		moveDir = Math::Vector3::TransformNormal(moveDir, _owner.m_camRotMat);
	}

	//加速度
	float acc = _owner.nowAccel;
	//最大速
	float speedMax = _owner.nowSpeedMax;
	//速度低下倍率
	float downSpeed = 0.f;
	downSpeed = _owner.nowDownSpeed;

	//移動
	//移動ベクトルが最大速度以下なら加速
	if (_owner.m_moveVec.Length() < speedMax)
	{
		//加速
		_owner.m_moveVec += moveDir * acc;
		//最大速度以上にならない処理
		if (_owner.m_moveVec.Length() >= speedMax)
		{
			//一度正規化して
			_owner.m_moveVec.Normalize();
			//最大速度をかけて速度を一定に（ガタガタしないように）
			_owner.m_moveVec *= speedMax;
		}
	}
}