#include <iostream>
#include <cstdlib>      /* srand, rand */
#include <ctime>       /* time */
#include <mpi.h>

#include "Structs.h"
#include "Model.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (system("CLS")) system("clear");

	// Initialize the MPI environment.
	MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);

	// Get the rank of the process
	int	world_rank = MPI::COMM_WORLD.Get_rank();

	srand(time(NULL) + world_rank);

	MPI_Barrier(MPI::COMM_WORLD);
	if ( argc == 3 ) {
		// this is the name of the folder that contains the model configuration
		std::string	folderName = argv[1];
		std::string	inference = argv[2];
		std::string	auxiliary = "inference";

		if ( inference.compare(auxiliary) != 0 ) {
			if ( world_rank == 0 ) {
				std::cout << "\nsecond argument must be \"inference\"" << std::endl;
			}
			MPI_Barrier(MPI::COMM_WORLD);
			MPI_Abort(MPI::COMM_WORLD, 1);
		}

		MPI_Barrier(MPI::COMM_WORLD);
		// generates object of Model class 
		Model	model(folderName);

		// run inference
		model.run(folderName);
		MPI_Barrier(MPI::COMM_WORLD);

		return	0;
	}
	else if ( argc == 5 ) {
		// this is the name of the folder that contains the model configuration
		std::string	folderName = argv[1];
		std::string	training = argv[2];
		std::size_t	iterations = std::atoi(argv[3]);	// in the model conditions we advise to use iterations = 2;
		std::size_t	stages = std::atoi(argv[4]);		// in the model conditions we advise to use stages = 3;
		std::string	auxiliary = "training";

		if ( training.compare(auxiliary) != 0 ) {
			if ( world_rank == 0 ) {
				std::cout << "\nsecond argument must be \"training\"" << std::endl;
			}
			MPI_Barrier(MPI::COMM_WORLD);
			MPI_Abort(MPI::COMM_WORLD, 1);
		}

		if ( world_rank == 0 ) {
			std::cout << "\nThe number of stages in each layer training process is: " << stages << std::endl;
			std::cout << "\nThe number of iterations in every stage of the training process is: " << iterations << std::endl;
		}

		MPI_Barrier(MPI::COMM_WORLD);
		// generates object of Model class 
		Model	model(folderName);

		// train the model 
		model.train(folderName, iterations, stages);
		MPI_Barrier(MPI::COMM_WORLD);

		return	0;
	}
	else {
		// Tell the user how to run the program
		if (world_rank == 0) {
			std::cerr << "\nUsage: " << argv[0] << " folderName inference" << std::endl;
			std::cerr << "Or usage: " << argv[0] << " folderName training iterations stages" << std::endl;
			std::cerr << "folderName: this is the name of the folder where the model structure resides" << std::endl;
			std::cerr << "inference: the model executes inference with the input corpus" << std::endl;
			std::cerr << "training: the model executes training with the input corpus" << std::endl;
			std::cerr << "iterations: the number of iterations in every stage of the training process" << std::endl;
			std::cerr << "stages: the number of stages in each layer training process" << std::endl;
		}
		return 1;
       }
}
