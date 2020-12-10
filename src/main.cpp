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
    int command = 0, idx = 0, type = 0;
    std::string s;
    std::string filename;
    EventChannel channel;
    Event event;
    int curr_file_count = 1;
    PrinterOnScreen screen_print();
    PrinterInFile file_print();
    std::thread routine(HandleLoop, channel);
    while (std::cout << "Cmd: " && std::cin >> s) {
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
        switch (command) {
            case 1:
                if (buf.size() < max_cap) {
                    std::cin >> type;
                    switch (type) {
                        case 1:
                            buf.push_back(std::make_shared<Figure>(new Octagon(std::cin)));
                            break;
                        case 2:
                            buf.push_back(std::make_shared<Figure>(new Square(std::cin)));
                            break;
                        case 3:
                            buf.push_back(std::make_shared<Figure>(new Triangle(std::cin)));
                            break;
                        default:
                            std::cout << "Invalid type." << std::endl;
                            break;
                    }
                } else {
                    channel.push({EventCode::screen,
                                buf,
                                "",
                                std::make_shared<Handler>(screen_print)});
                    channel.push({EventCode::file,
                                buf,
                                "file_" + std::to_string(curr_file_count),
                                std::make_shared<Handler>(screen_print)});
                    curr_file_count++;
                    buf.clear();
                }
                break;
            case 2:
                help();
                break;
            default:
                std::cout << "Invalid command!" << std::endl; 
                break;
        }
    }
    routine.join();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./lr8 <buffer_capacity>\n";
        return;
    }
    int max_cap = std::atoi(argv[1]);
    help();
    mainLoop(max_cap);
    return 0;
}
