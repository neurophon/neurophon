##################################################################################################################
##                                Author:                Dematties Dario Jesus                                  ##
##                                Contact:        dariodematties@hotmail.com.ar                                 ##
##                                                dariodematties@yahoo.com.ar                                   ##
##                                                dario.dematties@frm.utn.edu.ar                                ##
##                                Project:        Engineering PhD Project                                       ##
##                                Institution:        Universidad de Buenos Aires                               ##
##                                                Facultad de Ingeniería (FIUBA)                                ##
##                                Workplace:        Instituto de Ingeniería                                     ##
##                                                Biomédica FIUBA        &                                      ##
##                                                CCT CONICET Mendoza INCIHUSA                                  ##
##################################################################################################################

# File Name:                GenerateTextCorpora.m
# Language:                 Python interpreted high-level programming language for general-purpose programming.

# This program is a python 3 script that generates text corpora for mono, di, tri and tetrasyllabic words.
# It generates VocabulariesSizes.size random vocabularies each with VocabulariesSizes number of words for each case.
# Then it generates the corpora from such vocabularies.
# This program generates VocabulariesSizes.size * NumberOfVoicesTurns.size corpora for each voice array and for each
# number of syllables calling Generate_SABLE
# Generate_SABLE function also generates the corresponding audio files calling festival

import os
import Generate_SABLE as SABLE
import numpy as np
from mpi4py import MPI
import scipy.io as sio
import time

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

comm.barrier()
if (rank==0):
        start_time = time.time()


# Variables initialization

VocabulariesSizes = np.array([5,5,5,5,5,5,5,5,5,5]) #([5, 15, 25])
NumberOfVoicesTurns = np.array([250,250]) #([25, 50, 250])
UtterancePerTurn = 2

# Festival Voices

voicesNormal1 = np.array(["cmu_us_ahw_cg","cmu_us_aup_cg","cmu_us_axb_cg","cmu_us_eey_cg"])
voicesNormal2 = np.array(["cmu_us_fem_cg","cmu_us_gka_cg","cmu_us_ksp_cg","cmu_us_rxr_cg"])
voices1 = np.array(["cmu_us_awb_cg","cmu_us_bdl_cg","cmu_us_clb_cg","cmu_us_ljm_cg"])
# voices1 = np.array(["cmu_us_awb_cg","cmu_us_bdl_cg","cmu_us_clb_cg"])
voices2 = np.array(["cmu_us_jmk_cg","cmu_us_rms_cg","cmu_us_slt_cg"])
voices3 = np.array(["cmu_us_bdl_arctic_clunits","cmu_us_clb_arctic_clunits"])#,"cmu_us_awb_arctic_clunits"])
# voices3 = np.array(["cmu_us_bdl_arctic_clunits","cmu_us_clb_arctic_clunits","cmu_us_awb_arctic_clunits"])
voices4 = np.array(["cmu_us_jmk_arctic_clunits","cmu_us_rms_arctic_clunits","cmu_us_slt_arctic_clunits"])
voices5 = np.array(["cmu_indic_guj_kt_cg","cmu_indic_tel_ss_cg"])
voices6 = np.array(["cmu_indic_hin_ab_cg","cmu_indic_tel_sk_cg"])
voices7 = np.array(["cmu_indic_mar_aup_cg","cmu_indic_mar_slp_cg"])
voices8 = np.array(["cmu_indic_pan_amp_cg","cmu_indic_ben_rm_cg"])
voices9 = np.array(["cmu_indic_tam_sdr_cg","cmu_indic_guj_ad_cg"])
voices10 = np.array(["cmu_indic_tel_kpn_cg","cmu_indic_guj_dp_cg"]) #"cmu_indic_kan_plv_cg"
voicesBasic = np.array(["kal_diphone","rab_diphone"])








