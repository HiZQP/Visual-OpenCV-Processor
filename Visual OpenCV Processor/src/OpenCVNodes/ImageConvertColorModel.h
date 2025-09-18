#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QComboBox>
#include <QLabel>

class ImageConvertColorModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QComboBox* _codeBox;
	QLabel* _infoLabel;

	cv::Mat _inputImage;
	cv::Mat _outputImage;

	void calculate();
	int isConversionCompatible(int conversionCode);
public:
	ImageConvertColorModel();
	virtual ~ImageConvertColorModel() override {}
	QString caption() const override { return "图像颜色空间转换组件"; }
	QString name() const override { return "颜色空间转换"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

