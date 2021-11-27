#include <fstream>
#include <sstream>
#include "process.hpp"

void readYaml(std::string path, std::vector<int>& configs) {
    configs.resize(7);
    std::ifstream yaml_reader(path);
    std::string s;
    while (getline(yaml_reader, s)) {
        if (s.length() < 0)
            continue;
        std::stringstream stream;
        stream << s;
        std::string tmpname;
        stream >> tmpname;
        if (tmpname[0] == '#') {
            continue;
        } else if (tmpname == "arrive_ddl:") {
            stream >> configs[0];
        } else if (tmpname == "min_life_period:") {
            stream >> configs[1];
        } else if (tmpname == "max_life_period:") {
            stream >> configs[2];
        } else if (tmpname == "max_level:") {
            stream >> configs[3];
        } else if (tmpname == "process_number:") {
            stream >> configs[4];
        } else if (tmpname == "use_fifo:") {
            stream >> configs[5];
        } else if (tmpname == "fixed_seed:") {
            stream >> configs[6];
        }
    }
}

int main(int argc, char** argv) {
    std::vector<int> configs;
    std::string path = "../config.yaml";

    readYaml(path, configs);
    int arrive_ddl = configs[0], min_life_period = configs[1], max_life_period = configs[2], proc_num = configs[4];
    uint8_t max_level = configs[3];
    bool use_fifo = (configs[5] > 0), fixed_seed = (configs[6] > 0);

    std::vector<PCB> process;
    ProcessCtrl pctrl(arrive_ddl, min_life_period, max_life_period, max_level);

    pctrl.processGenerator(process, proc_num, fixed_seed);
    pctrl.printProcessInfo(process, -1, "On intialization");
    int complete_cnt = 0, all_proc_num = static_cast<int>(process.size());
    for (int i = 0; complete_cnt < all_proc_num; i++) {
        pctrl.schedule(process, complete_cnt, i, use_fifo);
    }
    printf("All processes are completed.\n");
    return 0;
}