SRC += keyboards/crkbd/lib/host_led_state_reader.c
SRC += keyboards/crkbd/lib/layer_state_reader.c
SRC += keyboards/crkbd/lib/rgb_matrix_state_reader.c

ifeq ($(ENABLE_PET), yes)
    OPT_DEFS += -DENABLE_PET
    SRC += keyboards/crkbd/lib/pets/bongocat.c
endif
