#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <ostream>

#define SIMULATED_PROCESS_NUM 8

enum STATUS {
    WAIT = 0,               // 就绪
    RUN = 1,                // 执行
    FINISH = 2,             // 阻塞
};

const std::string status_strings[3] = {"WAIT", "RUN", "FINISH"};

struct PCB {
    std::string name;
    uint8_t priority;
    int arrival_time;
    int required_time;
    int consumed_time;
    STATUS status;

    PCB(): name("default"), priority(0), arrival_time(0), required_time(0), consumed_time(0), status(WAIT) {}
    PCB(std::string name, int avt, int rqt, int cst, uint8_t priority = 0):
        name(name), priority(priority), arrival_time(avt), required_time(rqt), consumed_time(cst), status(WAIT) {}

    friend std::ostream& operator<<(std::ostream& os, const PCB& pcb) {
        os << pcb.name << "\t" << int(pcb.priority) <<"\t" << pcb.arrival_time << "\t" << pcb.required_time <<
         "\t" << pcb.consumed_time;
        const int id = int(pcb.status);
         os << "\t" << status_strings[id] << std::endl;
    }
};

struct PCBCompFunctor {
    bool operator() (const PCB& p1, const PCB& p2) const {
        return p1.priority > p2.priority;
    }
};

class ProcessCtrl {
private:
    std::queue<PCB> fifo;           // 先来先服务队列
    std::priority_queue<PCB, std::vector<PCB>, PCBCompFunctor> heap;        // 优先数法小顶堆：id小的优先级高
    /// 每个进程的到达只会在整时间片时，如果同一时刻有多个进程同时到达，则使用randomShuffle的方式随机放入就绪队列中（先来先服务）
};