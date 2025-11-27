#pragma once

#include <string>
#include <mutex>
#include "settings.h"

struct Task {
    std::string kind;
    int field;
    int value;
};

struct TaskProfile {
    int read0, write0;
    int read1, write1;
    int read2, write2;
    int stringOp;
};

class SharedStorage {
public:
    SharedStorage();
    SharedStorage(const SharedStorage&) = delete;
    SharedStorage& operator=(const SharedStorage&) = delete;

    void set(int index, int value);
    int get(int index);
    explicit operator std::string() const;

private:
    int fields[NUM_FIELDS];
    mutable std::mutex coldMutex;
    mutable std::mutex hotMutex;
};
