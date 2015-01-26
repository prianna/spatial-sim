"""
This is a short script that parses simulation data and captures
the dynamics that result from variation of parameter p:
1. Avg time for the epidemic to reach the city.
2. Avg time to extinction.
3. P(inf reaches the city).
4. P(inf dies out).
"""
import math
import numpy as np
import matplotlib.pyplot as plt
import glob

# Number of unique nodes the simulation tracks.
# Useful for selecting tuples corresponding to node k, where k is
# in range(num_nodes)
num_nodes = 21

# Specify number of parameter sets to analyze
num_paramsets = 21

# Array to hold average time taken for inf to reach city,
# given that the infection reaches city.
avg_time_to_city = np.zeros(num_paramsets)

# Array to hold average time taken for inf to go extinct,
# given that the infection dies out
avg_time_to_extinction = np.zeros(num_paramsets)

# array to hold P(inf) given parameter values
prob_inf_city = np.zeros(num_paramsets)

# Array to hold P(extinction) given param vals
prob_extinct_city = np.zeros(num_paramsets)

mu_vals = np.arange(0, 1, 0.05)


for i in range(1, num_paramsets+1):
    paramno = str(i)
    filenames = glob.glob("../paramset"+paramno+"/*")

    time_to_city = np.zeros([len(filenames), 1])
    time_to_extinction = np.zeros([len(filenames), 1])

    # Counter variable for the number of extinctions per parameter set.
    extinctions = 0
    # Counter variable for the number of infections that reach the city
    # per parameter set.
    infections = 0

    for j in range(len(filenames)):
        data = np.genfromtxt(filenames[j], skip_header=1, usecols=(0,1,2,3,4))
        # Check to see if infection dies out.
        if int(data[len(data)-num_nodes, 3]) == 0:
            extinctions += 1
            time_to_extinction[j] = data[len(data)-num_nodes, 1]
        # Iterate through data tuple-by-tuple to check for time at which
        # first infection occurs.
        for l in range(len(data)):
            if math.isnan(data[l, 0]):
                continue
            i = int(data[l, 0])
            # Check to see if the current tuple corresponds to a city node.
            if i == 0:
                inf_pop = int(data[l, 3])
                if inf_pop > 0:
                    # Record time to infection for datafile j.
                    time_to_city[j] = data[l, 1]
                    infections += 1
                    break

    # Need to remove 0 values in time_to_city and time_to_extinction.
    time_to_city = time_to_city[time_to_city != 0]
    time_to_extinction = time_to_extinction[time_to_extinction != 0]

    # Computing desired values and inserting into appropriate arrays.
    avg_time_to_city[i-1] = np.mean(time_to_city)
    avg_time_to_city[i-1] = np.mean(time_to_extinction)
    prob_inf_city[i-1] = infections/len(filenames)
    prob_extinct_city[i-1] = extinctions/len(filenames)

# Create appropriate plots and save them

# Plotting avg time for infection to reach city vs movement rate
fig = plt.figure(figsize=(10, 7))
plt.set_title('Average Time for Infection to Reach City')
plt.set_ylabel('Time')
plt.set_xlabel('Mu')
plt.scatter(mu_vals, avg_time_to_city)
fig.saveas('timetoinf_vs_mu.png')

# Plotting avg time to extinction vs movement rate
fig = plt.figure(figsize=(10, 7))
plt.set_title('Average Time to Extinction')
plt.set_ylabel('Time')
plt.set_xlabel('Mu')
plt.scatter(mu_vals, avg_time_to_extinction)
fig.saveas('timetoextinct_vs_mu.png')

# Plotting p(infection) in city vs movement rate
fig = plt.figure(figsize=(10, 7))
plt.set_title('P(Inf in City) vs movement rate, mu')
plt.set_ylabel('P(inf)')
plt.set_xlabel('Mu')
plt.scatter(mu_vals, prob_inf_city)
fig.saveas('probinf_vs_mu.png')

# Plotting p(extinction) in city vs movement rate
fig = plt.figure(figsize=(10, 7))
plt.set_title('P(Extinction) vs movement rate, mu')
plt.set_ylabel('P(extinction)')
plt.set_xlabel('Mu')
plt.scatter(mu_vals, prob_extinct_city)
fig.saveas('probextinct_vs_mu.png')
