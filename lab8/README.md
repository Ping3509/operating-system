Thread Control and Synchronization

Understanding critical resources, the dining philosophers problem, deadlock, and other classic problems.

1. Design and implement thread control and synchronization in a Linux programming environment.

2. Implement the philosopher tasks using threads, assigning unique identifiers to both philosophers and chopsticks (passed as parameters).

3. Implement mutual exclusion access between philosophers (threads) and chopsticks (critical resources) using semaphore-based locking. Observe the occurrence of deadlock after the program runs for a certain period of time. If deadlock does not occur, increase the probability of deadlock by modifying the number of philosophers and adjusting the delay settings.

4. Modify the locking operation of the mutex from a blocking mode to a non-blocking mode to prevent the occurrence of deadlock.