#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <time.h>
#include <iomanip>      // std::setprecision
using namespace std;

int main(){
	srand(time(NULL));
	ofstream file;
	file.open("randomInputFile.txt");
	int temp, k,p,t,c,n;

	// Take input for processing time
	cout << "\nProcessing time : ";
	cin >> temp;
	file << temp << endl;

	// Take input for k (the number of papers per session)
	cout << "\nk (the number of papers per session) : ";
	cin >> k;
	file << k << endl;

	// Take input  p (the number of parallel sessions)
	cout << "\np (the number of parallel sessions) : ";
	cin >> p;
	file << p << endl;

	// Take input  t (the number of time slots)
	cout << "\nt (the number of time slots) : ";
	cin >> t;
	file << t << endl;

	//Take input  C (the tradeoff parameter)
	cout << "\nC (the tradeoff parameter) : ";
	cin >> c;
	file << c << endl;

	n = p*k*t;
	cout << "\nTotal no. of papers : " << n << "\n\n";

	double distance = 0;
	vector< vector<double> > mat(n,(vector<double>(n,0)));				// distance matrix initialized to zero
	
	// insert random values between 0 and 1
	for (int i=0;i<n;i++){
		for (int j=0;j<n;j++){
			if (i<j) {
				mat[i][j] = ((double) rand() / (RAND_MAX));				
			} else if (i>j) mat[i][j] = mat[j][i];
		}
	}


	// output to file
	for (int i=0;i<n;i++){
		for (int j=0;j<n;j++){
			file << setprecision(2) << mat[i][j] << " ";
		}
		file << "\n";
	}	

	file.close();
}