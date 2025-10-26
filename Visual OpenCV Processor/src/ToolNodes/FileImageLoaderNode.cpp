#include "FileImageLoaderNode.h"
#include <QFileDialog>

using namespace QtNodes;

void FileImageLoaderNode::loadImageFromFile()
{
	QString fileName = QFileDialog::getOpenFileName(nullptr, "选择图像文件", "", "Image Files (*.png *.jpg *.bmp *.jpeg *.tiff);;All Files (*)");
	if (fileName.isEmpty())
		return;

	// 先获取文件实际大小
	QFileInfo fileInfo(fileName);
	qint64 fileSizeBytes = fileInfo.size();
	double fileSizeKB = fileSizeBytes / 1024.0;

	// 使用 QFile 读取文件，避免中文路径问题
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText("无法打开文件");
		return;
	}

	QByteArray imageData = file.readAll();
	file.close();

	// 将数据转换为 cv::Mat
	std::vector<uchar> buffer(imageData.begin(), imageData.end());
	_image = cv::imdecode(buffer, cv::IMREAD_UNCHANGED);

	if (_image.empty()) {
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText("加载失败");
	}
	else {
		// 显示图像预览
		cv::Mat rgbImage;
		if (_image.channels() == 1) {
			cv::cvtColor(_image, rgbImage, cv::COLOR_GRAY2RGB);
		}
		else if (_image.channels() == 3) {
			cv::cvtColor(_image, rgbImage, cv::COLOR_BGR2RGB);
		}
		else if (_image.channels() == 4) {
			cv::cvtColor(_image, rgbImage, cv::COLOR_BGRA2RGBA);
		}
		else {
			rgbImage = _image.clone();
		}
		QImage qimg((const uchar*)rgbImage.data, rgbImage.cols, rgbImage.rows, rgbImage.step, QImage::Format_RGB888);
		_ui.image_label->setPixmap(QPixmap::fromImage(qimg).scaled(_ui.image_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

		// 计算内存中的图像大小
		qint64 memorySizeBytes = _image.total() * _image.elemSize();
		double memorySizeKB = memorySizeBytes / 1024.0;

		// 显示图像信息
		_ui.channel_label->setText(QString::number(_image.channels()));
		_ui.size_label->setText(QString("%1 x %2").arg(_image.cols).arg(_image.rows));

		// 改进数据类型显示
		QString dataTypeStr;
		switch (_image.depth()) {
		case CV_8U: dataTypeStr = "8位无符号"; break;
		case CV_8S: dataTypeStr = "8位有符号"; break;
		case CV_16U: dataTypeStr = "16位无符号"; break;
		case CV_16S: dataTypeStr = "16位有符号"; break;
		case CV_32S: dataTypeStr = "32位有符号"; break;
		case CV_32F: dataTypeStr = "32位浮点"; break;
		case CV_64F: dataTypeStr = "64位浮点"; break;
		default: dataTypeStr = QString("类型%1").arg(_image.type());
		}
		_ui.dataType_label->setText(dataTypeStr);

		_ui.dataSize_label->setText(QString("文件: %1 KB\n内存: %2 KB")
			.arg(fileSizeKB, 0, 'f', 1)
			.arg(memorySizeKB, 0, 'f', 1));

		_ui.info_label->setStyleSheet("QLabel { color : green; }");
		_ui.info_label->setText(QString("加载完成"));
	}
	// 通知数据更新
	emit dataUpdated(0);
	emit dataUpdated(1);
	emit dataUpdated(2);
}

FileImageLoaderNode::FileImageLoaderNode() {
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("未选择图像");

	_ui.channel_label->setText("-");
	_ui.size_label->setText("-");
	_ui.dataType_label->setText("-");
	_ui.dataSize_label->setText("-");
	connect(_ui.browse_pushButton, &QPushButton::clicked, this, &FileImageLoaderNode::loadImageFromFile);
}

unsigned int FileImageLoaderNode::nPorts(PortType portType) const {
	if (portType == PortType::In)
		return 0;
	else
		return 3;
}

NodeDataType FileImageLoaderNode::dataType(PortType portType, PortIndex portIndex) const {
	if (portType == PortType::Out) {
		switch (portIndex) {
		case 0:
			return NodeDataType{ "Image", "图像输出"};
		case 1:
			return NodeDataType{ "Number", "宽度" };
		case 2:
			return NodeDataType{ "Number", "高度" };
		}
	}
}

std::shared_ptr<QtNodes::NodeData> FileImageLoaderNode::outData(QtNodes::PortIndex port) {
	if (_image.empty())
		return nullptr;
	switch (port) {
	case 0:
		return std::make_shared<ImageData>(_image);
	case 1:
		return std::make_shared<NumberData>(_image.cols);
	case 2:
		return std::make_shared<NumberData>(_image.rows);
	}
}
