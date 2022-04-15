#!/usr/bin/env python3.8
import subprocess
import matplotlib.pyplot as plt
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

    print("Running vector with custom log")
    for i in range(count):
        result = subprocess.run(
            "./NPB3.0-omp-C/bin/vector-custom-log", stdout=subprocess.PIPE)
        splited_lst = result.stdout.decode("utf-8").split()
        cpu_index = splited_lst.index("CPU")
        vector_log_times.append(float(splited_lst[cpu_index + 3]))

    median_scalar_t = median(scalar_times)
    median_vector_t = median(vector_times)
    median_vector_log_t = median(vector_log_times)

    # Plotting
    x = ["Scalar", "Vector", "Vector with custom log"]
    y = [median_scalar_t, median_vector_t, median_vector_log_t]
    
    fig = plt.figure()
    
    # creating the bar plot
    plt.bar(x, y)

    for index, data in enumerate(y):
        plt.text(x=index, y =data*1.01, s=f"{data}", ha='center')

    plt.ylabel("Time (s)")
    plt.title("Gcc result for NASEP")
    plt.savefig("result.png")
