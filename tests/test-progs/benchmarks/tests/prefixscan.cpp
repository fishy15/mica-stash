#include <iostream>
#include <thread>
#include <pthread.h>

using namespace std;

void prefix_sum(int *list, int *blocks, int tid, int threads, int num_values, int num_blocks)
{

    if (tid >= num_values) return;
    int min_num = num_values / threads;
    int num_extra = num_values % threads;
    int startj;
    if (tid < num_extra) {
        startj = (min_num + 1) * tid;
    }
    else {
        startj = (min_num + 1) * num_extra + min_num * (tid - num_extra);
    }

    int block_end = startj + (tid < num_extra ? min_num + 1 : min_num);

    for (int j = startj + 1; j < block_end; j++) {
        list[j] = list[j - 1] + list[j];
    }

    blocks[tid] = list[block_end - 1];
    // pthread_barrier_wait(bar);
    
    // pthread_barrier_wait(bar);
    
}

void prefix_2(int* blocks, int tid, int num_blocks) {
    if (tid == 0) {
        for (uint32_t j = 1; j < num_blocks; j++) {
            blocks[j] = blocks[j-1] + blocks[j];
        }
    }
}

void prefix_3(int* list, int* blocks, int tid, int num_values, int threads) {
    if (tid >= num_values) return;
    int min_num = num_values / threads;
    int num_extra = num_values % threads;
    int startj;
    if (tid < num_extra) {
        startj = (min_num + 1) * tid;
    }
    else {
        startj = (min_num + 1) * num_extra + min_num * (tid - num_extra);
    }

    int block_end = startj + (tid < num_extra ? min_num + 1 : min_num);

    if (tid > 0) {
        for (uint32_t j = startj; j < block_end; j++) {
            list[j] = blocks[tid - 1] + list[j];
        }
    }
}

int main(int argc, char *argv[])
{
    unsigned num_values;
    unsigned add_method = 1;
    if (argc == 1) {
        num_values = N;
    } else if (argc >= 2) {
        num_values = atoi(argv[1]);
        if (num_values <= 0) {
            cerr << "Usage: " << argv[0] << " [num_values]" << endl;
            return 1;
        }
    } 
    else {
        cerr << "Usage: " << argv[0] << " [num_values]" << endl;
        return 1;
    }

    unsigned cpus = thread::hardware_concurrency();

    cout << "Running on " << cpus << " cores. ";
    cout << "with " << num_values << " values" << endl;

    int *list, *blocks;
    list = new int[num_values];
    blocks = new int[cpus];

    if (!(list && blocks)) {
        cerr << "Allocation error!" << endl;
        return 2;
    }

    for (int i = 0; i < num_values; i++) {
        list[i] = i + 1;
    }
    for (int i = 0; i < cpus; i++) {
        blocks[i] = 0;
    }

    thread **threads = new thread*[cpus];
    // pthread_barrier_t* bar = new pthread_barrier_t;
    // pthread_barrier_init(bar, nullptr, cpus);
    // barrier* bar = new barrier(cpus);
    int num_blocks = std::min(num_values, cpus);

    // NOTE: -1 is required for this to work in SE mode.
    for (int i = 0; i < cpus - 1; i++) {
        threads[i] = new thread(prefix_sum, list, blocks, i, cpus, num_values, num_blocks);
    }
    // Execute the last thread with this thread context to appease SE mode
    prefix_sum(list, blocks, cpus - 1, cpus, num_values, num_blocks);

    cout << "Waiting for other threads to complete" << endl;

    for (int i = 0; i < cpus - 1; i++) {
        threads[i]->join();
    }

    for (int i = 0; i < cpus - 1; i++) {
        threads[i] = new thread(prefix_2, blocks, i, num_blocks);
    }
    // Execute the last thread with this thread context to appease SE mode
    prefix_2(blocks, cpus - 1, num_blocks);

    cout << "Waiting for other threads to complete" << endl;

    for (int i = 0; i < cpus - 1; i++) {
        threads[i]->join();
    }

    for (int i = 0; i < cpus - 1; i++) {
        threads[i] = new thread(prefix_3, list, blocks, i, num_values, cpus);
    }
    // Execute the last thread with this thread context to appease SE mode
    prefix_3(list, blocks, cpus - 1, num_values, cpus);

    cout << "Waiting for other threads to complete" << endl;

    for (int i = 0; i < cpus - 1; i++) {
        threads[i]->join();
    }

    delete[] threads;

    cout << "Validating..." << flush;

    int num_valid = 0;
    int cur_sum = 1;
    for (int i = 0; i < num_values; i++) {
        if (list[i] == cur_sum) {
            num_valid++;
        } else {
            cerr << "c[" << i << "] is wrong.";
            cerr << " Expected " << num_values;
            cerr << " Got " << list[i] << "." << endl;
        }
        cur_sum += (i + 2);
    }

    if (num_valid == num_values) {
        cout << "Success!" << endl;
        return 0;
    } else {
        return 2;
    }
}
