#pragma once
#include "ParamTypeBase.h"
#include <QLineEdit>
#include <QDoubleValidator>

class NumberParam : public ParamTypeBase
{
private:
	double _value;
public:
	NumberParam() : _value(0.0) {}
	NumberParam(double value) : _value(value) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Number", "数字" }; }
	QWidget* createWidget() const override {
		auto lineEdit = new QLineEdit();
		lineEdit->setText(QString::number(_value));
		lineEdit->setValidator(new QDoubleValidator(lineEdit));
		return lineEdit;
	}
	double getValue() const { return _value; }
	void setValue(double value) { _value = value; }
};