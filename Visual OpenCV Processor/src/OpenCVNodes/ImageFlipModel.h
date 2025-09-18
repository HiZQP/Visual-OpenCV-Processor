#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include <QComboBox>

class ImageFlipModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QComboBox* _flipCodeComboBox;

	cv::Mat _originalImage;
	cv::Mat _flippedImage;
	void calculate();
public:
	ImageFlipModel();
	virtual ~ImageFlipModel() override {}
	QString caption() const override { return "图像翻转组件"; }
	QString name() const override { return "翻转"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