if rank == 0:
        # Vocabularies of one syllable

        OneS3L = np.array(["oft.", "Sun.", "bus.", "few.", "act.", "bid.", "red.", "one.", "cat."])

        OneS4L = np.array(["raid.", "dumb.", "Moon.", "slew.", "life.", "base.", "head.", "plot.", "high.", "soul.", "fear.", "love.", "week.", "rage.", "door.", "drop.", "yard.", "heat.", "pole.", "cord.", "vile.", "path.", "step.", "sack."])

        OneS5L = np.array(["sword.", "steel.", "sweep.", "tongue.", "wrong.", "spoke.", "cause.", "worse.", "crown.", "blame.", "mourn.", "scene.", "break.", "faith.", "count.", "glove.", "steep.", "knees.", "snake.", "pause.", "month.", "bomb."])

        OneS6L = np.array(["cheeks.", "yelled.", "rhythm.", "school."])

        # Vocabularies of two syllables

        TwoS5L = np.array(["issue.", "Bible.", "every.", "avoid.", "party.", "tiger.", "enter.", "lotus.", "cynic.", "polar.", "offin.", "abide.", "noble.", "women.", "balue.", "viper.", "cobra.", "moral."])

        TwoS6L = np.array(["defeat.", "socket.", "island.", "dozen.", "stupid.", "sullen.", "russel.", "forcep.", "frigid.", "donkey.", "detect.", "syntax.", "beyond.", "insist.", "debate.", "advent.", "senses.", "senate.", "wisdom.", "effect.", "silent.", "grassy.", "milam.", "saucer.", "bamboo.", "spirit.", "facile.", "weapon.", "honour.", "London.", "daring.", "police.", "forget.", "dining.", "listen.", "public.", "player.", "lavish.", "ladies.", "master.", "fellow.", "beggar.", "ragged.", "narrow.", "facing.", "finger.", "cereal.", "gallon.", "thirty.", "bitter.", "lentil.", "result.", "number.", "refuce.", "labour.", "tribal.", "employ.", "Europe.", "winter.", "pastor.", "travel.", "seldom.", "faster.", "valour.", "evince.", "mortar.", "create.", "design.", "regime.", "marvel.", "symbol.", "scanty.", "vision.", "reflect.", "murals."])

        TwoS7L = np.array(["unleash.", "bestial.", "fortune.", "targets.", "censure.", "believe.", "ancient.", "billion.", "between.", "fearful.", "invent.", "support.", "scarely.", "closest.", "conflict.", "forceps.", "outside.", "basking.", "charmer.", "rejoice.", "against.", "measure.", "coffers.", "patient.", "defour.", "demons.", "crevase.", "dormant.", "complex.", "current.", "glacier.", "fissure.", "purpose.", "objects.", "quarter.", "respect.", "meaning.", "morning.", "routine.", "journey.", "concert.", "express.", "profile.", "painter.", "wizened.", "cobbled.", "absense.", "sincere.", "acquire.", "suffice.", "degrees.", "defines.", "sunrise.", "plastic.", "blazing.", "torrent.", "furnace.", "heaving.", "jamming.", "teenage.", "decades.", "punches.", "workers.", "pursuit.", "demands.", "climate.", "bondage.", "private.", "surplus.", "species.", "attract.", "ringing.", "placing.", "central.", "cuckoos.", "parents.", "reaches.", "confuse.", "welfare.", "nuclear.", "sanctum.", "fastest.", "baffled.", "display.", "deities.", "bonding."])

        TwoS8L = np.array(["function.", "captured.", "practice.", "keyboard.", "scarbble.", "response.", "airspace.", "daughter.", "thousand.", "freekled.", "platform.", "winkled.", "battered.", "reversed.", "foundded.", "scramble.", "concerns.", "disputes.", "houshold.", "claiming.", "displace.", "instance.", "breeding.", "darkness.", "problems.", "stresses.", "woodcock.", "smallest.", "guidance.", "grandeur.", "standing.", "distinct.", "fortress.", "disprove.", "marriage.", "inspired.", "shrouded.", "followed.", "midnight.", "recourse.", "tranfix.", "business.", "firewood.", "trekking.", "scarcely.", "bestowed.", "slippery.", "symptoms.", "machines.", "vanquish.", "captives.", "outbreak.", "contempt.", "mushroom."])

        TwoS9L = np.array(["statesmen.", "scientist.", "slithered.", "ceaseless.", "monstrons.", "diarrohea.", "outspread.", "spattered.", "mountains.", "plaything.", "thousands.", "movements.", "birthright.", "shoulders.", "parchment.", "stammered.", "judgement.", "sheltered.", "challenge.", "spoonfuls.", "themselves.", "warblers.", "landmarks."])

        TwoS10L = np.array(["maintained.", "throughout.", "discharged.", "stronghold.", "threshhold.", "scriptures.", "speechless.", "conscience.", "struggling."])

        # Vocabularies of three syllables

        ThreeS6L = np.array(["interval.", "mutiny.", "heroic.", "icicle.", "magical.", "energy.", "luxury.", "remedy.", "Indian.", "oration.", "legacy.", "Africa."])


        ThreeS7L = np.array(["colonel.", "gratify.", "element.", "Siberia.", "excited.", "another.", "kalasam.", "clarity.", "artisan.", "century.", "million.", "visitor.", "capital.", "piteous.", "eagerly.", "popular.", "imagine.", "tiniest.", "impetus.", "imagery.", "mollify.", "euphony.", "creator."])

        ThreeS8L = np.array(["assembly.", "crusader.", "personal.", "votaries.", "majestic.", "opposite.", "verbiage.", "derision.", "heritage.", "dramatic.", "adequate.", "advocate.", "sanctity.", "eminence.", "dilemma.", "ultimate.", "properly.", "engineer.", "internal.", "imprison.", "religion.", "leftover.", "resident.", "survival.", "exercise.", "attitude.", "definite.", "overhead.", "alleyway.", "recently.", "tomorrow.", "domestic.", "employer.", "policies.", "extended.", "reminder.", "seasonal.", "earliest.", "evidence.", "Niltergiris.", "sererral.", "commonly.", "dedicate.", "resemble.", "intricate.", "treasury.", "opulence.", "integral.", "fanciful.", "masterly.", "towering.", "carnatic.", "cultural.", "expertise.", "unkindly.", "triumvirs.", "numerous.", "caprices.", "delusive.", "embodied.", "intutive.", "cucumber.", "sanctify.", "observer.", "serenely.", "argument."])

        ThreeS9L = np.array(["forbidden.", "curelties.", "postulate.", "undernate.", "absollute.", "preserver.", "destroyer.", "narrative.", "witticism.", "different.", "endeavour.", "qualities.", "abandoned.", "forgotten.", "suffering."])

        # Vocabularies of four syllables

        FourS7L = np.array(["usually.", "economy."])

        FourS8L = np.array(["locality.", "enormity.", "security.", "military.", "equality.", "morality.", "efficacy.", "atrocity.", "futility."])

        FourS9L = np.array(["memorable.", "publicity.", "dominated.", "kilometer.", "immediate.", "inherited."])

        FourS10L = np.array(["especially.", "cultivator.", "incredible.", "occupation.", "generation.", "technology.", "experience.", "ambassador.", "multimedia.", "ornamental.", "university.", "recreation.", "interested.", "particular.", "irreverent.", "metropolis.", "astrologer.", "ultimately.", "capacities.", "subservient.", "individual.", "delicately.", "fascinating.", "decorative.", "casuallies.", "nauseating.", "indiferent.", "illiterate.", "methylated.", "advantages.", "mechanical.", "competitor.", "associates."])


















