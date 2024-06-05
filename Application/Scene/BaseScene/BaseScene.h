#pragma once

class BaseScene
{
public:
	BaseScene() { Init(); }
	~BaseScene() {}

	void ImGuiUpdate();
	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();

	void DrawDebug();

	//イテレータ
	void ListIterator()
	{
		//objListの整理
		//イテレータ作成	
		//イテレータにobjListの先頭アドレスを代入
		auto it = m_objList.begin();

		//最後じゃなかったら
		while (it != m_objList.end())
		{
			//期限切れじゃないか
			if ((*it)->IsExpired())
			{
				it = m_objList.erase(it);
			}
			else //切れていなければ次へ
			{
				//次のデータへ
				++it;
			}
		}
	}

	//イテレータ
	void HitListIterator()
	{
		auto it = m_hitObjList.begin();

		//最後じゃなかったら
		while (it != m_hitObjList.end())
		{
			//期限切れじゃないか
			if ((*it)->IsExpired())
			{
				it = m_hitObjList.erase(it);
			}
			else //切れていなければ
			{
				++it;
			}
		}
	}

	//オブジェクトリストを取得する
	const std::list< std::shared_ptr<KdGameObject>>& GetObjList()const { return m_objList; }

	//オブジェクトリストにオブジェクト追加
	void AddObject(const std::shared_ptr<KdGameObject>& _obj) { m_objList.push_back(_obj); }

	const std::list< std::shared_ptr<KdGameObject>>& GetHitObjList()const { return m_hitObjList; }

	//オブジェクトリストにオブジェクト追加
	void AddHitObject(const std::shared_ptr<KdGameObject>& _hitObj) { m_hitObjList.push_back(_hitObj); }

protected:

	//継承先で使えないが、起動はする
	virtual void Init() {}
	virtual void Event() {}

	//オブジェクトリスト
	std::list< std::shared_ptr<KdGameObject>> m_objList;

	//当たり判定用リスト
	std::list< std::shared_ptr<KdGameObject>> m_hitObjList;

	//ポインタでベースに持たせておく
	//必要に応じてmake_shard
	std::unique_ptr<KdCamera> m_camera = nullptr;
};