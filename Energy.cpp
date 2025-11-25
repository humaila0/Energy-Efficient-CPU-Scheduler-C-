#include <bits/stdc++.h>
#include <thread>
#include <chrono>
#include <cmath>

using namespace std;

// Task structure with priority
struct Task {
    int id;
    int burstTime;
    int remainingTime;
    int priority;
    long seq; // insertion sequence to preserve FIFO among equal-priority tasks

    Task(int id_, int burst_, int priority_, long seq_)
        : id(id_), burstTime(burst_), remainingTime(burst_), priority(priority_), seq(seq_) {}
};

// Comparator for priority_queue: higher priority first; if equal priority, lower seq first (FIFO)
struct TaskCompare {
    bool operator()(const Task &a, const Task &b) const {
        if (a.priority == b.priority) return a.seq > b.seq; // earlier seq => higher priority in queue
        return a.priority < b.priority; // higher numeric priority => comes first
    }
};

class DvfsPriorityScheduler {
public:
    DvfsPriorityScheduler(const vector<Task> &tasks, int quantum)
        : quantum(quantum), cpuFrequency(1.0), idle(false), seqCounter(0)
    {
        // push tasks into priority queue preserving input order
        for (auto t : tasks) {
            Task copy = t;
            copy.seq = seqCounter++;
            taskQueue.push(copy);
        }
    }

    void run() {
        long cycle = 0;
        printInitialQueue();

        while (!taskQueue.empty()) {
            ++cycle;

            // peek highest-priority task
            Task current = taskQueue.top();
            taskQueue.pop();

            int exec = min(quantum, current.remainingTime);

            // compute CPU frequency based on effective load (number of tasks left + current if not finished)
            int tasksLeftIfNotFinished = (int)taskQueue.size() + (current.remainingTime > 0 ? 1 : 0);
            double freqBefore = getCpuFrequency(tasksLeftIfNotFinished);

            cout << "Cycle " << cycle << ":\n";
            cout << "Task Selected: Task " << current.id << " (P=" << current.priority << ")\n";
            cout << "Executing Task " << current.id << " for " << exec << " ms at CPU frequency "
                 << fixed << setprecision(1) << freqBefore << "x\n";

            // Simulate execution: sleep = exec / freq (ms)
            double sleepMs = exec / freqBefore;
            int sleepMsInt = max(1, (int)round(sleepMs));
            this_thread::sleep_for(chrono::milliseconds(sleepMsInt));

            // reduce remaining time
            current.remainingTime -= exec;

            // adjust frequency after execution based on updated queue size (including reinserted task)
            int queueSizeAfter = (int)taskQueue.size() + (current.remainingTime > 0 ? 1 : 0);
            double freqAfter = getCpuFrequency(queueSizeAfter);
            cout << "Adjusting CPU frequency to " << fixed << setprecision(1) << freqAfter
                 << "x (Load: " << (queueSizeAfter * 25) << "%)\n\n";

            if (current.remainingTime > 0) {
                // reinsert preserving original sequence
                current.seq = seqCounter++;
                taskQueue.push(current);
            } else {
                cout << "Task " << current.id << " completed.\n\n";
            }
        }

        // All tasks done -> idle
        cpuFrequency = 0.2;
        idle = true;
        cout << "Queue size = 0; Load = 0%.\n";
        cout << "Switching CPU to idle state at frequency " << fixed << setprecision(1) << cpuFrequency << "x\n";
    }

private:
    priority_queue<Task, vector<Task>, TaskCompare> taskQueue;
    int quantum;
    double cpuFrequency;
    bool idle;
    long seqCounter;

    void printInitialQueue() {
        // print tasks in queue order (without destroying queue)
        vector<Task> tmp;
        cout << "Priority Queue after initialization:\n";
        while (!taskQueue.empty()) {
            Task t = taskQueue.top(); taskQueue.pop();
            cout << "Task " << t.id << " (P=" << t.priority << ")";
            if (!taskQueue.empty()) cout << " > ";
            tmp.push_back(t);
        }
        cout << "\n\n";
        for (auto &t : tmp) taskQueue.push(t);
    }

    double getCpuFrequency(int queueSize) {
        // queueSize * 25 -> Load percentage (this follows the mapping used in the assignment)
        int load = queueSize * 25;
        if (load >= 100) return 1.0;
        if (load >= 75)  return 0.8;
        if (load >= 50)  return 0.6;
        if (load >= 25)  return 0.5;
        return 0.2; // very low load or idle
    }
};

int main() {
    // Initialize tasks (id, burstTime, priority)
    vector<Task> tasks;
    tasks.emplace_back(1, 100, 2, 0); // Task 1
    tasks.emplace_back(2, 150, 3, 1); // Task 2
    tasks.emplace_back(3, 120, 1, 2); // Task 3
    tasks.emplace_back(4, 150, 2, 3); // Task 4

    int quantum = 50; // ms

    cout << "Input Data\n\nTasks:\n";
    for (auto &t : tasks) {
        cout << "Task " << t.id << ": Burst Time = " << t.burstTime << " ms, Priority = " << t.priority << "\n";
    }
    cout << "\nQuantum = " << quantum << " ms\n\nInitialization\n";

    DvfsPriorityScheduler scheduler(tasks, quantum);
    scheduler.run();

    return 0;
}
