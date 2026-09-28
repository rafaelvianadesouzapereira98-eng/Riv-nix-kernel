class VirtualMachine {
    constructor() {
        // 1MB de RAM simulada
        this.memory = new Uint8Array(1024 * 1024);
    }

    loadKernel(binaryArrayBuffer, offset = 0x7c00) {
        const kernelBytes = new Uint8Array(binaryArrayBuffer);
        // Copia o binário do kernel para o endereço de boot padrão na RAM
        this.memory.set(kernelBytes, offset);
        console.log(`Kernel carregado na memória no endereço 0x${offset.toString(16)}! Tamanho: ${kernelBytes.length} bytes`);
    }

    readMemory(address) {
        return this.memory[address];
    }

    writeMemory(address, value) {
        this.memory[address] = value;
    }
}
