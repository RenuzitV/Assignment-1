#include <iostream>
#include <math.h>

using namespace std;

//Function to sort the array from low high
void Sort(int arr[], int n) {
	for (int i = 0; i <= n; i++) {
		for (int j = 0; j < n; j++) {
			if (arr[i] > arr[j]) {
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

//Function to find the Median of the array
double findMedian(int arr[], int n) {
	Sort(arr, n);
	if (n % 2 == 0) {
		return (arr[(n-1)/2] +arr[n/2]) /2;
	}
	else return arr[n / 2];
}

// Function to find the Mean of the array
double findMean(int arr[], int n) {
	double sum = 0;
	for (int i = 0; i < n; i++) {
		sum += arr[i];
	}
	return sum / n;
}

//Function to find the Mean Absolute Deviation 
double findMAD(int arr[], int n) {
	double sum = 0;
	for (int i = 0; i < n; i++) {
		sum = sum + abs(arr[i] - findMean(arr, n));
	}
	return sum / n;
}

//Function to find First Quartile
double findQ1(int arr[], int n) {
	double q2 = findMedian(arr, n);
	double q1 = findMedian(arr, q2);
	return 0;
}

/* int main()
{
	int a[]={ 34,21,56,76,45,11 };
 	int n = sizeof(a) / sizeof(a[0]);
	cout << Sort(a, n);
	return 0;
} */
