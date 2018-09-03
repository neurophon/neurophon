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
# It generates NumberOfVocabularies random vocabularies each with VocabulariesSize number of words for each case.
# Then it generates the corpora from such vocabularies.

import Generate_SABLE as SABLE
import numpy as np

# Variables initialization

VocabulariesSizes = np.array([5]) #([5, 15, 25])
NumberOfVoicesTurns = np.array([2]) #([25, 50, 250])
UtterancePerTurn = 1 #2

# Festival Voices

voicesNormal1 = np.array(["cmu_us_ahw_cg","cmu_us_aup_cg","cmu_us_axb_cg"])
voicesNormal2 = np.array(["cmu_us_fem_cg","cmu_us_gka_cg","cmu_us_ksp_cg","cmu_us_rxr_cg"])
voices1 = np.array(["cmu_us_awb_cg","cmu_us_bdl_cg","cmu_us_clb_cg"])
voices2 = np.array(["cmu_us_jmk_cg","cmu_us_rms_cg","cmu_us_slt_cg"])
voices3 = np.array(["cmu_us_bdl_arctic_clunits","cmu_us_clb_arctic_clunits","cmu_us_slt_arctic_clunits"]) #"cmu_us_awb_arctic_clunits"
voices4 = np.array(["cmu_us_jmk_arctic_clunits","cmu_us_rms_arctic_clunits","cmu_us_slt_arctic_clunits"])
voicesBasic = np.array(["kal_diphone","rab_diphone"])

# Vocabularies of one syllable

OneS3L = np.array(["oft.", "Sun.", "bus.", "few.", "act.", "bid.", "red.", "one.", "cat."])

OneS4L = np.array(["raid.", "dumb.", "Moon.", "slew.", "life.", "base.", "head.", "plot.", "high.", "soul.", "fear.", "love.", "week.", "head.", "rage.", "base.", "door.", "drop.", "yard.", "heat.", "pole.", "cord.", "vile.", "path.", "step.", "sack.", "soul.", "week.", "high.", "fear."])

OneS5L = np.array(["sword.", "steel.", "sweep.", "tongue.", "wrong.", "spoke.", "cause.", "worse.", "crown.", "blame.", "mourn.", "scene.", "break.", "dumb.", "faith.", "count.", "glove.", "steep.", "knees.", "snake.", "pause.", "month.", "bomb.", "month."])

OneS6L = np.array(["cheeks.", "yelled.", "tongue.", "rhythm.", "school."])

# Vocabularies of two syllables

TwoS5L = np.array(["issue.", "Bible.", "every.", "avoid.", "party.", "tiger.", "enter.", "lotus.", "cynic.", "polar.", "offin.", "abide.", "noble.", "women.", "balue.", "viper.", "cobra.", "moral."])

TwoS6L = np.array(["defeat.", "socket.", "island.", "dozen.", "stupid.", "sullen.", "russel.", "forcep.", "frigid.", "donkey.", "detect.", "syntax.", "beyond.", "insist.", "debate.", "advent.", "senses.", "senate.", "wisdom.", "effect.", "silent.", "advent.", "grassy.", "milam.", "saucer.", "bamboo.", "spirit.", "facile.", "weapon.", "honour.", "London.", "daring.", "police.", "forget.", "dining.", "listen.", "public.", "player.", "lavish.", "ladies.", "master.", "fellow.", "beggar.", "ragged.", "master.", "narrow.", "facing.", "finger.", "cereal.", "gallon.", "thirty.", "bitter.", "lentil.", "result.", "number.", "refuce.", "labour.", "tribal.", "employ.", "Europe.", "winter.", "pastor.", "travel.", "seldom.", "faster.", "valour.", "evince.", "mortar.", "create.", "design.", "senses.", "regime.", "marvel.", "symbol.", "scanty.", "vision.", "reflect.", "murals."])

