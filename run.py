#!/usr/bin/env python3
import subprocess
from statistics import median

if __name__ == "__main__":
    count = 15
    scalar_times = []
    vector_times = []
    vector_log_times = []
    
    print("Running scalar")
    for i in range(count):
        result = subprocess.run(
            "./NPB3.0-omp-C/bin/scalar", stdout=subprocess.PIPE)
        splited_lst = result.stdout.decode("utf-8").split()
        cpu_index = splited_lst.index("CPU")
        scalar_times.append(float(splited_lst[cpu_index + 3]))

    print("Running vector")
    for i in range(count):
        result = subprocess.run(
            "./NPB3.0-omp-C/bin/vector", stdout=subprocess.PIPE)
        splited_lst = result.stdout.decode("utf-8").split()
        cpu_index = splited_lst.index("CPU")
        vector_times.append(float(splited_lst[cpu_index + 3]))

    median_scalar_t = median(scalar_times)
    median_vector_t = median(vector_times)

    print(f"Scalar time: {median_scalar_t}, Vector time: {median_vector_t}")
