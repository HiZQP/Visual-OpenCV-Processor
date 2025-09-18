#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QSpinBox>
#include <QComboBox>

class ImageGaussianBlurModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _kernelSize;
	QComboBox* _borderType;

	cv::Mat _originalImage;
	cv::Mat _blurredImage;
	void calculate();
public:
	ImageGaussianBlurModel();
	virtual ~ImageGaussianBlurModel() override {}
	QString caption() const override { return "图像高斯模糊组件"; }
	QString name() const override { return "高斯模糊"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

