#include "Rifle.h"
#include"../../Player/Player.h"
#include"../Bullet/Bullet.h"

void Rifle::Update()
{
	//銃口の情報
	const KdModelWork::Node* pNode = GetModel()->FindNode("ShotPoint");
	if (pNode) { m_muzzleMat = pNode->m_worldTransform * m_mWorld; }

	//選択されていたら発射可能に
	if (m_selectFlg)
	{
		Shot();
	}

	//攻撃間隔
	CoolTime();

	//残弾なし
	if (STR_Weapon.bulletCount <= 0)
	{
		m_relFlg = true;
	}

	//リロード
	Reload();
}

void Rifle::PostUpdate()
{
	m_scaleMat = Math::Matrix::CreateScale(STR_Weapon.scale);

	m_rotateMat = Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_ang.y),
		DirectX::XMConvertToRadians(m_ang.x),
		DirectX::XMConvertToRadians(m_ang.z)
	);
	m_transMat = Math::Matrix::CreateTranslation(m_pos);

	//拡大＊移動＊設置場所
	m_mWorld = m_scaleMat * m_rotateMat * m_transMat * m_rackMat;
}

void Rifle::DrawLit_SkinMesh()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Rifle::DrawDebug()
{
	m_debugWire.Draw();
}

void Rifle::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Weapon/Rifle/RifleT.gltf"));


	//json初期化===================================================================
	//ファイルを開く
	std::ifstream ifs("Asset/Data/Object.json");
	if (ifs.fail()) { assert(0 && "jsonファイルを開けませんでした"); };

	//全読み込み
	std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	//読み込んだ文字列を解析する
	std::string err;
	json11::Json jsonObj = json11::Json::parse(strJson, err);
	if (err.size() > 0) { assert(0 && "変換失敗"); };

	auto& weaponData = jsonObj["Weapon"].object_items();
	auto& rifleData = weaponData["Rifle"].object_items();

	STR_Weapon.scale = (float)rifleData["Scale"].number_value();
	m_ang = { 0.f,-90.f,180.f };
	m_pos = { 1.f,0.5f,0.f };

	//弾数
	STR_Weapon.bulletMax = 30;
	STR_Weapon.bulletCount = STR_Weapon.bulletMax;

	//リロード時間
	STR_Weapon.relTime = 60.0f * 2.5f;
	STR_Weapon.nowRelTime = STR_Weapon.relTime;

	//発射間隔
	STR_Weapon.coolTime = STR_Weapon.defaultCoolTime;
}

void Rifle::Reload()
{
	//自動又は手動リロード
	if (m_relFlg)
	{
		//カウント開始
		--STR_Weapon.nowRelTime;

		//時間になったら
		if (STR_Weapon.nowRelTime <= 0)
		{
			//リロード完了
			m_relFlg = false;
			m_enemyFlg = false;
			STR_Weapon.bulletCount = STR_Weapon.bulletMax;
			STR_Weapon.nowRelTime = STR_Weapon.relTime;
		}
	}
}

void Rifle::Shot()
{
	//発射
	if (m_shotFlg)
	{
		//発射していなかったら
		if (STR_Weapon.coolTime == STR_Weapon.defaultCoolTime && !m_weaBase.expired())
		{
			//残弾がなかったら、何も発射せずリロード警告Ui起動
			if (!m_relFlg)
			{
				//弾数使用
				--STR_Weapon.bulletCount;
				//弾を発射
				m_weaBase.lock()->ShotBullet(m_muzzleMat, { 27.f,3.f,0.f }, 10, 5);
				//マズルフラッシュ
				effObj = KdEffekseerManager::GetInstance().Play("09-1.efkefc", m_muzzleMat.Translation(), false);
				if (!effObj)return;
				effObj->SetWorldMatrix(Math::Matrix::CreateScale(2.f) * m_muzzleMat);
			}
		}

	}
	m_debugWire.AddDebugLine(m_muzzleMat.Translation(), m_weaBase.lock()->CenterShot(m_muzzleMat), 100.f, kRedColor);
	//選択解除
	m_selectFlg = false;
	m_shotFlg = false;
}

void Rifle::CoolTime()
{
	//クールタイム
	if (STR_Weapon.coolTime <= STR_Weapon.defaultCoolTime)
	{
		//連射間隔
		--STR_Weapon.coolTime;
		if (STR_Weapon.coolTime <= 0.f)
		{
			STR_Weapon.coolTime = STR_Weapon.defaultCoolTime;
		}
	}
}