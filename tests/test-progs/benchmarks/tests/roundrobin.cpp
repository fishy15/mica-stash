#include <iostream>
#include <thread>

using namespace std;

void add_all(int *a, int tid, int iter_number, int num_values)
{
    if (tid == iter_number) {
        for (int i = 0; i < num_values; i ++) {
            a[i] += (tid + 1);
        }
    }
}

void read_all(int *a, int num_values) {
    for (int i = 0; i < num_values; i ++) {
        // just read the value
        int x = a[i];
    }
}

int a[N];
thread *threads[100]; // there are probably <100 cpus

int main(int argc, char *argv[])
{
    int length = N;

    unsigned cpus = thread::hardware_concurrency();

    cout << "Running on " << cpus << " cores. ";
    cout << "with " << length << " length" << endl;

    for (int i = 0; i < length; i++) {
        a[i] = 0;
    }

    for (int iter = 0; iter < cpus; iter++) {
        // NOTE: -1 is required for this to work in SE mode.
        for (int i = 0; i < cpus - 1; i++) {
            threads[i] = new thread(add_all, a, i, iter, length);
        }

        // Execute the last thread with this thread context to appease SE mode
        add_all(a, cpus - 1, iter, length);

        for (int i = 0; i < cpus - 1; i++) {
            threads[i]->join();
        }

        for (int i = 0; i < cpus - 1; i++) {
            delete threads[i];
        }

        for (int i = 0; i < cpus - 1; i++) {
            threads[i] = new thread(read_all, a, length);
        }

        read_all(a, length);

        for (int i = 0; i < cpus - 1; i++) {
            threads[i]->join();
        }

        for (int i = 0; i < cpus - 1; i++) {
            delete threads[i];
        }
    }

    cout << "Validating..." << flush;
    unsigned target = (cpus * (cpus + 1))/2;

    int num_valid = 0;
    for (int i = 0; i < length; i++) {
        if (a[i] == target) {
            num_valid++;
        } else {
            cerr << "a[" << i << "] is wrong.";
            cerr << " Expected " << target;
            cerr << " Got " << a[i] << "." << endl;
        }
    }

    if (num_valid == length) {
        cout << "Success!" << endl;
        return 0;
    } else {
        return 2;
    }
}
