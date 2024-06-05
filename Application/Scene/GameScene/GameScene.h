#pragma once
#include"../BaseScene/BaseScene.h"

class GameCamera;
class Player;
class EnemyBase;
class UiBase;
class ResultObject;

class GameScene :public BaseScene
{
public:

	GameScene() { Init(); };
	~GameScene() {};

	//�ǂ�Json��ǂݍ��ނ̂��������Ŕ��f���A�ǂݍ���Json�����e�X�̃I�u�W�F�N�g�ɗ^����
	//SelectScene���烂�[�h�������炤
	void SelectMode(std::string_view _modeName)
	{
		m_nowMode = _modeName;
	}

	//�X�e�[�W�����e�I�u�W�F�N�g�֓`�B
	//���O�̂�SelectScene���������ė��邱�ƂŁA�ǂ̃��[�h���𔻒f����
	json11::Json StageData(std::string_view _modeName = "")
	{
		std::ifstream ifs("Asset/Data/Stage.json");
		if (ifs.fail()) { assert(0 && "Json�t�@�C���̃p�X���Ⴂ�܂�"); }

		//������Ƃ��đS�ǂݍ���
		std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		//�������JSON����́i�p�[�X�j����
		std::string err;
		json11::Json jsonData = json11::Json::parse(strJson, err);
		if (err.size() > 0) { assert(0 && "�ǂݍ��񂾃t�@�C����Json�ϊ��Ɏ��s"); }
		auto& stageMode = jsonData[_modeName.data()].object_items();
		return stageMode;
	}

	//�����蔻���������̂��Q�[���V�[���Ń��X�g�ɓ���邽�߂̊֐�
	void AddHitObject(const std::shared_ptr<KdGameObject>& _hitObj)
	{
		m_hitObjList.push_back(_hitObj);
	}

	const std::list<std::shared_ptr<KdGameObject>>& GetList()
	{
		return m_hitObjList;
	}

private:
	void Init()override;
	void Event()override;

	//�e�X�g
	std::string_view m_nowMode = "Traninng";

	//�e�X�g
	std::weak_ptr<GameCamera> m_wpTpsCam;
	std::weak_ptr<ResultObject> m_wpReOb;
	std::weak_ptr<Player> m_wpPla;
	std::weak_ptr<EnemyBase> m_wpEne;
	std::weak_ptr<UiBase> m_wpUi;
};