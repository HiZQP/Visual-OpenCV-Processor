#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

class ImageImthresholeModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _thresh;
	QSpinBox* _maxval;
	QComboBox* _type;
	cv::Mat _originalImage;
	cv::Mat _threshImage;
	void calculate();
public:
	ImageImthresholeModel();
	virtual ~ImageImthresholeModel() override {}
	QString caption() const override { return "图像阈值处理组件"; }
	QString name() const override { return "阈值处理"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

