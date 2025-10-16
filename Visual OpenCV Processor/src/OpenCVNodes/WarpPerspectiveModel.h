// WarpPerspectiveNode.h
#pragma once

#include <opencv2/opencv.hpp>
#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QFormLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

class WarpPerspectiveModel : public QtNodes::NodeDelegateModel
{
    Q_OBJECT

public:
    WarpPerspectiveModel();
    virtual ~WarpPerspectiveModel() = default;

    QString caption() const override { return "透视变换"; }
    QString name() const override { return "WarpPerspectiveNode"; }
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
    QWidget* embeddedWidget() override { return _widget; }

    QJsonObject save() const override;
    void load(QJsonObject const& json) override;

private slots:
    void onCalculate();
    void onWidthChanged(int value);
    void onHeightChanged(int value);
    void onInterpolationChanged(int index);
    void onBorderModeChanged(int index);
    void onAutoSizeToggled(bool checked);

private:
    void calculatePerspective();
    void updateOutputSize();
    bool validatePoints();
    void showError(const QString& message);
    void clearError();

private:
    QWidget* _widget;

    // 输入控件
    QSpinBox* _widthSpinBox;
    QSpinBox* _heightSpinBox;
    QCheckBox* _autoSizeCheckBox;
    QComboBox* _interpolationCombo;
    QComboBox* _borderModeCombo;
    QLabel* _statusLabel;

    // 数据
    cv::Mat _inputImage;
    cv::Mat _outputImage;
    std::vector<cv::Point2f> _srcPoints;
    std::vector<cv::Point2f> _dstPoints;

    // 状态
    bool _hasValidInput;
    bool _hasValidSrcPoints;
    bool _hasValidDstPoints;
};
