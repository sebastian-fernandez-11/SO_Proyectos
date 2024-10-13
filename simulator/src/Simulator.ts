import Page from "./classes/Page";
import MMU from "./classes/MMU";
import Random from "./classes/algorithms/Random";
import FIFO from "./classes/algorithms/FIFO"

import seedrandom from 'seedrandom';

const random = seedrandom('1');


// let symbolTable = new Map<number, number[]>();
const algorithm = new Random();
const mmu = new MMU(algorithm);
let actualPageId = 0

function main(){
    // generateSentences();
    // return; 

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

    mmu.delete(ptr);
    // mmu.kill(pid);
    console.log('Memory', mmu.realMemory);
    console.log('Virtual', mmu.virtualMemory);
}

function generateSentences(){
    const cant_processes = Math.floor(random() * 10);
    for(let i = 0; i < cant_processes; i++){
        const pid = Math.floor(random() * 100);
        const size = Math.floor(random() * 1000);
        console.log('Making new process with pid:', pid, 'and size:', size);
        // const ptr = mmu.new(pid, size);
        // console.log('Memory', mmu.realMemory);
        // console.log('Virtual', mmu.virtualMemory);
    }

    const cant_use = Math.floor(random() * 10);
    for(let i = 0; i < cant_use; i++){
        const ptr = Math.floor(random() * 100);
        console.log('Using ptr:', ptr);
        // mmu.use(ptr);
        // console.log('Memory', mmu.realMemory);
        // console.log('Virtual', mmu.virtualMemory);
    }

    const cant_delete = Math.floor(random() * 10);
    for(let i = 0; i < cant_delete; i++){
        const ptr = Math.floor(random() * 100);
        console.log('Deleting ptr:', ptr);
        // mmu.delete(ptr);
        // console.log('Memory', mmu.realMemory);
        // console.log('Virtual', mmu.virtualMemory);
    }

    const cant_kill = Math.floor(random() * 10);
    for(let i = 0; i < cant_kill; i++){
        const pid = Math.floor(random() * 10);
        console.log('Killing pid:', pid);
        // mmu.kill(pid);
        // console.log('Memory', mmu.realMemory);
        // console.log('Virtual', mmu.virtualMemory);
    }
}

export { actualPageId, main };




