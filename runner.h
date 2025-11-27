#pragma once

#include <vector>
#include "storage.h"

void worker_thread(SharedStorage& storage,
                   const std::vector<Task>& tasks);
