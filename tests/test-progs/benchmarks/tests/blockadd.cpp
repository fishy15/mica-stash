#include <iostream>
#include <thread>

using namespace std;

void block_add(int *a, int *b, int *c, int tid, int threads, int num_values)
{
    int work = num_values/threads;
    int extra = num_values % threads;
    int start = 0;
    if (tid < extra) {
        work++;
        start = work * tid;
    }
    else {
        start = (work + 1) * extra + (tid - extra) * work;
    }
    for (int i = start; i < start + work; i ++) {
        c[i] = a[i] + b[i];
    }
}

int main(int argc, char *argv[])
{
    unsigned num_values = N;
    unsigned cpus = thread::hardware_concurrency();

    cout << "Running on " << cpus << " cores. ";
    cout << "with " << num_values << " values" << endl;

    int *a, *b, *c;
    a = new int[num_values];
    b = new int[num_values];
    c = new int[num_values];

    if (!(a && b && c)) {
        cerr << "Allocation error!" << endl;
        return 2;
    }

    for (int i = 0; i < num_values; i++) {
        a[i] = i;
        b[i] = num_values - i;
        c[i] = 0;
    }

    thread **threads = new thread*[cpus];

    auto addFunc = &block_add;

    // NOTE: -1 is required for this to work in SE mode.
    for (int i = 0; i < cpus - 1; i++) {
        threads[i] = new thread(addFunc, a, b, c, i, cpus, num_values);
    }
    // Execute the last thread with this thread context to appease SE mode
    addFunc(a, b, c, cpus - 1, cpus, num_values);

    cout << "Waiting for other threads to complete" << endl;

    for (int i = 0; i < cpus - 1; i++) {
        threads[i]->join();
    }

    delete[] threads;

    cout << "Validating..." << flush;

    int num_valid = 0;
    for (int i = 0; i < num_values; i++) {
        if (c[i] == num_values) {
            num_valid++;
        } else {
            cerr << "c[" << i << "] is wrong.";
            cerr << " Expected " << num_values;
            cerr << " Got " << c[i] << "." << endl;
        }
    }

    if (num_valid == num_values) {
        cout << "Success!" << endl;
        return 0;
    } else {
        return 2;
    }
}
