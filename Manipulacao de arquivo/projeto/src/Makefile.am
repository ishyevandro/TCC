AM_CXXFLAGS = -I.
AM_LDFLAGS = -mwindows

bin_PROGRAMS = TCC

TCCdir = .

TCC_SOURCES = classe_arquivo.C classe_var.C classe_reg.C classe_intermediaria.C
TCC_HEADERS = classe_arquivo.h classe_var.h classe_reg.h classe_intermediaria.h variavel.h
TCC_CPPFLAGS = $(GTKMM_CFLAGS)
TCC_LDADD = $(GTKMM_LIBS) -lboost_regex
