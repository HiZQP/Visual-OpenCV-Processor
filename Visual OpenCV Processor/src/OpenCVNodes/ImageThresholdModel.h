#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

class ImageThresholdModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _thresholdSpinBox;
	QSpinBox* _maxValueSpinBox;
	QComboBox* _typeComboBox;


	cv::Mat _originalImage;
	cv::Mat _outputImage;
	void calculate();
public:
	ImageThresholdModel();
	virtual ~ImageThresholdModel() override {}
	QString caption() const override { return "图像二值化组件"; }
	QString name() const override { return "二值化"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

