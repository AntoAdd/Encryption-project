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
GtkWidget *output_label;
GtkWidget *encrypt_btn;
GtkWidget *decrypt_btn;
GtkWidget *file_chooser;
GtkWidget *send_btn;
GtkWidget *encrypt_key_spin;
GtkWidget *decrypt_key_spin;
GtkWidget *hostname_entry;
GtkBuilder *builder;

GdkColor red;
GdkColor green;

char *file_path = "";
char *hostname = "";

void set_output_error_msg(const char *msg);
void set_output_success_msg(const char *msg);

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

        output_label = GTK_WIDGET(gtk_builder_get_object(builder, "output-lbl"));
        encrypt_btn = GTK_WIDGET(gtk_builder_get_object(builder, "encrypt-btn"));
        decrypt_btn = GTK_WIDGET(gtk_builder_get_object(builder, "decrypt-btn"));
        file_chooser = GTK_WIDGET(gtk_builder_get_object(builder, "file-chooser"));
        encrypt_key_spin = GTK_WIDGET(gtk_builder_get_object(builder, "encrypt-key-spin"));
        decrypt_key_spin = GTK_WIDGET(gtk_builder_get_object(builder, "decrypt-key-spin"));
        hostname_entry = GTK_WIDGET(gtk_builder_get_object(builder, "hostname-entry"));
        send_btn = GTK_WIDGET(gtk_builder_get_object(builder, "send-btn"));

        gdk_color_parse ("red", &red);
        gdk_color_parse ("green", &green);

        gtk_window_set_default_size(GTK_WINDOW(window), 520, 620);

        gtk_widget_show(window);

        gtk_main();

        return kill(pid, SIGTERM);
    }
    return kill(pid, SIGTERM
    );
}

void on_hostname_change(GtkEntry *e) {
    hostname = (char *) gtk_entry_get_text(e);
}

void on_file_chooser_set(GtkFileChooserButton *f) {
    file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
}

void on_encrypt_clicked(GtkButton *b) {
    if (strcmp(file_path, "") == 0) {
        set_output_error_msg("Error: please selct a file to encrypt.");
        return;
    }

    int key = (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(encrypt_key_spin));

    if(encrypt_file(file_path, key) == 0) {
        set_output_success_msg("File successfully encrypted!");
    } else {
        set_output_error_msg("Error: encryption not allowed");
    }
}

void on_decrypt_clicked(GtkButton *b) {
    if (strcmp(file_path, "") == 0) {
        set_output_error_msg("Error: please select a file to decrypt.");
        return;
    }

    int key = (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(decrypt_key_spin));

    if (decrypt_file(file_path, key) == 0) {
        set_output_success_msg("File successfully decrypted!");
    } else {
        set_output_error_msg("Error: decryption not allowed.");
    }
}

void on_send_clicked(GtkButton *b) {
    if (strcmp(file_path, "") == 0) {
        set_output_error_msg("Error: please select a file to send.");
        return;
    }

    if (strcmp(hostname, "") == 0) {
        set_output_error_msg("Error: please enter a hostname.");
        return;
    }

    int rc = send_file(file_path, hostname);

    if (rc == -2) {
        set_output_error_msg("Error: can only send encrypted files.");
    } else if (rc == 0) {
        set_output_success_msg("File successfully transfered!");
    } else {
        set_output_error_msg("Error: something went wrong during transfer.");
    }
}

void set_output_error_msg(const char *msg) {
    gtk_widget_modify_fg(output_label, GTK_STATE_NORMAL, &red);
    gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) msg);
}

void set_output_success_msg(const char *msg) {
    gtk_widget_modify_fg(output_label, GTK_STATE_NORMAL, &green);
    gtk_label_set_text(GTK_LABEL(output_label), (const gchar *) msg);
}



