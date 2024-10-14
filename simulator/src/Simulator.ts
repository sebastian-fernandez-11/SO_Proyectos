import Page from "./classes/Page";
import MMU from "./classes/MMU";

import AlgorithmStrategy from "./classes/algorithms/AlgorithmStrategy";
import Random from "./classes/algorithms/Random";
import FIFO from "./classes/algorithms/FIFO"
import MRU from "./classes/algorithms/MRU"
import SecondChance from "./classes/algorithms/SecondChance";

import seedrandom from 'seedrandom';

import { saveAs } from 'file-saver';

let random: () => number;
let algorithm: AlgorithmStrategy;
let mmu: MMU;

function setAlgorithm(algtm: string) {
    switch (algtm) {
        case 'fifo':
            algorithm = new FIFO();
            break;
        case 'mru':
            algorithm = new MRU();
            break;
        case 'random':
            algorithm = new Random();
            break;
        case 'sc':
            algorithm = new SecondChance();
            break;
        default:
            console.error('Algoritmo desconocido');
    }  
    mmu = new MMU(algorithm);
}

function start(seed: string, cant_processes: number, cant_instructions: number, algtm: string) {
    random = seedrandom(seed);

    setAlgorithm(algtm);

    if(cant_processes <= 0 || cant_instructions <= 0){
        console.error('Número de procesos o instrucciones inválido');
        return;
    }
    
    generateInstructions(cant_processes, cant_instructions);
}

function generateInstructions(cant_processes: number, cant_instructions: number) {
    let processes = new Map<number, number[]>();
    let contPtr = 1;
    let instructions = '';

    for (let i = 1; i <= cant_processes; i++) {
        const size = Math.floor(Math.random() * 400000);
        instructions += `new(${i},${size})\n`;
        processes.set(i, [contPtr++]);
    }

    for (let i = 0; i < cant_instructions - cant_processes; i++) {
        let pid = Math.floor(Math.random() * cant_processes) + 1;
        while (!processes.has(pid)) {
            pid = Math.floor(Math.random() * cant_processes) + 1;
        }

        let pointers = processes.get(pid)!;
        if (pointers.length === 0) {
            continue; 
        }

        let ptr = pointers[Math.floor(Math.random() * pointers.length)];

        const size = Math.floor(Math.random() * 400000);
        const operation = Math.floor(Math.random() * 6);
        switch (operation) {
            case 0:
                instructions += `new(${pid}, ${size})\n`;
                processes.set(pid, [...pointers, contPtr++]);
                break;
            case 1:
            case 4:
            case 5:
                instructions += `use(${ptr})\n`;
                break;
            case 2:
                instructions += `delete(${ptr})\n`;
                processes.set(pid, processes.get(pid)!.filter(p => p !== ptr));
                break;
            case 3:
                instructions += `kill(${pid})\n`;
                processes.delete(pid);
                break;
        }
    }

    const blob = new Blob([instructions], { type: 'text/plain' });
    saveAs(blob, 'instructions.txt');
}

function readInstructions(instructions: string, algtm: string) {
    setAlgorithm(algtm);

    const lines = instructions.split('\n');

    lines.forEach(line => {
        const match = line.match(/(\w+)\((\d+)(?:,\s*(\d+))?\)/);
        if (match) {
            const operation = match[1];
            const id = parseInt(match[2], 10);
            const size = match[3] ? parseInt(match[3], 10) : undefined;

            switch (operation) {
                case 'new':
                    console.log('New process with pid:', id, 'and size:', size);
                    console.log('Types:', typeof id, typeof size);
                    mmu.new(id, size!);
                    console.log('Memory:', mmu.realMemory);
                    break;
                case 'use':
                    console.log('Using ptr:', id);
                    console.log('Types:', typeof id);
                    mmu.use(id);
                    console.log('Memory:', mmu.realMemory);
                    break;
                case 'delete':
                    console.log('Deleting ptr:', id);
                    console.log('Types:', typeof id);
                    mmu.delete(id);
                    break;
                case 'kill':
                    console.log('Killing pid:', id);
                    console.log('Types:', typeof id);
                    mmu.kill(id);
                    break;
                default:
                    console.error(`Operación desconocida: ${operation}`);
            }
        }
    });
}
export { start, readInstructions };




