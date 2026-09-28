const vm = new VirtualMachine();
const vga = new VGAScreen(vm, 'screen');

// Trata o upload do binário do kernel direto pelo input da página
document.getElementById('kernelFile').addEventListener('change', (event) => {
    const file = event.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = function(e) {
        const buffer = e.target.result;
        // Carrega o kernel na posição 0x7c00 (padrão do bootloader)
        vm.loadKernel(buffer, 0x7c00);
        
        // Roda um loop contínuo atualizando a tela VGA na web
        setInterval(() => {
            vga.render();
        }, 100); // Atualiza a cada 100ms
        
        alert("Kernel injetado na VM com sucesso! Verifique a tela VGA.");
    };
    reader.readAsArrayBuffer(file);
});
