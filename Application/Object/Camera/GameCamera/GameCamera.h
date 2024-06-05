#pragma once
#include"../CameraBase/CameraBase.h"

class GameScene;

class GameCamera :public CameraBase
{
public:
	GameCamera() { Init(); }
	~GameCamera() {}

	void Update()      override;
	void PostUpdate()  override;
	void Init()        override;

	void FreeCamera(bool _freeFlg) { m_freeFlg = _freeFlg; }

	void UpdateCollision();

	//ゲームシーンのアドレスをもらう
	void SetScene(GameScene* _scene) { m_scene = _scene; }

private:

	void UpdateRotateByMouse();

	//コントロールフラグ
	bool m_freeFlg = false;

	static const int m_delayFrame = 8;

	Math::Vector3 m_pos = {};
	bool controlFlg = true;

	Math::Matrix m_delayMat[m_delayFrame];

	//ステージの情報（生ポ）
	GameScene* m_scene;

	POINT m_FixMousePos;
};