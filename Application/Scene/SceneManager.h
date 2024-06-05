#pragma once

//前方宣言
class BaseScene;

class SceneManager
{
public:
	//シーン情報
	enum class SceneType
	{
		Title,
		Select,
		Game,
		Result,
	};

	//モード情報
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
		//なにで始まるか
		ChangeScene(SceneType::Title);
	}

	//void Release();

	//マネージャーオリジナル

	//シーン切り替えを行うのではなく、次の予約
	void SetNextScene(SceneType _nextScene, GameMode _nowMode = GameMode::Training)
	{
		//予約
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

	//当たり判定後の消去処理
	void HitListIterator();

	const std::list< std::shared_ptr<KdGameObject>>& GetObjList()const;

	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

	const std::list< std::shared_ptr<KdGameObject>>& GetHitObjList()const;

	void AddHitObject(const std::shared_ptr<KdGameObject>& _hitObj);


private:

	//変更しかできない
	void ChangeScene(SceneType _sceneType);

	//現在のシーンを管理しているポインタ
	std::shared_ptr<BaseScene> m_currentScene = nullptr;

	//現在のシーンを管理している変数
	SceneType m_currentSceneType = SceneType::Game;
	//現在のモード
	GameMode m_gameMode = GameMode::Story;

	//次のシーンを管理している変数
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