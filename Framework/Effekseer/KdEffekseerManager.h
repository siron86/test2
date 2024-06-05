#pragma once

class KdEffekseerObject;
class CameraBase;

class KdEffekseerManager
{
public:

	void Create(int w, int h);

	void Update();
	void Draw();

	struct PlayEfkInfo
	{
		std::string FileName = "";
		Math::Vector3 Pos = {};
		float Size = 1.0f;
		float Speed = 1.0f;
		bool IsLoop = false;
	};

	// Json�t�@�C������f�[�^���擾���AEffekseer�G�t�F�N�g���Đ�����
	// �� �L�����N�^�[�̈ʒu�ɃG�t�F�N�g���Đ�����ꍇ�Ȃǂ����邽�߁A���W����n���Ă���
	std::shared_ptr<KdEffekseerObject> Play(const std::string& effName, const DirectX::SimpleMath::Vector3& pos,bool loop=true);

	void StopAllEffect();
	void StopEffect(const std::string& name);

	void OnPauseEfkUpdate()
	{
		m_isPause = true;
	}

	void Release();
	void Reset();

	void SetPos(const std::string& name, const Math::Vector3& pos);
	void SetRotation(const std::string& name, const Math::Vector3& axis, const float angle);
	void SetWorldMatrix(const std::string& name, const Math::Matrix& mWorld);

	void SetScale(const std::string& name, const Math::Vector3& scale);
	void SetScale(const std::string& name, const float scale);

	void SetSpeed(const std::string& name, const float speed = 1.0f);

	void SetPause(const std::string& name, const bool isPause);

	// �Đ������ǂ���
	const bool IsPlaying(const std::string& name) const;

	void SetCamera(const std::shared_ptr<CameraBase>& camera);

private:
#pragma warning(push)
#pragma warning(disable: 4172)

	const Effekseer::Vector3D& GetEfkVec3D(const Math::Vector3& vec) const
	{
		return Effekseer::Vector3D(vec.x, vec.y, vec.z);
	}
#pragma warning(pop)

	// Effekseer�G�t�F�N�g�̍Đ�
	std::shared_ptr<KdEffekseerObject> Play(const PlayEfkInfo& info);
	std::shared_ptr<KdEffekseerObject> Play(const std::shared_ptr<KdEffekseerObject> spObject);

	void UpdateEffekseerEffect();
	void UpdateEkfCameraMatrix();

	// �G�t�F�N�g�̃����_���[
	EffekseerRendererDX11::RendererRef m_efkRenderer = nullptr;

	// �G�t�F�N�g�̃}�l�[�W���[
	Effekseer::ManagerRef m_efkManager = nullptr;

	// �G�t�F�N�g���X�g
	std::unordered_map<std::string, std::shared_ptr<KdEffekseerObject>> m_effectMap;

	// �J����
	std::weak_ptr<CameraBase> m_wpCamera;

	bool m_isPause = false;

public:

	static KdEffekseerManager& GetInstance()
	{
		static KdEffekseerManager instance;

		return instance;
	}

private:

	KdEffekseerManager() {}
	~KdEffekseerManager() { Release(); }
};

class KdEffekseerObject
{
public:

	// �Đ�����
	bool IsPlaying();

	// �Đ��o�^�����}�l�[�W���[�ݒ�
	void SetParentManager(Effekseer::ManagerRef& parentManager)
	{
		m_parentManager = parentManager;
	}

	// �Đ�����G�t�F�N�g�ݒ�
	void SetEffect(Effekseer::EffectRef& effect) { m_effect = effect; }

	// �n���h���ݒ�
	void SetHandle(Effekseer::Handle& handle) { m_handle = handle; }

	// ���W�ݒ�
	void SetPos(const Math::Vector3& pos);

	// �T�C�Y�ݒ�
	void SetScale(const float scale = 1.0f);

	// ���x�ݒ�
	void SetSpeed(const float speed = 1.0f);

	// ���[���h�s��ݒ�
	void SetWorldMatrix(const Math::Matrix& mWorld);

	// ���[�v�ݒ�
	void SetLoop(const bool isLoop = false) { m_info.IsLoop = isLoop; }

	// �Đ�����G�t�F�N�g�̊e����ݒ�
	void SetPlayEfkInfo(const KdEffekseerManager::PlayEfkInfo& info) { m_info = info; }

	// �G�t�F�N�g�擾
	const Effekseer::EffectRef& GetEffect() const { return m_effect; }

	// �n���h���擾
	const Effekseer::Handle& GetHandle() const { return m_handle; }

	// ���W�擾
	const Math::Vector3 GetPos() { return m_info.Pos; }

	// �T�C�Y�擾
	const float GetSize() const { return m_info.Size; }

	// ���x�擾
	const float GetSpeed() const { return m_info.Speed; }

	// ���[�v���邩�ǂ���
	const bool IsLoop() const { return m_info.IsLoop; }

	// �G�t�F�N�g�̊e����擾
	KdEffekseerManager::PlayEfkInfo& WorkPlayEfkInfo() { return m_info; }
	const KdEffekseerManager::PlayEfkInfo& GetPlayEfkInfo() const { return m_info; }

private:

	Effekseer::ManagerRef				m_parentManager = nullptr;
	Effekseer::EffectRef				m_effect = nullptr;
	Effekseer::Handle					m_handle = -1;

	KdEffekseerManager::PlayEfkInfo		m_info = {};
};

#define EffekseerPath "Asset/Effekseer/"