#pragma once

class Animation
{
public:
	Animation() {}
	~Animation() {}

	enum class AnimationType
	{
		Idle,
		Walk,
		Dash,
		Attack
	};

	//�A�j���[�V�����̓o�^
	void AddAnimation();

	//���݂̃A�j���[�V�����Ɨ\�񂪍����Ă��邩�ǂ���
	void CheckAnimation();

	//�\��ƈ���Ă�����ύX
	void ChangeAnimation(AnimationType );

	void init()
	{
		ChangeAnimation(AnimationType::Idle);
	}
private:

};