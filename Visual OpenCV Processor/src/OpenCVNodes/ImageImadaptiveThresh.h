#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>

class ImageImadaptiveThresh : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
 private:
	QWidget* _widget;
	QComboBox* _method;
	QComboBox* _type;
	QSpinBox* _blockSize;
	QSpinBox* _C;
	QLabel* _info;

	cv::Mat _originalImage;
	cv::Mat _threshImage;
	void calculate();
 public:
	ImageImadaptiveThresh();
	virtual ~ImageImadaptiveThresh() override {}
	QString caption() const override { return "图像自适应阈值组件"; }
	QString name() const override { return "自适应阈值"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

