#include <cstdlib>
#include <chrono>
#include "process.hpp"

void ProcessCtrl::processGenerator(std::vector<PCB>& tl, int proc_num, bool fixed_random_seed) const {
    tl.reserve(proc_num);
    uint64_t random_seed = 0;
    if (fixed_random_seed == false)
        random_seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(random_seed);
    for (int i = 0; i < proc_num; i++) {
        int arrive_time = rand() % arrive_ddl;    // 最晚到达时间不超过arrive_ddl
        int required_time = rand() % (max_life_period - min_life_period) + min_life_period;
        uint8_t p_level = rand() % max_level + 1;   // 优先级最高为1，最低为max_level
        tl.emplace_back(std::string("Process--") + std::to_string(i), arrive_time, required_time, p_level, NEW);
    }
}

void ProcessCtrl::schedule(std::vector<PCB>& tl, int& complete_cnt, int cnt, bool use_fifo) {
    // 新进程插入阶段
    for (PCB& pc: tl) {
        if (pc.status == NEW && pc.arrival_time <= cnt) {
            pc.status = WAIT;
            if (use_fifo == true)
                fifo.push(&pc);
            else
                heap.push(&pc);
        }
    }
    if (use_fifo == true) {         // 先来先服务
        if (current_task == nullptr) {
            if (fifo.empty() == true) return;
            current_task = fifo.front();
            current_task->status = RUN;
            printProcessInfo(tl, cnt, "First process");
            fifo.pop();
        } else {
            if (current_task->consumed_time >= current_task->required_time) {
                current_task->status = FINISH;
                complete_cnt++;
                if (fifo.empty() == true) {
                    current_task = nullptr;
                    printProcessInfo(tl, cnt);
                    return;
                }
                current_task = fifo.front();
                current_task->status = RUN;
                printProcessInfo(tl, cnt);
                fifo.pop();
            }
        }
    } else {
        if (current_task == nullptr) {
            if (heap.empty() == true) return;
            current_task = heap.top();
            current_task->status = RUN;
            printProcessInfo(tl, cnt, "First process");
            heap.pop();
        } else {
            if (current_task->consumed_time >= current_task->required_time) {
                current_task->status = FINISH;
                complete_cnt++;
                if (heap.empty() == true) {
                    current_task = nullptr;
                    printProcessInfo(tl, cnt);
                    return;
                }
                current_task = heap.top();
                current_task->status = RUN;
                printProcessInfo(tl, cnt);
                heap.pop();
            } else {
                if (heap.top()->priority < current_task->priority) {        // 抢占开始
                    current_task->status = WAIT;
                    heap.push(current_task);
                    current_task = heap.top();
                    current_task->status = RUN;
                    printProcessInfo(tl, cnt, "Preemption");
                    heap.pop();
                }
            }
        }
    }
    if (current_task != nullptr) {
        current_task->consumed_time++;
    }
}

void ProcessCtrl::printProcessInfo(const std::vector<PCB>& tl, int cnt, std::string info) const {
    std::cout << "======================== " + info + " ========================\n";
    std::cout << "Current CPU time: " << cnt << ", scheduling occurs\n";
    std::cout << "Process name\tPRT\tAVT\tRQT\tCST\tStatus\n";
    for (const PCB& pc: tl)
        std::cout << pc;
    std::cout << std::endl;
}
