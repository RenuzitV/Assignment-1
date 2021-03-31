#include <iostream>
#include <fstream>
#include <string>
#define sqr(x) ((x)*(x))
#define cub(x) ((x)*sqr(x))
#define qud(x) (sqr(x)*sqr(x))

using namespace std;

//main input variables
//*x and *y are pointers to int aka arrays
int *x, *y, n;

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

//reallocates x and y to some size nn, error-proof as far as testing goes
void reloc(int** x, int** y, int nn) {
	if (nn < 1) {
		cerr << "cannot allocate memory";
		exit(-1);
	}
	//if either x or y is uninitialized, call malloc instead of realloc
	if (!*x || !*y) {
		*x = (int*)malloc(sizeof(int) * nn);
		*y = (int*)malloc(sizeof(int) * nn);
		if (!*x || !*y) {
			cerr << "cannot allocate memory";
			exit(-1);
		}
		return;
	}

	//try to realloc x and y to tx and ty of size nn
	int* tx = (int*)realloc(*x, sizeof(int) * nn), * ty = (int*)realloc(*y, sizeof(int) * nn);

	//if either tx or ty cannot be allocated
	if (!tx || !ty) {
		cerr << "cannot allocate memory";
		exit(-1);
	}
	else *x = tx, *y = ty; //assigns pointer x and y to tx and ty
}

//main input function, X and Y is read in 1-indexed from a file
void input(const char s[], int **x, int **y, int *n) {
	*n = 1;
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
	while (scanf_s("%d,%d", &((*x)[*n]), &((*y)[*n])) != EOF) { 
		++*n;
		if (*n == nn - 1) {
			nn *= 2; //2 is arbitrary, can adjust to any number > 1
			//the idea is to increase the size of the array by a dramatic ammount, so we dont have to reallocate per input
			//for inputs up to 6e4, we only need to reallocate 8 times
			//the number of times we need to reallocate can be computed as log(size, multiplier)
			reloc(&*x, &*y, nn);
		}
	}
	--*n; //sets n to the correct number of items
	//reallocates x and y to the correct size of items + 5 for safety measures
	reloc(&*x, &*y, *n + 5);
}

//processes data in some sort of way
void process() {
	//sorts x and y in non-decreasing order
	sort(x + 1, x + 1 + n);
	sort(y + 1, y + 1 + n);
}

//returns the average number of the input aka arithmetic mean
double mean(int *x, int n) {
	double res = 0;
	for (int i = 1; i <= n; ++i) {
		res += x[i];
	}
	return res / n;
}

//calculates variance from a set of input
double calVariance(int* a, int n, double mean) {
	double sum = 0;
	for (int i = 1; i <= n; i++) {
		sum += sqr(a[i] - mean);
	}
	return sum / ((double)n - 1);
}

//calculates stdardard deviation via variance
double calStandardDeviation(double variance) {
	return sqrt(variance);
}

//calculates skew of a data set
double skew(int x[], double sd, double m, int n) {
	double res = 0;
	for (int i = 1; i <= n; ++i) {
		//cub is a macro for cube function aka f(x) = x^3
		res += cub(x[i] - m);
	}

	return res / n / cub(sd);
}

//calculates kurtosis from a set, using precomputed mean and standard deviation
double kurt(int x[], double sd, double m, int n) {
	double res = 0;
	double qsd = qud(sd);
	for (int i = 1; i <= n; ++i) {
		//qud is a macro for quad function aka f(x) = x^4
		//we divide each qud() by n and qsd to avoid overflowing
		res += qud((double)x[i] - m) / ((long long)n) / qsd;
	}
	return res - 3;
}

//calculates covariance from two sets of data, using meanX and meanY as precomputed values
double calCovariance(int x[], int y[], int n, double meanX, double meanY) {
	double sum = 0;
	for (int i = 1; i <= n; i++) {
		sum += (x[i] - meanX) * (y[i] - meanY);
	}
	return sum / ((double)n - 1);
}

//calculates pearson's correlation coefficient r using covariance and standard deviation function
double pearson(double cov, double stdevx, double stdevy) {
	return cov / stdevx / stdevy;
}


//********************************************			main function			********************************************
int main(int argc, const char* argv[]){
	//gets cmd line argument and runs it
	/*
	if (argc != 2) {
		cerr << "wrong argument";
		return -1;
	}
	input(argv[1], &n);
	*/

	//for testing purposes if no cmd line arguments are given, use data1.csv as default data file
	if (argc == 2) {
		input(argv[1], &x, &y, &n);
	}
	else input("data1.csv", &x, &y, &n);

	//variables for each quesinton, we store each function's answer in here for later use
	double meanx, meany, modex, modey, varx, vary, stdevx, stdevy, madx, mady, q1x, q1y, skewx, skewy, kurtx, kurty, cov, r, a, b;

	//processes the input
	//commented out because order matters for some functions
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