#include "StageObject.h"

void StageObject::ImGuiUpdate()
{
	//ImGui::SliderFloat("Pos", &m_pos.x, 0, 100);

	//�����Ƒ���\���ǂ�����ύX����
	if (ImGui::Button("Apartment"))
	{
		//�e�X�g
		//�����ꂽ�烂�f������ς���
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Building/Apartment.gltf"));
	}

	if (ImGui::Button("Building"))
	{
		//�e�X�g
		//�����ꂽ�烂�f������ς���
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Building/Building.gltf"));
	}

	if (m_selectFlg)
	{
		//�ϐ����K�v��
		//�����@Json�ŊǗ�����Ȃ炠���
		ImGui::DragFloat("Scale", &m_scale, 0.1f);
		ImGui::DragFloat3("Rotate", &m_ang.x, 0.5f);
		ImGui::DragFloat3("Pos", &m_pos.x,0.5f);
	}

	//Json�FImGui�����e�X�g(��������)[]
	if (ImGui::Button("Save"))
	{
		//Json�̕ۑ�
		//�ǂ��ɕۑ����邩
		std::string file = "Asset/Data/StageData.json";

		//�ۑ�������Q
		json11::Json::object newData;

		//�ۑ�������
		json11::Json newObjData=m_objectData;

		//for�ŉ񂷂Ƃ��āA�����ő�l�i�I���j�Ƃ��ǂ�����ĕ����ۑ��ł���悤�ɂ��邩
		//��]�ɂ̓i���o�[���g�������i�T�C�Y���ƍŏI�i���o�[���ꏏ�Ȃ�I���j
		//�����ɂ�Json�̔z����g�������Ȃ����ǂǂ�����Ď��̃I�u�W�F�N�g�ɕύX���邩
		//for(int i=0;i<10;i++)
		{
			//���W
			json11::Json::array  pos(3);
			pos[0] = m_pos.x;
			pos[1] = m_pos.y;
			pos[2] = m_pos.z;

			newObjData["Pos"] = pos;

			//�ԍ�
			newObjData["number"] = 1;

			//��{�Ƀf�[�^�ړ�
			newData["Building"] = newObjData;
		}

		//�ۑ��J�n
		std::ofstream ofs(file);

		//�t�@�C�������邩�ǂ���
		if (ofs)
		{
			//������ւ̕ϊ�
			json11::Json newJsonData = newData;

			//Json�̃t�H�[�}�b�g�ɕϊ�
			std::string strNewJson = newJsonData.dumpDebug();

			//�w��t�@�C���ɏ�������
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
	//3D���f��
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void StageObject::DrawDebug()
{
	m_debugWire.Draw();
}

void StageObject::Init()
{
	{
		//���f���^�C�v�I��
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
	//���f���i�[�p�̓��ꕨ�쐬
	m_spModel = std::make_shared<KdModelWork>();

	//���f���S���I
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData(m_objectData["ModelData"].string_value()));


	//�����蔻��p
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageModel", m_spModel, KdCollider::TypeBump);
}

void StageObject::Collider(std::shared_ptr<KdModelWork> _objName)
{
	_objName = std::make_shared<KdModelWork>();
}