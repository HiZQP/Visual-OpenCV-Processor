#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_SobelEdgeDetectionNode.h"

class SobelEdgeDetectionNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::SobelEdgeDetectionNode _ui;
	QWidget* _widget;

	cv::Mat _originalImage;
	cv::Mat _outputImage;
	void calculate();
public:
	SobelEdgeDetectionNode();
	virtual ~SobelEdgeDetectionNode() override {}
	QString caption() const override { return "图像Sobel边缘检测组件"; }
	QString name() const override { return "Sobel边缘检测"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

