// Merge sort in C++
#include <iostream>
#include <vector>
#include <sys/time.h>
using namespace std;


void merge(int arr[], int p, int q, int r) {


    int n1 = q - p + 1;
    int n2 = r - q;


    vector<int> L(n1);
    vector<int> M(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[p + i];

    for (int j = 0; j < n2; j++)
        M[j] = arr[q + 1 + j];


    int i = 0, j = 0, k = p;


    while (i < n1 && j < n2) {
        if (L[i] <= M[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = M[j];
            j++;
        }
        k++;
    }


    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = M[j];
        j++;
        k++;
    }
}


void mergeSort(int arr[], int l, int r) {
    if (l < r) {

        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Print the array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
}

// Driver program
int main() {
    struct timeval ts, te;
    int N;
    int *arr;
    cin >> N;
    arr = new int[N];
    srand(time(NULL));
    for(int i = 0; i < N; i++)
    {
        arr[i] = rand() % N;
    }
//    int size = sizeof(arr) / sizeof(arr[0]);
    gettimeofday(&ts, NULL);
    int size = N;
    mergeSort(arr, 0, size - 1);
    gettimeofday(&te, NULL);
    double elapsed;
    elapsed = (te.tv_sec - ts.tv_sec) * 1000;
    elapsed += (te.tv_usec - ts.tv_usec) / 1000;
    cout << "Elapsed time: " << elapsed << endl;
//    printArray(arr, size);
    return 0;
}
