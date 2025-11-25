 Energy-Efficient-CPU-Scheduler-C++

 # Energy-Efficient CPU Scheduler (C++)

Repository: humaila/energy-efficient-cpu-scheduler

Description: Energy-Efficient CPU Scheduling — a C++ simulator demonstrating a priority-based, round-robin scheduler with simple DVFS (Dynamic Voltage and Frequency Scaling) behaviour. The simulator chooses highest-priority tasks first, executes them for a fixed quantum (time slice), reinserts unfinished tasks, adjusts a CPU frequency multiplier based on load, and simulates execution using std::this_thread::sleep_for.

Files
- scheduler.cpp — C++ simulator
- README.md — this file
- .gitignore (suggested) — to ignore build artefacts and virtual envs
- LICENSE (optional) — MIT recommended

Build & Run
- Build:
  - g++ -std=c++17 scheduler.cpp -o scheduler
- Run:
  - ./scheduler

What the program demonstrates
- Priority ordering in task selection (higher numeric priority chosen first).
- Round-robin time slicing with a configurable quantum.
- Simple DVFS mapping: CPU frequency multiplier is chosen based on queue/load.
- Execution simulation using sleep scaled by cpu frequency (timeSlice / cpuFrequency).
- Console logs showing selection, execution time, frequency adjustments, and idle transition.

Sample (hard-coded) tasks used in the program (matches assignment example)
- Task 1: Burst = 100 ms, Priority = 2
- Task 2: Burst = 150 ms, Priority = 3
- Task 3: Burst = 120 ms, Priority = 1
- Task 4: Burst = 150 ms, Priority = 2
- Quantum = 50 ms

Notes
- This is a user-space simulator for educational/demo purposes — not a kernel scheduler.
- Suggested improvements: command-line input for tasks, EDP/energy metric calculations, logging to CSV, ML workload prediction, or multi-core simulation.

Git: create repo locally and push
1. git init
2. git add .
3. git commit -m "Initial commit: Energy-efficient CPU scheduler simulator"
4. Create a repo on GitHub (or use gh CLI):
   - gh repo create humaila/energy-efficient-cpu-scheduler --public --source=. --remote=origin
5. git branch -M main
6. git push -u origin main

License
- MIT recommended. Add a LICENSE file if you want to make it explicit.
