#include "vga.h"

// Ponteiro volátil para a memória de vídeo (modo texto)
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

// Funções de baixo nível para interagir com as portas de hardware (Assembly inline)
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

// Pega a posição atual do cursor na tela via portas do VGA
int get_cursor_offset() {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset * 2; // Multiplica por 2 porque cada célula usa 2 bytes (caractere + cor)
}

// Atualiza a posição visual do cursor na tela
void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

// Calcula o deslocamento (offset) baseado na linha e coluna
int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset) {
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) {
    return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}

// Limpa a tela preenchendo com espaços vazios
void vga_clear() {
    char* screen = (char*) VIDEO_ADDRESS;
    int i = 0;
    while (i < MAX_ROWS * MAX_COLS * 2) {
        screen[i] = ' ';
        screen[i+1] = 0x07; // Letra cinza claro, fundo preto
        i += 2;
    }
    set_cursor_offset(get_offset(0, 0));
}

// Manipula a rolagem da tela quando atinge a última linha
int handle_scrolling(int cursor_offset) {
    if (cursor_offset < MAX_ROWS * MAX_COLS * 2) {
        return cursor_offset;
    }

    // Move todas as linhas uma linha acima
    char* screen = (char*) VIDEO_ADDRESS;
    for (int i = 1; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS * 2; j++) {
            screen[(i - 1) * MAX_COLS * 2 + j] = screen[i * MAX_COLS * 2 + j];
        }
    }

    // Limpa a última linha
    char* last_line = (char*) (VIDEO_ADDRESS + get_offset(0, MAX_ROWS - 1));
    for (int i = 0; i < MAX_COLS * 2; i += 2) {
        last_line[i] = ' ';
        last_line[i+1] = 0x07;
    }

    // Retorna o cursor na última linha
    return cursor_offset - 2 * MAX_COLS;
}

// Imprime um caractere na posição atual do cursor com uma cor específica
void vga_print_char(char c, int color) {
    unsigned char* vidmem = (unsigned char*) VIDEO_ADDRESS;
    
    // Se a cor não foi especificada (0), usa branco sobre preto padrão (0x0f)
    if (!color) {
        color = 0x0f;
    }

    int cursor_offset = get_cursor_offset();

    // Trata quebra de linha ('\n')
    if (c == '\n') {
        int rows = get_offset_row(cursor_offset);
        cursor_offset = get_offset(0, rows + 1);
    } else {
        vidmem[cursor_offset] = c;
        vidmem[cursor_offset + 1] = color;
        cursor_offset += 2;
    }

    // Faz a rolagem se ultrapassar o limite da tela
    cursor_offset = handle_scrolling(cursor_offset);
    
    // Atualiza o cursor físico
    set_cursor_offset(cursor_offset);
}

// Imprime uma string completa na tela
void vga_print(char* message, int color) {
    int i = 0;
    while (message[i] != 0) {
        vga_print_char(message[i], color);
        i++;
    }
}
