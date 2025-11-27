#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <map>
#include <iomanip>

#include "settings.h"
#include "storage.h"
#include "io_ops.h"
#include "runner.h"

int main() {
    generate_all_op_files();

    std::vector<std::string> scenariosList = {
        "scenario_a",
        "scenario_b",
        "scenario_c"
    };

    std::vector<int> threadsList = {1, 2, 3};
    std::map<std::string, std::map<int, double>> timeResults;

    for (const auto& scenarioName : scenariosList) {
        for (int threadsCount : threadsList) {
            SharedStorage storageInstance;

            std::vector<std::vector<Task>> tasksPerThread(threadsCount);
            for (int idx = 0; idx < threadsCount; ++idx) {
                std::string filePath =
                    scenarioName + "_" + std::to_string(idx) + ".txt";
                tasksPerThread[idx] = load_commands(filePath);
            }

            auto timeStart = std::chrono::high_resolution_clock::now();

            std::vector<std::thread> activeThreads;
            for (int idx = 0; idx < threadsCount; ++idx) {
                activeThreads.emplace_back(worker_thread,
                                           std::ref(storageInstance),
                                           std::cref(tasksPerThread[idx]));
            }

            for (auto& currentThread : activeThreads) {
                currentThread.join();
            }

            auto timeEnd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> execDuration =
                timeEnd - timeStart;

            timeResults[scenarioName][threadsCount] =
                execDuration.count();
        }
    }

    std::cout << std::left
              << std::setw(15) << "Scenario"
              << std::setw(12) << "1 thread"
              << std::setw(12) << "2 threads"
              << std::setw(12) << "3 threads" << std::endl;

    std::cout << std::string(51, '-') << std::endl;

    auto printLine = [&](const std::string& title,
                         const std::string& key) {
        std::cout << std::left << std::setw(15) << title
                  << std::setw(12) << std::fixed << std::setprecision(4)
                  << timeResults[key][1]
                  << std::setw(12) << std::fixed << std::setprecision(4)
                  << timeResults[key][2]
                  << std::setw(12) << std::fixed << std::setprecision(4)
                  << timeResults[key][3]
                  << std::endl;
    };

    printLine("Scenario (a)", "scenario_a");
    printLine("Scenario (b)", "scenario_b");
    printLine("Scenario (c)", "scenario_c");

    return 0;
}
