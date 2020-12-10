// Киреев Александр Константинович 206
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "pubsub.hpp"

// мануал
void help() {
    std::cout << "Command -- Description\n" <<
                "1 <type> -- Add new figure (1 -- Octagon, 2 -- Square, 3 -- Triangle)\n" << 
                "2 -- Display help\n" <<
                "3 -- End program" << std::endl;
}

void mainLoop(int max_cap) {
    std::vector<std::shared_ptr<Figure>> buf;
    int command = 0, type = 0;
    std::string s;
    std::string filename;
    EventChannel channel;
    Event event;
    int curr_file_count = 1;
    std::thread routine(HandleLoop, std::ref(channel));
    while (true) {
        if (!(std::cin >> s)) {
            break;
        }
        if (s.length() > 1) {
            std::cout << "Invalid command." << std::endl;
            continue;
        }
        std::stringstream ss(s);
        ss >> command;
        if (command == 3) {
            channel.push({EventCode::quit,
            buf,
            "",
            NULL});
            break;
        }
        if (command == 1) {
            if (buf.size() < max_cap) {
                std::cin >> type;
                if (type == 1) {
                    buf.push_back(std::shared_ptr<Figure>(new Octagon(std::cin)));
                } else if (type == 2) {
                    buf.push_back(std::shared_ptr<Figure>(new Square(std::cin)));
                } else if (type == 3) {
                    buf.push_back(std::shared_ptr<Figure>(new Triangle(std::cin)));
                } else {
                    std::cout << "Invalid type." << std::endl;
                }
            }
            if (buf.size() == max_cap) {
                channel.push({EventCode::screen,
                            buf,
                            "",
                            std::shared_ptr<Handler>(new PrinterOnScreen)});
                channel.push({EventCode::file,
                            buf,
                            "file_" + std::to_string(curr_file_count),
                            std::shared_ptr<Handler>(new PrinterInFile)});
                curr_file_count++;
                buf.clear();
            }
        } else if (command == 2) {
            help();
        } else {
            std::cout << "Invalid command!" << std::endl; 
        }
    }
    routine.join();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./lr8 <buffer_capacity>\n";
        return 0;
    }
    int max_cap = std::atoi(argv[1]);
    help();
    mainLoop(max_cap);
    return 0;
}
