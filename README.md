## Kent-CS-33211-OS-Semaphores-Project
# Author: Cornelius Pavlic
# Spring 2024
# CS-33211 Operating Systems
# Dr. Qiang Guan
# Kent State University

**End Goal as per instructions:**

The producer generates items and puts them onto the table. The consumer will pick up items. The table can only hold two items at the same time. When the table is full, the producer will wait. When there are no items, the consumer will wait. We use semaphores to synchronize the producer and consumer. Mutual exclusion should be considered. We use threads in the producer program and consumer program. Shared memory is used for the "table".

**Steps Taken to Achieve the Goal**

The implementation is split into three major parts: the producer.cpp file, the consumer.cpp file, and the memoryspace.hpp file. The consumer and producer files each have a header, but that just contains libraries used for semaphores and shared memory space. The producer and consumer files are very similar. Both processes access shared memory, have a mutex lock on their critical section, and increment/decrement semaphores to keep track of items on the table. The differences are that the producer is used to set up the shared memory space and initialize the semaphores, and the consumer accesses the shared memory space after creation. The producer and consumer each increment only one of the flags, and they each handle the semaphores slightly differently.

The memoryspace.hpp file sets up a structure that contains the flags, semaphores, and table. As of completion, it runs in a Linux environment without errors. Though, in compilation, g++ needs to be used instead of gcc, at least on the wasp server.

**Running the processes:**

Once compiled, they can be run with the command `./producer & ./consumer &` and will produce the output:

```
Produced: 0
Consumed: 0
Produced: 1
Consumed: 1
Produced: 2
Produced: 3
Consumed: 2
Produced: 4
Consumed: 3
Produced: 5
Consumed: 4
Produced: 6
Consumed: 5
Produced: 7
Consumed: 6
Produced: 8
Consumed: 7
Produced: 9
Consumed: 8
Consumed: 9
```

This ensures that the table is full when it can be and the consumer gets to consume every item.


(For clarity this README file has been edited by chatGPT for spelling, grammar and clarity. There is also an uneditedREADME should you prefer that.)
