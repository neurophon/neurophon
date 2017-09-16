#include <iostream>

#include "Structs.h"
#include "Model.h"

using namespace std;

int main()
{
	srand(time(NULL));

	// this is the name of the folder that contains the model configuration
	std::string	folderName = "Model2";

	// generates object of Model class 
	Model	model(folderName);

	std::size_t	iterations = 2;
	std::size_t	stages = 3;
	if (true)
		model.train(folderName, iterations, stages);
	else
		model.run(folderName);

	std::cout << '\a';

	return	0;
}
