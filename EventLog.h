#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <vector>
#include <string>
#include <iostream>

class EventLog {
public:
    std::vector<std::string> events;

    void record(const std::string& event) {
        events.push_back(event);
        std::cout << "  [LOG] " << event << std::endl;
    }

    void printAll() const {
        std::cout << "\n========== REGISTRO COMPLETO DE EVENTOS ==========\n";
        for (size_t i = 0; i < events.size(); i++) {
            std::cout << "  " << (i + 1) << ". " << events[i] << "\n";
        }
        std::cout << "===================================================\n";
    }

    void clear() {
        events.clear();
    }
};

#endif
