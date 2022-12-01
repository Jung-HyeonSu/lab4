#ifndef CALC_H
#define CALC_H
#include <gtk/gtk.h>
typedef struct calbutton
{
	char *value;
	GtkWidget *button;
}calbutton;

gint delete_event(GtkWidget *window, GdkEvent *event, gpointer data);
void create_num_button(GtkWidget *box, GtkWidget *button, char* button_num);
void click_number(GtkWidget *widget, gpointer data);
void clear(GtkWidget *widget, gpointer data);
void submit(GtkWidget *widget, gpointer data);
void add(GtkWidget *widget, gpointer data);
void sub(GtkWidget *widget, gpointer data);
void mul(GtkWidget *widget, gpointer data);
void division(GtkWidget *widget, gpointer data);
void click_symbol(GtkWidget *widget, gpointer data);
int calculator(char symbol);
#endif
