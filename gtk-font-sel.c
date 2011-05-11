/*
 * GTK Font Selection Dialog
 * For seeing which fonts I have :P
 *
 * Compile with: gcc -o gtk-font-sel gtk-font-sel.c `pkg-config --cflags --libs gtk+-2.0`
 */

#include <gtk/gtk.h>

int main(int argc, char **argv) {
  gtk_init(&argc, &argv);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget* fontsel = gtk_font_selection_new();

  gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);

  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  gtk_container_border_width(GTK_CONTAINER(window), 20);

  gtk_container_add(GTK_CONTAINER(window), fontsel);

  gtk_widget_show(fontsel);
  gtk_widget_show(window);

  gtk_main();

  return 0;
}
