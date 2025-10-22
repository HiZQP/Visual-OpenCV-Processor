#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_HSVColorSegmentationNode.h"

#include <QSpinBox>

class HSVColorSegmentationNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::HSVColorSegmentationNode _ui;
	QWidget* _widget;

	cv::Mat _originalImage;
	cv::Mat _outputImage;
	void calculate();
public:
	HSVColorSegmentationNode();
	virtual ~HSVColorSegmentationNode() override {}
	QString caption() const override { return "HSV颜色分割组件"; }
	QString name() const override { return "HSV颜色分割"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

