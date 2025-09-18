#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

class ImageImbilateralFilter : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _d;
	QDoubleSpinBox* _sigmaColor;
	QDoubleSpinBox* _sigmaSpace;
	cv::Mat _originalImage;
	cv::Mat _blurredImage;
	void calculate();
public:
	ImageImbilateralFilter();
	virtual ~ImageImbilateralFilter() override {}
	QString caption() const override { return "图像双边滤波组件"; }
	QString name() const override { return "双边滤波"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

