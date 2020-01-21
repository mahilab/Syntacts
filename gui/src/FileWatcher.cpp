#include "FileWatcher.hpp"

#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

FileWatcher::FileWatcher(std::string watchPath, int delay) :
    m_watchPath{watchPath}, 
    m_delay{std::chrono::milliseconds(delay)},
    m_running(false)
{
    for (auto &file : std::filesystem::recursive_directory_iterator(m_watchPath))
    {
        m_paths[file.path().string()] = std::filesystem::last_write_time(file);
    }
}

void FileWatcher::start(const std::function<void(std::string, FileStatus)> &action)
{
    m_running = true;
    while (m_running)
    {
        // Wait for "delay" milliseconds
        std::this_thread::sleep_for(m_delay);

        auto it = m_paths.begin();
        while (it != m_paths.end())
        {
            if (!std::filesystem::exists(it->first))
            {
                action(it->first, FileStatus::Erased);
                it = m_paths.erase(it);
            }
            else
            {
                it++;
            }
        }

        // Check if a file was created or modified
        for (auto &file : std::filesystem::recursive_directory_iterator(m_watchPath))
        {
            auto current_file_last_write_time = std::filesystem::last_write_time(file);

            // File creation
            if (!contains(file.path().string()))
            {
                m_paths[file.path().string()] = current_file_last_write_time;
                action(file.path().string(), FileStatus::Created);
                // File modification
            }
            else
            {
                if (m_paths[file.path().string()] != current_file_last_write_time)
                {
                    m_paths[file.path().string()] = current_file_last_write_time;
                    action(file.path().string(), FileStatus::Modified);
                }
            }
        }
    }
}

void FileWatcher::stop() {
    m_running = false;
}

bool FileWatcher::contains(const std::string &key)
{
    auto el = m_paths.find(key);
    return el != m_paths.end();
}
