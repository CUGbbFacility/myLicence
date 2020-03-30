// pthreadTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <pthread.h>
#include <vector>
#include <iostream>
using namespace std;
#define SIZE 8
#define NUM_THREAD 4

struct{
	vector<vector<int>> rankMatrix;
	int thread_per_length;
}myData;

vector<vector<long>> myMatrix = {
	{ 1, 2, 3, 4, 5, 6, 7, 8 },
	{ 9, 10, 11, 12, 13, 14, 15, 16 },
	{ 17, 18, 19, 20, 21, 22, 23, 24 },
	{ 25, 26, 27, 28, 29, 30, 31, 32 },
	{ 33, 34, 35, 36, 37, 38, 39, 40 },
	{ 41, 42, 43, 44, 45, 46, 47, 48 },
	{ 49, 50, 51, 52, 53, 54, 55, 56 },
	{ 57, 58, 59, 60, 61, 62, 63, 64 },
};

vector<int> inivec(int n)
{
	vector<int> vec;
	for (int i = 0; i < n; i++)
		vec.push_back(i);
	return vec;
}

void swap(vector<int>& vec, int i, int j)
{
	int temp = vec[i];
	vec[i] = vec[j];
	vec[j] = temp;
}

void Perm(vector<int>& vec, vector<vector<int> >& vec_seq, int current_index = 0)
{
	if (current_index == ((int)vec.size() - 1))
	{
		vec_seq.push_back(vec);
	}
	else
	{
		for (int i = current_index; i < (int)vec.size(); i++)
		{
			swap(vec, i, current_index);
			Perm(vec, vec_seq, current_index + 1);
			swap(vec, i, current_index);
		}
	}
}
bool nxs(vector<int>& vec){
	int count = 0;
	for (int i = 0; i < (int)vec.size(); i++)
	{
		for (int j = i + 1; j < (int)vec.size(); j++)
		{
			if (vec[i] > vec[j])
			{
				count += 1;
			}
		}
	}
	return ((count & 1) == 0);
}

long colMatrix(vector<vector<int> >& allRank, vector<vector<long>>& matrix){
	long count = 0;
	long aa = 1;//逆序数
	long bb;
	for (int i = 0; i < allRank.size(); i++){
		bb = 1;
		if (nxs(allRank[i]) == true)
			aa = 1;
		else
			aa = -1;
		for (int j = 0; j < SIZE; j++)
			bb *= matrix[j][allRank[i][j]];
		count += aa*bb;
	}
	return count;
}

long count1 = 0;
void* col(void *args){
	long aa = 1;//逆序数
	long bb;
	int start;
	start = (int)(*((int*)args));
	for (int i = myData.thread_per_length * start; i < start + myData.thread_per_length; i++){
		bb = 1;
		if (nxs(myData.rankMatrix[i]) == true)
			aa = 1;
		else
			aa = -1;
		for (int j = 0; j < SIZE; j++)
			bb *= myMatrix[j][myData.rankMatrix[i][j]];

		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		count1 += (aa*bb);
		int pthread_mutex_destroy(pthread_mutex_t *mutex);
	}
	return NULL;
}

int main(){
	vector<int> vec1 = inivec(SIZE);
	Perm(vec1, myData.rankMatrix);
	myData.thread_per_length = myData.rankMatrix.size() / NUM_THREAD;
	int rc;
	pthread_t thread[NUM_THREAD];
	
	for (int t = 0; t < NUM_THREAD; t++){
		rc = pthread_create(&thread[t], NULL, col, &t);
	}
	for (int i = 0; i < NUM_THREAD; i++){
		pthread_join(thread[i], NULL);
	}
	free(thread);
	cout << count1 << endl;
	return 0;
}