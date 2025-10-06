#include "VisualOpenCVProcessor.h"
#include <opencv2/opencv.hpp>

#include <QVBoxLayout>
#define QUTF8(s) QString::fromUtf8(s) 

void VisualOpenCVProcessor::setupMainWindowWidget(){
    setStyle();
    m_registry = registerDataModels();
    m_dataFlowGraphModel = std::make_shared<QtNodes::DataFlowGraphModel>(m_registry);
    m_scene = std::make_unique<QtNodes::DataFlowGraphicsScene>(*m_dataFlowGraphModel, ui.centralWidget);
    m_view = std::make_unique<QtNodes::GraphicsView>(m_scene.get());

    QVBoxLayout* vLayout = new QVBoxLayout(ui.centralWidget);
    vLayout->addWidget(m_view.get());
	
}

void VisualOpenCVProcessor::connects() {
    connect(m_scene.get(), &QtNodes::DataFlowGraphicsScene::sceneLoaded, m_view.get(), &QtNodes::GraphicsView::centerScene);
    connect(m_scene.get(), &QtNodes::DataFlowGraphicsScene::modified, this, [this]() {
        ui.centralWidget->setWindowModified(true);
        });
}

std::shared_ptr<QtNodes::NodeDelegateModelRegistry> VisualOpenCVProcessor::registerDataModels()
{
	auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
	// 在这里注册自定义的数据模型
	ret->registerModel<ImageLoaderModel>("工具");
	ret->registerModel<CameraReadModel>("工具");
	ret->registerModel<CaptureTriggerModel>("工具");
	ret->registerModel<ImageSaveModel>("工具");
	ret->registerModel<ColorPicker>("工具");

	ret->registerModel<ImageResizeModel>("图像操作");
	ret->registerModel<ImageCropModel>("图像操作");
	ret->registerModel<ImageRotateModel>("图像操作");
	ret->registerModel<ImageFlipModel>("图像操作");
	ret->registerModel<ImageConvertColorModel>("图像操作");
	ret->registerModel<ImageBlurModel>("图像操作");
	ret->registerModel<ImageGaussianBlurModel>("图像操作");
	ret->registerModel<ImageImmedianBlurModel>("图像操作");
	ret->registerModel<ImageImbilateralFilter>("图像操作");
	ret->registerModel<ImageImadaptiveThresh>("图像操作");
	ret->registerModel<ImageCannyModel>("图像操作");
	ret->registerModel<ImageSobelModel>("图像操作");
	ret->registerModel<ImageLaplacianModel>("图像操作");
	ret->registerModel<ImageHistEqualizeModel>("图像操作");
	ret->registerModel<ImageFindContoursModel>("图像操作");
	ret->registerModel<ImageDrawContoursModel>("图像操作");
	ret->registerModel<ImageThresholdModel>("图像操作");
	ret->registerModel<ImageChannelExtract>("图像操作");
	ret->registerModel<InRange>("图像操作");

	ret->registerModel<DrawLineModel>("图像操作");
	ret->registerModel<DrawRectangleModel>("图像操作");
	ret->registerModel<DrawCircleModel>("图像操作");
	ret->registerModel<DrawTextModel>("图像操作");\

	ret->registerModel<BitwiseAnd>("数学");
	ret->registerModel<BitwiseOr>("数学");

	ret->registerModel<TextDisplayModel>("显示");
	ret->registerModel<ImageDisplayModel>("显示");

	ret->registerModel<StrToNumModel>("数据类型转换");
	ret->registerModel<NumToStrModel>("数据类型转换");
	ret->registerModel<NumToPointModel>("数据类型转换");
	ret->registerModel<PointToNumModel>("数据类型转换");
    return ret;
}

void VisualOpenCVProcessor::setStyle() {
    QtNodes::ConnectionStyle::setConnectionStyle(
        R"(
  {
    "ConnectionStyle": {
      "ConstructionColor": "gray",
      "NormalColor": "black",
      "SelectedColor": "gray",
      "SelectedHaloColor": "deepskyblue",
      "HoveredColor": "deepskyblue",

      "LineWidth": 3.0,
      "ConstructionLineWidth": 2.0,
      "PointDiameter": 10.0,

      "UseDataDefinedColors": true
    }
  }
  )");
}

VisualOpenCVProcessor::VisualOpenCVProcessor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setupMainWindowWidget();
    connects();
}

VisualOpenCVProcessor::~VisualOpenCVProcessor() {
    m_view.reset();     // 先释放view
    m_scene.reset();    // 再释放scene
    m_dataFlowGraphModel.reset();  // 然后释放graph model
    m_registry.reset(); // 最后释放registry
}

