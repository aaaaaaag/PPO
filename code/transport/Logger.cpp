//
// Created by denis on 27.08.2022.
//

#include "Logger.h"
#include <fstream>
#include <memory>

class FileSingleton {
public:

    FileSingleton() {
        file.open("/home/denis/ppo/PPO/code/log.txt");
    }

    ~FileSingleton() {
        file.close();
    };

    std::ofstream file;
    polytour::transport::Logger::LogLevel level;
};

static std::unique_ptr<FileSingleton> _pInstance = std::make_unique<FileSingleton>();

void polytour::transport::Logger::setLogLevel(polytour::transport::Logger::LogLevel level) {
    _pInstance->level = level;
}

void polytour::transport::Logger::error(const std::string &msg) {
    _pInstance->file << "Critical error: " + msg + '\n';
}

void polytour::transport::Logger::warning(const std::string &msg) {
    if (static_cast<int>(_pInstance->level) > static_cast<int>(Logger::LogLevel::Error)) {
        _pInstance->file << "Warning: " + msg + '\n';
    }
}

void polytour::transport::Logger::info(const std::string &msg) {
    if (static_cast<int>(_pInstance->level) > static_cast<int>(Logger::LogLevel::Warnings)) {
        _pInstance->file << "Info: " + msg + '\n';
    }
}

void polytour::transport::Logger::debug(const std::string &msg) {
    if (static_cast<int>(_pInstance->level) > static_cast<int>(Logger::LogLevel::Info)) {
        _pInstance->file << "Debug info: " + msg + '\n';
    }
}

void polytour::transport::Logger::trace(const std::string &msg) {
    if (static_cast<int>(_pInstance->level) > static_cast<int>(Logger::LogLevel::Debug)) {
        _pInstance->file << "Trace info: " + msg + '\n';
    }
}
