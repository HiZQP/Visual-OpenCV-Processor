#pragma once

#include <opencv2/opencv.hpp>

#include <QtNodes/NodeDelegateModel>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QComboBox>

class ImageSaveModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QPushButton* _button;
	QLabel* _imageLabel;
	QLineEdit* _label;
	QLineEdit* _name;
	QComboBox* _format;

	QString _lastValidPath;
	bool _pathExists = false;
public:
	ImageSaveModel();
	QString caption() const override { return QStringLiteral("图像输出组件"); }
	QString name() const override { return QStringLiteral("输出至文件"); }

	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override { return nullptr; }
	QWidget* embeddedWidget() override { return _widget; }
};

