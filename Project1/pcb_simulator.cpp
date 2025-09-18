#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

// Define the PCB structure (YOU MUST IMPLEMENT THIS)
struct PCB {
    int pid;
    int pc;
    std::string state;
    int total_work;
    int remaining_work;

    PCB(int processId, int work)
        : pid(processId), pc(0), state("Ready"), total_work(work), remaining_work(work) {}
};

// Compare function for sorting by PID
bool comparePCB(const PCB &a, const PCB &b) {
    return a.pid < b.pid;
}

// Print states of all processes, sorted by PID (PROVIDED - DO NOT MODIFY)
void printProcessStates(const std::vector<PCB>& pcbs, int timeSlice) {
    std::cout << "Interrupt " << timeSlice << ":" << std::endl;
    std::vector<PCB> sorted_pcbs = pcbs;
    std::sort(sorted_pcbs.begin(), sorted_pcbs.end(), comparePCB);

    for (size_t i = 0; i < sorted_pcbs.size(); ++i) {
        std::cout << "PID " << sorted_pcbs[i].pid << ": "
                  << sorted_pcbs[i].state << ", at pc "
                  << sorted_pcbs[i].pc << std::endl;
    }
    std::cout << std::flush;
}

// Kernel simulator (YOU MUST IMPLEMENT THIS)
void kernelSimulator(std::vector<PCB>& pcbs, int timeQuantum) {
    std::queue<int> readyQueue;
    for (int i = 0; i < (int)pcbs.size(); ++i)
        readyQueue.push(i);

    int interruptCount = 0;

    while (!readyQueue.empty()) {
        int idx = readyQueue.front();
        readyQueue.pop();
        PCB &proc = pcbs[idx];

        // Set all unfinished processes to Ready
        for (int j = 0; j < (int)pcbs.size(); ++j) {
            if (pcbs[j].remaining_work > 0)
                pcbs[j].state = "Ready";
        }

        // Current process is Running
        proc.state = "Running";

        // Calculate actual run units
        int runUnits = (proc.remaining_work < timeQuantum) ? proc.remaining_work : timeQuantum;

        // Update pc temporarily for printing
        int old_pc = proc.pc;
        proc.pc += runUnits;

        // Print snapshot
        interruptCount++;
        printProcessStates(pcbs, interruptCount);

        // Update remaining work and final state
        proc.remaining_work -= runUnits;
        if (proc.remaining_work == 0) {
            proc.state = "Terminated";
        } else {
            proc.state = "Ready";
            readyQueue.push(idx);
        }
    }
}

int main() {
    // Read from standard input via redirection (PROVIDED)
    int numProcesses;
    if (!(std::cin >> numProcesses)) {
        std::cerr << "Error: Invalid input for number of processes" << std::endl;
        return 1;
    }
    if (numProcesses <= 0) {
        std::cerr << "Error: Invalid number of processes" << std::endl;
        return 1;
    }

    std::vector<PCB> pcbs;
    std::vector<int> pids; // Track unique PIDs
    for (int i = 0; i < numProcesses; ++i) {
        int pid, work;
        if (!(std::cin >> pid >> work)) {
            std::cerr << "Error: Invalid input format for process data" <<
            std::endl;
            return 1;
        }
        if (work <= 0) {
            std::cerr << "Error: Invalid work units for PID " << pid << std::endl;
            return 1;}
        if (std::find(pids.begin(), pids.end(), pid) != pids.end()) {
            std::cerr << "Error: Duplicate PID " << pid << " detected" <<
            std::endl;
            return 1;
        }
        pids.push_back(pid);
        pcbs.emplace_back(pid, work);
    }

    int timeQuantum = 2;
    kernelSimulator(pcbs, timeQuantum);

    std::cout << "All processes completed." << std::endl;
    return 0;
}