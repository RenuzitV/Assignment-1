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

//returns the average number of the input
double mean(int *x, int n) {
	double res = 0;
	for (int i = 1; i <= n; ++i) {
		res += 1.0*x[i]/n;
	}
	return res;
}

//calculates variance based on a set of input
double calVariance(int* a, int n, double mean) {
	double sum = 0;
	for (int i = 1; i <= n; i++) {
		sum += sqr(a[i] - mean);
	}
	return sum / ((double)n - 1);
}

//calculates std deviation via variance
double calStandardDeviation(double variance) {
	return sqrt(variance);
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
	double qsd = qud(sd);
	for (int i = 1; i <= n; ++i) {
		//qud is a macro for quad function aka f(x) = x^4
		//we divide the each qud() by n and qsd to minimize overflowing
		res += qud((double)x[i] - m) / ((long long)n) / qsd;
	}
	return res - 3;
}

//calculates covariance from two sets of data
double calCovariance(int x[], int y[], int n, double meanX, double meanY) {
	double sum = 0;
	for (int i = 1, nn = n / 2; i <= nn; i++) {
		sum += (x[i] - meanX) * (y[i] - meanY);
		sum += (x[n - i + 1] - meanX) * (y[n - i + 1] - meanY);
	}
	return sum / ((double)n - 1);
}

//calculates pearson's correlation coefficient r using covariance function
double pearson(double cov, double stdevx, double stdevy) {
	return cov / stdevx / stdevy;
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
		input(argv[1], &n);
	}
	else input("data1.csv", &n);

	//variables for each quesinton, we store each function's answer in here for later use
	double meanx, meany, modex, modey, varx, vary, stdevx, stdevy, madx, mady, q1x, q1y, skewx, skewy, kurtx, kurty, cov, r, a, b;

	//processes the input
	//process();
	
	//calculates the function and puts them in their respective variable
	meanx = mean(x, n);
	meany = mean(y, n);

	varx = calVariance(x, n, meanx);
	vary = calVariance(y, n, meany);

	stdevx = calStandardDeviation(varx);
	stdevy = calStandardDeviation(vary);

	skewx = skew(x, stdevx, meanx, n);
	skewy = skew(y, stdevy, meany, n);

	kurtx = kurt(x, stdevx, meanx, n);
	kurty = kurt(y, stdevy, meany, n);

	cov = calCovariance(x, y, n, meanx, meany);

	r = pearson(cov, stdevx, stdevy);

	//sets precision to 6 digits after decimal place
	cout.precision(6);
	cout << fixed;

	//print answers
	cout << "mean : " << meanx << ' ' << meany << endl;
	cout << "variance : " << varx << ' ' << vary << endl;
	cout << "standard deviation : " << stdevx << ' ' << stdevy << endl;
	cout << "skew : " << skewx << ' ' << skewy << endl;
	cout << "kurt : " << kurtx << ' ' << kurty << endl;
	cout << "Covariance : " << cov << endl;
	cout << "pearson : " << r << endl;
	return 0;
}