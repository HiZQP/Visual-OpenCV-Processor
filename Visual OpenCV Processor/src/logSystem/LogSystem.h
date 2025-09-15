#pragma once

#include "LogSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>


#define COLORED 1
#define COLORLESS 0
#define LOG_ENABLE true
#define LOG_DISABLE false

#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define DEFAULT "\033[0m"
#define BLUE "\033[34m"

#define TIME std::put_time(&resultTime, "[%H:%M:%S]")
#define LOG(logLevel, message) LogSystem::getInstance().log(logLevel, message)
	
enum class LogLevel {
		INFO,
		WARN,
		ERROR,
		FATAL,
		DEBUG,
	};
enum class LogColor {
		LOG_COLOR_GREEN,
		LOG_COLOR_YELLOW,
		LOG_COLOR_RED,
		LOG_COLOR_BLUE,
		LOG_COLOR_DEFAULT,
	};

class LogSystem {
private:

	std::ofstream m_LogFile;
	std::string m_logObject;
	std::string m_logFilePath;
	bool m_logToFile = true;

	LogSystem(); // ˽�й��캯������ֹ�ⲿʵ����
	
	~LogSystem();

public:
	static LogSystem& getInstance();
	LogSystem(const LogSystem&) = delete; // ��ֹ�������캯��
	LogSystem& operator=(const LogSystem&) = delete; // ��ֹ��ֵ������

	void init(const std::string& logFilePath, const std::string& logObject, const bool& logToFile);

	void LogCustom(const std::string& customLogLevel, const LogColor& logColor, const std::string& message);

	void log(const LogLevel& logLevel, const std::string& message);
};