path = "/projects/neurophon/TestsData/NewCorpora/"
if rank == 0:
        if not os.path.exists(path):
            os.makedirs(path)

        # Vocabularies of one syllable
        filename = "OneSyllable.mat"
        if not os.path.isfile(path + filename):
                OneSyllable = np.concatenate((OneS3L, OneS4L, OneS5L, OneS6L), axis = 0)
                np.random.shuffle(OneSyllable)
                sio.savemat(path + filename, {'OneSyllable': OneSyllable})
        else:
                auxiliary = sio.loadmat(path + filename)
                OneSyllable = auxiliary['OneSyllable']

        # OneSyllable number of words verification
        if np.sum(VocabulariesSizes) > OneSyllable.size:
                raise ValueError("There are not enough one syllable words to supply the vocabulary demands")

        if len(np.unique(OneSyllable)) != len(OneSyllable): 
                raise ValueError("Duplications in OneSyllable")


        # Vocabularies of two syllable
        filename = "TwoSyllables.mat"
        if not os.path.isfile(path + filename):
                TwoSyllables = np.concatenate((TwoS5L, TwoS6L, TwoS7L, TwoS8L, TwoS9L, TwoS10L), axis = 0)
                np.random.shuffle(TwoSyllables)
                sio.savemat(path + filename, {'TwoSyllables': TwoSyllables})
        else:
                auxiliary = sio.loadmat(path + filename)
                TwoSyllables = auxiliary['TwoSyllables']

        # TwoSyllables number of words verification
        if np.sum(VocabulariesSizes) > TwoSyllables.size:
                raise ValueError("There are not enough two syllables words to supply the vocabulary demands")

        if len(np.unique(TwoSyllables)) != len(TwoSyllables): 
                raise ValueError("Duplications in TwoSyllables")


        # Vocabularies of three syllable
        filename = "ThreeSyllables.mat"
        if not os.path.isfile(path + filename):
                ThreeSyllables = np.concatenate((ThreeS6L, ThreeS7L, ThreeS8L, ThreeS9L), axis = 0)
                np.random.shuffle(ThreeSyllables)
                sio.savemat(path + filename, {'ThreeSyllables': ThreeSyllables})
        else:
                auxiliary = sio.loadmat(path + filename)
                ThreeSyllables = auxiliary['ThreeSyllables']

        # ThreeSyllables number of words verification
        if np.sum(VocabulariesSizes) > ThreeSyllables.size:
                raise ValueError("There are not enough three syllables words to supply the vocabulary demands")

        if len(np.unique(ThreeSyllables)) != len(ThreeSyllables): 
                raise ValueError("Duplications in ThreeSyllables")


        # Vocabularies of four syllable
        filename = "FourSyllables.mat"
        if not os.path.isfile(path + filename):
                FourSyllables = np.concatenate((FourS7L, FourS8L, FourS9L, FourS10L), axis = 0)
                np.random.shuffle(FourSyllables)
                sio.savemat(path + filename, {'FourSyllables': FourSyllables})
        else:
                auxiliary = sio.loadmat(path + filename)
                FourSyllables = auxiliary['FourSyllables']

        # FourSyllables number of words verification
        if np.sum(VocabulariesSizes) > FourSyllables.size:
                raise ValueError("There are not enough four syllables words to supply the vocabulary demands")

        if len(np.unique(FourSyllables)) != len(FourSyllables): 
                raise ValueError("Duplications in FourSyllables")




        print("There are enough words for generate the vocabularies")















