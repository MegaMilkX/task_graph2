#include "faerie.hpp"

#include "../util/log.hpp"

#include <map>
#include <functional>

class Resource {
public:
    int depth;
};

class TaskGraph;
class Job {
public:
    typedef std::function<void(void)> task_fn_t;

    Job(TaskGraph* tg, task_fn_t fn, const std::string& name)
    : graph(tg), fn(fn), name(name), priority(0) {}
    Job* Requires(const std::string& req);
    Job* Produces(const std::string& prod);
    void _run() {
        LOG(_name() << ": " << _getPriority());
        fn();
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
private:
    int priority;
    std::string name;
    task_fn_t fn;
    TaskGraph* graph;
};

class TaskGraph {
public:
    

    Resource* GetResource(const std::string& name) {
        return 0;
    }
    Job* Add(const std::string& name, Job::task_fn_t fn) {
        Job* job = new Job(this, fn, name);
        jobs.emplace_back(job);
        return job;
    }

    void Sort() {
        for(auto j : jobs) {
            _walkConnections(j);
        }
        for(auto kv : req_to_jobs) {
            Resource& r = resources[kv.first];
            for(auto j : kv.second) {
                if(j->_getPriority() < r.depth) {
                    j->_setPriority(r.depth);
                }
            }
        }
        std::sort(jobs.begin(), jobs.end(), [](const Job* a, const Job* b){
            return a->_getPriority() < b->_getPriority();
        });
    }

    void Run() {
        for(auto j : jobs) {
            j->_run();
        }
    }

    void _walkConnections(Job* j) {
        for(auto prod : products[j]) {
            resources[prod].depth++;
            for(auto j2 : req_to_jobs[prod]) {
                _walkConnections(j2);
            }
        }
    }

    void _requires(Job* j, const std::string& req) {
        req_to_jobs[req].emplace_back(j);
    }
    void _produces(Job* j, const std::string& prod) {
        products[j].emplace_back(prod);
    }
private:
    std::vector<Job*> jobs;
    std::map<std::string, Resource> resources;
    std::map<std::string, std::vector<Job*>> req_to_jobs;
    std::map<Job*, std::vector<std::string>> products;
};

Job* Job::Requires(const std::string& req) {
    graph->_requires(this, req);
    return this;
}
Job* Job::Produces(const std::string& prod) {
    graph->_produces(this, prod);
    return this;
}

int main() {
    LOG("Faerie start...");

    TaskGraph tg;

    tg.Add("Animation", [](){
        LOG("Animating...");
    })
    ->Requires("FrameBegunFlag")
    ->Produces("AnimationDoneFlag");

    tg.Add("WorldProcess", [](){
        LOG("Processing the world...");
    })
    ->Requires("FrameBegunFlag")
    ->Requires("AnimationDoneFlag")
    ->Requires("DynamicsProcessedFlag")
    ->Produces("WorldProcessedFlag");

    tg.Add("Physics", [](){
        LOG("Process dynamics");
    })
    ->Requires("FrameBegunFlag")
    ->Requires("AnimationDoneFlag")
    ->Requires("Scene")
    ->Requires("FrameCommon")
    ->Produces("DynamicsProcessedFlag")
    ->Produces("RenderStartFlag");

    tg.Add("GBufferPass", [](){
        LOG("Draw GBuffer");
    })
    ->Requires("RenderStartFlag")
    ->Requires("FrameCommon")
    ->Requires("Scene")
    ->Produces("GBuffer");

    tg.Add("LightPass", [](){
        LOG("Light pass");
    })
    ->Requires("GBuffer")
    ->Requires("LightData")
    ->Produces("RenderFinishFlag");

    tg.Add("FrameStart", [](){
        LOG("The game frame starts here");
    })
    ->Produces("FrameBegunFlag")
    ->Produces("RenderStartFlag")
    ->Produces("FrameCommon")
    ->Produces("Scene");

    tg.Add("FrameEnd", [](){
        LOG("Frame finalized");
    })
    ->Requires("RenderFinishFlag");

    tg.Sort();
    tg.Run();

    FaerieInit();
    FaerieRun();
    FaerieCleanup();

    LOG("Faerie end")
    return 0;
}