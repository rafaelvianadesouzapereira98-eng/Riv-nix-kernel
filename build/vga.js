class VGAScreen {
    constructor(vm, canvasId) {
        this.vm = vm;
        this.canvas = document.getElementById(canvasId);
        this.ctx = this.canvas.getContext('2d');
        
        // Configuração padrão VGA Text Mode 80x25
        this.cols = 80;
        this.rows = 25;
        this.charWidth = 9;
        this.charHeight = 16;
        
        this.vgaBaseAddress = 0xB8000;
        
        // Inicializa fonte básica no canvas
        this.ctx.font = "16px monospace";
    }

    render() {
        // Limpa a tela
        this.ctx.fillStyle = "black";
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
        
        this.ctx.fillStyle = "#00ff00"; // Cor de texto verde clássica de terminal

        let index = 0;
        for (let r = 0; r < this.rows; r++) {
            for (let c = 0; c < this.cols; c++) {
                // Cada caractere na VGA ocupa 2 bytes: [Byte do Caractere] [Byte de Atributo/Cor]
                let memAddress = this.vgaBaseAddress + (index * 2);
                let charCode = this.vm.readMemory(memAddress);
                
                if (charCode !== 0 && charCode !== 32) { // Ignora nulos e espaços vazios
                    let char = String.fromCharCode(charCode);
                    let x = c * this.charWidth;
                    let y = (r + 1) * this.charHeight;
                    this.ctx.fillText(char, x, y);
                }
                index++;
            }
        }
    }
}
