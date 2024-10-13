import Page from "./classes/Page";
import MMU from "./classes/MMU";
import Random from "./classes/algorithms/Random";
import FIFO from "./classes/algorithms/FIFO"
import MRU from "./classes/algorithms/MRU"
import SecondChance from "./classes/algorithms/SecondChance";

import seedrandom from 'seedrandom';

import { saveAs } from 'file-saver';
import { read, readFileSync } from "fs";

const random = seedrandom('1');


// let symbolTable = new Map<number, number[]>();
const algorithm = new SecondChance();
const mmu = new MMU(algorithm);

function main() {
    generateSentences(5, 5);
    return;

    const pid = 1;
    const size = 409600;
    const ptr = mmu.new(pid, size);
    // symbolTable.set(ptr, [pid, size]);
    // console.log('Symbol Table',symbolTable);
    // console.log(mmu.realMemory);
    // console.log(mmu.pageTable);
    console.log('Memory', mmu.realMemory);

    const pid2 = 1;
    const size2 = 10000;
    const ptr2 = mmu.new(pid2, size2);
    // symbolTable.set(ptr, [pid, size]);
    // console.log('Symbol Table',symbolTable);
    // console.log(mmu.realMemory);
    // console.log(mmu.pageTable);
    console.log('Memory', mmu.realMemory);

   

    mmu.use(ptr);
    console.log('Memory', mmu.realMemory);
    console.log('Virtual', mmu.virtualMemory);
    
    mmu.use(ptr2);
    console.log('Memory', mmu.realMemory);
    console.log('Virtual', mmu.virtualMemory);

    mmu.use(ptr2);
    console.log('Memory', mmu.realMemory);
    console.log('Virtual', mmu.virtualMemory);
    //return;
    mmu.use(ptr);
    console.log('Memory', mmu.realMemory);
    console.log('Virtual', mmu.virtualMemory);
    return;
   
    mmu.delete(ptr);
    // mmu.kill(pid);
    console.log('Memory', mmu.realMemory);
    console.log('Virtual', mmu.virtualMemory);
}

function generateSentences(cant_processes: number, cant_instructions: number) {

    let instructions = '';

    for (let i = 0; i < cant_processes; i++) {
        const size = Math.floor(random() * 400000);
        instructions += `new(${i},${size})\n`;
    }

    for (let i = 0; i < cant_instructions; i++) {
        const pid = Math.floor(random() * cant_processes);
        const operation = Math.floor(random() * 3);
        switch (operation) {
            case 0:
                instructions += `use(${pid})\n`;
                break;
            case 1:
                instructions += `delete(${pid})\n`;
                break;
            case 2:
                instructions += `kill(${pid})\n`;
                break;
        }
    }

    const blob = new Blob([instructions], { type: 'text/plain' });
    saveAs(blob, 'instructions.txt');
    
}

function readInstructions(instructions: string) {
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
                    break;
                case 'use':
                    console.log('Using ptr:', id);
                    console.log('Types:', typeof id);
                    mmu.use(id);
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

export { main, readInstructions };




