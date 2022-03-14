#include <gtk/gtk.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

extern "C"
G_MODULE_EXPORT void l_clk( GtkButton * button, gpointer user_data ) {
	system("dconf write /org/gnome/desktop/interface/gtk-theme '\"Yaru-unity\"'");
	system("dconf write /org/gnome/desktop/interface/icon-theme '\"Yaru-unity\"'");
}
extern "C"
G_MODULE_EXPORT void d_clk( GtkButton * button, gpointer user_data ) {
	system("dconf write /org/gnome/desktop/interface/gtk-theme '\"Yaru-unity-dark\"'");
	system("dconf write /org/gnome/desktop/interface/icon-theme '\"Yaru-unity\"'");
}

#define UI_FILE "/opt/ubunity-style/ui.glade"

GtkWidget * utworz_okno( void )
{
    GtkWidget* okno;
    GtkToggleButton* tog_l;
    GtkToggleButton* tog_d;
    GtkBuilder* builder;
    GError* error = NULL;
   
    builder = gtk_builder_new();
    if( !gtk_builder_add_from_file( builder, UI_FILE, & error ) )
    {
        g_warning( "Couldn't load builder file: %s", error->message );
        g_error_free( error );
    }
   
    gtk_builder_connect_signals( builder, NULL );
   
    okno = GTK_WIDGET( gtk_builder_get_object( builder, "wnd" ) );
    
    tog_l = GTK_TOGGLE_BUTTON( gtk_builder_get_object( builder, "s_1" ) );
    tog_d = GTK_TOGGLE_BUTTON( gtk_builder_get_object( builder, "s_2" ) );
    
    std::string gettheme = exec("dconf read /org/gnome/desktop/interface/gtk-theme");
    if(gettheme=="'Yaru-unity-dark'\n"){
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tog_l), FALSE);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tog_d), TRUE);
    } else if(gettheme=="'Yaru-unity'\n"){
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tog_d), FALSE);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tog_l), TRUE);
    } else {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tog_l), FALSE);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tog_d), FALSE);
    }
   
    g_object_unref( builder );
   
    return okno;
}


int
main( int argc, char * argv[] )
{
    GtkWidget * okno;
   
    gtk_init( & argc, & argv );
   
    okno = utworz_okno();
    gtk_widget_show( okno );
   
    gtk_main();
    return 0;
}
