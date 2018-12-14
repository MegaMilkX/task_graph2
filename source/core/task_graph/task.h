#ifndef TASK_H
#define TASK_H

#include <functional>

namespace tg {

class TaskGraph;
class Task {
public:
    virtual ~Task() {}
    virtual void Init(TaskGraph* tg) = 0;
    virtual void Run() = 0;
};

class LambdaTask : public Task {
public:
    typedef std::function<void(TaskGraph*)> init_fn_t;
    typedef std::function<void(void)> run_fn_t;

    LambdaTask(init_fn_t init_fn, run_fn_t run_fn)
    : init_fn(init_fn), run_fn(run_fn) {}
    virtual void Init(TaskGraph* tg) {
        init_fn(tg);
    }
    virtual void Run() {
        run_fn();
    }
private:
    init_fn_t init_fn;
    run_fn_t run_fn;
};

}

#endif
