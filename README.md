# Dining philosophers problem solution
## Description
This is a solution to the dining philosophers problem using the monitor concept. The solution is implemented in C using the pthread library.

## How to run
To run the program, you need to have the pthread library installed. If you are using a Linux distribution, you can install it using the following command:
```bash
sudo apt-get install libpthread-stubs0-dev
```

After installing the library, you can compile the program using the following command:
```bash
make run
```

## Explanation
In this implementation, each philosopher is represented by a separate thread, and each chopstick is represented by a semaphore with an initial value of 1. The state of each philosopher is maintained in an array of integers, and is either THINKING, HUNGRY, or EATING.


For each philosopher/thread we have this process:
```
 loop forever :
     p1 : think        
     p2 : pickup(i)                
     p3 : eat                      
     p4 : putdown(i)
```

### ```test(i)```
This function is responsible for testing whether a philosopher with index i can start eating. It checks if the philosopher has eaten less than both of their neighbors and if both neighbors are not currently eating. If these conditions are met, the philosopher's state is updated to EATING, and a signal is sent to wake up the philosopher.

```c
void test(int i) {
    if(times_eaten[i] <= times_eaten[left(i)] && times_eaten[i] <= times_eaten[right(i)]) {
        if (state[i] == HUNGRY && state[left(i)] != EATING && state[right(i)] != EATING) {
            state[i] = EATING;
            pthread_cond_signal(&cond[i]);
        }
    }
}
```

### ```pickup(i)```
This function is called when a philosopher with index i wants to pick up the forks and start eating. It locks the mutex to ensure mutual exclusion, sets the philosopher's state to HUNGRY, and calls test(i) to check if the philosopher can start eating. If the philosopher is not able to start eating, it waits on the condition variable cond[i] until it receives a signal indicating that it can proceed. After acquiring the forks, it unlocks the mutex to allow other philosophers to access the test() and pickup() functions.

```c
void pickup(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    test(i);

    if (state[i] != EATING) {
        pthread_cond_wait(&cond[i],&mutex);
    }

    pthread_mutex_unlock(&mutex);
}
```

### ```putdown(i)```
This function is called when a philosopher with index i finishes eating and wants to put down the forks. Similar to pickup(), it locks the mutex, sets the philosopher's state to THINKING, and calls test() for its left and right neighbors to check if they can start eating now. Then it signals both neighbors by using pthread_cond_signal() to wake them up in case they were waiting. Finally, it unlocks the mutex.

```c
void putdown(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    test(left(i));
    test(right(i));

    pthread_cond_signal(&cond[left(i)]);
    pthread_cond_signal(&cond[right(i)]);

    pthread_mutex_unlock(&mutex);
}
```

### ```philosopher(void *arg)```
This is the function executed by each philosopher thread. It receives an argument arg, which is a pointer to the philosopher's ID. The philosopher enters an infinite loop where it thinks for some time, calls pickup() to start eating, increments its times_eaten count, checks for starvation by printing the times_eaten array, eats for some time, and finally calls putdown() to finish eating.

```c
void *philosopher(void *arg) {
    int id = *(int *)arg;
    while (1) {
        printf("Philosopher %d is thinking\n", id);
        sleep(rand() % 5 + 1); // Thinking for some time
        printf("Philosopher %d is hungry\n", id);
        pickup(id);
        printf("Philosopher %d is eating\n", id);
        times_eaten[id]++;
        printf("Philosopher %d has eaten\n", id);
        printf("%d | %d | %d | %d | %d\n", times_eaten[0], times_eaten[1], times_eaten[2], times_eaten[3], times_eaten[4]); // <- check for starvation
        sleep(rand() % 5 + 1); // Eating for some time
        putdown(id);
    }
    return NULL;
}
```



## Results
We used the ```times_eaten[NUM_PHILOSOPHERS] ``` array to count and show us how many times each philosopher ate. The results are shown below:

```
0 | 1 | 0 | 1 | 0
0 | 1 | 1 | 1 | 0
1 | 1 | 1 | 1 | 0
1 | 1 | 1 | 1 | 1
1 | 1 | 1 | 2 | 1
1 | 2 | 1 | 2 | 1
1 | 2 | 1 | 2 | 2

[...]

81 | 85 | 82 | 81 | 79
81 | 85 | 83 | 81 | 79
82 | 85 | 83 | 81 | 79
82 | 85 | 83 | 81 | 80
83 | 85 | 83 | 81 | 80
83 | 86 | 83 | 81 | 80
84 | 86 | 83 | 81 | 80
```

As we can see, the philosophers are eating in a fair way, and no philosopher is starving.

## Problems
The implementation was fairly straightforward and we didn't have any problems. Using the resources from the book and the references below, i was able to implement the solution without any major problems.

## Authors
* [**Matheus Cirillo**](matheusscirillo@usp.br) - 12547750

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## References
* [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
* [Monitors](https://en.wikipedia.org/wiki/Monitor_(synchronization))
* [Pthread library](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html)
* [Dining philosophers problem - Geeks for Geeks](https://www.geeksforgeeks.org/dining-philosophers-problem/)
* [Operating System Concepts - 10th Edition](https://edisciplinas.usp.br/pluginfile.php/7563912/mod_resource/content/2/Silberschatz_Operating_System_Concepts_10e_2018.pdf)



