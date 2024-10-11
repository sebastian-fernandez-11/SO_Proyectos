import Page from "./classes/Page";
import MMU from "./classes/MMU";
import Random from "./classes/algorithms/Random";
import FIFO from "./classes/algorithms/FIFO"


// let symbolTable = new Map<number, number[]>();
const algorithm = new FIFO();
const mmu = new MMU(algorithm);
let actualPageId = 0

function main(){
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

export { actualPageId, main };




