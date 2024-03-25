#include <iostream>
using namespace std;

static int add(int a, int b){
	return a + b;
}

int main(){

	int x  = add(100, 99);

	cout << x << endl;
	
	return 0;
}
