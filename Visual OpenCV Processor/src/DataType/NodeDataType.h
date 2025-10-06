#pragma once

#include <QtNodes/NodeData>
#include <opencv2/opencv.hpp>

#include <memory>
#include <string>
#include <vector>

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
	StringData() : _string("") {}
	StringData(const QString& str) : _string(str) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "String", "str" }; }
	QString get() const { return _string; }
};

class ContoursData : public QtNodes::NodeData
{
private:
	std::vector<std::vector<cv::Point>> _contours;
public:
	ContoursData() : _contours() {}
	ContoursData(const std::vector<std::vector<cv::Point>>& contours) : _contours(contours) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Contours", "contours" }; }
	std::vector<std::vector<cv::Point>> get() const { return _contours; }
};

class ScalarData : public QtNodes::NodeData
{	
private:
	cv::Scalar _scalar;
public:
	ScalarData() : _scalar(cv::Scalar(0,0,0,0)) {}
	ScalarData(const cv::Scalar& scalar) : _scalar(scalar) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Scalar", "scalar" }; }
	cv::Scalar get() const { return _scalar; }
};

class PointData : public QtNodes::NodeData
{
private:
	cv::Point _point;
public:
	PointData() : _point(cv::Point(0, 0)) {}
	PointData(const cv::Point& point) : _point(point) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Point", "point" }; }
	cv::Point get() const { return _point; }
};