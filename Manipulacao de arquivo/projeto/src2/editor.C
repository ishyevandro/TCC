
#include <gtk-2.0/gtk/gtkenums.h>

//           editor.C
//  Qui agosto 30 19:42:53 2012
//  Copyright  2012  Alcione Ferreira
//  <user@host>

#include <editor.h>

using namespace Gtk::Menu_Helpers;

EDITOR::EDITOR() {
    this->titulo.clear();
    this->titulo = "Editor de Textos";
    this->caminho.clear();
    this->caminho = "Sem nome";
    modificado = false;
    this->conexao = new BANCOSL(DB_FILE);
    this->set_title(this->titulo + " - " + this->caminho);
    this->maximize();
    this->signal_hide().connect(sigc::mem_fun(*this, &EDITOR::sair));
    this->Principal.set_homogeneous(false);
    this->Principal.set_spacing(0);
    H_Menu.set_homogeneous(false);
    H_Menu.set_spacing(0);
    Menu = Gtk::manage(new Gtk::MenuBar());
    {
        Gtk::Menu::MenuList& menulist = SM_Recente.items();
        //...
        //menulist.push_back(MenuElem(Glib::ustring("/home/alcione/C++/editor/ChangeLog1"), sigc::bind(sigc::mem_fun(*this, &EDITOR::testar), "/home/alcione/C++/editor/ChangeLog1"))); /* chamar função para abrir arquivo*/
    }
    {
        Gtk::Menu::MenuList& menulist = Menu_Arquivo.items();
        //menulist.push_back (MenuElem(Glib::ustring("_Novo"),Gtk::AccelKey ("<control>n"),sigc::mem_fun(*this,&EDITOR::novo)));
        menulist.push_back(MenuElem(Glib::ustring("_Abrir"), Gtk::AccelKey("<control>o"), sigc::mem_fun(*this, &EDITOR::abrir)));
        menulist.push_back(MenuElem(Glib::ustring("Abrir recentes..."), Gtk::AccelKey("<control>r"), SM_Recente));
        //menulist.push_back (MenuElem(Glib::ustring("_Salvar"),Gtk::AccelKey ("<control>s"),sigc::bind<bool>(sigc::mem_fun(*this,&EDITOR::salvar),true)));
        //menulist.push_back (MenuElem(Glib::ustring("Sa_lvar como..."),Gtk::AccelKey ("<control><shift>s"),sigc::bind<bool>(sigc::mem_fun(*this,&EDITOR::salvar),false)));
        menulist.push_back(SeparatorElem());
        menulist.push_back(MenuElem(Glib::ustring("Sai_r"), Gtk::AccelKey("<alt>F4"), sigc::mem_fun(*this, &EDITOR::sair)));
    }
    Menu->items().push_back(MenuElem(Glib::ustring("_Arquivo"), Gtk::AccelKey("<alt>a"), Menu_Arquivo));
    Menu->items().push_back(MenuElem(Glib::ustring("_Editar"), Gtk::AccelKey("<alt>e"), Menu_Editar));
    Menu->items().push_back(MenuElem(Glib::ustring("Aj_uda"), Gtk::AccelKey("<alt>u"), Menu_Ajuda));
    H_Menu.pack_start(*Menu, true, true, 0);
    Principal.pack_start(H_Menu, false, false, 0);
    H_ToolBar.set_homogeneous(false);
    H_ToolBar.set_spacing(0);

    TB_Abrir.set_stock_id(Gtk::Stock::OPEN); //seta icone
    TB_Abrir.set_tooltip_text("Abrir"); //seta o que ira aparecer ao passar o mouse encima
    TB_Abrir.signal_clicked().connect(sigc::mem_fun(*this, &EDITOR::abrir)); //liga a uma funcao
    ToolBar.append(TB_Abrir); //adiciona ao Toolbar

    TB_Aplicar.set_stock_id(Gtk::Stock::EXECUTE);
    TB_Aplicar.set_tooltip_text("Verificar");
    TB_Aplicar.signal_clicked().connect(sigc::mem_fun(*this, &EDITOR::aplicar));
    ToolBar.append(TB_Aplicar);

    TB_Inserir.set_stock_id(Gtk::Stock::PREFERENCES);
    TB_Inserir.set_tooltip_text("Adicionar valor a base de dados");
    TB_Inserir.signal_clicked().connect(sigc::mem_fun(*this, &EDITOR::base_de_dados), 1);
    ToolBar.append(TB_Inserir);

    H_ToolBar.pack_start(ToolBar, true, true, 0);
    Principal.pack_start(H_ToolBar, false, false, 0);
    this->H_Conteudo = new Gtk::HBox(false, 0);
    SProcess.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    SLog.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    SProcess.add(TProcess);
    SLog.add(TLog);
    PProcess = TProcess.get_buffer();
    PProcess->set_text("");
    PLog = TLog.get_buffer();
    PLog->set_text("");
    tag = PProcess->create_tag("geral");
    tag->property_font() = "Arial 19";
    tag->property_wrap_mode() = Gtk::WRAP_WORD;
    tag->property_left_margin() = 10;
    tag->property_right_margin() = 10;
    TProcess.set_buffer(PProcess);
    TLog.set_buffer(PLog);
    Abas.set_border_width(10);
    Abas.append_page(SProcess, "Processamento");
    Abas.append_page(SLog, "Debugger");
    H_Conteudo->pack_start(Abas, true, true, 0);
    this->Principal.pack_start(*this->H_Conteudo, true, true, 0);
    statusbar = BarraStatus.push(Glib::ustring("Editor de Texto Simples"));
    Principal.pack_end(BarraStatus, false, true, 0);
    this->add(this->Principal);
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &EDITOR::relogio), 1000);
    this->show_all();
}

