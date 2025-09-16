#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

// Define the PCB structure
struct PCB {
    int pid;            // Process ID
    std::string state;  // Ready, Running, Terminated
    int pc;             // Program counter / progress
    int total_work;     // Total work units needed

    // Constructor: initialize with PID and total work units
    PCB(int processId, int work) {
        pid = processId;
        state = "Ready";  // all new processes start as Ready
        pc = 0;           // program counter starts at 0
        total_work = work;
    }
};

// Comparator function for sorting PCBs by PID
bool comparePCB(const PCB& a, const PCB& b) {
    return a.pid < b.pid;
}

// Print states of all processes, sorted by PID
void printProcessStates(const std::vector<PCB>& pcbs, int timeSlice) {
    std::cout << "Interrupt " << timeSlice << ":" << std::endl;

    // Make a copy to sort
    std::vector<PCB> sorted_pcbs = pcbs;
    std::sort(sorted_pcbs.begin(), sorted_pcbs.end(), comparePCB);

    // Pre-C++11: use index-based for loop
    for (size_t i = 0; i < sorted_pcbs.size(); ++i) {
        const PCB& pcb = sorted_pcbs[i];
        std::cout << "PID " << pcb.pid << ": " << pcb.state << ", at pc " << pcb.pc << std::endl;
    }

    std::cout << std::flush;
}

// Kernel simulator using round-robin scheduling
void kernelSimulator(std::vector<PCB>& pcbs, int timeQuantum) {
    std::queue<int> readyQueue; // Queue of indices of processes in pcbs vector
    int timeSlice = 1;

    // Initialize ready queue
    for (size_t i = 0; i < pcbs.size(); ++i) {
        readyQueue.push(i);
    }

    // Continue until all processes are terminated
    while (!readyQueue.empty()) {
        int idx = readyQueue.front();
        readyQueue.pop();
        PCB& current = pcbs[idx];

        if (current.state != "Terminated") {
            // Set running state
            current.state = "Running";

            // Calculate work done in this time slice
            int workDone = timeQuantum;
            if (current.pc + workDone > current.total_work) {
                workDone = current.total_work - current.pc;
            }
            current.pc += workDone;

            // Check if process completed
            if (current.pc >= current.total_work) {
                current.state = "Terminated";
            } else {
                current.state = "Ready";
                // Re-add to queue for next round
                readyQueue.push(idx);
            }

            // Print state after this interrupt
            printProcessStates(pcbs, timeSlice);
            timeSlice++;
        }
    }
}

int main() {
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
    std::set<int> pids; // Track unique PIDs
    for (int i = 0; i < numProcesses; ++i) {
        int pid, work;
        if (!(std::cin >> pid >> work)) {
            std::cerr << "Error: Invalid input format for process data" << std::endl;
            return 1;
        }
        if (work <= 0) {
            std::cerr << "Error: Invalid work units for PID " << pid << std::endl;
            return 1;
        }
        if (pids.count(pid) > 0) {
            std::cerr << "Error: Duplicate PID " << pid << " detected" << std::endl;
            return 1;
        }

        pids.insert(pid);          // add PID to set
        pcbs.push_back(PCB(pid, work)); // create PCB
    }

    int timeQuantum = 2;
    kernelSimulator(pcbs, timeQuantum);

    std::cout << "All processes completed." << std::endl;
    return 0;
}