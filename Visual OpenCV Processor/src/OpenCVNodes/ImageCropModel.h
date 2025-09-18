#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>

class ImageCropModel :public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QLineEdit* _x;
	QLineEdit* _y;
	QLineEdit* _width;
	QLineEdit* _height;

	cv::Mat _originalImage;
	cv::Mat _croppedImage;
	void calculate();
public:
	ImageCropModel();
	virtual ~ImageCropModel() override {}

	QString caption() const override { return QUTF8("裁剪"); }
	QString name() const override { return QUTF8("图像裁剪组件"); }

	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

