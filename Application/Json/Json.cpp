#include "Json.h"

json11::Json Json::JsonLoad(const std::string_view& _jsonName)
{
	std::ifstream ifs(_jsonName.data());
	if (ifs.fail()) { assert(0 && "Json�t�@�C���̃p�X���Ⴂ�܂�"); }

	//������Ƃ��đS�ǂݍ���
	std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	//�������JSON����́i�p�[�X�j����
	std::string err;
	json11::Json jsonObj = json11::Json::parse(strJson, err);
	if (err.size() > 0) { assert(0 && "�ǂݍ��񂾃t�@�C����Json�ϊ��Ɏ��s"); }

	return jsonObj;
}