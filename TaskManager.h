#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include <vector>

class TaskManager {
private:
    std::vector<Task> tasks;
    int nextId;

public:
    TaskManager() : nextId(1) {}

    void addTask(const std::string &description);
    const std::vector<Task>& getTasks() const;
    void completeTask(int id);
    void deleteTask(int id);
};

#endif // TASKMANAGER_H
