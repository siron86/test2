#pragma once

// ゲーム上に存在するすべてのオブジェクトの基底となるクラス
class KdGameObject : public std::enable_shared_from_this<KdGameObject>
{
public:

	// どのような描画を行うのかを設定するTypeID：Bitフラグで複数指定可能
	enum
	{
		eDrawTypeLit = 1 << 0,
		eDrawTypeUnLit = 1 << 1,
		eDrawTypeBright = 1 << 2,
		eDrawTypeUI = 1 << 3,
		eDrawTypeDepthOfShadow = 1 << 4,
	};

	KdGameObject() {}
	virtual ~KdGameObject() { Release(); }

	// 生成される全てに共通するパラメータに対する初期化のみ
	virtual void Init() {}

	virtual void ImGuiUpdate() {}
	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	// それぞれの状況で描画する関数
	virtual void GenerateDepthMapFromLight() {}
	virtual void GenerateDepthMapFromLight_SkinMesh() {}
	virtual void PreDraw() {}
	virtual void DrawLit() {}
	virtual void DrawLit_SkinMesh() {}
	virtual void DrawUnLit() {}
	virtual void DrawBright() {}
	virtual void DrawSprite() {}
	virtual void DrawDebug() {};

	virtual void SetAsset(const std::string&) {}

	virtual void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }
	virtual Math::Vector3 GetPos() const { return m_mWorld.Translation(); }

	// 拡大率を変更する関数
	void SetScale(float scalar);
	virtual void SetScale(const Math::Vector3& scale);
	virtual Math::Vector3 GetScale() const;

	virtual void SetMatrix(const Math::Matrix& mat) { m_mWorld = mat; }
	const Math::Matrix& GetMatrix() const { return m_mWorld; }

	//===========================================================================================================
	//自作
	//===========================================================================================================

	//変数(いらない)
	std::shared_ptr<const KdModelWork> _Model;
	KdCollider::SphereInfo sphereInfo;
	KdCollider::RayInfo rayInfo;
	int damage = 0;

	//関数
	virtual const std::shared_ptr<const KdModelWork> GetModel() { return _Model; }
	virtual KdCollider::SphereInfo GetHitPointTypeSphere() { return sphereInfo; }
	virtual KdCollider::RayInfo GetHitPointTypeRay() { return rayInfo; }
	virtual void Hit(
		const int& = 0,							     //当たったオブジェクトのタイプ
		const Math::Vector3 & = Math::Vector3::Zero, //当たった方向
		const Math::Vector3 & = Math::Vector3::Zero, //当たった場所
		const int& =0                                //ダメージを格納する変数
	) {}
	virtual bool LockOnFlg() { return false; }
	virtual KdCollider::Type GetType() { return KdCollider::TypeGround; }

	//緊急
	virtual void TestHit() {};
	//===========================================================================================================

	virtual bool IsExpired() const { return m_isExpired; }

	virtual bool IsVisible() const { return false; }

	// 視錐台範囲内に入っているかどうか
	virtual bool CheckInScreen(const DirectX::BoundingFrustum&) const { return false; }

	// カメラからの距離を計算
	virtual void CalcDistSqrFromCamera(const Math::Vector3& camPos);

	float GetDistSqrFromCamera() const { return m_distSqrFromCamera; }

	UINT GetDrawType() const { return m_drawType; }

	bool Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);

protected:

	void Release() {}

	// 描画タイプ・何の描画を行うのかを決める / 最適な描画リスト作成用
	UINT m_drawType = 0;

	// カメラからの距離
	float m_distSqrFromCamera = 0;

	// 存在消滅フラグ
	bool m_isExpired = false;

	// 3D空間に存在する機能
	Math::Matrix	m_mWorld;

	// 当たり判定クラス
	std::unique_ptr<KdCollider> m_pCollider = nullptr;
};