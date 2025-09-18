#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QLabel>


class ImageResizeModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QLineEdit* _width;
	QLineEdit* _height;
	QComboBox* _interpolation;

	cv::Mat _originalImage;
	cv::Mat _resizedImage;
	void calculate();
public:
	ImageResizeModel();
	virtual ~ImageResizeModel() override {}

	QString caption() const override { return QUTF8("缩放"); }
	QString name() const override { return QUTF8("图像缩放组件"); }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

