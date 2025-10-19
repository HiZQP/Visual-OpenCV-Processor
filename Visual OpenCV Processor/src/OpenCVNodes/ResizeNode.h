#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_ResizeNode.h"

#include <QSpinBox>
#include <QComboBox>
#include <QLabel>


class ResizeNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::ResizeNode _ui;
	QWidget* _widget;
	bool _ratio;

	cv::Mat _originalImage;
	cv::Mat _resizedImage;
	void calculate();
	void ratio();
public:
	ResizeNode();
	QString caption() const override { return "图像缩放组件"; }
	QString name() const override { return "缩放"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

