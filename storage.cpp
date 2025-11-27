#include "storage.h"
#include <stdexcept>
#include <sstream>

SharedStorage::SharedStorage()
    : fields{0, 0, 0}
{}

void SharedStorage::set(int index, int value) {
    if (index < 0 || index >= NUM_FIELDS)
        throw std::out_of_range("set");

    if (index == 2) {
        std::lock_guard<std::mutex> lock(hotMutex);
        fields[2] = value;
    } else {
        std::lock_guard<std::mutex> lock(coldMutex);
        fields[index] = value;
    }
}

int SharedStorage::get(int index) {
    if (index < 0 || index >= NUM_FIELDS)
        throw std::out_of_range("get");

    if (index == 2) {
        std::lock_guard<std::mutex> lock(hotMutex);
        return fields[2];
    } else {
        std::lock_guard<std::mutex> lock(coldMutex);
        return fields[index];
    }
}

SharedStorage::operator std::string() const {
    std::scoped_lock lock(coldMutex, hotMutex);
    std::ostringstream out;
    out << "Fields: [" << fields[0] << ", " << fields[1] << ", " << fields[2] << "]";
    return out.str();
}
