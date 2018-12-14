#ifndef TASK_RESOURCE_H
#define TASK_RESOURCE_H

namespace tg {

class resource_base {
public:
    virtual ~resource_base() {}
};

template<typename T>
class resource_in {
public:
    resource_in() : resource_in(0) {}
    resource_in(T* value) : value(value) {}
    const T& get() const { return *value; }
    const T& operator*() const { return *value; }
    const T& operator->() const { return *value; }
private:
    T* value;
};

template<typename T>
class resource_out {
public:
    resource_out() : resource_out(0) {}
    resource_out(T* value) : value(value) {}
    T& get() { return *value; }
    T& operator*() { return *value; }
    T& operator->() { return *value; }
private:
    T* value;
};

template<typename T>
class resource : public resource_base {
public:
    resource_in<T> acquire_read() { return resource_in<T>(&value); }
    resource_out<T> acquire_write() { return resource_out<T>(&value); }
private:
    T value;
};

}

#endif
