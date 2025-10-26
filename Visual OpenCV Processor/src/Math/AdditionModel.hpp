#pragma once

#include "NodeDataType.h"
#include "MathOperationDataModel.hpp"

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class AdditionModel : public MathOperationDataModel
{
public:
    ~AdditionModel() = default;

public:
    // 添加菜单栏中的名称
    QString caption() const override { return QStringLiteral("加法"); }

    // 在布局中显示的名称
    QString name() const override { return QStringLiteral("加法"); }

private:
    void compute() override
    {
        PortIndex const outPortIndex = 0;

        auto n1 = _number1.lock();
        auto n2 = _number2.lock();

        if (n1 && n2) {
            _result = std::make_shared<NumberData>(n1->get() + n2->get());
        } else {
            _result.reset();
        }

        Q_EMIT dataUpdated(outPortIndex);
    }
};
