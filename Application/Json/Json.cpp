#include "Json.h"

json11::Json Json::JsonLoad(const std::string_view& _jsonName)
{
	std::ifstream ifs(_jsonName.data());
	if (ifs.fail()) { assert(0 && "Jsonファイルのパスが違います"); }

	//文字列として全読み込み
	std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	//文字列のJSONを解析（パース）する
	std::string err;
	json11::Json jsonObj = json11::Json::parse(strJson, err);
	if (err.size() > 0) { assert(0 && "読み込んだファイルのJson変換に失敗"); }

	return jsonObj;
}