TwoS7L = np.array(["unleash.", "bestial.", "fortune.", "targets.", "censure.", "believe.", "ancient.", "billion.", "between.", "fearful.", "invent.", "support.", "scarely.", "closest.", "conflict.", "forceps.", "outside.", "basking.", "charmer.", "targets.", "bestial.", "fortune.", "believe.", "rejoice.", "against.", "measure.", "weapon.", "coffers.", "patient.", "censure.", "defour.", "demons.", "crevase.", "dormant.", "complex.", "current.", "glacier.", "fissure.", "crevase.", "purpose.", "objects.", "quarter.", "respect.", "meaning.", "morning.", "routine.", "journey.", "concert.", "express.", "profile.", "painter.", "wizened.", "cobbled.", "absense.", "sincere.", "acquire.", "suffice.", "degrees.", "defines.", "sunrise.", "plastic.", "blazing.", "torrent.", "furnace.", "heaving.", "jamming.", "teenage.", "decades.", "punches.", "jamming.", "workers.", "pursuit.", "demands.", "climate.", "bondage.", "private.", "surplus.", "species.", "species.", "attract.", "ringing.", "placing.", "central.", "cuckoos.", "parents.", "reaches.", "confuse.", "welfare.", "surplus.", "nuclear.", "sanctum.", "fastest.", "baffled.", "display.", "deities.", "bonding."])

TwoS8L = np.array(["function.", "captured.", "practice.", "keyboard.", "scarbble.", "response.", "airspace.", "daughter.", "thousand.", "freekled.", "platform.", "winkled.", "battered.", "reversed.", "foundded.", "scramble.", "concerns.", "disputes.", "houshold.", "claiming.", "displace.", "instance.", "breeding.", "darkness.", "problems.", "stresses.", "woodcock.", "smallest.", "guidance.", "grandeur.", "standing.", "distinct.", "fortress.", "disprove.", "marriage.", "inspired.", "shrouded.", "followed.", "midnight.", "recourse.", "tranfix.", "shrouded.", "business.", "firewood.", "trekking.", "scarcely.", "bestowed.", "slippery.", "symptoms.", "measure.", "machines.", "vanquish.", "captives.", "outbreak.", "contempt.", "battered.", "mushroom."])

TwoS9L = np.array(["statesmen.", "scientist.", "slithered.", "ceaseless.", "monstrons.", "diarrohea.", "slithered.", "outspread.", "spattered.", "mountains.", "plaything.", "thousands.", "movements.", "birthright.", "ceaseless.", "shoulders.", "parchment.", "stammered.", "judgement.", "sheltered.", "foundded.", "challenge.", "spoonfuls.", "themselves.", "mountains.", "warblers.", "landmarks."])

TwoS10L = np.array(["maintained.", "throughout.", "discharged.", "stronghold.", "threshhold.", "scriptures.", "speechless.", "conscience.", "struggling."])

# Vocabularies of three syllables

ThreeS6L = np.array(["interval.", "mutiny.", "heroic.", "icicle.", "magical.", "energy.", "luxury.", "remedy.", "Indian.", "oration.", "legacy.", "Africa."])


ThreeS7L = np.array(["colonel.", "gratify.", "element.", "Siberia.", "excited.", "another.", "kalasam.", "clarity.", "artisan.", "century.", "million.", "glacier.", "visitor.", "capital.", "piteous.", "eagerly.", "popular.", "imagine.", "tiniest.", "impetus.", "imagery.", "mollify.", "euphony.", "creator."])

ThreeS8L = np.array(["interval.", "assembly.", "crusader.", "personal.", "votaries.", "believe.", "majestic.", "opposite.", "verbiage.", "derision.", "heritage.", "dramatic.", "adequate.", "advocate.", "sanctity.", "eminence.", "dilemma.", "ultimate.", "properly.", "engineer.", "internal.", "imprison.", "religion.", "leftover.", "resident.", "survival.", "exercise.", "attitude.", "definite.", "overhead.", "alleyway.", "recently.", "tomorrow.", "domestic.", "employer.", "policies.", "extended.", "reminder.", "seasonal.", "earliest.", "evidence.", "Niltergiris.", "sererral.", "commonly.", "heritage.", "dedicate.", "resemble.", "intricate.", "treasury.", "opulence.", "integral.", "fanciful.", "masterly.", "towering.", "carnatic.", "cultural.", "expertise.", "unkindly.", "triumvirs.", "numerous.", "caprices.", "delusive.", "embodied.", "intutive.", "cucumber.", "sanctify.", "observer.", "serenely.", "argument."])

ThreeS9L = np.array(["forbidden.", "curelties.", "postulate.", "undernate.", "absollute.", "preserver.", "destroyer.", "narrative.", "witticism.", "different.", "endeavour.", "qualities.", "abandoned.", "forgotten.", "suffering."])

# Vocabularies of four syllables

FourS7L = np.array(["usually.", "economy."])

FourS8L = np.array(["locality.", "enormity.", "security.", "military.", "equality.", "morality.", "efficacy.", "atrocity.", "futility."])

