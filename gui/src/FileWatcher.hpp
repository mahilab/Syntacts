#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <atomic>

enum class FileStatus
{
    Created,
    Modified,
    Erased
};

class FileWatcher
{
public:
    // Keep a record of files from the base directory and their last modification time
    FileWatcher(std::string watchPath, int delayMilliseconds);
    // Monitor "watchPath" for changes and in case of a change execute the user supplied "action" function
    void start(const std::function<void(std::string, FileStatus)> &action);
    void stop();
private:
    std::string m_watchPath;
    std::chrono::duration<int, std::milli> m_delay;
    std::unordered_map<std::string, std::filesystem::file_time_type> m_paths;
    std::atomic_bool m_running;
private:
    bool contains(const std::string &key);
};
