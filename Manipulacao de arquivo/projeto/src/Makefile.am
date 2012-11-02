AM_CXXFLAGS = -I. 
AM_LDFLAGS = -mwindows

bin_PROGRAMS = TCC

TCCdir = ${prefix}

TCC_SOURCES = main.C editor.C classe_codigo.C classe_var.C classe_reg.C classe_linha.C classe_aspas.C
TCC_HEADERS = editor.h classe_codigo.h classe_var.h classe_reg.h classe_linha.h classe_aspas.h variavel.h

TCC_CPPFLAGS = $(GTKMM_CFLAGS) $(BSL_CFLAGS) $(EX_CFLAGS) $(CPPFLAGS)
TCC_LDADD = $(GTKMM_LIBS) $(BSL_LIBS) $(EX_LIBS) $(LDFLAGS) $(BOOST_REGEX_LIB)
