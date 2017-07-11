clear -all

voicesNormal1 = ["cmu_us_ahw_cg";"cmu_us_aup_cg";"cmu_us_axb_cg"];
voicesNormal2 = ["cmu_us_fem_cg";"cmu_us_gka_cg";"cmu_us_ksp_cg";"cmu_us_rxr_cg"];
voices1 = ["cmu_us_awb_cg";"cmu_us_bdl_cg";"cmu_us_clb_cg"];
voices2 = ["cmu_us_jmk_cg";"cmu_us_rms_cg";"cmu_us_slt_cg"];
voices3 = ["cmu_us_bdl_arctic_clunits";"cmu_us_clb_arctic_clunits"]; #"cmu_us_awb_arctic_clunits";
voices4 = ["cmu_us_jmk_arctic_clunits";"cmu_us_rms_arctic_clunits";"cmu_us_slt_arctic_clunits"];
voicesBasic = ["kal_diphone";"rab_diphone"];









      
#{
voice_cstr_us_ked_timit_hts

kal_diphone_us
voice_nitech_us_slt_arctic_hts
                
voice_reset

voice_nitech_us_awb_arctic_hts
voice_nitech_us_bdl_arctic_hts
voice_nitech_us_clb_arctic_hts
voice_nitech_us_jmk_arctic_hts
voice_nitech_us_rms_arctic_hts
voice_nitech_us_slt_arctic_hts


voice_kal_diphone_us_phone_maps
voice_cmu_us_kal_com_hts
#}











vocabulary1 = ["map."; "dog."; "cat."; "with."; "lamp."; "mouse."; "truck."; "bear."; "wolf."; "bell."; "school."; "chair."; "desk."; "shoe."; "plant."; "car."; "road."; "bus."; "cup."; "pen."; "nose."; "pot."; "shop."; "lamp."; "tree."; "fence."];

vocabulary1R = ["map."; "dog."; "mouse."; "with."; "truck."];

vocabulary2 = ["answer."; "doctor."; "teacher."; "summer."; "tennis."; "guitar."; "balloon."; "paper."; "picture."; "giraffe."; "tower."; "zebra."; "popcorn."; "pizza."; "blanket."; "table."; "marker."; "picture."; "movie."];

vocabulary3 = ["computer."; "telephone."; "rectangle."; "tomato."; "magazine."; "volunteer."; "seventeen."; "fantastic."; "basketball."; "banana."; "astronaut."; "animal."; "September."; "Saturday."];

vocabulary4 = ["kindergarten."; "alligator."; "watermelon."; "harmonica."; "television."; "delivery."; "understanding."; "dictionary."; "Monopoly."; "helicopter."; "elevator."; "discovery."];

vocabulary5 = ["hippopotamus."; "refrigerator."; "curiosity."; "electricity."; "creativity."; "opportunity."; "vocabulary."; "imaginary."; "multiplication."; "imagination."; "cafeteria."; "enthusiastic."];

# with this configuration every speaker will pronounce ten times every word in the vocabulary
#Generate_Training_SABLE("Corpus", voices4, vocabulary1, 390, 2);


Generate_Training_SABLE("Corpus", [voicesNormal2; voices2; voices4], vocabulary1R, 250, 2);



#Generate_Training_SABLE("Corpus", [voicesNormal1; voices1; voices3], vocabulary1R, 250, 2);



#Generate_Training_SABLE("Corpus", [voices4], vocabulary1R, 250, 2);
