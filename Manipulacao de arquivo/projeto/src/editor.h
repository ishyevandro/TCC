/***************************************************************************
 *            editor.h
 *
 *  Qui agosto 30 19:42:53 2012
 *  Copyright  2012  Alcione Ferreira
 *  <user@host>
 ****************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <gtkmm.h>
#include <libex.h>
#include <bancosl.h>
#include <config.h>
#include "classe_arquivo.h"

#ifndef __EDITOR_H__
#define __EDITOR_H__ 1

class EDITOR : public Gtk::Window 
	{
	public:
		EDITOR();
		~EDITOR();
		_arq *A;
	protected:
		Gtk::VBox Principal;
		Gtk::HBox *H_Conteudo, H_Menu, H_ToolBar;
		//Menu do Sistema
		Gtk::MenuBar *Menu;
		Gtk::Menu Menu_Arquivo, Menu_Editar, Menu_Ajuda, SM_Recente;
		//Toolbar do Sistema
		Gtk::Toolbar ToolBar;
		Gtk::ToolButton TB_Novo, TB_Abrir, TB_Salvar, TB_Pesquisar, TB_Aplicar, TB_Imprimir, TB_Verificar;
		Gtk::SeparatorToolItem TB_Separador;
		//Barra de Status
		Gtk::Statusbar BarraStatus;/**<Gtk::Statusbar barra de status do sistema.*/
		guint statusbar;/**<Atributo controlador da PRINCIPAL::BarraStatus1.*/
		Gtk::Notebook Abas;
		Gtk::Label Aba1, Aba2;
		Gtk::ScrolledWindow SProcess, SLog;
		Gtk::TextView TProcess, TLog;			
		Glib::RefPtr<Gtk::TextBuffer> Panalise, PProcess, PLog;
		Gtk::TextIter iter, inicio, final;
		Glib::RefPtr<Gtk::TextTag> tag;

			
	private:
		BANCOSL *conexao;
		bool modificado;
		Glib::ustring titulo;
		string caminho;
		void testar(string);
		bool relogio();
		void verifica();
		void aplicar();
		void abrir();
		//void salvar(bool );
		//void foi_modificado();
		void sair();
		//void novo();
	};
                              

#endif /*__EDITOR_H__*/