EDITOR::~EDITOR() {
    if (this->conexao->conectado) {
        delete this->conexao;
    }
    this->hide();
}

bool EDITOR::relogio() {
    string status;
    status.clear();
    status = "Editor de Texto Simples ";
    status.append(agora());
    this->statusbar = this->BarraStatus.push(Glib::ustring(status));
    return true;
}

void EDITOR::aplicar() {
    int pos;
    char texto [100];
    this->A = new _codigo;
    string codigo;
    Panalise = TProcess.get_buffer();
    codigo = Panalise->get_text();
    this->A->linha(codigo);

    PLog = TLog.get_buffer();
    PLog->get_bounds(inicio, final);
    iter = PLog->erase(inicio, final);
    PLog->insert(iter, Glib::ustring(this->A->debug));
    TLog.set_buffer(PLog);
    GtkWidget *message;
    message = gtk_message_dialog_new(GTK_WINDOW(NULL), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Concluido. Favor verificar aba debug.");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
    delete this->A;
}

void EDITOR::abrir() {
    ifstream arquivo;
    string texto, aux, clas_codigo;
    int resultado;

    Gtk::FileChooserDialog dialog("Abrir arquivo...", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
    Gtk::FileFilter filtro;
    filtro.set_name("Text files");
    filtro.add_mime_type("text/plain");
    dialog.add_filter(filtro);
    resultado = dialog.run();
    switch (resultado) {
        case Gtk::RESPONSE_OK:
        {
            caminho.clear();
            caminho = dialog.get_filename();
            cout << caminho << endl;
            arquivo.open(caminho.c_str());
            texto.clear();
            while (arquivo.good()) {
                aux.clear();
                getline(arquivo, aux, '\n');
                texto += aux + "\n";
            }
            arquivo.close();
            PProcess = TProcess.get_buffer();
            PProcess->get_bounds(inicio, final);
            iter = PProcess->erase(inicio, final);
            PProcess->insert(iter, Glib::ustring(texto));
            TProcess.set_buffer(PProcess);
            //1Panalise = TProcess.get_buffer(); //TESTE PARA VER COMO IRIA SAIR A IMPRESSAO
            //1cout << Panalise->get_text() << endl;

            //aplicar();
            modificado = false;
            break;
        }
        case Gtk::RESPONSE_CANCEL:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    set_title(this->titulo + " - " + caminho);
}

void EDITOR::sair() {
    this->hide();
}

void EDITOR::base_de_dados() {
    cout << "ta funcionando" << endl;


}