FourS9L = np.array(["memorable.", "publicity.", "dominated.", "kilometer.", "immediate.", "inherited."])

FourS10L = np.array(["especially.", "cultivator.", "incredible.", "occupation.", "generation.", "technology.", "experience.", "ambassador.", "multimedia.", "ornamental.", "university.", "recreation.", "interested.", "particular.", "irreverent.", "metropolis.", "astrologer.", "ultimately.", "capacities.", "subservient.", "individual.", "delicately.", "fascinating.", "decorative.", "casuallies.", "nauseating.", "indiferent.", "illiterate.", "methylated.", "advantages.", "mechanical.", "competitor.", "associates."])

# Vocabularies of one syllable
OneSyllable = np.concatenate((OneS3L, OneS4L, OneS5L, OneS6L), axis = 0)

# Vocabularies of two syllable
TwoSyllables = np.concatenate((TwoS5L, TwoS6L, TwoS7L, TwoS8L, TwoS9L, TwoS10L), axis = 0)

# Vocabularies of three syllable
ThreeSyllables = np.concatenate((ThreeS6L, ThreeS7L, ThreeS8L, ThreeS9L), axis = 0)

# Vocabularies of four syllable
FourSyllables = np.concatenate((FourS7L, FourS8L, FourS9L, FourS10L), axis = 0)






# Composition of voices
Voices1 = np.concatenate((voicesNormal1, voices1, voices3), axis = 0)
Voices2 = np.concatenate((voicesNormal2, voices2, voices4), axis = 0)
Voices3 = np.concatenate((voicesBasic, Voices1, Voices2), axis = 0)

# Sample monosyllabic vocabularies
# Generate monosyllabic corpora
MonosyllabicVocabularies = {}
for i in range(0, VocabulariesSizes.size):
        MonosyllabicVocabularies["Vocabulary_" + str(i)] = np.random.choice(OneSyllable, VocabulariesSizes[i])

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


                SABLE.Generate_SABLE("Voices_3",\
                                     "Syllables_1",\
                                     "Corpus_" + str(j),\
                                     "Vocabulary_" + str(i),\
                                     Voices3,\
                                     MonosyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);



# Sample disyllabic vocabularies
# Generate disyllabic corpora
DisyllabicVocabularies = {}
for i in range(0, VocabulariesSizes.size):
        DisyllabicVocabularies["Vocabulary_" + str(i)] = np.random.choice(TwoSyllables, VocabulariesSizes[i])

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


                SABLE.Generate_SABLE("Voices_3",\
                                     "Syllables_2",\
                                     "Corpus_" + str(j),\
                                     "Vocabulary_" + str(i),\
                                     Voices3,\
                                     DisyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);





# Sample trisyllabic vocabularies
# Generate trisyllabic corpora
TrisyllabicVocabularies = {}
for i in range(0, VocabulariesSizes.size):
        TrisyllabicVocabularies["Vocabulary_" + str(i)] = np.random.choice(ThreeSyllables, VocabulariesSizes[i])

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


                SABLE.Generate_SABLE("Voices_3",\
                                     "Syllables_3",\
                                     "Corpus_" + str(j),\
                                     "Vocabulary_" + str(i),\
                                     Voices3,\
                                     TrisyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);





# Sample tetrasyllabic vocabularies
# Generate tetrasyllabic corpora
TetrasyllabicVocabularies = {}
for i in range(0, VocabulariesSizes.size):
        TetrasyllabicVocabularies["Vocabulary_" + str(i)] = np.random.choice(FourSyllables, VocabulariesSizes[i])

        # Generate SABLE files
        for j in range(0, NumberOfVoicesTurns.size):
                SABLE.Generate_SABLE("Voices_1",\
                                     "Syllables_4",\
                                     "Corpus_" + str(j),\
                                     "Vocabulary_" + str(i),\
                                     Voices1,\
                                     TetrasyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                SABLE.Generate_SABLE("Voices_2",\
                                     "Syllables_4",\
                                     "Corpus_" + str(j),\
                                     "Vocabulary_" + str(i),\
                                     Voices2,\
                                     TetrasyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);


                SABLE.Generate_SABLE("Voices_3",\
                                     "Syllables_4",\
                                     "Corpus_" + str(j),\
                                     "Vocabulary_" + str(i),\
                                     Voices3,\
                                     TetrasyllabicVocabularies["Vocabulary_" + str(i)], NumberOfVoicesTurns[j], UtterancePerTurn);





