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

	//�C�e���[�^
	void ListIterator()
	{
		//objList�̐���
		//�C�e���[�^�쐬	
		//�C�e���[�^��objList�̐擪�A�h���X����
		auto it = m_objList.begin();

		//�Ōザ��Ȃ�������
		while (it != m_objList.end())
		{
			//�����؂ꂶ��Ȃ���
			if ((*it)->IsExpired())
			{
				it = m_objList.erase(it);
			}
			else //�؂�Ă��Ȃ���Ύ���
			{
				//���̃f�[�^��
				++it;
			}
		}
	}

	//�C�e���[�^
	void HitListIterator()
	{
		auto it = m_hitObjList.begin();

		//�Ōザ��Ȃ�������
		while (it != m_hitObjList.end())
		{
			//�����؂ꂶ��Ȃ���
			if ((*it)->IsExpired())
			{
				it = m_hitObjList.erase(it);
			}
			else //�؂�Ă��Ȃ����
			{
				++it;
			}
		}
	}

	//�I�u�W�F�N�g���X�g���擾����
	const std::list< std::shared_ptr<KdGameObject>>& GetObjList()const { return m_objList; }

	//�I�u�W�F�N�g���X�g�ɃI�u�W�F�N�g�ǉ�
	void AddObject(const std::shared_ptr<KdGameObject>& _obj) { m_objList.push_back(_obj); }

	const std::list< std::shared_ptr<KdGameObject>>& GetHitObjList()const { return m_hitObjList; }

	//�I�u�W�F�N�g���X�g�ɃI�u�W�F�N�g�ǉ�
	void AddHitObject(const std::shared_ptr<KdGameObject>& _hitObj) { m_hitObjList.push_back(_hitObj); }

protected:

	//�p����Ŏg���Ȃ����A�N���͂���
	virtual void Init() {}
	virtual void Event() {}

	//�I�u�W�F�N�g���X�g
	std::list< std::shared_ptr<KdGameObject>> m_objList;

	//�����蔻��p���X�g
	std::list< std::shared_ptr<KdGameObject>> m_hitObjList;

	//�|�C���^�Ńx�[�X�Ɏ������Ă���
	//�K�v�ɉ�����make_shard
	std::unique_ptr<KdCamera> m_camera = nullptr;
};