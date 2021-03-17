#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//main input size
const int N = 5e4 + 5;

//main input variables
int x[N], y[N], n = 1;

//variables for each quesinton
double meanx, meany, modex, modey, varx, vary, stdevx, stdevy, madx, mady, q1x, q1y, skewx, skewy, kurtx, kurty, cov, r, a, b;

//quicksort algorithm
void sort(int* x, int* y) {
	int* mid = x; //choose pivot as first element
	for (int* i = x + 1; i != y; ++i) {
		//if pivot is bigger than an element to the right, swap it to the left of pivot
		//ex : 1 5[pivot] 6 7 2[i] 9 -> 1 2 5[pivot] 7 6[i] 9 
		if (*mid > *i) { 
			swap(*mid, *i); 
			++mid; //go to the next index
			swap(*mid, *i);
		}
	}
	if (mid - x > 1){
		sort(x, mid); //if the left side of the pivot has more than 1 element, sort it
	}
	if (y - mid > 1) {
		sort(mid + 1, y); //if the right side of the pivot has more than 1 element, sort it
	}
}


//basic input function
void input() {
	//scans the first line and ignore it
	scanf_s("%*[^\n]s%*c");

	//while there is still input, read it
	while (scanf_s("%d,%d", &x[n], &y[n]) != EOF) ++n; 
	--n;

	//sort the input
	sort(x + 1, x + n + 1);
	sort(y + 1, y + n + 1);
}

//returns the median of the input
int mean(int *x, int n) {
	return x[n / 2];
}

int main(){
	FILE* F;
	freopen_s(&F, "data1.csv", "r", stdin);
	input(); //input is read in X and Y, 1-indexed and sorted
	return 0;
}