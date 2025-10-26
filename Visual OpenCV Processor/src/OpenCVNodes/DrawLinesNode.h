#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class DrawLinesNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QComboBox* _thickness;
	QComboBox* _lineType;
	QPushButton* _colorPickerButton;
	QLabel* _infoLabel;

	cv::Scalar _color;
	cv::Mat _originalImage;
	cv::Mat _outputImage;
	std::vector<cv::Vec4f> _lines;
	void calculate();
	void openColorDialog();
public:
	DrawLinesNode();
	virtual ~DrawLinesNode() override = default;
	QString caption() const override { return "线段集绘制组件"; }
	QString name() const override { return "线段集绘制"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

