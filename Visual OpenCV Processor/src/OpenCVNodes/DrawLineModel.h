#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

class DrawLineModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _x1SpinBox;
	QSpinBox* _y1SpinBox;
	QSpinBox* _x2SpinBox;
	QSpinBox* _y2SpinBox;
	QSpinBox* _thicknessSpinBox;
	QComboBox* _lineTypeComboBox;
	QSpinBox* _colorRSpinBox;
	QSpinBox* _colorGSpinBox;
	QSpinBox* _colorBSpinBox;

	cv::Mat _originalImage;
	cv::Mat _outputImage;
	void calculate();
public:
	DrawLineModel();
	virtual ~DrawLineModel() override {}
	QString caption() const override { return "直线绘制组件"; }
	QString name() const override { return "绘制直线"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

