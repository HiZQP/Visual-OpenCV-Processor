#include "ImageSaveModel.h"
#include "NodeDataType.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <filesystem>

ImageSaveModel::ImageSaveModel()
{
	_button = new QPushButton("浏览");
	_label = new QLineEdit();
	_name = new QLineEdit();
	_name->setPlaceholderText("文件名");
	_label->setPlaceholderText("未选择");
	_imageLabel = new QLabel("无输出");
	_imageLabel->setAlignment(Qt::AlignCenter);
	_imageLabel->setMinimumSize(200, 150);
	_format = new QComboBox();
	_format->addItems({ ".png", ".jpg", ".bmp" });

	connect(_button, &QPushButton::clicked, [this]() {
		QString filePath = QFileDialog::getExistingDirectory(nullptr, "选择保存路径", ".", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (!filePath.isEmpty())
			_label->setText(filePath);
		else
			_label->clear();
		});
	connect(_label, &QLineEdit::textChanged, [this]() {
		QString newPath = _label->text();
		if (newPath != _lastValidPath) {
			_lastValidPath = newPath;
			_pathExists = !newPath.isEmpty() && std::filesystem::exists(newPath.toStdString());
		}
		});
	auto hLayout1 = new QHBoxLayout();
	auto hLayout2 = new QHBoxLayout();
	hLayout1->addWidget(_label);
	hLayout1->addWidget(_button);
	hLayout2->addWidget(_name);
	hLayout2->addWidget(_format);
	auto layout = new QVBoxLayout();
	layout->addLayout(hLayout1);
	layout->addLayout(hLayout2);
	layout->addWidget(_imageLabel);
	_widget = new QWidget();
	_widget->setLayout(layout);
}

unsigned int ImageSaveModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 0;
}

QtNodes::NodeDataType ImageSaveModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageSaveModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	auto imgData = std::dynamic_pointer_cast<ImageData>(nodeData);
	if (imgData) {
		QString filePath = _label->text();
		QString fileName = _name->text() + _format->currentText();

		if (!filePath.isEmpty() && !fileName.isEmpty() && _pathExists) {
			std::string savePath = filePath.toStdString() + "/" + fileName.toStdString();
			cv::imwrite(savePath, imgData->get());
			cv::Mat rgbImage;
			cv::cvtColor(imgData->get(), rgbImage, cv::COLOR_BGR2RGB);
			QImage qimg((const uchar*)rgbImage.data, rgbImage.cols, rgbImage.rows, rgbImage.step, QImage::Format_RGB888);
			_imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
			return;
		}
		else {
			_imageLabel->setText("无输出");
			return;
		}
	}
	_imageLabel->setText("无输出");
}
