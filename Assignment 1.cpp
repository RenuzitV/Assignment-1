#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef int ti;
const int N = 5e4 + 5;
ti x[N], y[N], n = 1;
double meanx, meany, modex, modey, varx, vary, stdevx, stdevy, madx, mady, q1x, q1y, skewx, skewy, kurtx, kurty, cov, r, a, b;
// khanh dang iu
void sort(ti* x, ti* y) {
	ti* mid = x;
	for (int* i = x + 1; i != y; ++i) {
		if (*mid > *i) {
			swap(*mid, *i);
			++mid;
			swap(*mid, *i);
		}
	}
	if (mid - x > 1){
		sort(x, mid);
	}
	if (y - mid > 1) {
		sort(mid + 1, y);
	}
}

void input() {
	scanf_s("%*[^\n]s%*c");
	while (scanf_s("%d,%d", &x[n], &y[n]) != EOF) ++n; 
	--n;
	sort(x + 1, x + n + 1);
	sort(y + 1, y + n + 1);
}

double mean(ti *x, int n) {

	return 0;
}

int main(){
	FILE* F;
	freopen_s(&F, "data1.csv", "r", stdin);
	input(); //input is read in X and Y, 1-indexed and sorted
	return 0;
}