/**
  * Programa Exemplo do site Gtkmm
  */

#include <editor.h>
int main (int argc, char *argv[])
	{
  	Gtk::Main kit(argc, argv);

  	EDITOR janela;
  	//Shows the window and returns when it is closed.
  	Gtk::Main::run(janela);

 

  	return 0;
	}
