#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include "MathOperationDataModel.hpp"

#include "NodeDataType.h"

class MultiplicationModel : public MathOperationDataModel
{
public:
    virtual ~MultiplicationModel() {}

public:
    QString caption() const override { return QStringLiteral("乘法"); }

    QString name() const override { return QStringLiteral("乘法"); }

private:
    void compute() override
    {
        PortIndex const outPortIndex = 0;

        auto n1 = _number1.lock();
        auto n2 = _number2.lock();

        if (n1 && n2) {
            _result = std::make_shared<NumberData>(n1->get() * n2->get());
        } else {
            _result.reset();
        }

        Q_EMIT dataUpdated(outPortIndex);
    }
};
