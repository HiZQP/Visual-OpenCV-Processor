#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_ThresholdNode.h"

class ThresholdNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::ThresholdNode _ui;
	QWidget* _widget;

	cv::Mat _originalImage;
	cv::Mat _outputImage;
	void calculate();
public:
	ThresholdNode();
	virtual ~ThresholdNode() override {}
	QString caption() const override { return "图像基本阈值化组件"; }
	QString name() const override { return "基本阈值化"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

