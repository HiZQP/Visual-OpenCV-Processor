#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <iostream>
#include <memory>

#include "NodeDataType.h"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class MathOperationDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    ~MathOperationDataModel() = default;

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

    QWidget *embeddedWidget() override { return nullptr; }

protected:
    virtual void compute() = 0;

protected:
    std::weak_ptr<NumberData> _number1;
    std::weak_ptr<NumberData> _number2;

    std::shared_ptr<NumberData> _result;
};
