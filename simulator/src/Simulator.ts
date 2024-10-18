import Page from "./classes/Page";
import MMU from "./classes/MMU";

import AlgorithmStrategy from "./classes/algorithms/AlgorithmStrategy";
import Random from "./classes/algorithms/Random";
import FIFO from "./classes/algorithms/FIFO"
import MRU from "./classes/algorithms/MRU"
import SecondChance from "./classes/algorithms/SecondChance";
import Optimal from "./classes/algorithms/Optimal";

import seedrandom from 'seedrandom';

import { saveAs } from 'file-saver';

let random: () => number;
let algorithm: AlgorithmStrategy;
let mmu: MMU;
let optimalMMU: MMU;
let instructions: string;

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

function makeInstructionsFile(seed: string, cant_processes: number, cant_instructions: number) {
    random = seedrandom(seed);

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
        const size = Math.floor(Math.random() * 100000);
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

function getUsesOptimal(instructions: string): number[] {
    let usesLines = instructions.split('\n').filter(line => line.includes('use'));
    let uses: number[] = [];

    usesLines.forEach(line => {
        let ptr = line.split('(')[1].split(')')[0];
        uses.push(parseInt(ptr));
    });

    return uses;
}

function configSimulation(algtm: string, inst: string) {
    setAlgorithm(algtm);
    optimalMMU = new MMU(new Optimal());
   
    instructions = inst;

    optimalMMU.setUsesArray(getUsesOptimal(instructions));
}

async function newProcess(pid: number, size: number): Promise<MMU[]> {
    await Promise.all([mmu.new(pid, size), optimalMMU.new(pid, size)]);
    return [mmu, optimalMMU];
}

async function usePtr(ptr: number): Promise<MMU[]> {
    await Promise.all([mmu.use(ptr), optimalMMU.use(ptr)]);
    return [mmu, optimalMMU];
}

async function deletePtr(ptr: number): Promise<MMU[]> {
    await Promise.all([mmu.delete(ptr), optimalMMU.delete(ptr)]);
    return [mmu, optimalMMU];
}

async function killProcess(pid: number): Promise<MMU[]> {
    await Promise.all([mmu.kill(pid), optimalMMU.kill(pid)]);
    return [mmu, optimalMMU];
}

export { makeInstructionsFile, configSimulation, algorithm, instructions, newProcess, usePtr, deletePtr, killProcess };




