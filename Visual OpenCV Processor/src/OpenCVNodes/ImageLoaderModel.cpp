#include "ImageLoaderModel.h"
#include "NodeDataType.h"

#include <QFileDialog>
#include <QVBoxLayout>


using namespace QtNodes;

ImageLoaderModel::ImageLoaderModel()
	: m_widget(new QWidget), m_button(new QPushButton(QUTF8("浏览"))), m_label(new QLabel(QUTF8("无图像"))) {
	m_label->setAlignment(Qt::AlignCenter);
	m_label->setMinimumSize(200, 150);
	
	
	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->addWidget(m_button);
	vLayout->addWidget(m_label);
	m_widget->setLayout(vLayout);

	connect(m_button, &QPushButton::clicked, [this]() {
		QString fileName = QFileDialog::getOpenFileName(nullptr, QUTF8("选择图像"), "", QUTF8("Image Files (*.png *.jpg *.bmp)"));
		if (!fileName.isEmpty()) {
			m_image = cv::imread(fileName.toStdString());
			if (!m_image.empty()) {
				// 显示图片
				cv::Mat rgbImage;
				cv::cvtColor(m_image, rgbImage, cv::COLOR_BGR2RGB);
				QImage qimg((const uchar*)rgbImage.data, rgbImage.cols, rgbImage.rows, rgbImage.step, QImage::Format_RGB888);
				m_label->setPixmap(QPixmap::fromImage(qimg).scaled(m_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
			}
			else {
				m_label->setText(QUTF8("无法加载图像"));
			}
			// 通知所有输出端口数据更新
			Q_EMIT dataUpdated(0);
			Q_EMIT dataUpdated(1);
			Q_EMIT dataUpdated(2);
		}
		});
}

ImageLoaderModel::~ImageLoaderModel() {

}

unsigned int ImageLoaderModel::nPorts(PortType portType) const {
	if (portType == PortType::In)
		return 0;
	else
		return 3;
}

NodeDataType ImageLoaderModel::dataType(PortType portType, PortIndex portIndex) const {
	if (portType == PortType::Out) {
		switch (portIndex) {
		case 0:
			return NodeDataType{ "Image", "图像输出"};
		case 1:
			return NodeDataType{ "Number", "宽" };
		case 2:
			return NodeDataType{ "Number", "高" };
		}
	}
}

std::shared_ptr<QtNodes::NodeData> ImageLoaderModel::outData(QtNodes::PortIndex port) {
	if (m_image.empty())
		return nullptr;
	switch (port) {
	case 0:
		return std::make_shared<ImageData>(m_image);
	case 1:
		return std::make_shared<NumberData>(m_image.cols);
	case 2:
		return std::make_shared<NumberData>(m_image.rows);
	}
}
