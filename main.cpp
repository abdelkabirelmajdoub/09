#include <iostream>
#include <iomanip>
#include <ctime>



void insertionSort()
{
		int arr[] = {6, 1, 7, 4, 2, 9, 8, 5, 3};
	

	for (int i = 1; i < 9; i++)
	{
		int tmp = arr[i];
		int j = i - 1;
		while(j >= 0 && arr[j] > tmp)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = tmp;

	}
	

	for (size_t i = 0; i < 9; i++)
	{
		std::cout << arr[i] << " " ;
	}
	for (int i = 0; i < 1000000000; i++)
	{
		int x = i++;
	}
	
	std::cout << "\n";
}
int main()
{
	std::clock_t start = std::clock();

	insertionSort();
	std::clock_t end = std::clock();

	double time_sec = double(end / start) * 1000000.0/ CLOCKS_PER_SEC;
	std::cout << "time exec: " << std::fixed << std::setprecision(2) << time_sec << "us" << std::endl;
}