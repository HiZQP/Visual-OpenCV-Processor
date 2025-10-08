#pragma once
#include <QtNodes/NodeData>
#include <QWidget>

class ParamTypeBase
{
public:
	virtual ~ParamTypeBase() = default;
	virtual QtNodes::NodeDataType type() const = 0;
	virtual QWidget* createWidget() const = 0;
};