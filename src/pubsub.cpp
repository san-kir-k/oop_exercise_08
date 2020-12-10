#include "pubsub.hpp"

void PrinterOnScreen::handle(const std::vector<std::shared_ptr<Figure>> fv, std::string filename) {
    for (auto& f: fv) {
        f->print(std::cout);
        std::cout << "\n";
    }
}

void PrinterInFile::handle(const std::vector<std::shared_ptr<Figure>> fv, std::string filename) {
    std::ofstream ofs;
    ofs.open(filename);
    for (auto& f: fv) {
        f->print(ofs);
        ofs << "\n";
    }
    ofs.close();
}

void EventChannel::push(Event e) {
    std::lock_guard<std::mutex> lock(mx);
    buf.push(e);
    condv.notify_one();
}

Event EventChannel::back() const {
    return buf.back();
}

void EventChannel::pop() {
    std::lock_guard<std::mutex> lock(mx);
    buf.pop();
}

bool EventChannel::is_empty() const {
    return buf.empty();
}

void HandleLoop(EventChannel& channel) {
    bool is_over = false;
    while(!is_over) {
        std::unique_lock<std::mutex> lock(channel.condmx);
        channel.condv.wait(lock, [](const EventChannel& channel) { return !channel.is_empty(); });
        Event next_event = channel.back();
        channel.pop();
        switch (next_event.code) {
            case EventCode::screen:
                next_event.handler->handle(next_event.data, "");
                break;
            case EventCode::file:
                next_event.handler->handle(next_event.data, next_event.filename);
                break;
            case EventCode::quit:
                is_over = true;
                break;
            default:
                break;
        }
    }
}