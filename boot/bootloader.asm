[org 0x7c00]
bits 16

    mov [BOOT_DRIVE], dl      ; O BIOS armazena o drive de boot em DL
    mov bp, 0x9000            ; Configura a pilha (stack)
    mov sp, bp

    call load_kernel          ; Carrega o kernel do disco
    call switch_to_pm         ; Muda para o Modo Protegido de 32-bits
    jmp $

%include "boot/disk.asm"
%include "boot/32bit-gdt.asm"
%include "boot/32bit-switch.asm"

bits 32
BEGIN_PM:
    call KERNEL_OFFSET        ; Pula para o código do kernel em C
    jmp $

BOOT_DRIVE db 0
KERNEL_OFFSET equ 0x1000      ; Onde o kernel será carregado na memória

times 510-($-$$) db 0
dw 0xaa55
