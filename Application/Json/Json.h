#pragma once

//=========================================================
//
//=========================================================

class Json
{
public:
	Json() {}
	~Json() {}

	json11::Json JsonLoad(const std::string_view& _jsonName);


private:

};