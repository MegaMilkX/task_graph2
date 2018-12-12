#ifndef FAERIE_LOG_HPP
#define FAERIE_LOG_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include <queue>
#include <mutex>
#include <fstream>

class FaerieLog {
public:
    static FaerieLog* GetInstance() {
        static FaerieLog fl;
        return &fl;
    }
    static void Write(const std::string& str) {
        GetInstance()->_write(str);
    }
private:
    FaerieLog()
    : working(true) {
        thread_writer = std::thread([this](){
            std::ofstream f("log.log", std::ios::out | std::ios::app);

            do {
                std::queue<std::string> lines_copy;
                {
                    std::lock_guard<std::mutex> lock(sync);
                    if(!working && lines.empty())
                        break;
                    
                    lines_copy = lines;
                    while(!lines.empty()) {
                        lines.pop();
                    }
                }
                while(!lines_copy.empty()) {
                    std::string l = lines_copy.front();
                    lines_copy.pop();
                    f << l << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            } while(1);

            f.flush();
            f.close();
        });
    }
    ~FaerieLog() {
        working = false;
        if(thread_writer.joinable()) {
            thread_writer.join();
        }
    }

    void _write(const std::string& str) {
        std::lock_guard<std::mutex> lock(sync);
        lines.push(str);
        std::cout << str << std::endl;
    }

    bool working;
    std::mutex sync;
    std::queue<std::string> lines;
    std::thread thread_writer;
};

#define MKSTR(LINE) \
static_cast<std::ostringstream&>(std::ostringstream() << LINE).str()

//#define LOG(LINE) std::cout << MKSTR(LINE) << std::endl;
#define LOG(LINE) FaerieLog::Write(MKSTR(LINE));
#define LOG_WARN(LINE) LOG("WARNING: " << LINE)
#define LOG_ERR(LINE) LOG("ERROR: " << LINE)
#define LOG_DBG(LINE) LOG("DEBUG: " << LINE)

#endif
