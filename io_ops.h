#pragma once

#include <string>
#include <vector>
#include "storage.h"

void generate_all_op_files();
std::vector<Task> load_commands(const std::string& filename);

