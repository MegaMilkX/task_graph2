#ifndef JOB_H
#define JOB_H

#include "task.h"

#include <string>

namespace tg {

class Job {
public:
    Job(TaskGraph* tg, Task* task, const std::string& name)
    : graph(tg), task(task), name(name), priority(0) {}
    void _run() {
        //LOG(_name() << ": " << _getPriority());
        task->Run();
    }
    void _setPriority(int p) {
        priority = p;
    }
    int _getPriority() const {
        return priority;
    }
    const std::string& _name() const {
        return name;
    }
    Task* _task() { return task; }
private:
    int priority;
    std::string name;
    Task* task;
    TaskGraph* graph;
};

}

#endif
