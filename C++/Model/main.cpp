#include <iostream>
#include <cstdlib>      /* srand, rand */
#include <ctime>       /* time */
#include <mpi.h>

#include "Structs.h"
#include "Model.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Initialize the MPI environment.
	MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);

	// Get the rank of the process
	int	world_rank = MPI::COMM_WORLD.Get_rank();

	if (world_rank == 0)
		if (system("CLS")) system("clear");

	srand(time(NULL) + world_rank);

	MPI_Barrier(MPI::COMM_WORLD);
	if ( argc == 3 ) {
		// this is the name of the folder that contains the model configuration
		std::string	folderName = argv[1];
		std::string	mode = argv[2];
		std::string	inference = "inference";
		std::string	training = "training";

		if ( mode.compare(inference) == 0 ) {
			MPI_Barrier(MPI::COMM_WORLD);
			// generates object of Model class 
			Model	model(folderName, false);

			// run inference
			model.run(folderName);
			MPI_Barrier(MPI::COMM_WORLD);
		}
		else if ( mode.compare(training) == 0 ) {
			MPI_Barrier(MPI::COMM_WORLD);
			// generates object of Model class 
			Model	model(folderName, true);

			// train the model 
			model.train(folderName);
			MPI_Barrier(MPI::COMM_WORLD);
		}
		else {
			if ( world_rank == 0 ) {
				std::cout << "\nsecond argument must be \"inference\"" << std::endl;
				std::cout << "or \"training\"" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD, 1);
		}

		MPI::Finalize();
		return	0;
	}
	else {
		// Tell the user how to run the program
		if (world_rank == 0) {
			std::cerr << "\nUsage: " << argv[0] << " folderName inference" << std::endl;
			std::cerr << "Or usage: " << argv[0] << " folderName training" << std::endl;
			std::cerr << "folderName: this is the name of the folder where the model structure resides" << std::endl;
			std::cerr << "inference: the model executes inference with the input corpus" << std::endl;
			std::cerr << "training: the model executes training with the input corpus" << std::endl;
		}
		MPI::Finalize();
		return 1;
       }
}
