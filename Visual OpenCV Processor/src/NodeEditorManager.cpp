#include "NodeEditorManager.h"

#include "OpenCVNodes/OpenCVNodes.h"
#include "ToolNodes/ToolNodes.h"
#include "DataNodeModel.h"
#include "Math/MathNodes.h"

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
	ret->registerModel<FileImageLoaderNode>("工具");
	ret->registerModel<CameraCaptureNode>("工具");
	ret->registerModel<CaptureTriggerNode>("工具");
	ret->registerModel<ImageSaveModel>("工具");
	ret->registerModel<ColorPickerNode>("工具");
	ret->registerModel<ColorSpaceConversionNode>("工具");
	ret->registerModel<MakeBorderNode>("工具");

	ret->registerModel<ImageBlurModel>("图像平滑");
	ret->registerModel<ImageGaussianBlurModel>("图像平滑");
	ret->registerModel<ImageImmedianBlurModel>("图像平滑");
	ret->registerModel<ImageImbilateralFilter>("图像平滑");

	ret->registerModel<MorphologyModel>("形态学");
	ret->registerModel<MorphologyFindLineNode>("形态学");

	ret->registerModel<DrawLineModel>("绘制");
	ret->registerModel<DrawRectangleModel>("绘制");
	ret->registerModel<DrawCircleModel>("绘制");
	ret->registerModel<DrawTextModel>("绘制");
	ret->registerModel<HoughLinesNode>("图像操作");
	ret->registerModel<DrawLinesNode>("绘制");
	ret->registerModel<ImageDrawContoursModel>("绘制");
	ret->registerModel<DrawCirclesNode>("绘制");

	ret->registerModel<ResizeNode>("图像操作");
	ret->registerModel<ImageCropModel>("图像操作");
	ret->registerModel<ImageRotateModel>("图像操作");
	ret->registerModel<ImageFlipModel>("图像操作");
	ret->registerModel<ImageImadaptiveThresh>("图像操作");
	ret->registerModel<ImageCannyModel>("图像操作");
	ret->registerModel<SobelEdgeDetectionNode>("图像操作");
	ret->registerModel<LaplacianEdgeDetectionModel>("图像操作");
	ret->registerModel<ImageHistEqualizeModel>("图像操作");
	ret->registerModel<ImageFindContoursModel>("图像操作");
	ret->registerModel<ThresholdNode>("图像操作");
	ret->registerModel<ImageChannelExtract>("图像操作");
	ret->registerModel<HSVColorSegmentationNode>("图像操作");
	ret->registerModel<WarpPerspectiveModel>("图像操作");
	ret->registerModel<MatCreateNode>("图像操作");
	ret->registerModel<BCGAdjustment>("图像操作");
	ret->registerModel<HoughCirclesNode>("图像操作");

	ret->registerModel<BitwiseAnd>("数学");
	ret->registerModel<BitwiseOr>("数学");
	ret->registerModel<AddWeightedNode>("数学");
	ret->registerModel<AdditionModel>("数学");
	ret->registerModel<SubtractionModel>("数学");
	ret->registerModel<MultiplicationModel>("数学");
	ret->registerModel<DivisionModel>("数学");

	ret->registerModel<TextDisplayModel>("显示");
	ret->registerModel<ImageDisplayModel>("显示");

	ret->registerModel<StrToNumModel>("数据类型转换");
	ret->registerModel<NumToStrModel>("数据类型转换");
	ret->registerModel<NumToPointModel>("数据类型转换");
	ret->registerModel<PointToNumModel>("数据类型转换");
	ret->registerModel<PointToPointsModel>("数据类型转换");

	ret->registerModel<NumberNodeModel>("数据节点");
	ret->registerModel<StringNodeModel>("数据节点");
	ret->registerModel<PointNodeModel>("数据节点");
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

