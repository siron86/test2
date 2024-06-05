#pragma once
class GameScene;


class Stage :public KdGameObject
{
public:
	Stage() {}
	~Stage() {}

	void ImGuiUpdate()  override;
	void Update()       override;
	void DrawLit()      override;
	void DrawUnLit()    override;
	void DrawDebug()    override;
	void Init()         override;

	void Collider(const std::shared_ptr<KdModelWork> _objName);

	//�������Ăق����ꏊ
	virtual KdCollider::SphereInfo GetHitPointTypeSphere()
	{
		KdCollider::SphereInfo m;
		return m;
	}

	//�Q�[���V�[���̃A�h���X�����炤
	void SetScene(GameScene* _scene) { m_scene = _scene; }

	void AddStageObject(std::shared_ptr<Stage>& _stageObj) { m_stageList.push_back(_stageObj); }

	virtual bool GetSelect() { return m_selectFlg = false; }

	json11::Json JsonLoad()
	{
		std::ifstream ifs("Asset/Data/StageData.json");
		if (ifs.fail()) { assert(0 && "Json�t�@�C���̃p�X���Ⴂ�܂�"); }

		//������Ƃ��đS�ǂݍ���
		std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		//�������JSON����́i�p�[�X�j����
		std::string err;
		json11::Json jsonData = json11::Json::parse(strJson, err);
		if (err.size() > 0) { assert(0 && "�ǂݍ��񂾃t�@�C����Json�ϊ��Ɏ��s"); }
		auto& stageData = jsonData["Stage"].object_items();

		return stageData;
	}

protected:

	//�f�o�b�N�p======================================================================================
	KdDebugWireFrame m_debugWire;
	//================================================================================================

	//�e�X�g
	bool m_selectFlg = false;
	float m_scale = 2.f;

	//���f��
	std::shared_ptr<KdModelWork> m_spModel;

	//���W
	Math::Vector3 m_pos = {};
	//��]
	Math::Vector3 m_ang = {};

	//�g�k�s��
	Math::Matrix m_scaleMat = Math::Matrix::Identity;
	//��]�s��
	Math::Matrix m_rotatMat = Math::Matrix::Identity;
	//�ړ��s��
	Math::Matrix m_transMat = Math::Matrix::Identity;

	//�X�e�[�W�̏��i���|�j
	GameScene* m_scene;

	//�X�e�[�W���X�g
	std::list<std::shared_ptr<Stage>> m_stageList;

	//�����蔻�胊�X�g
	std::list<std::shared_ptr<Stage>> m_hitObjList;
};