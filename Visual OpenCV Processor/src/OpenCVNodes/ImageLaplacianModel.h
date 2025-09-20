#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

class ImageLaplacianModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _ksize;
	QComboBox* _scale;
	QComboBox* _delta;
	QComboBox* _ddepth;
	cv::Mat _originalImage;
	cv::Mat _laplacianImage;
	void calculate();
public:
	ImageLaplacianModel();
	virtual ~ImageLaplacianModel() override {}
	QString caption() const override { return "图像Laplacian边缘检测组件"; }
	QString name() const override { return "Laplacian边缘检测"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

