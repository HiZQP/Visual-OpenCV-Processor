#include "WarpPerspectiveModel.h"
#include "NodeDataType.h"

#include <QDebug>
#include <QMessageBox>

WarpPerspectiveModel::WarpPerspectiveModel()
    : _hasValidInput(false)
    , _hasValidSrcPoints(false)
    , _hasValidDstPoints(false)
{
    // 创建主控件
    _widget = new QWidget();
    auto mainLayout = new QVBoxLayout(_widget);

    // 输出尺寸设置组
    auto sizeGroup = new QGroupBox("输出尺寸");
    auto sizeLayout = new QFormLayout(sizeGroup);

    _widthSpinBox = new QSpinBox();
    _widthSpinBox->setRange(1, 10000);
    _widthSpinBox->setValue(640);
    _widthSpinBox->setEnabled(false);

    _heightSpinBox = new QSpinBox();
    _heightSpinBox->setRange(1, 10000);
    _heightSpinBox->setValue(480);
    _heightSpinBox->setEnabled(false);

    _autoSizeCheckBox = new QCheckBox("自动计算尺寸");
    _autoSizeCheckBox->setChecked(true);

    sizeLayout->addRow(_autoSizeCheckBox);
    sizeLayout->addRow("宽度:", _widthSpinBox);
    sizeLayout->addRow("高度:", _heightSpinBox);

    // 变换参数组
    auto paramGroup = new QGroupBox("变换参数");
    auto paramLayout = new QFormLayout(paramGroup);

    _interpolationCombo = new QComboBox();
    _interpolationCombo->addItem("最近邻", cv::INTER_NEAREST);
    _interpolationCombo->addItem("线性", cv::INTER_LINEAR);
    _interpolationCombo->addItem("立方", cv::INTER_CUBIC);
    _interpolationCombo->addItem("兰索斯", cv::INTER_LANCZOS4);
    _interpolationCombo->setCurrentIndex(1); // 默认线性

    _borderModeCombo = new QComboBox();
    _borderModeCombo->addItem("常量边界", cv::BORDER_CONSTANT);
    _borderModeCombo->addItem("反射边界", cv::BORDER_REFLECT);
    _borderModeCombo->addItem("边缘复制", cv::BORDER_REPLICATE);
    _borderModeCombo->setCurrentIndex(0); // 默认常量边界

    paramLayout->addRow("插值方法:", _interpolationCombo);
    paramLayout->addRow("边界模式:", _borderModeCombo);

    // 状态显示
    _statusLabel = new QLabel("等待输入数据...");
    _statusLabel->setStyleSheet("QLabel { color: gray; }");

    // 布局组装
    mainLayout->addWidget(sizeGroup);
    mainLayout->addWidget(paramGroup);
    mainLayout->addWidget(_statusLabel);
    mainLayout->addStretch();

    // 连接信号槽
    connect(_autoSizeCheckBox, &QCheckBox::toggled, this, &WarpPerspectiveModel::onAutoSizeToggled);
    connect(_widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &WarpPerspectiveModel::onWidthChanged);
    connect(_heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &WarpPerspectiveModel::onHeightChanged);
    connect(_interpolationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &WarpPerspectiveModel::onInterpolationChanged);
    connect(_borderModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &WarpPerspectiveModel::onBorderModeChanged);
}

unsigned int WarpPerspectiveModel::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In)
        return 3; // 图像输入、源点集、目标点集
    else
        return 1; // 变换后图像输出
}

QtNodes::NodeDataType WarpPerspectiveModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
        switch (portIndex) {
        case 0: return QtNodes::NodeDataType{ "Image", "输入图像" };
        case 1: return QtNodes::NodeDataType{ "Points", "源点集" };
        case 2: return QtNodes::NodeDataType{ "Points", "目标点集" };
        default: return QtNodes::NodeDataType{ "", "" };
        }
    }
    else {
        return QtNodes::NodeDataType{ "Image", "输出图像" };
    }
}

void WarpPerspectiveModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
    if (!nodeData) {
        // 数据断开连接
        switch (portIndex) {
        case 0:
            _inputImage.release();
            _hasValidInput = false;
            break;
        case 1:
            _srcPoints.clear();
            _hasValidSrcPoints = false;
            break;
        case 2:
            _dstPoints.clear();
            _hasValidDstPoints = false;
            break;
        }
    }
    else {
        // 数据连接
        switch (portIndex) {
        case 0: {
            auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            if (imageData) {
                _inputImage = imageData->get();
                _hasValidInput = !_inputImage.empty();
                clearError();
            }
            break;
        }
        case 1: {
            auto pointsData = std::dynamic_pointer_cast<PointsData>(nodeData);
            if (pointsData) {
                auto points = pointsData->get();
                // 转换为 Point2f
                _srcPoints.clear();
                for (const auto& pt : points) {
                    _srcPoints.push_back(cv::Point2f(static_cast<float>(pt.x), static_cast<float>(pt.y)));
                }
                _hasValidSrcPoints = _srcPoints.size() >= 4;
                clearError();
            }
            break;
        }
        case 2: {
            auto pointsData = std::dynamic_pointer_cast<PointsData>(nodeData);
            if (pointsData) {
                auto points = pointsData->get();
                // 转换为 Point2f
                _dstPoints.clear();
                for (const auto& pt : points) {
                    _dstPoints.push_back(cv::Point2f(static_cast<float>(pt.x), static_cast<float>(pt.y)));
                }
                _hasValidDstPoints = _dstPoints.size() >= 4;

                // 如果启用自动尺寸，根据目标点集计算输出尺寸
                if (_autoSizeCheckBox->isChecked()) {
                    updateOutputSize();
                }
                clearError();
            }
            break;
        }
        }
    }

    // 更新按钮状态
    bool canCalculate = _hasValidInput && _hasValidSrcPoints && _hasValidDstPoints;

    if (canCalculate) {
        _statusLabel->setText("数据就绪");
        _statusLabel->setStyleSheet("QLabel { color: green; }");
    }
    else {
        QString status = "等待数据: ";
        if (!_hasValidInput) status += "图像 ";
        if (!_hasValidSrcPoints) status += "源点集 ";
        if (!_hasValidDstPoints) status += "目标点集";
        _statusLabel->setText(status);
        _statusLabel->setStyleSheet("QLabel { color: orange; }");
    }

     //自动计算（可选）
     if (canCalculate) {
         calculatePerspective();
     }
}

std::shared_ptr<QtNodes::NodeData> WarpPerspectiveModel::outData(QtNodes::PortIndex port)
{
    if (!_outputImage.empty()) {
        return std::make_shared<ImageData>(_outputImage);
    }
    return nullptr;
}

void WarpPerspectiveModel::onCalculate()
{
    if (validatePoints()) {
        calculatePerspective();
    }
}

void WarpPerspectiveModel::onAutoSizeToggled(bool checked)
{
    _widthSpinBox->setEnabled(!checked);
    _heightSpinBox->setEnabled(!checked);

    if (checked && _hasValidDstPoints) {
        updateOutputSize();
    }
}

void WarpPerspectiveModel::onWidthChanged(int value)
{
    Q_UNUSED(value)
        // 尺寸改变时重新计算
        if (_hasValidInput && _hasValidSrcPoints && _hasValidDstPoints) {
            calculatePerspective();
        }
}

void WarpPerspectiveModel::onHeightChanged(int value)
{
    Q_UNUSED(value)
        // 尺寸改变时重新计算
        if (_hasValidInput && _hasValidSrcPoints && _hasValidDstPoints) {
            calculatePerspective();
        }
}

void WarpPerspectiveModel::onInterpolationChanged(int index)
{
    Q_UNUSED(index)
        if (_hasValidInput && _hasValidSrcPoints && _hasValidDstPoints) {
            calculatePerspective();
        }
}

void WarpPerspectiveModel::onBorderModeChanged(int index)
{
    Q_UNUSED(index)
        if (_hasValidInput && _hasValidSrcPoints && _hasValidDstPoints) {
            calculatePerspective();
        }
}

