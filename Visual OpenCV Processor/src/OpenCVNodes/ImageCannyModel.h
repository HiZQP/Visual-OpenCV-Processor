#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

class ImageCannyModel : public QtNodes::NodeDelegateModel 
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _threshold1;
	QSpinBox* _threshold2;
	QCheckBox* _L2gradient;

	cv::Mat _originalImage;
	cv::Mat _cannyImage;
	void calculate();
public:
	ImageCannyModel();
	virtual ~ImageCannyModel() override {}
	QString caption() const override { return "图像Canny边缘检测组件"; }
	QString name() const override { return "Canny边缘检测"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

