#include "DataTypeCastModel.h"
#include "NodeDataType.h"

#include <QVBoxLayout>

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
		_string = QString::number(numData->get());
		Q_EMIT dataUpdated(0);
	}
	else {
		_string.clear();
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> NumToStrModel::outData(QtNodes::PortIndex port)
{
	if (_string.isEmpty()) {
		return nullptr;
	}
	return std::make_shared<StringData>(_string);
}

unsigned int NumToPointModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 2;
	else
		return 1;
}

QtNodes::NodeDataType NumToPointModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		if(portIndex == 0)
			return QtNodes::NodeDataType{ "Number", "X" };
		else
			return QtNodes::NodeDataType{ "Number", "Y" };
	else
		return QtNodes::NodeDataType{ "Point", "point" };
}

void NumToPointModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto numData = std::dynamic_pointer_cast<NumberData>(nodeData);
		if (portIndex == 0) {
			_point.x = numData->get();
		}
		else if (portIndex == 1) {
			_point.y = numData->get();
		}
		Q_EMIT dataUpdated(0);
	}
	else {
		if (portIndex == 0) {
			_point.x = 0.0;
		}
		else if (portIndex == 1) {
			_point.y = 0.0;
		}
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> NumToPointModel::outData(QtNodes::PortIndex port)
{
	return std::make_shared<PointData>(_point);
}

unsigned int PointToNumModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 2;
}

QtNodes::NodeDataType PointToNumModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Point", "point" };
	else {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Number", "X" };
		else
			return QtNodes::NodeDataType{ "Number", "Y" };
	}
}

void PointToNumModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto pointData = std::dynamic_pointer_cast<PointData>(nodeData);
		_numberX = pointData->get().x;
		_numberY = pointData->get().y;
		Q_EMIT dataUpdated(0);
		Q_EMIT dataUpdated(1);
	}
	else {
		_numberX = 0.0;
		_numberY = 0.0;
		Q_EMIT dataUpdated(0);
		Q_EMIT dataUpdated(1);
	}
}

std::shared_ptr<QtNodes::NodeData> PointToNumModel::outData(QtNodes::PortIndex port)
{
	if (port == 0)
		return std::make_shared<NumberData>(_numberX);
	else
		return std::make_shared<NumberData>(_numberY);
}

PointToPointsModel::PointToPointsModel()
{
	_widget = new QWidget();
	_addPointButton = new QPushButton();
	_addPointButton->setText("+");
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setAlignment(Qt::AlignTop);
	layout->addWidget(_addPointButton);
	_widget->setLayout(layout);

	connect(_addPointButton, &QPushButton::clicked, [this]() {
		pointInPortNum++;
		_points.push_back(cv::Point2d(0.0, 0.0));
		static QSize widgetSize(50, 30);
		_widget->setMinimumSize(widgetSize.width(), widgetSize.height() + pointInPortNum * 30);
		Q_EMIT portsAboutToBeDeleted(QtNodes::PortType::In, 0, pointInPortNum - 1);
		Q_EMIT portsDeleted();
		});
}

unsigned int PointToPointsModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return pointInPortNum;
	else
		return 1;
}

QtNodes::NodeDataType PointToPointsModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Point", "点" };
	else
		return QtNodes::NodeDataType{ "Points", "点集" };
}

void PointToPointsModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex >= 0) {
			auto pointData = std::dynamic_pointer_cast<PointData>(nodeData);
			_points[portIndex] = pointData->get();
			Q_EMIT dataUpdated(0);
		}
	}
}

std::shared_ptr<QtNodes::NodeData> PointToPointsModel::outData(QtNodes::PortIndex port)
{
	return std::make_shared<PointsData>(_points);
}
