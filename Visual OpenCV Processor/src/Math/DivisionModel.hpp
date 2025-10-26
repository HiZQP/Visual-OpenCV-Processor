#pragma once

#include "NodeDataType.h"
#include "MathOperationDataModel.hpp"

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

class DivisionModel : public MathOperationDataModel
{
public:
    virtual ~DivisionModel() {}

public:
    QString caption() const override { return QStringLiteral("除法"); }

    bool portCaptionVisible(PortType portType, PortIndex portIndex) const override
    {
        Q_UNUSED(portType);
        Q_UNUSED(portIndex);
        return true;
    }

    QString portCaption(PortType portType, PortIndex portIndex) const override
    {
        switch (portType) {
        case PortType::In:
            if (portIndex == 0)
                return QStringLiteral("被除数");
            else if (portIndex == 1)
                return QStringLiteral("除数");

            break;

        case PortType::Out:
            return QStringLiteral("结果");

        default:
            break;
        }
        return QString();
    }

    QString name() const override { return QStringLiteral("除法"); }

private:
    void compute() override
    {
        PortIndex const outPortIndex = 0;

        auto n1 = _number1.lock();
        auto n2 = _number2.lock();

        if (n2 && (n2->get() == 0.0)) {
            _result.reset();
        } else if (n1 && n2) {
            _result = std::make_shared<NumberData>(n1->get() / n2->get());
        } else {
            _result.reset();
        }

        Q_EMIT dataUpdated(outPortIndex);
    }
};
