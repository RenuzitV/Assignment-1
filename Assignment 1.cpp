#include <iostream>
#include <fstream>
#include <string>
#define MAX_SIZE 50002

using namespace std;

int x[MAX_SIZE], y[MAX_SIZE];
//int* x = (int*)malloc(sizeof(int) * MAX_SIZE);
//int* y = (int*)malloc(sizeof(int) * MAX_SIZE);


// function to store data into array and return the size of the array
void store(int* a, int* b, int* size) {
    scanf_s("%*[^\n]%*c");
    while (scanf_s("%d,%d", &a[*size], &b[*size]) != EOF) {
        (*size)++;
    }
    cout << "Stored data successfully!\n";
}

// function to sort data in ascending order
void quicksort(int* a, int* b) {
    int* mid = a;
    for (int* i = a + 1; i != b; ++i) {
        if (*mid > *i) {
            swap(*mid, *i);
            ++mid;
            swap(*mid, *i);
        }
    }
    if (mid - a > 1) {
        quicksort(a, mid);
    }
    if (b - mid > 1) {
        quicksort(mid + 1, b);
    }
}


// function to return the most frequent element
int mode(int* a, int size) {
    int max_count = 0, most_freq = a[0], i = 0;
    while (i < size - 1) {
        for (int j = i + 1; j < size; j++) {
            if (a[i] != a[j] && j - i > max_count) {
                max_count = j - i;
                most_freq = a[i];
                i = j;
                break;
            }
            else if (j == size - 1 && a[i] == a[j] && j - i + 1 > max_count) {
                max_count = j - i + 1;
                most_freq = a[i];
                i = j;
            }
        }
    }
    return most_freq;
}


// function find mean
double mean(int* a, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += a[i];
    }
    return sum / size;
}


// function find stdev
double stdev(int* a) {
    double stdev_a;
    return stdev_a;
}


// function find cor_coeff
double cor_coeff(int* a, int* b) {
    double cc;
    return cc;
}


// fnction find and print out linear regression
void linear_regression(double mean_a, double mean_b,
    double stdev_a, double stdev_b, double cor_coeff) {
    double slope = cor_coeff * stdev_b / stdev_a;
    double intercept = mean_b - slope * mean_a;
    cout << "y = " << slope << "x + " << intercept << "\n";
}



int main(int argc, char* argv[]) {

    // open file and check
    FILE* F;
    freopen_s(&F, "D:\\Soft-Eng Design\\data1.csv", "r", stdin);
    if (!F) {
        cout << "Error opening file!" << endl;
        exit(-1);
    }
    else cout << "File opened!\n";


    int size = 0;
    store(x, y, &size);
    quicksort(x, x + size - 1);
    quicksort(y, y + size - 1);


    cout << "Most frequent element: \n" <<
        "mode_x = " << mode(x, size) << "\nmode_y = " << mode(y, size) << "\n";
    double mean_x = mean(x, size);
    double mean_y = mean(y, size);
    double stdev_x = stdev(x);
    double stdev_y = stdev(y);
    double r = cor_coeff(x, y);

    linear_regression(mean_x, mean_y, stdev_x, stdev_y, r);

    return 0;
}
