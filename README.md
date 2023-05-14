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

### ```pickup(i)```
The ```pickup()``` function is called by a philosopher when they want to pick up both chopsticks and start eating. This function first sets the philosopher's state to HUNGRY, and then checks whether both of their neighboring philosophers are not currently eating. If both neighboring philosophers are not eating, the philosopher is allowed to start eating by setting their state to EATING and calling ```sem_post()``` on the semaphore representing their right chopstick.

If one or both neighboring philosophers are currently eating, the philosopher has to wait until both chopsticks are available before being allowed to start eating. This is achieved by calling ```sem_wait()``` on the semaphore representing their left chopstick.

```c
void pickup(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    test(i);
    pthread_mutex_unlock(&mutex);
    sem_wait(&chopsticks[i]);
}
```

### ```putdown(i)```
The ```putdown()``` function is called by a philosopher when they have finished eating and want to put down both chopsticks. This function sets the philosopher's state to THINKING and then checks whether either of their neighboring philosophers is currently hungry and waiting for chopsticks. If either neighboring philosopher is currently hungry, the ```putdown()``` function calls ```test()``` on that philosopher to allow them to start eating if both chopsticks are available.

```c
void putdown(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    test(left(i));
    test(right(i));
    pthread_mutex_unlock(&mutex);
}
```

### ```test(i)```
The ```test()``` function is called by a philosopher whenever their state changes. This function checks whether the philosopher is currently hungry and both of their neighboring philosophers are not currently eating. If both neighboring philosophers are not currently eating, the philosopher is allowed to start eating by setting their state to EATING and calling ```sem_post()``` on the semaphore representing their right chopstick.

```c
void test(int i) {
    if (state[i] == HUNGRY && state[left(i)] != EATING && state[right(i)] != EATING) {
        state[i] = EATING;
        sem_post(&chopsticks[i]);
    }
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

57 | 66 | 50 | 62 | 49
58 | 66 | 50 | 62 | 49
58 | 67 | 50 | 62 | 49
58 | 67 | 50 | 62 | 50
58 | 67 | 51 | 62 | 50
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



