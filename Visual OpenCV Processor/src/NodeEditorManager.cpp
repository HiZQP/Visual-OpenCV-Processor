#include "NodeEditorManager.h"

#include "OpenCVNodes/OpenCVNodes.h"
#include "ToolNodes/ToolNodes.h"
#include "DataNodeModel.h"

void NodeEditorManager::registerParamType(RegistryItemCreator creator)
{
		_registry->registerModel(std::move(creator), "__HIDDEN__");
}

void NodeEditorManager::createNode(const QString& paramId)
{
	auto id = _dataFlowGraphModel->addNode(paramId);
	_dataFlowGraphModel->setNodeData(id, QtNodes::NodeRole::Position, QPointF(0, 0));
}

std::shared_ptr<QtNodes::NodeDelegateModelRegistry> NodeEditorManager::registerDataModels()
{
	auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
	// 在这里注册自定义的数据模型
	ret->registerModel<ImageLoaderModel>("工具");
	ret->registerModel<CameraReadModel>("工具");
	ret->registerModel<CaptureTriggerModel>("工具");
	ret->registerModel<ImageSaveModel>("工具");
	ret->registerModel<ColorPicker>("工具");

	ret->registerModel<ResizeNode>("图像操作");
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
	ret->registerModel<MorphologyModel>("图像操作");
	ret->registerModel<WarpPerspectiveModel>("图像操作");
	ret->registerModel<DrawLineModel>("图像操作");
	ret->registerModel<DrawRectangleModel>("图像操作");
	ret->registerModel<DrawCircleModel>("图像操作");
	ret->registerModel<DrawTextModel>("图像操作");
	ret->registerModel<HoughLinesNode>("图像操作");
	ret->registerModel<DrawLinesNode>("图像操作");
	ret->registerModel<MatCreateNode>("图像操作");

	ret->registerModel<BitwiseAnd>("数学");
	ret->registerModel<BitwiseOr>("数学");
	ret->registerModel<AddWeightedNode>("数学");

	ret->registerModel<TextDisplayModel>("显示");
	ret->registerModel<ImageDisplayModel>("显示");

	ret->registerModel<StrToNumModel>("数据类型转换");
	ret->registerModel<NumToStrModel>("数据类型转换");
	ret->registerModel<NumToPointModel>("数据类型转换");
	ret->registerModel<PointToNumModel>("数据类型转换");
	ret->registerModel<PointToPointsModel>("数据类型转换");

	ret->registerModel<NumberNodeModel>("数据节点");
	ret->registerModel<StringNodeModel>("数据节点");
	return ret;
}

void NodeEditorManager::setStyle() {
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

NodeEditorManager::NodeEditorManager(QLayout* layout, QObject* parent)
{
	setStyle();

	_registry = registerDataModels();
	_dataFlowGraphModel = std::make_shared<QtNodes::DataFlowGraphModel>(_registry);
	_scene = std::make_unique<VCVP::DataFlowGraphicsScene>(*_dataFlowGraphModel);
	_view = std::make_unique<QtNodes::GraphicsView>(_scene.get());
	layout->addWidget(_view.get());

	connect(_scene.get(), &VCVP::DataFlowGraphicsScene::sceneLoaded, _view.get(), &QtNodes::GraphicsView::centerScene);
	connect(_scene.get(), &VCVP::DataFlowGraphicsScene::modified, this, &NodeEditorManager::senceModified);
	/*connect(_scene.get(), &VCVP::DataFlowGraphicsScene::modified, this, [this]() {
		ui.centralWidget->setWindowModified(true);
		}); 
	*/
}

NodeEditorManager::~NodeEditorManager()
{
	_view.reset();     // 先释放view
	_scene.reset();    // 再释放scene
	_dataFlowGraphModel.reset();  // 然后释放graph model
	_registry.reset(); // 最后释放registry
}

