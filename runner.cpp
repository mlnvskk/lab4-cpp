#include "runner.h"

void worker_thread(SharedStorage& storage,
                   const std::vector<Task>& tasks)
{
    for (const auto& task : tasks) {
        if (task.kind == "read") {
            volatile int value = storage.get(task.field);
            (void)value;
        }
        else if (task.kind == "write") {
            storage.set(task.field, task.value);
        }
        else {
            volatile std::string state = static_cast<std::string>(storage);
            (void)state;
        }
    }
}
