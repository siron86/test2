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

	//アニメーションの登録
	void AddAnimation();

	//現在のアニメーションと予約が合っているかどうか
	void CheckAnimation();

	//予約と違っていたら変更
	void ChangeAnimation(AnimationType );

	void init()
	{
		ChangeAnimation(AnimationType::Idle);
	}
private:

};