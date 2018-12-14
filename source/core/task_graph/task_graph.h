#ifndef TASK_GRAPH_H
#define TASK_GRAPH_H

#include "job.h"
#include "task_resource.h"

#include <map>
#include <typeindex>
#include <algorithm>
#include <vector>
#include <memory>

namespace tg {

class Resource {
public:
    int depth;
};



class TaskGraph {
public:
    Job* Add(const std::string& name, Task* task) {
        Job* job = new Job(this, task, name);
        jobs.emplace_back(job);
        return job;
    }
    Job* Add(const std::string& name, LambdaTask::init_fn_t init_fn, LambdaTask::run_fn_t run_fn) {
        Job* job = new Job(
            this,
            new LambdaTask(init_fn, run_fn),
            name
        );
        jobs.emplace_back(job);
        return job;
    }

    void Sort() {
        resources.clear();

        for(auto j : jobs) {
            currentlyInitializedJob = j;
            j->_task()->Init(this);
        }
        currentlyInitializedJob = 0;
        
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

    template<typename T>
    resource_in<T> AcquireRead(const std::string& res) {
        _requires(currentlyInitializedJob, res);
        if(resources_[typeid(T)].count(res) == 0) {
            resources_[typeid(T)][res] = std::shared_ptr<resource_base>(new resource<T>());
        }
        return ((resource<T>*)resources_[typeid(T)][res].get())->acquire_read();
    }
    template<typename T>
    resource_out<T> AcquireWrite(const std::string& res) {
        _produces(currentlyInitializedJob, res);
        if(resources_[typeid(T)].count(res) == 0) {
            resources_[typeid(T)][res] = std::shared_ptr<resource_base>(new resource<T>());
        }
        return ((resource<T>*)resources_[typeid(T)][res].get())->acquire_write();
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
        if(!j) return;
        req_to_jobs[req].emplace_back(j);
    }
    void _produces(Job* j, const std::string& prod) {
        if(!j) return;
        products[j].emplace_back(prod);
    }
private:
    Job* currentlyInitializedJob = 0;
    std::vector<Job*> jobs;
    std::map<std::string, Resource> resources;
    std::map<std::string, std::vector<Job*>> req_to_jobs;
    std::map<Job*, std::vector<std::string>> products;

    std::map<
        std::type_index, 
        std::map<
            std::string,
            std::shared_ptr<resource_base>
        >
    > resources_;
};

}

#endif
