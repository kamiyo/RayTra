
#include <cstdlib>
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <cstring>
#include <cmath>
#include <Eigen/Core>
#include <cmath>
#include <boost/math/distributions/students_t.hpp>

#define M_PI 3.14159265358979323846

using namespace boost::math;
using namespace std;



bool _startup(const char* lpAppName, int argc, char* argv) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	int success = CreateProcess(lpAppName,
		argv,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);
	if (!success) {
		std::cout << "failed to run " << lpAppName << endl;
		return success;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return true;
}

int main(int argc, char** argv) {
	if (argc < 4) {
		std::cout << "invalid number of arguments" << endl;
	}
	if (strcmp(argv[1], "/c") == 0) {
		std::cout << "comparing " << argv[3] << " and " << argv[4] << " using " << argv[2] << " samples." << endl;
		int n = atoi(argv[2]);
		Eigen::VectorXd times_1;
		Eigen::VectorXd times_2;
		times_1.resize(n);
		times_2.resize(n);
		string s1, s2; s1 = s2 = "";
		for (int i = 3; i < argc; i++) {
			if (i == 3) {
				s1 = argv[i];
			} else if (i == 4) {
				s2 = argv[i];
			}
			else {
				s1 += " ";
				s2 += " ";
				s1 += argv[i];
				s2 += argv[i];
			}

		}
		
		std::cout << s1 << endl << s2 << std::endl;
		char* cmd1 = (char*) malloc(s1.length() + 1);
		char* cmd2 = (char*) malloc(s2.length() + 1);
		strcpy_s(cmd1, s1.length() + 1, s1.c_str());
		strcpy_s(cmd2, s2.length() + 1, s2.c_str());

		for (int i = 0; i < n; i++) {
			std::cout << argv[3] << ": run " << i << endl << endl;
			clock_t start = clock();
			_startup(argv[3], argc - 4, cmd1);
			clock_t stop = clock();
			times_1[i] = (double) (stop - start);
		}

		for (int i = 0; i < n; i++) {
			std::cout << argv[4] << ": run " << i << endl << endl;
			clock_t start = clock();
			_startup(argv[4], argc - 4, cmd2);
			clock_t stop = clock();
			times_2[i] = (double) (stop - start);
		}

		double mean_1 = times_1.mean();
		times_1.array() -= mean_1;
		times_1.array() *= times_1.array();
		double var_1 = times_1.sum() / (n - 1);
		std::cout << argv[3] << " mean: " << mean_1 / CLOCKS_PER_SEC << " stdev: " << sqrt(var_1) / CLOCKS_PER_SEC << endl;

		double mean_2 = times_2.mean();
		times_2.array() -= mean_2;
		times_2.array() *= times_2.array();
		double var_2 = times_2.sum() / (n - 1);
		std::cout << argv[4] << " mean: " << mean_2 / CLOCKS_PER_SEC << " stdev: " << sqrt(var_2) / CLOCKS_PER_SEC << endl;

		double sx1x2 = sqrt((var_1 + var_2) / n);
		double t = (mean_1 - mean_2) / sx1x2;
		double df = (var_1 + var_2) * (var_1 + var_2) * (n - 1) / (var_1 * var_1 + var_2 * var_2);

		std::cout << "t: " << t << " df: " << df << std::endl;

		students_t dist(df);
		double q = cdf(complement(dist, abs(t)));
		double qless = cdf(dist, t);
		double qmore = cdf(complement(dist, t));
		double alpha = 0.05;

		std::cout << "probability difference due to chance: " << q << endl;
		std::cout << argv[3] << " != " << argv[4] << ((q > (alpha / 2)) ? (" REJECTED (NO)") : (" NOT REJECTED (YES)")) << endl;
		std::cout << argv[3] << " < " << argv[4] << ((qless > alpha) ? (" REJECTED (NO)") : (" NOT REJECTED (YES)")) << endl;
		std::cout << argv[3] << " > " << argv[4] << ((qmore > alpha) ? (" REJECTED (NO)") : (" NOT REJECTED (YES)")) << endl;

	}
	else {
		std::cout << "running " << argv[2] << " " << argv[1] << " times." << endl;
		int n = atoi(argv[1]);
		Eigen::VectorXd times;
		times.resize(n);
		string s = "";
		for (int i = 2; i < argc; i++) {
			s += argv[i];
			if (i != argc - 1) {
				s += " ";
			}
		}

		std::cout << s << std::endl;
		char* cmd = (char*) malloc(s.length() + 1);
		strcpy_s(cmd, s.length() + 1, s.c_str());

		std::cout << cmd << std::endl;
		for (int i = 0; i < n; i++) {
			clock_t start = clock();
			_startup(argv[2], argc - 2, cmd);
			clock_t stop = clock();
			times[i] = (double) (stop - start);
		}

		double mean = times.mean();
		times.array() -= mean;
		times.array() *= times.array();
		double var = times.sum() / (n - 1);
		double stdev = sqrt(var);
		std::cout << "mean: " << mean / CLOCKS_PER_SEC << endl;
		std::cout << "stdev: " << stdev / CLOCKS_PER_SEC << endl;
	}
	//tgamma
	return 0;
}