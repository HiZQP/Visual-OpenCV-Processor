#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

class BitwiseAnd : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	cv::Mat _inputImage1;
	cv::Mat _inputImage2;
	cv::Mat _outputImage;
	void calculate();
public:
	virtual ~BitwiseAnd() override {}
	QString caption() const override { return "图像按位与组件"; }
	QString name() const override { return "按位与"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return nullptr; }
};

