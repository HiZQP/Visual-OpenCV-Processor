#pragma once

#include <QtNodes/NodeDelegateModel>

class StrToNumModel : public QtNodes::NodeDelegateModel{
	Q_OBJECT
private:
	double _number;
public:
	QString caption() const override { return QStringLiteral("字符串->数字"); }
	QString name() const override { return QStringLiteral("字符串->数字"); }

	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return nullptr; }
};

class NumToStrModel : public QtNodes::NodeDelegateModel {
	Q_OBJECT
private:
	QString _string;
public:
	QString caption() const override { return QStringLiteral("数字->字符串"); }
	QString name() const override { return QStringLiteral("数字->字符串"); }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return nullptr; }
};