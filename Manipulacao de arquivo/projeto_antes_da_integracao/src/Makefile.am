AM_CXXFLAGS = -I.
AM_LDFLAGS = -mwindows

bin_PROGRAMS = TCC

TCCdir = .

TCC_SOURCES = classe_arquivo.C classe_var.C classe_reg.C classe_intermediaria.C classe_aspas.C
TCC_HEADERS = classe_arquivo.h classe_var.h classe_reg.h classe_intermediaria.h classe_aspas.h variavel.h
TCC_CPPFLAGS = $(GTKMM_CFLAGS) $(CPPFLAGS)
TCC_LDADD = $(GTKMM_LIBS) $(LDFLAGS) $(BOOST_REGEX_LIB)
