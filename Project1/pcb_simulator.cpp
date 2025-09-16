#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

// Define the PCB structure (YOU MUST IMPLEMENT THIS)
struct PCB {
    // TODO: Add fields: pid, state, pc, total_work
    int pid;
    std::string state;
    int pc;
    int total_work;

    // TODO: Add constructor PCB
   PCB(int processId, const std::string& processState, int programCounter, int work) {
          pid = processId;
        state = "Ready";  // all new processes start as Ready
        pc = 0;           // program counter starts at 0
        total_work = work;
    }
};

// Print states of all processes, sorted by PID (PROVIDED - DO NOT MODIFY)
void printProcessStates(const std::vector<PCB>& pcbs, int timeSlice) {
    std::cout << "Interrupt " << timeSlice << ":" << std::endl;
    std::vector<PCB> sorted_pcbs = pcbs;
    std::sort(sorted_pcbs.begin(), sorted_pcbs.end(), 
              [](const PCB& a, const PCB& b) { return a.pid < b.pid; });
    for (const PCB& pcb : sorted_pcbs) {
        std::cout << "PID " << pcb.pid << ": " << pcb.state  << ", at pc " << pcb.pc << std::endl;
    }
    std::cout << std::flush;
}

// Kernel simulator (YOU MUST IMPLEMENT THIS)
void kernelSimulator(std::vector<PCB>& pcbs, int timeQuantum) {
   std::queue<int> readyQueue; // Queue of indices of processes in pcbs vector
    int timeSlice = 1;

    // Initialize ready queue
    for (int i = 0; i < pcbs.size(); ++i) {
        readyQueue.push(i);
    }

    // Continue until all processes are terminated
    while (!readyQueue.empty()) {
        int idx = readyQueue.front();
        readyQueue.pop();
        PCB &current = pcbs[idx];

        if (current.state != "Terminated") {
            // Set running state
            current.state = "Running";

            // Calculate work done in this time slice
            int workDone = std::min(timeQuantum, current.total_work - current.pc);
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
            ++timeSlice;
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
        // TODO: Add check for unique PIDs (e.g insert pid into the set pids)
         pids.insert(pid); // add PID to set
        // TODO: Create PCB and add to pcbs (e.g., pcbs.emplace_back(pid, work))
        pcbs.emplace_back(pid, work); // create PCB
    }
    
    int timeQuantum = 2;
    kernelSimulator(pcbs, timeQuantum);
    
    std::cout << "All processes completed." << std::endl;
    return 0;
}