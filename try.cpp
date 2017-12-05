#include <iostream>

using namespace std;

float power(float a, int n) {
	float res = 1;
	for(int i=0; i<n; i++) {
		res *= a;
	}
	return res;
}

int main() {
	float a = power(0.7, 3);
	cout<<a<<endl;
	return 0;
}