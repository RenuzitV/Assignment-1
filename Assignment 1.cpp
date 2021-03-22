#include <iostream>
#include <fstream>
#include <string>
#define sqr(x) ((x)*(x))
#define cub(x) ((x)*sqr(x))
#define qud(x) (sqr(x)*sqr(x))

using namespace std;

//main input size
const int N = (int)5e4 + 5;

//main input variables
int x[N], y[N], n = 1;

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

//basic input function, X and Y is read in 1-indexed from a command line-argument file 
void input(const char s[], int *n) {
	//opens the file and transfer it to the standard input
	FILE* F;
	freopen_s(&F, s, "r", stdin);
	if (!F) {
		cout << "cant open file";
		exit(-1);
	}

	//scans the first line and ignore it
	scanf_s("%*[^\n]s%*c");

	//while there is still input, read it
	while (scanf_s("%d,%d", &x[*n], &y[*n]) != EOF) ++*n;
	--*n; //sets n to the correct number of items
}

void process() {
	//sorts x and y in non-decreasing order
	sort(x + 1, x + 1 + n);
	sort(y + 1, y + 1 + n);
}

//returns the median of the input, which is the value at the middle of a non-decreasing set
double mean(int *x, int n) {
	double res = 0;
	for (int i = 1; i <= n; ++i) {
		res += 1.0*x[i]/n;
	}
	return res;
	return x[n / 2];
}

//calculates variance based on a set of input
double var(int x[], int n, double m) {
	double res = 0;
	for (int i = 1; i <= n; ++i) {
		res += sqr(x[i] - m);
	}
	return res / n;
}

//calculates standard variance using variance
double stdvar(double var, int n) {
	return sqrt(var);
}

//calculates skew of a set
double skew(int x[], double sd, double m, int n) {
	double res = 0;
	int nn = n / 2;
	for (int i = 1; i <= nn; ++i) {
		//cub is a macro for cube function aka f(x) = x^3
		//since the input is sorted, we do two operations at a time : getting a negative x[i] - m and add with a positive x[n-i+1] - m
		//this prevents overflowing of variable res
		res += cub(x[i] - m) + cub(x[n - i + 1] - m);
	}

	//if n is odd, add the last middle number from the input
	if (n & 1) {
		res += cub(x[n / 2 + 1] - m);
	}

	return res / n / cub(sd);
}

double kurt(int x[], double sd, double m, int n) {
	double res = 0;
	int nn = n / 2;
	for (int i = 1; i <= nn; ++i) {
		//qudis a macro for quad function aka f(x) = x^4
		//since the input is sorted, we do two operations at a time : getting a negative x[i] - m and add with a positive x[n-i+1] - m
		//this prevents overflowing of variable res
		res += qud((double)x[i] - m) + qud((double)x[n - i + 1] - m);
	}

	//if n is odd, add the last middle number from the input
	if (n & 1) {
		res += qud(x[n / 2 + 1] - m);
	}

	return res / ((long long)n) / qud(sd) - 3;
}

int main(int argc, const char* argv[]){
	/*
	if (argc != 2) {
		cerr << "wrong argument";
		return -1;
	}

	//main input function
	//input("data1.csv", &n);
	input(argv[1], &n);
	*/
	if (argc == 2) {
		input("argv[1]", &n);
	}
	else input("data1.csv", &n);

	//variables for each quesinton, we store each function's answer in here for later use
	double meanx, meany, modex, modey, varx, vary, stdevx, stdevy, madx, mady, q1x, q1y, skewx, skewy, kurtx, kurty, cov, r, a, b;


	//processes the input
	process();
	
	//calculates the function and puts them in their respective variable
	meanx = mean(x, n);
	meany = mean(y, n);

	varx = var(x, n, meanx);
	vary = var(y, n, meany);

	stdevx = stdvar(varx, n);
	stdevy = stdvar(vary, n);

	skewx = skew(x, stdevx, meanx, n);
	skewy = skew(y, stdevy, meany, n);

	kurtx = kurt(x, stdevx, meanx, n);
	kurty = kurt(y, stdevy, meany, n);

	//sets precision to 6 digits after decimal place
	cout.precision(6);
	cout << fixed;

	//print answers
	cout << meanx << ' ' << meany << endl;
	cout << varx << ' ' << vary << endl;
	cout << stdevx << ' ' << stdevy << endl;
	cout << skewx << ' ' << skewy << endl;
	cout << kurtx << ' ' << kurty << endl;
	return 0;
}