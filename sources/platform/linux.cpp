#include <platform/linux.hpp>

#include <gtk/gtk.h>

static bool g_init = false;

inline static void gtk_update()
{
    while (gtk_events_pending())
        gtk_main_iteration();
}

namespace Platform
{
    std::string file_open(std::string_view rootFolder)
    {
        std::string result;

        if (!g_init)
            g_init = gtk_init_check(nullptr, nullptr);

        GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
        GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                        nullptr,
                                                        action,
                                                        "_Cancel",
                                                        GTK_RESPONSE_CANCEL,
                                                        "_Open",
                                                        GTK_RESPONSE_ACCEPT,
                                                        nullptr);

        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), rootFolder.data());
        gtk_window_set_keep_above(GTK_WINDOW(dialog), true);
        
        gint res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == GTK_RESPONSE_ACCEPT)
        {
            char *filename;
            GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
            filename = gtk_file_chooser_get_filename(chooser);
            result = filename;
            g_free(filename);
        }

        gtk_update();
        gtk_widget_destroy(dialog);
        gtk_update();

        return result;
    }

    std::string file_save(std::string_view rootFolder)
    {
        std::string result;

        if (!g_init)
            g_init = gtk_init_check(nullptr, nullptr);

        GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
        GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",
                                                        nullptr,
                                                        action,
                                                        "_Cancel",
                                                        GTK_RESPONSE_CANCEL,
                                                        "_Save",
                                                        GTK_RESPONSE_ACCEPT,
                                                        nullptr);

        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), rootFolder.data());
        gtk_window_set_keep_above(GTK_WINDOW(dialog), true);

        gint res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == GTK_RESPONSE_ACCEPT)
        {
            char *filename;
            GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
            filename = gtk_file_chooser_get_filename(chooser);
            result = filename;
            g_free(filename);
        }

        gtk_update();
        gtk_widget_destroy(dialog);
        gtk_update();

        return result;
    }
}; // namespace Platform
