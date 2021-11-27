#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <ostream>

enum STATUS {
    WAIT = 0,               // 就绪
    RUN = 1,                // 执行
    FINISH = 2,             // 阻塞
    NEW = 3,                // 创建状态
};

const std::string status_strings[4] = {"WAIT", "RUN", "FINISH", "NEW"};

struct PCB {
    std::string name;
    uint8_t priority;
    int arrival_time;
    int required_time;
    int consumed_time;
    STATUS status;

    PCB(): name("default"), priority(0), arrival_time(0), required_time(0), consumed_time(0), status(NEW) {}
    PCB(std::string name, int avt, int rqt, uint8_t priority = 0, STATUS stat = NEW):
        name(name), priority(priority), arrival_time(avt), required_time(rqt), consumed_time(0), status(stat) {}

    friend std::ostream& operator<<(std::ostream& os, const PCB& pcb) {
        os << pcb.name << "\t" << int(pcb.priority) <<"\t" << pcb.arrival_time << "\t" << pcb.required_time <<
         "\t" << pcb.consumed_time;
        const int id = int(pcb.status);
         os << "\t" << status_strings[id] << std::endl;
        return os;
    }
};

struct PCBCompFunctor {
    bool operator() (const PCB* const p1, const PCB* const p2) const {
        if (p1->priority != p2->priority)
            return p1->priority > p2->priority;
        return p1->arrival_time > p2->arrival_time;     // 优先级相同则按照先来先服务的方式
    }
};

class ProcessCtrl {
using PriorityList = std::priority_queue<PCB*, std::vector<PCB*>, PCBCompFunctor>;
private:
    const int arrive_ddl;
    const int min_life_period;
    const int max_life_period;
    const uint8_t max_level;
    std::queue<PCB*> fifo;              // 先来先服务队列
    PriorityList heap;                  // 优先数法小顶堆：id小的优先级高
    PCB* current_task;                  // 当前任务
public:
    ProcessCtrl(int arrive_ddl, int min_life_period, int max_life_period, uint8_t max_level = 8):
        arrive_ddl(arrive_ddl), min_life_period(min_life_period), max_life_period(max_life_period), max_level(max_level)
    {
        current_task = nullptr;
    }
    ~ProcessCtrl() {}
    /**
     * @brief 随机生成进程 
     */
    void processGenerator(std::vector<PCB>& tl, int proc_num, bool fixed_random_seed = false) const;

    /**
     * @brief 调度：插入新进程，并且进行进程的调度
     * @param tl 进程列表（是arrival time的小顶堆）
     * @param use_fifo 是否使用先来先服务
     */
    void schedule(std::vector<PCB>& tl, int& complete_cnt, int cnt, bool use_fifo = true); 
    void printProcessInfo(const std::vector<PCB>& tl, int cnt, std::string info = "Slice expired") const;
};
