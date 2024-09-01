#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include "encryption.h"
#include "decryption.h"
#include "file_encryptor.h"
#include "file_decryptor.h"
#include "file_transfer_client.h"

GtkWidget *window;
GtkWidget *container;
GtkWidget *title_label;
GtkWidget *output_label;
GtkWidget *encrypt_btn;
GtkWidget *decrypt_btn;
GtkWidget *file_chooser;
GtkWidget *file_label;
GtkWidget *send_btn;
GtkWidget *encrypt_key_spin;
GtkWidget *decrypt_key_spin;
GtkWidget *hostname_entry;
GtkBuilder *builder;

char *file_path = "";
char *hostname = "";


int main(int argc, char* argv[]){
    pid_t pid = fork();

    // if it's the child process created
    if (pid == 0) {
        char *args[] = {"./file_transfer_server/server", NULL};
        execve("./file_transfer_server/server", args, NULL);
    } else {
        gtk_init(&argc, &argv);

        builder = gtk_builder_new_from_file("./UI/UI.glade");

        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        // builds table of callbacks
        gtk_builder_connect_signals(builder, NULL);

        container = GTK_WIDGET(gtk_builder_get_object(builder, "container"));
        title_label = GTK_WIDGET(gtk_builder_get_object(builder, "title-lbl"));
        output_label = GTK_WIDGET(gtk_builder_get_object(builder, "output-lbl"));
        encrypt_btn = GTK_WIDGET(gtk_builder_get_object(builder, "encrypt-btn"));
        decrypt_btn = GTK_WIDGET(gtk_builder_get_object(builder, "decrypt-btn"));
        file_chooser = GTK_WIDGET(gtk_builder_get_object(builder, "file-chooser"));
        file_label = GTK_WIDGET(gtk_builder_get_object(builder, "file-choose-lbl"));
        encrypt_key_spin = GTK_WIDGET(gtk_builder_get_object(builder, "encrypt-key-spin"));
        decrypt_key_spin = GTK_WIDGET(gtk_builder_get_object(builder, "decrypt-key-spin"));
        hostname_entry = GTK_WIDGET(gtk_builder_get_object(builder, "hostname-entry"));
        send_btn = GTK_WIDGET(gtk_builder_get_object(builder, "send-btn"));

        gtk_widget_show(window);

        gtk_main();

        return 0;
    }
    return 0;
}

void on_hostname_change(GtkEntry *e) {
    hostname = (char *) gtk_entry_get_text(e);
}

void on_file_chooser_set(GtkFileChooserButton *f) {
    file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
}

void on_encrypt_clicked(GtkButton *b) {
    if (strcmp(file_path, "") == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "Error: please selct a file to encrypt.");
        return;
    }

    int key = (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(encrypt_key_spin));

    if(encrypt_file(file_path, key) == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "File successfully encrypted!");
    } else {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "Error during encryption.");
    }
}

void on_decrypt_clicked(GtkButton *b) {
    if (strcmp(file_path, "") == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "Error: please select a file to decrypt.");
        return;
    }

    int key = (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(decrypt_key_spin));

    if (decrypt_file(file_path, key) == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "File successfully decrypted!");
    } else {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "Error during decryption.");
    }
}

void on_send_clicked(GtkButton *b) {
    if (strcmp(file_path, "") == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "Error: please select a file to send.");
        return;
    }

    if (strcmp(hostname, "") == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "Error: please enter a hostname.");
        return;
    }
    int rc = send_file(file_path, hostname);
    if (rc == -2) {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "Error: can only send encrypted files.");
    } else if (rc == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "File successfully transfered!");
    } else {
        gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) "Error: something went wrong during transfer.");
    }
}

