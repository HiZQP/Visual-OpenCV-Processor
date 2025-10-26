#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_HoughCirclesNode.h"

class HoughCirclesNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::HoughCirclesNode _ui;
	QWidget* _widget;

	cv::Mat _inputImage;
	std::vector<cv::Vec4f> _circles;
	void calculate();
public:
	HoughCirclesNode();
	QString caption() const override { return "霍夫圆检测组件"; }
	QString name() const override { return "霍夫圆检测"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