void WarpPerspectiveModel::calculatePerspective()
{
    try {
        // 验证数据
        if (!validatePoints()) {
            return;
        }

        // 获取参数
        int interpolation = _interpolationCombo->currentData().toInt();
        int borderMode = _borderModeCombo->currentData().toInt();
        int width = _widthSpinBox->value();
        int height = _heightSpinBox->value();

        // 计算透视变换矩阵
        cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(_srcPoints, _dstPoints);

        // 执行透视变换
        cv::warpPerspective(_inputImage, _outputImage, perspectiveMatrix,
            cv::Size(width, height), interpolation, borderMode);

        // 更新状态
        _statusLabel->setText("变换完成");
        _statusLabel->setStyleSheet("QLabel { color: green; }");

        // 通知输出端口数据更新
        Q_EMIT dataUpdated(0);

    }
    catch (const cv::Exception& e) {
        showError(QString("OpenCV错误: %1").arg(e.what()));
    }
    catch (const std::exception& e) {
        showError(QString("计算错误: %1").arg(e.what()));
    }
}

void WarpPerspectiveModel::updateOutputSize()
{
    if (_dstPoints.size() >= 4) {
        // 计算目标点集的边界框
        float minX = std::min({ _dstPoints[0].x, _dstPoints[1].x, _dstPoints[2].x, _dstPoints[3].x });
        float maxX = std::max({ _dstPoints[0].x, _dstPoints[1].x, _dstPoints[2].x, _dstPoints[3].x });
        float minY = std::min({ _dstPoints[0].y, _dstPoints[1].y, _dstPoints[2].y, _dstPoints[3].y });
        float maxY = std::max({ _dstPoints[0].y, _dstPoints[1].y, _dstPoints[2].y, _dstPoints[3].y });

        int width = static_cast<int>(maxX - minX);
        int height = static_cast<int>(maxY - minY);

        _widthSpinBox->setValue(std::max(1, width));
        _heightSpinBox->setValue(std::max(1, height));
    }
}

bool WarpPerspectiveModel::validatePoints()
{
    if (_srcPoints.size() < 4) {
        showError("源点集需要至少4个点");
        return false;
    }

    if (_dstPoints.size() < 4) {
        showError("目标点集需要至少4个点");
        return false;
    }

    // 检查点是否有效（没有NaN或无限值）
    for (const auto& pt : _srcPoints) {
        if (std::isnan(pt.x) || std::isnan(pt.y) ||
            std::isinf(pt.x) || std::isinf(pt.y)) {
            showError("源点集中包含无效坐标");
            return false;
        }
    }

    for (const auto& pt : _dstPoints) {
        if (std::isnan(pt.x) || std::isnan(pt.y) ||
            std::isinf(pt.x) || std::isinf(pt.y)) {
            showError("目标点集中包含无效坐标");
            return false;
        }
    }

    return true;
}

void WarpPerspectiveModel::showError(const QString& message)
{
    _statusLabel->setText("错误: " + message);
    _statusLabel->setStyleSheet("QLabel { color: red; }");
    _outputImage.release();
    Q_EMIT dataUpdated(0); // 通知输出数据已清除
}

void WarpPerspectiveModel::clearError()
{
    if (_statusLabel->styleSheet().contains("red")) {
        _statusLabel->setStyleSheet("QLabel { color: gray; }");
        _statusLabel->setText("等待输入数据...");
    }
}

QJsonObject WarpPerspectiveModel::save() const
{
    QJsonObject modelJson;
    modelJson["name"] = name();
    modelJson["width"] = _widthSpinBox->value();
    modelJson["height"] = _heightSpinBox->value();
    modelJson["auto_size"] = _autoSizeCheckBox->isChecked();
    modelJson["interpolation"] = _interpolationCombo->currentIndex();
    modelJson["border_mode"] = _borderModeCombo->currentIndex();
    return modelJson;
}

void WarpPerspectiveModel::load(QJsonObject const& json)
{
    _widthSpinBox->setValue(json["width"].toInt());
    _heightSpinBox->setValue(json["height"].toInt());
    _autoSizeCheckBox->setChecked(json["auto_size"].toBool());
    _interpolationCombo->setCurrentIndex(json["interpolation"].toInt());
    _borderModeCombo->setCurrentIndex(json["border_mode"].toInt());
}
