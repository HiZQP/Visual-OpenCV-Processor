#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_ColorSpaceConversionNode.h"

class ColorSpaceConversionNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::ColorSpaceConversionNode _ui;
	QWidget* _widget;

	cv::Mat _inputImage;
	cv::Mat _outputImage;

	void calculate();
	int isConversionCompatible(int conversionCode);
public:
	ColorSpaceConversionNode();
	virtual ~ColorSpaceConversionNode() override {}
	QString caption() const override { return "色彩空间转换组件"; }
	QString name() const override { return "色彩空间转换"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

