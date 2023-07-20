#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SERIAL_PORT "/dev/ttyS1"

GtkWidget *input_entry;
GtkWidget *output_label;
GtkWidget *serial_output_label;

// 按钮点击事件的回调函数
void on_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(input_entry));

    // 创建一个新的字符串，用于存储转换后的文本
    char converted_text[1024];
    int converted_index = 0;
    
    // 遍历输入的文本
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            // 将'\n'转换为回车字符，并存储到新的字符串中
            converted_text[converted_index++] = '\r';
            converted_text[converted_index++] = '\n';
        } else {
            // 其他字符直接复制到新的字符串中
            converted_text[converted_index++] = text[i];
        }
    }
    
    // 在新字符串的末尾添加一个终止符
    converted_text[converted_index] = '\n';
    
    int fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("Failed to open serial port");
        return;
    }
    
    write(fd, converted_text, strlen(converted_text));
    close(fd);
}

// 更新输出的回调函数
gboolean update_output(gpointer data) {
    // 运行命令并获取输出
    FILE *fp;
    char buffer[128];
    char result[1024] = "";

    fp = popen("cat msg.txt", "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(result, buffer);
    }

    pclose(fp);

    // 更新标签文本
    gtk_label_set_text(GTK_LABEL(output_label), result);

    return G_SOURCE_CONTINUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "铁路中央控制系统");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    // 创建左侧垂直容器
    GtkWidget *left_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox), left_vbox, TRUE, TRUE, 0);

    // 创建右侧垂直容器
    GtkWidget *right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox), right_vbox, TRUE, TRUE, 0);

    // 左侧部分：输出
    output_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(left_vbox), output_label, TRUE, TRUE, 0);

    // 右侧部分：串口输入输出
    input_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(right_vbox), input_entry, FALSE, FALSE, 0);

    GtkWidget *send_button = gtk_button_new_with_label("发送至铁路");
    g_signal_connect(send_button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(right_vbox), send_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

    // 每.1秒更新输出
    g_timeout_add_seconds(0.1, update_output, NULL);

    gtk_main();

    return 0;
}
