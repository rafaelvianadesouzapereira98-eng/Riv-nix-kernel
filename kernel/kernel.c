#include "vga.h"

void main() {
    // Limpa a tela ao iniciar o kernel
    vga_clear();

    // Exibe mensagens estilizadas usando as cores do VGA
    vga_print("=== RIV-NIX KERNEL v0.1 ===\n", (VGA_COLOR_LIGHT_CYAN << 4) | VGA_COLOR_BLACK);
    vga_print("Status: Bare-metal inicializado com sucesso!\n\n", VGA_COLOR_LIGHT_GREEN);
    
    vga_print("[INFO] Carregando gerenciadores...\n", VGA_COLOR_YELLOW);
    vga_print("[OK] Gerenciador de memoria pronto.\n", VGA_COLOR_WHITE);
    vga_print("[OK] Driver VGA integrado via C e Assembly.\n", VGA_COLOR_WHITE);

    vga_print("\nRiv-Nix CLI > ", VGA_COLOR_LIGHT_MAGENTA);

    // Trava o kernel em loop infinito
    while(1);
}