if rank == 0:
        OneSyllableSize = OneSyllable.size 
        OneSyllableType = OneSyllable.dtype 
        TwoSyllablesSize = TwoSyllables.size 
        TwoSyllablesType = TwoSyllables.dtype 
        ThreeSyllablesSize = ThreeSyllables.size 
        ThreeSyllablesType = ThreeSyllables.dtype 
        FourSyllablesSize = FourSyllables.size 
        FourSyllablesType = FourSyllables.dtype 
else:
        OneSyllableSize = None
        OneSyllableType = None
        TwoSyllablesSize = None
        TwoSyllablesType = None
        ThreeSyllablesSize = None
        ThreeSyllablesType = None
        FourSyllablesSize = None
        FourSyllablesType = None

OneSyllableSize = comm.bcast(OneSyllableSize, root=0)
OneSyllableType = comm.bcast(OneSyllableType, root=0)
TwoSyllablesSize = comm.bcast(TwoSyllablesSize, root=0)
TwoSyllablesType = comm.bcast(TwoSyllablesType, root=0)
ThreeSyllablesSize = comm.bcast(ThreeSyllablesSize, root=0)
ThreeSyllablesType = comm.bcast(ThreeSyllablesType, root=0)
FourSyllablesSize = comm.bcast(FourSyllablesSize, root=0)
FourSyllablesType = comm.bcast(FourSyllablesType, root=0)











if rank != 0:
        OneSyllable = np.empty(OneSyllableSize, dtype=OneSyllableType)
        TwoSyllables = np.empty(TwoSyllablesSize, dtype=TwoSyllablesType)
        ThreeSyllables = np.empty(ThreeSyllablesSize, dtype=ThreeSyllablesType)
        FourSyllables = np.empty(FourSyllablesSize, dtype=FourSyllablesType)










