# Types
* `customer_t`
* `typedef enum { HIGH_PRIORITY, MEDIUM_PRIORITY, LOW_PRIORITY } priority_t`
* `seat_t`: simple `{ row, col }` struct or something?

# Functions
* `customer_t *create_customer_queue(int num_customers, priority_t priority)`: Creates an array of length `num_customers`with customers in it. Priority is whether it's High, Medium or Low. Each has different requirements for wait time. No need for `free_XXXX`, I dont think the professor cares about mem leaks.
* `void seller(customer_t *queue, priority_t priority, int id)`: this is the 'threaded function'. This is what we are going to pass to `pthread_create`, it will simulate a sales line. It will print to output.
* `seat_t seat_finder(priority_t priority)`: Based on how the professor has worded this, this is supposed to use a condition variable (like the consumer producer stuff in class), with 4 cursors. Needs to be thread safe of course.

# Seat Finder
Imagine there are 12 seats, this is what the cursors will look like

    1   2   3   4   5   6   7   8   9   10  11  12
    ^                       ^^                  ^

    H = 0 // high priority
    HM = 6 // medium priority
    ML = 6 // medium priority
    L = 11 // low priority

This is how it's initialized. As a high priority seat request comes in, H is incremented one. As a low priority request comes in, L is decremented 1. HM will be decremented, ML will be incremented.

Based on how he worded the document he wants us to do it this way I think (remember the producer consumer problem / condition variable slides).

When all of the seats have been allocated it will look something like:

    1   2   3   4   5   6   7   8   9   10  11  12
                        ^^              ^^        

    H = 5 // high priority
    HM = 5 // medium priority
    ML = 9 // medium priority
    L = 9 // low priority

This means that there were 6 high priority seats given out, 4 medium priority seats given out, and 2 low priority seats given out.
