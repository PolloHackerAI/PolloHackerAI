#ifndef TASK_H
#define TASK_H

#include <string>

struct Task {
    int id;
    std::string description;
    bool completed;

    Task(int id, const std::string &description, bool completed = false)
        : id(id), description(description), completed(completed) {}
};

#endif // TASK_H
