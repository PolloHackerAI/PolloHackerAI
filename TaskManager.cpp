#include "TaskManager.h"
#include <iostream>

void TaskManager::addTask(const std::string &description) {
    tasks.push_back(Task(nextId++, description));
}

const std::vector<Task>& TaskManager::getTasks() const {
    return tasks;
}

void TaskManager::completeTask(int id) {
    for (auto &task : tasks) {
        if (task.id == id) {
            task.completed = true;
            return;
        }
    }
}

void TaskManager::deleteTask(int id) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->id == id) {
            tasks.erase(it);
            return;
        }
    }
}
