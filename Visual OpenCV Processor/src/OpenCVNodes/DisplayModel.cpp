#include "DisplayModel.h"

TextDisplayModel::TextDisplayModel()
{
	m_lineEdit = new QLineEdit;
	m_lineEdit->setReadOnly(true);
	m_lineEdit->setText(QUTF8("无数据"));
}

TextDisplayModel::~TextDisplayModel()
{
	delete m_lineEdit;
}

unsigned int TextDisplayModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 0;
}

QtNodes::NodeDataType TextDisplayModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	return QtNodes::NodeDataType{ "String", "输入" };
}

void TextDisplayModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto strData = std::dynamic_pointer_cast<StringData>(nodeData);
		if (strData) {
			m_lineEdit->setText(strData->get());
			Q_EMIT dataUpdated(0);
		}
	}
	else {
		m_lineEdit->setText(QUTF8("无数据"));
		Q_EMIT dataUpdated(0);
	}
}
