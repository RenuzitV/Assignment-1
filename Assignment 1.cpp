#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#define sqr(x) ((x)*(x))
#define cub(x) ((x)*sqr(x))
#define qud(x) (sqr(x)*sqr(x))

using namespace std;

//main input variables
//*x and *y are pointers to int aka arrays
double* x, * y;
int n;

//quicksort algorithm
void sort(double* x, double* y) {
	double* mid = x; //choose pivot as first element
	for (double* i = x + 1; i != y; ++i) {
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

//reallocates x and y to some size nn, error-proof as far as testing goes
void reloc(double** x, double** y, int nn) {
	if (nn < 1) {
		cerr << "cannot allocate memory";
		exit(-1);
	}
	//if either x or y is uninitialized, call malloc instead of realloc
	if (!*x || !*y) {
		*x = (double*)malloc(sizeof(double) * nn);
		*y = (double*)malloc(sizeof(double) * nn);
		if (!*x || !*y) {
			cerr << "cannot allocate memory";
			exit(-1);
		}
		return;
	}

	//try to realloc x and y to tx and ty of size nn
	double* tx = (double*)realloc(*x, sizeof(double) * nn), * ty = (double*)realloc(*y, sizeof(double) * nn);

	//if either tx or ty cannot be allocated
	if (!tx || !ty) {
		cerr << "cannot allocate memory";
		exit(-1);
	}
	else *x = tx, *y = ty; //assigns pointer x and y to tx and ty
}

//main input function, X and Y is read in 1-indexed from a file
void input(const char s[], double **x, double **y, int *n) {
	//opens the file and transfer it to the standard input
	FILE* F;
	freopen_s(&F, s, "r", stdin);
	if (!F) {
		cout << "cant open file";
		exit(-1);
	}

	//scans the first line and ignore it
	scanf_s("%*[^\n]s%*c");

	int nn = 4; // current array size
	reloc(&*x, &*y, nn); //sets x and y to size nn

	//while there is still input, read it
	//bit of explaination : 
	//**x : pointer to array x
	//*x : array x
	//*x[*n] : element number *n in array x
	// &((*x)[*n]) : pointer to the former variable
	//we add in a bunch of parentheses to avoid compiler confusion, such as &*x just means x, and x[*n] is not something we want to access
	while (scanf_s("%lf,%lf", &((*x)[*n]), &((*y)[*n])) != EOF) { 
		++*n;
		if (*n == nn) {
			nn *= 2; //2 is arbitrary, can adjust to any number > 1
			//the idea is to increase the size of the array by a dramatic ammount, so we dont have to reallocate per input
			//for inputs up to 6e4, we only need to reallocate 8 times
			//the number of times we need to reallocate can be computed as log(size, multiplier)
			reloc(x, y, nn);
		}
	}
	//reallocates x and y to the correct size of items + 5 for safety measures
	reloc(x, y, *n + 5);
}

//processes data in some sort of way
void process() {
	//sorts x and y in non-decreasing order
	sort(x, x+n);
	sort(y, y+n);
}

//returns the average number of the input aka arithmetic mean
double mean(double *x, int n) {
	double res = 0;
	for (int i = 0; i < n; ++i) {
		res += x[i];
	}
	return res / n;
}

// function to return the most frequent element(s)
void mode(double* a, int size, int *nz, double **res) {
	if (size < 1) return;
	*res = (double*)malloc(sizeof(double) * size);
	if (!*res) return;
	int max_count = 0, i = 1, pre = 0, nres = 0;
	while (i < size) {
		if (a[i] != a[i - 1]) {
			int cur_count = i - pre;
			if (max_count <= cur_count) {
				if (max_count == cur_count) {
					if (nres < size) (*res)[nres++] = a[pre];
				}
				else {
					max_count = cur_count;
					(*res)[0] = a[pre];
					nres = 1;
				}
			}
			pre = i;
		}
		i++;
	}
	//one last check at i = size
	int cur_count = i - pre;
	if (max_count <= cur_count) {
		if (max_count == cur_count) {
			if (nres < size) (*res)[nres++] = a[pre];
		}
		else {
			max_count = cur_count;
			(*res)[0] = a[pre];
			nres = 1;
		}
	}
	* nz = nres;
	double *t = (double*)realloc(*res, sizeof(double) * nres);
	if (t) *res = t;
}

//Function to find the Median of the array
double findMedian(double arr[], int n) {
	if (n % 2 == 0) {
		return ((double)arr[(n - 1) / 2] + arr[n / 2]) / 2;
	}
	else return arr[n / 2];
}

//Function to find the Mean Absolute Deviation 
double findMAD(double arr[], int n, double m) {
	double sum = 0;
	for (int i = 0; i < n; i++) {
		sum += fabs(arr[i] - m);
	}
	return sum / ((double) n - 1);
}

//Function to find First Quartile accounting median
double findQ1(double arr[], int n) {
	return findMedian(arr, n/2);
}

//calculates variance from a set of input
double calVariance(double* a, int n, double mean) {
	double sum = 0;
	for (int i = 0; i < n; i++) {
		sum += sqr(a[i] - mean);
	}
	return sum / ((double)n - 1);
}

//calculates stdardard deviation via variance
double calStandardDeviation(double variance) {
	return sqrt(variance);
}

//calculates skew of a data set
double skew(double x[], double sd, double m, int n) {
	double res = 0;
	for (int i = 0; i < n; ++i) {
		//cub is a macro for cube function aka f(x) = x^3
		res += cub(x[i] - m);
	}

	return res / ((double)n - 1) / cub(sd);
}

//calculates kurtosis from a set, using precomputed mean and standard deviation
double kurt(double x[], double sd, double m, int n) {
	double res = 0;
	double qsd = qud(sd);
	for (int i = 0; i < n; ++i) {
		//qud is a macro for quad function aka f(x) = x^4
		//we divide each qud() by n and qsd to avoid overflowing
		res += qud((double)x[i] - m) / ((long long)n) / qsd;
	}
	return res - 3;
}

//calculates covariance from two sets of data, using meanX and meanY as precomputed values
double calCovariance(double x[], double y[], int n, double meanX, double meanY) {
	double sum = 0;
	for (int i = 0; i < n; i++) {
		sum += (x[i] - meanX) * (y[i] - meanY);
	}
	return sum / ((double)n - 1);
}

//calculates pearson's correlation coefficient r using covariance and standard deviation function
double pearson(double cov, double stdevx, double stdevy) {
	return cov / stdevx / stdevy;
}

// fnction find and print out linear regression
void linear_regression(double mean_a, double mean_b, double stdev_a, double stdev_b, double cor_coeff) {
	double slope = cor_coeff * stdev_b / stdev_a;
	double intercept = mean_b - slope * mean_a;
	cout << "y = " << slope << "x + " << intercept << "\n";
}
//********************************************			main function			********************************************
int main(int argc, const char* argv[]){
	if (argc != 2) {
		cerr << "wrong argument";
		return -1;
	}
	input(argv[1], &x, &y, &n);

	////for testing purposes, if no cmd line arguments are given, use data1.csv as default data file
	//if (argc == 2) {
	//	input(argv[1], &x, &y, &n);
	//}
	//else input("data1.csv", &x, &y, &n);

	//variables for each quesinton, we store each function's answer in here for later use
	double medx, medy, meanx, meany, *modex, *modey, varx, vary, stdevx, stdevy, madx, mady, q1x, q1y, skewx, skewy, kurtx, kurty, cov, r;

	//calculates the function and puts them in their respective variable
	meanx = mean(x, n);
	meany = mean(y, n);

	varx = calVariance(x, n, meanx);
	vary = calVariance(y, n, meany);

	stdevx = calStandardDeviation(varx);
	stdevy = calStandardDeviation(vary);

	madx = findMAD(x, n, meanx);
	mady = findMAD(y, n, meany);

	skewx = skew(x, stdevx, meanx, n);
	skewy = skew(y, stdevy, meany, n);

	kurtx = kurt(x, stdevx, meanx, n);
	kurty = kurt(y, stdevy, meany, n);

	cov = calCovariance(x, y, n, meanx, meany);

	r = pearson(cov, stdevx, stdevy);

	//processes the input
	process();

	medx = findMedian(x, n);
	medy = findMedian(y, n);

	q1x = findQ1(x, n);
	q1y = findQ1(y, n);

	int nmx = 0, nmy = 0;
	mode(x, n, &nmx, &modex);
	mode(y, n, &nmy, &modey);

	//sets precision to 6 digits after decimal place
	cout.precision(6);
	cout << fixed;

	//print answers
	cout << "median_x = " << medx << " - median_y = " << medy << endl;
	cout << "mode_x = {";
	for (int i = 0; i < nmx; ++i) {
		cout << modex[i] << (i == nmx - 1 ? "" : ", ");
	}
	cout << "} - mode_y : {";
	for (int i = 0; i < nmy; ++i) {
		cout << modey[i] << (i == nmy - 1 ? "" : ", ");
	}
	cout << "}\n";
	cout << "var_x = " << varx << " - var_y = " << vary << endl;
	cout << "stdev_x = " << stdevx << " - stdev_y = " << stdevy << endl;
	cout << "mad_x = " << madx << " - mad_y = " << mady << endl;
	cout << "q1_x = " << q1x << " - q1_y = " << q1y << endl;
	cout << "skew_x = " << skewx << " - skew_y = " << skewy << endl;
	cout << "kurt_x = " << kurtx << " - kurt_y = " << kurty << endl;
	cout << "cov(x_y) = " << cov << endl;
	cout << "r(x_y) = " << r << endl;
	linear_regression(meanx, meany, stdevx, stdevy, r);
	return 0;
}