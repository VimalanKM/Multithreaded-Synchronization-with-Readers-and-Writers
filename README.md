# Multithreaded-Synchronization-with-Readers-and-Writers
Synchronizing the shared parameters when multiple threads try approaching for different purposes. A priority based approach using mutexes and conditional variables is implemented to provide concurrency in the system.

This code implements a priority based solution to tacke the reader-writer problem using multithreading in C. When multiple threads apprach a shared variable to read and write simultaneously, there will be a problem in maintaining uniformity and concurreny of data. This is a huge problem in all multithreaded applications. We have implemented a soltuion to this problem by giving preference to the reader threads more than the writer threads. By this approach, when a reader thread is available in the system waiting for data access, then the writer thread is always made to wait until all the reader thread requests are completed. 

Mutexes (Mutual Exclusions) are implemented whenever a thread's request is being processed, so that the other threads cannot take control and are made to wait. Conditional variables are also present so that the mutexes are removed unless the condition is satisifed by the reader or writer threads.

When a writer thread is waiting for reader threads to complete operation, then the particular writer thread is signalled to proceed after the condition (where all reader have completed their tasks) is satisfied. It is not broadcasted because the information must be perceived only by the single writer thread. When the writer thread completes operation, the control is passed on again to reader threads to check for the conditional variable. Whereas if reader threads are waiting for a writer thread to finsih process, then all the reader threads are broadcasted information by the writer thread because all reader can access the variable at once after the writing process is finished.

This helps maintaining concurreny and synchronizes the data access.

# Environment for running the code
gcc must be installed in the system.

pthread library must be available.

The code can be compiled using gcc with the following command:

$ gcc readers_writers.c -o readers_writers -lpthread

The executable cab be run using

$ ./readers_writers
