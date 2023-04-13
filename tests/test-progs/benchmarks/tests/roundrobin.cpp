#include <iostream>
#include <thread>

using namespace std;

void add_all(int *a, int tid, int num_values)
{
    for (int i = 0; i < num_values; i ++) {
        a[i] += (tid + 1);
    }
}

int main(int argc, char *argv[])
{
    int length;
    if (argc == 1) {
        length = 100;
    } else if (argc >= 2) {
        length = atoi(argv[1]);
        if (length <= 0) {
            cerr << "Usage: " << argv[0] << " [length]" << endl;
            return 1;
        }
    } 
    else {
        cerr << "Usage: " << argv[0] << " [length]" << endl;
        return 1;
    }

    unsigned cpus = thread::hardware_concurrency();

    cout << "Running on " << cpus << " cores. ";
    cout << "with " << length << " length" << endl;

    int *a;
    a = new int[length];

    if (!a) {
        cerr << "Allocation error!" << endl;
        return 2;
    }

    for (int i = 0; i < length; i++) {
        a[i] = 0;
    }

    thread **threads = new thread*[cpus];

    // NOTE: -1 is required for this to work in SE mode.
    for (int i = 0; i < cpus - 1; i++) {
        threads[i] = new thread(add_all, a, i, length);
    }
    // Execute the last thread with this thread context to appease SE mode
    add_all(a, cpus - 1, length);

    cout << "Waiting for other threads to complete" << endl;

    for (int i = 0; i < cpus - 1; i++) {
        threads[i]->join();
    }

    delete[] threads;

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