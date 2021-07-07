# Recurrent-Network-Connectivity-Reconstruction
Use with firing rate and average voltage data from balanced integrate-and-and fire network dynamics to reconstruct recurrent connectivity.
The main file in this project is RecurrentRecoveryIF.m. 
cs_omp.m is a standard algorithm used in the main file to solve the necessary L1 minimization problem, and it 
may be replaced by other optimization methods without changing the general recovery process by only editing one line of code calling cs_omp in the main file.
For simulation data to be loaded in, the user should save firing rate data with convention firingRates1.txt, firingRates2.txt,... voltage data with convention volt1.txt, volt2.txt,..., and input stimuli with convention forcing1.txt, forcing2.txt,...with respective numerical values indicating the iteration numbers.
