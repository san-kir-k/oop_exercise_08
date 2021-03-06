#include <vector>
#include <queue>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "triangle.hpp"
#include "square.hpp"
#include "octagon.hpp"

class Handler {
    public:
        virtual void handle(const std::vector<std::shared_ptr<Figure>> fv, std::string filename) = 0;
        virtual ~Handler() = default;
};

class PrinterOnScreen: public Handler {
    public:
        void handle(const std::vector<std::shared_ptr<Figure>> fv, std::string filename) override;
};

class PrinterInFile: public Handler {
    public:
        void handle(const std::vector<std::shared_ptr<Figure>> fv, std::string filename) override;
};

enum class EventCode {
    screen = 0,
    file, 
    quit
};

struct Event {
    EventCode code;
    std::vector<std::shared_ptr<Figure>> data;
    std::string filename;
    std::shared_ptr<Handler> handler;
};

class EventChannel {
    private:
        std::queue<Event> buf;
        std::mutex mx;
    public:
        std::mutex condmx;
        std::condition_variable condv;
        EventChannel() = default;
        ~EventChannel() = default;
        void push(Event e);
        Event front();
        void pop();
        bool is_empty();
};

void HandleLoop(EventChannel& channel);
