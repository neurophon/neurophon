#include <iostream>
#include <mpi.h>

#include "MFCC.h"

#include "../Libraries/Model/GlobalVariables.h"
#include "../Libraries/Model/Constants.h"
#include "../Libraries/Model/DataTypes.h"
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/MatlabInterface.h"

using namespace std;

bool	big_endianness;
std::string	path;

int main(int argc, char* argv[])
{
	// Initialize the MPI environment.
	MPI::Init_thread(argc, argv, MPI_THREAD_FUNNELED);

	// Get the rank of the process
	int	world_rank = MPI::COMM_WORLD.Get_rank();

	// Get the number of processes
	int	world_size = MPI::COMM_WORLD.Get_size();

	std::cout << "world_size: " << world_size << "\n";
	if (world_rank == 0)
		if (system("CLS")) system("clear");

	std::string	my_path = "/projects/neurophon/TestsData/NewCorpora/";
	MPI_Barrier(MPI::COMM_WORLD);
	if ( argc == 5 ) {
		int	numberOfVoices = atoi(argv[1]);
		int	numberOfSyllableConfigurations = atoi(argv[2]);
		int	numbeOfCorpora = atoi(argv[3]);
        	int	numbeOfVocabularies = atoi(argv[4]);

		for ( int voice = 1; voice <= numberOfVoices; voice++ ) {
			for ( int syllables = 1; syllables <= numberOfSyllableConfigurations; syllables++ ) {
				for ( int task = world_rank; task < numbeOfCorpora*numbeOfVocabularies; task=task+world_size ) {
					int	corpus = (int)task/numbeOfVocabularies;
					int	vocabulary = task%numbeOfVocabularies;
					std::string	iterativePath = my_path + "Voices_" + std::to_string(voice) + "/"
										+ "Syllables_" + std::to_string(syllables) + "/"
									       	+ "Corpus_" + std::to_string(corpus) + "/"
									       	+ "Vocabulary_" + std::to_string(vocabulary) + "/";

					MFBS(iterativePath);
				}
			}
		}


		MPI::Finalize();
		return	0;
	}
	else if ( argc == 6 ) {
		int	numberOfVoices = atoi(argv[1]);
		int	numberOfSyllableConfigurations = atoi(argv[2]);
		int	numbeOfCorpora = atoi(argv[3]);
        	int	numbeOfVocabularies = atoi(argv[4]);
		std::string	mode = argv[5];

		if ( mode.compare("variance") == 0 ) {
			MPI_Barrier(MPI::COMM_WORLD);
			for ( int voice = 1; voice <= numberOfVoices; voice++ ) {
				for ( int syllables = 1; syllables <= numberOfSyllableConfigurations; syllables++ ) {
					for ( int task = world_rank; task < numbeOfCorpora*numbeOfVocabularies; task=task+world_size ) {
						int	corpus = (int)task/numbeOfVocabularies;
						int	vocabulary = task%numbeOfVocabularies;
						std::string	iterativePath = my_path + "Voices_" + std::to_string(voice) + "/"
											+ "Syllables_" + std::to_string(syllables) + "/"
											+ "Corpus_" + std::to_string(corpus) + "/"
											+ "Vocabulary_" + std::to_string(vocabulary) + "/";

						MFBS(iterativePath + "original/");
						MFBS(iterativePath + "whitenoise1/");
						MFBS(iterativePath + "whitenoise2/");
						MFBS(iterativePath + "reverberation30/");
						MFBS(iterativePath + "reverberation60/");
						MFBS(iterativePath + "pitchdown/");
						MFBS(iterativePath + "pitchup/");
					}
				}
			}
			MPI_Barrier(MPI::COMM_WORLD);
		}
		else {
			if ( world_rank == 0 ) {
				std::cout << "\nfifth argument must be a string with the word \"variance\" in it" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD, 1);
		}



		MPI::Finalize();
		return	0;
	}
	else {
		// Tell the user how to run the program
		if (world_rank == 0) {
			std::cerr << "\nUsage: " << argv[0] << " numberOfVoices numberOfSyllableConfigurations numbeOfCorpora numbeOfVocabularies" << std::endl;
			std::cerr << "\nExample of use: " << argv[0] << " 3 4 5 5" << std::endl;
			std::cerr << "\nOr usage: " << argv[0] << " numberOfVoices numberOfSyllableConfigurations numbeOfCorpora numbeOfVocabularies \"variance\""
				  << std::endl;
			std::cerr << "\nExample of use: " << argv[0] << " 3 4 5 5 variance" << std::endl;
			std::cerr << "numberOfVoices: number of set of voices" << std::endl;
			std::cerr << "numberOfSyllableConfigurations: number of sets of words with different number of syllables" << std::endl;
			std::cerr << "numbeOfCorpora: number of corpora per each configuration of voices and syllables" << std::endl;
			std::cerr << "numbeOfVocabularies: number of vocabularies per each corpus" << std::endl;
		}
		MPI::Finalize();
		return 1;
       }
}
