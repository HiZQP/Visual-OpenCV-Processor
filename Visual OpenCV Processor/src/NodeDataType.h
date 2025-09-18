#pragma once

#include <QtNodes/NodeData>
#include <opencv2/opencv.hpp>
#include <memory>
#include <string>

#define QUTF8(s) QString::fromUtf8(s)

class ImageData : public QtNodes::NodeData
{
private:
	cv::Mat _image;
public:
	ImageData() : _image(cv::Mat()) {}
	ImageData(const cv::Mat& image) : _image(image) {}

	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Image", "img" }; }
	cv::Mat get() const { return _image; }
};

class NumberData : public QtNodes::NodeData
{
private:
	double _number;
public:
	NumberData() : _number(0.0) {}
	NumberData(double number) : _number(number) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Number", "num" }; }
	double get() const { return _number; }
};

class StringData : public QtNodes::NodeData
{
private:
	QString _string;
public:
	StringData() : _string(QUTF8("")) {}
	StringData(const QString& str) : _string(str) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "String", "str" }; }
	QString get() const { return _string; }
};