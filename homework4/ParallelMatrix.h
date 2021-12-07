#pragma once
#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>>  matrix_tp;

class ParallelMatrix {
	matrix_tp  matrix;
	int row_count;
	int column_count;
public:	
	ParallelMatrix(int n, int m);
	void Init();
	int Sum();
	int SumParallel(int k);

};
