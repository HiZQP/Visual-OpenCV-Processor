#pragma once
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QLabel>

#include "NodeDataType.h"

class DisplayModel : public QtNodes::NodeDelegateModel {
	Q_OBJECT
public:
	DisplayModel() = default;
	~DisplayModel() override = default;
	// 明确禁止拷贝，允许移动
	DisplayModel(const DisplayModel&) = delete;
	DisplayModel& operator=(const DisplayModel&) = delete;
	DisplayModel(DisplayModel&&) = default;
	DisplayModel& operator=(DisplayModel&&) = default;

	unsigned int nPorts(QtNodes::PortType portType) const override {
		if (portType == QtNodes::PortType::In)
			return 1;
		else
			return 0;
	}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override { return nullptr; }
};

class TextDisplayModel : public DisplayModel {
	Q_OBJECT
private:
	std::unique_ptr<QLabel> _text;

public:
	TextDisplayModel();
	~TextDisplayModel() override = default;

	QString caption() const override { return "文本显示"; }
	QString name() const override { return "文本显示"; }

	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	QWidget* embeddedWidget() override { return _text.get(); }
};

class ImageDisplayModel : public DisplayModel {
	Q_OBJECT
private:
	std::unique_ptr<QWidget> _widget;
	QLabel* _size;
	QLabel* _imageLabel;
public:
	ImageDisplayModel();
	~ImageDisplayModel() override {}
	QString caption() const override { return "图像显示"; }
	QString name() const override { return "图像显示"; }
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	QWidget* embeddedWidget() override { return _widget.get(); }
};