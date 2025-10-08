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

	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Image", "图像" }; }
	cv::Mat get() const { return _image; }
	void set(const cv::Mat& image) { _image = image; }
};

class NumberData : public QtNodes::NodeData
{
private:
	double _number;
public:
	NumberData() : _number(0.0) {}
	NumberData(double number) : _number(number) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Number", "数字" }; }
	double get() const { return _number; }
	void set(double number) { _number = number; }
};

class StringData : public QtNodes::NodeData
{
private:
	QString _string;
public:
	StringData() : _string("") {}
	StringData(const QString& str) : _string(str) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "String", "字符串" }; }
	QString get() const { return _string; }
	void set(const QString& str) { _string = str; }
};

class ContoursData : public QtNodes::NodeData
{
private:
	std::vector<std::vector<cv::Point>> _contours;
public:
	ContoursData() : _contours() {}
	ContoursData(const std::vector<std::vector<cv::Point>>& contours) : _contours(contours) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Contours", "边缘" }; }
	std::vector<std::vector<cv::Point>> get() const { return _contours; }
	void set(const std::vector<std::vector<cv::Point>>& contours) { _contours = contours; }
};

class ScalarData : public QtNodes::NodeData
{	
private:
	cv::Scalar _scalar;
public:
	ScalarData() : _scalar(cv::Scalar(0,0,0,0)) {}
	ScalarData(const cv::Scalar& scalar) : _scalar(scalar) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Scalar", "四元量" }; }
	cv::Scalar get() const { return _scalar; }
	void set(const cv::Scalar& scalar) { _scalar = scalar; }
};

class PointData : public QtNodes::NodeData
{
private:
	cv::Point2f _point;
public:
	PointData() : _point(cv::Point(0, 0)) {}
	PointData(const cv::Point& point) : _point(point) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Point", "点" }; }
	cv::Point get() const { return _point; }
	void set(const cv::Point& point) { _point = point; }
};

class PointsData : public QtNodes::NodeData
{
private:
	std::vector<cv::Point2f> _points;
public:
	PointsData() : _points() {}
	PointsData(const std::vector<cv::Point2f>& points) : _points(points) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Points", "点集" }; }
	std::vector<cv::Point2f> get() const { return _points; }
	void set(const std::vector<cv::Point2f>& points) { _points = points; }
};