#include "DataTypeCastModel.h"
#include "NodeDataType.h"

unsigned int StrToNumModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType StrToNumModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "String", "str" };
	else
		return QtNodes::NodeDataType{ "Number", "num" };
}

void StrToNumModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto strData = std::dynamic_pointer_cast<StringData>(nodeData);
		if (strData) {
			bool ok;
			_number = strData->get().toDouble(&ok);
			if (!ok) {
				_number = 0.0;
			}
			Q_EMIT dataUpdated(0);
		}
	}
	else {
		_number = 0.0;
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> StrToNumModel::outData(QtNodes::PortIndex port)
{
	return std::make_shared<NumberData>(_number);
}

unsigned int NumToStrModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType NumToStrModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Number", "num" };
	else
		return QtNodes::NodeDataType{ "String", "str" };
}

void NumToStrModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto numData = std::dynamic_pointer_cast<NumberData>(nodeData);
		if (numData) {
			_string = QString::number(numData->get());
			Q_EMIT dataUpdated(0);
		}
	}
	else {
		_string = QUTF8("");
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> NumToStrModel::outData(QtNodes::PortIndex port)
{
	return std::make_shared<StringData>(_string);
}
