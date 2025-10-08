#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <memory>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

class MorphologyModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	std::unique_ptr<QWidget> _widget;
	QComboBox* _morphType;
	QSpinBox* _kernelSize;
	QComboBox* _shapeType;

	cv::Mat _originalImage;
	cv::Mat _morphImage;
	void calculate();
public:
	MorphologyModel();
	virtual ~MorphologyModel() override = default;

	MorphologyModel(const MorphologyModel&) = delete;
	MorphologyModel& operator=(const MorphologyModel&) = delete;
	MorphologyModel(MorphologyModel&&) = default;
	MorphologyModel& operator=(MorphologyModel&&) = default;

	QString caption() const override { return "图像形态学操作组件"; }
	QString name() const override { return "形态学操作"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget.get(); }
};

