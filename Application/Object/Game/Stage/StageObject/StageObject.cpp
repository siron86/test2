#include "StageObject.h"

void StageObject::ImGuiUpdate()
{
	//ImGui::SliderFloat("Pos", &m_pos.x, 0, 100);

	//押すと操作可能かどうかを変更する
	if (ImGui::Button("Apartment"))
	{
		//テスト
		//押されたらモデル情報を変える
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Building/Apartment.gltf"));
	}

	if (ImGui::Button("Building"))
	{
		//テスト
		//押されたらモデル情報を変える
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Building/Building.gltf"));
	}

	if (m_selectFlg)
	{
		//変数作る必要性
		//あるわ　Jsonで管理するならあるわ
		ImGui::DragFloat("Scale", &m_scale, 0.1f);
		ImGui::DragFloat3("Rotate", &m_ang.x, 0.5f);
		ImGui::DragFloat3("Pos", &m_pos.x,0.5f);
	}

	//Json：ImGui同期テスト(書き込み)[]
	if (ImGui::Button("Save"))
	{
		//Jsonの保存
		//どこに保存するか
		std::string file = "Asset/Data/StageData.json";

		//保存する情報群
		json11::Json::object newData;

		//保存する情報
		json11::Json newObjData=m_objectData;

		//forで回すとして、何を最大値（終わり）としどうやって複数保存できるようにするか
		//回転にはナンバーを使いたい（サイズ数と最終ナンバーが一緒なら終わり）
		//複数にはJsonの配列を使うしかないけどどうやって次のオブジェクトに変更するか
		//for(int i=0;i<10;i++)
		{
			//座標
			json11::Json::array  pos(3);
			pos[0] = m_pos.x;
			pos[1] = m_pos.y;
			pos[2] = m_pos.z;

			newObjData["Pos"] = pos;

			//番号
			newObjData["number"] = 1;

			//大本にデータ移入
			newData["Building"] = newObjData;
		}

		//保存開始
		std::ofstream ofs(file);

		//ファイルがあるかどうか
		if (ofs)
		{
			//文字列への変換
			json11::Json newJsonData = newData;

			//Jsonのフォーマットに変換
			std::string strNewJson = newJsonData.dumpDebug();

			//指定ファイルに書き込み
			ofs.write(strNewJson.c_str(), strNewJson.size());
		}
	}
}

void StageObject::Update()
{
	m_scaleMat = Math::Matrix::CreateScale(m_scale);
	m_rotatMat = Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_ang.y),
		DirectX::XMConvertToRadians(m_ang.x),
		DirectX::XMConvertToRadians(m_ang.z)
	);
	m_transMat = Math::Matrix::CreateTranslation(m_pos);

	m_mWorld = m_scaleMat * m_rotatMat * m_transMat;
}

void StageObject::DrawLit()
{
	//3Dモデル
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void StageObject::DrawDebug()
{
	m_debugWire.Draw();
}

void StageObject::Init()
{
	{
		//モデルタイプ選択
		//modelType = m_objectData["ModelType"].int_value();

		auto& pos = m_objectData["Pos"].array_items();
		m_pos.x = (float)pos[0].number_value();
		m_pos.y = (float)pos[1].number_value();
		m_pos.z = (float)pos[2].number_value();

		if (!m_objectData["Select"].bool_value())
		{
			m_selectFlg = false;
		}
		else m_selectFlg = m_objectData["Select"].bool_value();
	}
	//モデル格納用の入れ物作成
	m_spModel = std::make_shared<KdModelWork>();

	//モデル担当！
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData(m_objectData["ModelData"].string_value()));


	//当たり判定用
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageModel", m_spModel, KdCollider::TypeBump);
}

void StageObject::Collider(std::shared_ptr<KdModelWork> _objName)
{
	_objName = std::make_shared<KdModelWork>();
}