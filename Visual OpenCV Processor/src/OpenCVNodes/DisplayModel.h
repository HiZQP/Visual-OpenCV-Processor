#pragma once
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>

#include "NodeDataType.h"

class TextDisplayModel : public QtNodes::NodeDelegateModel {
	Q_OBJECT
private:
	QLineEdit* m_lineEdit;

public:
	TextDisplayModel();
	~TextDisplayModel() override;

	QString caption() const override { return QUTF8("文本显示"); }
	QString name() const override { return QUTF8("文本显示组件"); }

	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override { return nullptr; }

	QWidget* embeddedWidget() override { return m_lineEdit; }
};

