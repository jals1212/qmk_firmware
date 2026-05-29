ifeq ($(ENABLE_PET), yes)
    OPT_DEFS += -DENABLE_PET
    SRC += keyboards/crkbd/lib/pets/bongocat.c
endif
