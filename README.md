# Proc(ess)Man(agement)

---

​		操作系统课上机实验：进程管理。实现使用先来先服务调度以及：1. 固定优先级的优先数法 2. 可变优先级的优先数法的进程调度算法。进程调度的基本时间单位为：时间片。

---

### Installation & Compilation

​		推荐使用Linux + (C++17标准的编译器编译)：已经配置好了CMakeLists.txt，可以使用`cmake`进行编译。没有附加依赖库。理论上，C++11至C++20都支持：

```shell
git clone https://github.com/Enigmatisms/ProcMan.git
cd ProcMan/
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE .. && make -j
./pctrl
```

​		程序支持非编译配置参数：在根目录下的`config.yaml`文件中，有如下配置内容：

```yaml
arrive_ddl: 10        # 由于程序是完全随机的，需要设置进程最晚到达的时间，才能更好观察到并发
min_life_period: 2    # 进程执行最少需要的时间片数
max_life_period: 7    # 进程执行最多需要的时间片数
max_level: 3          # 最大优先等级（从1开始，到max_level）
process_number: 5     # 并发的进程数
use_fifo: 0			  # 是否使用先来先服务调度
fixed_seed: 1		  
# 随机数种子是否是固定的（如果为0（false），随机数种子为0，每次运行程序生成的进程都一致，否则随机数种子使用std::chrono获得当前时间作为种子，每次生成的进程都会不一样）
```

---

### Output

​		标准输出如下（本程序是优先级可变的优先数调度算法）：

| Name     | PRT                          | AVT          | RQT           | CST           | Status       |
| -------- | ---------------------------- | ------------ | ------------- | ------------- | ------------ |
| 进程名字 | Priority（优先级：越小越高） | Arrival time | Required time | Consumed time | 状态：共四种 |

​		默认刚创建的进程是NEW状态。程序将会显示在初始化时“On initialization”的进程状态以及每次 **<u>发生调度</u>** 时的情况（并不是每一个时间片结束都会输出一次）。

```C++
======================== On intialization ========================
Current CPU time: -1, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      1       3       3       0       NEW
Process--1      2       5       5       0       NEW
Process--2      1       6       4       0       NEW
Process--3      2       1       4       0       NEW
Process--4      3       0       6       0       NEW

======================== First process ========================
Current CPU time: 0, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      1       3       3       0       NEW
Process--1      2       5       5       0       NEW
Process--2      1       6       4       0       NEW
Process--3      2       1       4       0       NEW
Process--4      3       0       6       0       RUN

======================== Preemption ========================
Current CPU time: 1, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      1       3       3       0       NEW
Process--1      2       5       5       0       NEW
Process--2      1       6       4       0       NEW
Process--3      2       1       4       0       RUN
Process--4      4       0       6       1       WAIT

======================== Preemption ========================
Current CPU time: 3, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      1       3       3       0       RUN
Process--1      2       5       5       0       NEW
Process--2      1       6       4       0       NEW
Process--3      4       1       4       2       WAIT
Process--4      4       0       6       1       WAIT

======================== Preemption ========================
Current CPU time: 5, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       2       WAIT
Process--1      2       5       5       0       RUN
Process--2      1       6       4       0       NEW
Process--3      4       1       4       2       WAIT
Process--4      4       0       6       1       WAIT

======================== Preemption ========================
Current CPU time: 6, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       2       WAIT
Process--1      3       5       5       1       WAIT
Process--2      1       6       4       0       RUN
Process--3      4       1       4       2       WAIT
Process--4      4       0       6       1       WAIT

======================== Preemption ========================
Current CPU time: 9, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       2       RUN
Process--1      3       5       5       1       WAIT
Process--2      4       6       4       3       WAIT
Process--3      4       1       4       2       WAIT
Process--4      4       0       6       1       WAIT

======================== Slice expired ========================
Current CPU time: 10, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       3       FINISH
Process--1      3       5       5       1       RUN
Process--2      4       6       4       3       WAIT
Process--3      4       1       4       2       WAIT
Process--4      4       0       6       1       WAIT

======================== Preemption ========================
Current CPU time: 12, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       3       FINISH
Process--1      5       5       5       3       WAIT
Process--2      4       6       4       3       WAIT
Process--3      4       1       4       2       WAIT
Process--4      4       0       6       1       RUN

======================== Preemption ========================
Current CPU time: 13, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       3       FINISH
Process--1      5       5       5       3       WAIT
Process--2      4       6       4       3       WAIT
Process--3      4       1       4       2       RUN
Process--4      5       0       6       2       WAIT

======================== Preemption ========================
Current CPU time: 14, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       3       FINISH
Process--1      5       5       5       3       WAIT
Process--2      4       6       4       3       RUN
Process--3      5       1       4       3       WAIT
Process--4      5       0       6       2       WAIT

======================== Slice expired ========================
Current CPU time: 15, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       3       FINISH
Process--1      5       5       5       3       WAIT
Process--2      4       6       4       4       FINISH
Process--3      5       1       4       3       WAIT
Process--4      5       0       6       2       RUN

======================== Preemption ========================
Current CPU time: 16, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       3       FINISH
Process--1      5       5       5       3       WAIT
Process--2      4       6       4       4       FINISH
Process--3      5       1       4       3       RUN
Process--4      6       0       6       3       WAIT

======================== Slice expired ========================
Current CPU time: 17, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       3       FINISH
Process--1      5       5       5       3       RUN
Process--2      4       6       4       4       FINISH
Process--3      5       1       4       4       FINISH
Process--4      6       0       6       3       WAIT

======================== Slice expired ========================
Current CPU time: 19, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       3       FINISH
Process--1      6       5       5       5       FINISH
Process--2      4       6       4       4       FINISH
Process--3      5       1       4       4       FINISH
Process--4      6       0       6       3       RUN

======================== Slice expired ========================
Current CPU time: 22, scheduling occurs
Process name    PRT     AVT     RQT     CST     Status
Process--0      3       3       3       3       FINISH
Process--1      6       5       5       5       FINISH
Process--2      4       6       4       4       FINISH
Process--3      5       1       4       4       FINISH
Process--4      8       0       6       6       FINISH

All processes are completed.
```

---

#### Copyright @Enigmatisms