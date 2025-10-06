#pragma once
#include "NodeDataType.h"

class ParameterManager
{
private:
	ParameterManager() {}
	~ParameterManager() {}

	std::unordered_map<QString, std::shared_ptr<QtNodes::NodeData>> paramMap;
public:
	static ParameterManager& getInstance();
	ParameterManager(const ParameterManager&) = delete; // ½ûÖ¹¿½±´¹¹Ôìº¯Êý
	ParameterManager& operator=(const ParameterManager&) = delete; // ½ûÖ¹¸³Öµ²Ù×÷·û

	void setParam(const QString& key, std::shared_ptr<QtNodes::NodeData> value);
	std::shared_ptr<QtNodes::NodeData> getParam(const QString& key);

	void addParamNumber();
	void addParamString();
	void addParamImage();
	void addParamContours();
	void addParamScalar();

};

