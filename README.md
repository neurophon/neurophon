Neurocomputational cortical memory for spectro-temporal phonetic abstraction.
============================================================================

A new approach for deep phonetic acquisition
--------------------------------------------

The human brain is the most complex object created by evolution in the known universe.
Yet, how much of this complexity is devoted to exclusively carrying out its algorithmic capabilities
and how much of it has been inherited from biological paths of evolution in order
to work properly in its physical environment?
What if the information processing properties of the brain could be reduced to a few simple
columnar rules replicated throughout the neocortex? 
In our research project we seek for those principles by means of the elaboration of computational models
of the neocortex.
On the one hand, the simpler the rules, the more repetitions of such rules are needed in order to get
interesting -human like- behaviours from our models -such as perception invariance.
The mammalian brain -with its possible combinatorial states going beyond the number of atoms in the known universe-
leaves us categorical clues in favor of such a position. 
On the other hand, the more repetitions of those simple rules, the more computational resources
we need in order to implement our models.
We approached this endeavour trying to replicate human early language acquisition capabilities.
We have started with human early acquisition of phonotactic rules, specific to a particular language. 
We have implemented a neurocomputational model whose biological plausibility
-at the cortical level- allows us to test neurophysiological hypotheses incorporated in the algorithms.
With the desired number of layers, our model abstracts phonological features
in a completely unsupervised fashion.
The input is composed by a series of words which are processed by multiresolution
spectro-temporal analysis of complex sounds which simulates subcortical
nuclei in the auditory pathway.
The phonetic features extracted by the model have the function of improving
the performance of standardized supervised pattern classification techniques
whose main objective is to test the level of invariance achieved by the model layers.

The algorithms in our hierarchical cortical memory (HCM) are C++14 compliant,
they are composed by a set of classes interrelated by inheritance and composition.
The classes in the algorithms are parallelized by means of OpenMP
-for the classes located at the bottom of the inheritance hierarchy-
and MPI -for the classes at the top of the inheritance hierarchy.
This implementation has the capacity of storing its outputs in Matlab and Octave file formats
and it is suitable for the use of parallel I/O file systems.

Currently, this project is receiving High Performance Computing (HPC) mentoring
from Silvio Rizzi (HPC assistant at ALCF) and George K. Thiruvathukal
(Professor of Computer Science, Loyola University Chicago).
I have been provided with an Argonne Leadership Computing Facility
Allocation whose project name is *neurophon* and all the code is
allocated in a GitLab repository at Argonne. 
By means of Argonne staff assistance, we are performing strong and weak
scaling capacity tests in Cooley nodes which have returned encouraging results
in terms of multi-threading (OpenMP), multi-processing (MPI) and as an
hybrid implementation (MPI-OpenMP).

Our model has also shown promising phonetic classification capabilities
which make us outline it as a future alternative to current deep learning
architectures which do not pay enough attention to neurophysilogical features
in the cortex discovered in the last years. Such features are incorporated in our model
since we think they are key for information processing in cortical tissue.
Unlike current deep learning architectures, our model uses Sparse Distributed Representations
(SDRs) in order to process information.
SDRs have surprising mathematical properties which give them high noise rejection
and fault tolerance.
Those are typical characteristics in cortical tissue where individual cells are far from 100\%
reliable and they die and regenerate continuously.
According to recent findings in neuroscience,
the brain uses SDRs to process information.
This is true for all mammals, from mice to humans.

We are convinced that in a near future, HPC will get priceless contributions from works such as the one
we are developing here.
Nobody can deny the enormous progress in computing technology in the last years.
Yet, today most powerful supercomputers are still based on the same architecture
described by John Von Neumann in 1945.
We think there are very recent and important scientific milestones to which
scientific comunity have to pay attention to in this respect. 
Even though it has its limitations, deep learning -for example- has shown
unparalleled achievements which suggest alternative roads in future computer architecture
designs.
Moreover, it is the same Rick Stevens at Argonne who is warning about future changes in
the architecture of HPC systems in order to run artificial intelligence and deep learning
algorithms more efficiently.
But, regarding deep learning limitations, there are important people who are paying more
attention to key properties in cortical tissue in order to design dynamical computational
memory architectures based on the mammalian cortex.
Furthermore, some organizations have taken such people's ideas and parsimoniously run with them.
Companies like IBM and federal organizations like DARPA implement their ideas in silicon,
paving the way for neuromorphic processors that process
information in near-real time, develop representations of patterns, and make predictions.
This project is in such direction, a direction in which we share the idea
that in order to get really intelligent machines it is not necessary to copy all
the brain complexity, it is necessary to gather just key neuro-physiological
features for information processing mainly in cortical tissue.

