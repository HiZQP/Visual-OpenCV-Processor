#pragma once
#include <QtNodes/NodeDelegateModel>
#include "NodeDataType.h"

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLineEdit>

class DataNodeModel : public QtNodes::NodeDelegateModel {
	Q_OBJECT
public:
	unsigned int nPorts(QtNodes::PortType portType) const override {
		if (portType == QtNodes::PortType::Out)
			return 1;
		else
			return 0;
	}
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
};

class NumberNodeModel : public DataNodeModel {
	Q_OBJECT
private:
	QDoubleSpinBox* _spinBox;

	double _number;
	void setNumber(double number) { _number = number; Q_EMIT dataUpdated(0); }
public:
	NumberNodeModel() : _number(0.0) {
		_spinBox = new QDoubleSpinBox();
		_spinBox->setMinimum(-1e+10);
		_spinBox->setMaximum(1e+10);
		_spinBox->setDecimals(4);
		_spinBox->setSingleStep(0.1);
		connect(_spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &NumberNodeModel::setNumber);
	}
	~NumberNodeModel() = default;
	QString caption() const override { return QStringLiteral("数字"); }
	QString name() const override { return QStringLiteral("数字"); }
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override {
		if (portType == QtNodes::PortType::Out)
			return QtNodes::NodeDataType{ "Number", "数字" };
	}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override {
		return std::make_shared<NumberData>(_number);
	}
	QWidget* embeddedWidget() override { return _spinBox; }
};

class StringNodeModel : public DataNodeModel {
	Q_OBJECT
private:
	QLineEdit* _lineEdit;
	QString _string;
	void setString(const QString& str) { _string = str; Q_EMIT dataUpdated(0); }
public:
	StringNodeModel() {
		_lineEdit = new QLineEdit();
		connect(_lineEdit, &QLineEdit::textChanged, this, &StringNodeModel::setString);
	}
	~StringNodeModel() = default;
	QString caption() const override { return QStringLiteral("字符串"); }
	QString name() const override { return QStringLiteral("字符串"); }
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override {
		if (portType == QtNodes::PortType::Out)
			return QtNodes::NodeDataType{ "String", "字符串" };
	}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override {
		return std::make_shared<StringData>(_string);
	}
	QWidget* embeddedWidget() override { return _lineEdit; }
};

class PointNodeModel : public DataNodeModel {
	Q_OBJECT
private:
	QDoubleSpinBox* _spinBoxX;
	QDoubleSpinBox* _spinBoxY;
	double _x;
	double _y;
	void setX(double x) { _x = x; Q_EMIT dataUpdated(0); }
	void setY(double y) { _y = y; Q_EMIT dataUpdated(0); }
public:
	PointNodeModel() : _x(0.0), _y(0.0) {
		_spinBoxX = new QDoubleSpinBox();
		_spinBoxX->setMinimum(-1e+10);
		_spinBoxX->setMaximum(1e+10);
		_spinBoxX->setDecimals(4);
		_spinBoxX->setSingleStep(0.1);
		connect(_spinBoxX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PointNodeModel::setX);
		_spinBoxY = new QDoubleSpinBox();
		_spinBoxY->setMinimum(-1e+10);
		_spinBoxY->setMaximum(1e+10);
		_spinBoxY->setDecimals(4);
		_spinBoxY->setSingleStep(0.1);
		connect(_spinBoxY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PointNodeModel::setY);
	}
	~PointNodeModel() = default;
	QString caption() const override { return QStringLiteral("点"); }
	QString name() const override { return QStringLiteral("点"); }
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override {
		if (portType == QtNodes::PortType::Out)
			return QtNodes::NodeDataType{ "Point", "点" };
	}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override {
		return std::make_shared<ScalarData>(cv::Scalar(_x, _y));
	}
	QWidget* embeddedWidget() override {
		QWidget* widget = new QWidget();
		QHBoxLayout* layout = new QHBoxLayout();
		layout->addWidget(_spinBoxX);
		layout->addWidget(_spinBoxY);
		widget->setLayout(layout);
		return widget;
	}
};