for i in range(OneSyllableSize):
        if rank == 0:
                Size = len(OneSyllable[i])
        else:
                Size = None

        Size = comm.bcast(Size, root=0)

        if rank == 0:
                data = OneSyllable[i].encode('utf-8')
        else:
                data = bytearray(Size)

        comm.Bcast(data, root=0)

        if rank != 0:
                OneSyllable[i] = data.decode('utf-8')



for i in range(TwoSyllablesSize):
        if rank == 0:
                Size = len(TwoSyllables[i])
        else:
                Size = None

        Size = comm.bcast(Size, root=0)

        if rank == 0:
                data = TwoSyllables[i].encode('utf-8')
        else:
                data = bytearray(Size)

        comm.Bcast(data, root=0)

        if rank != 0:
                TwoSyllables[i] = data.decode('utf-8')



for i in range(ThreeSyllablesSize):
        if rank == 0:
                Size = len(ThreeSyllables[i])
        else:
                Size = None

        Size = comm.bcast(Size, root=0)

        if rank == 0:
                data = ThreeSyllables[i].encode('utf-8')
        else:
                data = bytearray(Size)

        comm.Bcast(data, root=0)

        if rank != 0:
                ThreeSyllables[i] = data.decode('utf-8')



for i in range(FourSyllablesSize):
        if rank == 0:
                Size = len(FourSyllables[i])
        else:
                Size = None

        Size = comm.bcast(Size, root=0)

        if rank == 0:
                data = FourSyllables[i].encode('utf-8')
        else:
                data = bytearray(Size)

        comm.Bcast(data, root=0)

        if rank != 0:
                FourSyllables[i] = data.decode('utf-8')






# Composition of voices
Voices1 = np.concatenate((voicesNormal1, voices1, voices3), axis = 0)
Voices2 = np.concatenate((voicesNormal2, voices2, voices4), axis = 0)
# Voices3 = np.concatenate((voicesBasic, Voices1, Voices2), axis = 0)


# Vocabularies indexing array
VocabulariesIndexes = np.cumsum(VocabulariesSizes)

# Generate the directory tree to hold the corpora
SABLE.Generate_Directory_Tree(2, 3, NumberOfVoicesTurns.size, VocabulariesSizes.size)

