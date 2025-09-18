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
	ret->registerModel<ImageLoaderModel>(QUTF8("图像操作"));
	ret->registerModel<ImageSaveModel>(QUTF8("图像操作"));
	ret->registerModel<ImageResizeModel>(QUTF8("图像操作"));
	ret->registerModel<ImageCropModel>(QUTF8("图像操作"));
	ret->registerModel<ImageRotateModel>(QUTF8("图像操作"));
	ret->registerModel<TextDisplayModel>(QUTF8("显示"));
	ret->registerModel<StrToNumModel>(QUTF8("数据类型转换"));
	ret->registerModel<NumToStrModel>(QUTF8("数据类型转换"));

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

