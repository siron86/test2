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

	//�Q�[���V�[���̃A�h���X�����炤
	void SetScene(GameScene* _scene) { m_scene = _scene; }

private:

	void UpdateRotateByMouse();

	//�R���g���[���t���O
	bool m_freeFlg = false;

	static const int m_delayFrame = 8;

	Math::Vector3 m_pos = {};
	bool controlFlg = true;

	Math::Matrix m_delayMat[m_delayFrame];

	//�X�e�[�W�̏��i���|�j
	GameScene* m_scene;

	POINT m_FixMousePos;
};