# Sample monosyllabic vocabularies
# Generate monosyllabic corpora
MonosyllabicVocabularies = {}
for i in range(0, VocabulariesSizes.size):
        if (i%size==rank):
                if i == 0:
                    MonosyllabicVocabularies["Vocabulary_" + str(i)] = OneSyllable[:VocabulariesIndexes[i]]
                else:
                    MonosyllabicVocabularies["Vocabulary_" + str(i)] = OneSyllable[VocabulariesIndexes[i-1]:VocabulariesIndexes[i]]

                # MonosyllabicVocabularies["Vocabulary_" + str(i)] = np.random.choice(OneSyllable, VocabulariesSizes[i])

                # Generate SABLE files
                for j in range(0, NumberOfVoicesTurns.size):
                        SABLE.Generate_SABLE("Voices_1",\
                                             "Syllables_1",\
                                             "Corpus_" + str(j),\
                                             "Vocabulary_" + str(i),\
                                             Voices1,\
                                             MonosyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                        SABLE.Generate_SABLE("Voices_2",\
                                             "Syllables_1",\
                                             "Corpus_" + str(j),\
                                             "Vocabulary_" + str(i),\
                                             Voices2,\
                                             MonosyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                        # SABLE.Generate_SABLE("Voices_3",\
                                             # "Syllables_1",\
                                             # "Corpus_" + str(j),\
                                             # "Vocabulary_" + str(i),\
                                             # Voices3,\
                                             # MonosyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);



# Sample disyllabic vocabularies
# Generate disyllabic corpora
DisyllabicVocabularies = {}
for i in range(0, VocabulariesSizes.size):
        if (i%size==rank):
                if i == 0:
                    DisyllabicVocabularies["Vocabulary_" + str(i)] = TwoSyllables[:VocabulariesIndexes[i]]
                else:
                    DisyllabicVocabularies["Vocabulary_" + str(i)] = TwoSyllables[VocabulariesIndexes[i-1]:VocabulariesIndexes[i]]

                # DisyllabicVocabularies["Vocabulary_" + str(i)] = np.random.choice(TwoSyllables, VocabulariesSizes[i])

                # Generate SABLE files
                for j in range(0, NumberOfVoicesTurns.size):
                        SABLE.Generate_SABLE("Voices_1",\
                                             "Syllables_2",\
                                             "Corpus_" + str(j),\
                                             "Vocabulary_" + str(i),\
                                             Voices1,\
                                             DisyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                        SABLE.Generate_SABLE("Voices_2",\
                                             "Syllables_2",\
                                             "Corpus_" + str(j),\
                                             "Vocabulary_" + str(i),\
                                             Voices2,\
                                             DisyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                        # SABLE.Generate_SABLE("Voices_3",\
                                             # "Syllables_2",\
                                             # "Corpus_" + str(j),\
                                             # "Vocabulary_" + str(i),\
                                             # Voices3,\
                                             # DisyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);





# Sample trisyllabic vocabularies
# Generate trisyllabic corpora
TrisyllabicVocabularies = {}
for i in range(0, VocabulariesSizes.size):
        if (i%size==rank):
                if i == 0:
                    TrisyllabicVocabularies["Vocabulary_" + str(i)] = ThreeSyllables[:VocabulariesIndexes[i]]
                else:
                    TrisyllabicVocabularies["Vocabulary_" + str(i)] = ThreeSyllables[VocabulariesIndexes[i-1]:VocabulariesIndexes[i]]

                # TrisyllabicVocabularies["Vocabulary_" + str(i)] = np.random.choice(ThreeSyllables, VocabulariesSizes[i])

                # Generate SABLE files
                for j in range(0, NumberOfVoicesTurns.size):
                        SABLE.Generate_SABLE("Voices_1",\
                                             "Syllables_3",\
                                             "Corpus_" + str(j),\
                                             "Vocabulary_" + str(i),\
                                             Voices1,\
                                             TrisyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                        SABLE.Generate_SABLE("Voices_2",\
                                             "Syllables_3",\
                                             "Corpus_" + str(j),\
                                             "Vocabulary_" + str(i),\
                                             Voices2,\
                                             TrisyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                        # SABLE.Generate_SABLE("Voices_3",\
                                             # "Syllables_3",\
                                             # "Corpus_" + str(j),\
                                             # "Vocabulary_" + str(i),\
                                             # Voices3,\
                                             # TrisyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);





# # Sample tetrasyllabic vocabularies
# # Generate tetrasyllabic corpora
# TetrasyllabicVocabularies = {}
# for i in range(0, VocabulariesSizes.size):
        # if (i%size==rank):
                # if i == 0:
                    # TetrasyllabicVocabularies["Vocabulary_" + str(i)] = FourSyllables[:VocabulariesIndexes[i]]
                # else:
                    # TetrasyllabicVocabularies["Vocabulary_" + str(i)] = FourSyllables[VocabulariesIndexes[i-1]:VocabulariesIndexes[i]]

                # # TetrasyllabicVocabularies["Vocabulary_" + str(i)] = np.random.choice(FourSyllables, VocabulariesSizes[i])

                # # Generate SABLE files
                # for j in range(0, NumberOfVoicesTurns.size):
                        # SABLE.Generate_SABLE("Voices_1",\
                                             # "Syllables_4",\
                                             # "Corpus_" + str(j),\
                                             # "Vocabulary_" + str(i),\
                                             # Voices1,\
                                             # TetrasyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                        # SABLE.Generate_SABLE("Voices_2",\
                                             # "Syllables_4",\
                                             # "Corpus_" + str(j),\
                                             # "Vocabulary_" + str(i),\
                                             # Voices2,\
                                             # TetrasyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                        # # SABLE.Generate_SABLE("Voices_3",\
                                             # # "Syllables_4",\
                                             # # "Corpus_" + str(j),\
                                             # # "Vocabulary_" + str(i),\
                                             # # Voices3,\
                                             # # TetrasyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);





comm.barrier()
if (rank==0):
        print("--- %s seconds ---" % (time.time() - start_time))
