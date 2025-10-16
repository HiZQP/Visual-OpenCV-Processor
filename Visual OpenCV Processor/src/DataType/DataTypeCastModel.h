#pragma once
#include <opencv2/opencv.hpp>

#include <QtNodes/NodeDelegateModel>

#include <QPushButton>

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

class NumToPointModel : public QtNodes::NodeDelegateModel {
	Q_OBJECT
private:
	cv::Point2d _point;
public:
	QString caption() const override { return QStringLiteral("数字->点"); }
	QString name() const override { return QStringLiteral("数字->点"); }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return nullptr; }
};

class PointToNumModel : public QtNodes::NodeDelegateModel {
	Q_OBJECT
private:
	double _numberX;
	double _numberY;
public:
	QString caption() const override { return QStringLiteral("点->数字"); }
	QString name() const override { return QStringLiteral("点->数字"); }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return nullptr; }
};

class PointToPointsModel : public QtNodes::NodeDelegateModel {
	Q_OBJECT
private:
	QWidget* _widget;
	QPushButton* _addPointButton;
	unsigned int pointInPortNum = 0;
	std::vector<cv::Point2d> _points;
public:
	PointToPointsModel();
	QString caption() const override { return QStringLiteral("点->点集"); }
	QString name() const override { return QStringLiteral("点->点集"); }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
	bool resizable() const override { return true; }
};