void main() {
    // Ponteiro para a memória de vídeo VGA (modo texto)
    char* video_memory = (char*) 0xb8000;
    
    // Escreve "Riv-Nix Kernel" na tela com cor branca/azul
    char *message = "Bem-vindo ao Riv-Nix Kernel!";
    int i = 0;
    
    while(message[i] != 0) {
        video_memory[i*2] = message[i];
        video_memory[i*2 + 1] = 0x0f; // Atributo de cor (texto branco, fundo preto)
        i++;
    }

    while(1); // Trava o kernel em loop infinito
}
