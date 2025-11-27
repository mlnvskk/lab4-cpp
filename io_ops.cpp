#include "io_ops.h"
#include "settings.h"
#include <iostream>
#include <fstream>
#include <random>

static void generate_single_file(const std::string& fileName,
                                 const TaskProfile& profileData,
                                 int totalOps)
{
    std::ofstream fileOut(fileName);
    if (!fileOut) return;

    std::vector<Task> taskPool;

    for (int i = 0; i < profileData.read0; ++i)
        taskPool.push_back({"read", 0, 0});
    for (int i = 0; i < profileData.write0; ++i)
        taskPool.push_back({"write", 0, 1});

    for (int i = 0; i < profileData.read1; ++i)
        taskPool.push_back({"read", 1, 0});
    for (int i = 0; i < profileData.write1; ++i)
        taskPool.push_back({"write", 1, 1});

    for (int i = 0; i < profileData.read2; ++i)
        taskPool.push_back({"read", 2, 0});
    for (int i = 0; i < profileData.write2; ++i)
        taskPool.push_back({"write", 2, 1});

    for (int i = 0; i < profileData.stringOp; ++i)
        taskPool.push_back({"string", 0, 0});

    std::mt19937 randomEngine(std::random_device{}());
    std::uniform_int_distribution<int> indexDist(
        0, static_cast<int>(taskPool.size()) - 1
    );

    for (int i = 0; i < totalOps; ++i) {
        const Task& currentTask = taskPool[indexDist(randomEngine)];

        if (currentTask.kind == "read")
            fileOut << "read " << currentTask.field << std::endl;
        else if (currentTask.kind == "write")
            fileOut << "write " << currentTask.field << " "
                    << currentTask.value << std::endl;
        else
            fileOut << "string" << std::endl;
    }
}

void generate_all_op_files() {
    TaskProfile profileA{10, 10, 10, 10, 40, 5, 15};
    TaskProfile profileB{14, 14, 14, 14, 14, 14, 16};
    TaskProfile profileC{5, 25, 5, 25, 5, 5, 30};

    for (int i = 0; i < 3; ++i) {
        generate_single_file("scenario_a_" + std::to_string(i) + ".txt", profileA, OPS_PER_FILE);
        generate_single_file("scenario_b_" + std::to_string(i) + ".txt", profileB, OPS_PER_FILE);
        generate_single_file("scenario_c_" + std::to_string(i) + ".txt", profileC, OPS_PER_FILE);
    }
}

std::vector<Task> load_commands(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return {};

    std::vector<Task> result;
    std::string op;

    while (in >> op) {
        if (op == "read") {
            int f;
            in >> f;
            result.push_back({"read", f, 0});
        } 
        else if (op == "write") {
            int f, v;
            in >> f >> v;
            result.push_back({"write", f, v});
        } 
        else if (op == "string") {
            result.push_back({"string", 0, 0});
        }
    }

    return result;
}
