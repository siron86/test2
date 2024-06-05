#pragma once

//�O���錾
class BaseScene;

class SceneManager
{
public:
	//�V�[�����
	enum class SceneType
	{
		Title,
		Select,
		Game,
		Result,
	};

	//���[�h���
	enum class GameMode
	{
		Training,
		Story,
	};

	void ImGuiUpdate();
	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();

	void DrawDebug();

	void Init()
	{
		//�ȂɂŎn�܂邩
		ChangeScene(SceneType::Title);
	}

	//void Release();

	//�}�l�[�W���[�I���W�i��

	//�V�[���؂�ւ����s���̂ł͂Ȃ��A���̗\��
	void SetNextScene(SceneType _nextScene, GameMode _nowMode = GameMode::Training)
	{
		//�\��
		m_nextSceneType = _nextScene;
		m_gameMode = _nowMode;
	}

	std::string_view GetGameMode() 
	{ 
		std::string_view modeName = "";
		switch (m_gameMode)
		{
		case SceneManager::GameMode::Training:
			modeName = "Training";
			break;
		case SceneManager::GameMode::Story:
			modeName = "Story";
			break;
		default:
			break;
		}

		return modeName; 
	}

	//�����蔻���̏�������
	void HitListIterator();

	const std::list< std::shared_ptr<KdGameObject>>& GetObjList()const;

	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

	const std::list< std::shared_ptr<KdGameObject>>& GetHitObjList()const;

	void AddHitObject(const std::shared_ptr<KdGameObject>& _hitObj);


private:

	//�ύX�����ł��Ȃ�
	void ChangeScene(SceneType _sceneType);

	//���݂̃V�[�����Ǘ����Ă���|�C���^
	std::shared_ptr<BaseScene> m_currentScene = nullptr;

	//���݂̃V�[�����Ǘ����Ă���ϐ�
	SceneType m_currentSceneType = SceneType::Game;
	//���݂̃��[�h
	GameMode m_gameMode = GameMode::Story;

	//���̃V�[�����Ǘ����Ă���ϐ�
	SceneType m_nextSceneType = m_currentSceneType;

private:
	SceneManager() {}
	~SceneManager() {}

public:

	static SceneManager& Instance()
	{
		static SceneManager instace;
		return instace;
	}
};