import Page from "./classes/Page";
import MMU from "./classes/MMU";
import Random from "./classes/algorithms/Random";
import { act } from "react";

let symbolTable = new Map<number, number[]>();
const algorithm = new Random();
const mmu = new MMU(algorithm);
let actualPageId = 0

function main(){
    const pid = 1;
    const size = 409600;
    const ptr = mmu.new(pid, size);
    symbolTable.set(ptr, [pid, size]);
    console.log('Symbol Table',symbolTable);
    console.log(mmu.realMemory);
    console.log(mmu.pageTable);

    const pid2 = 2;
    const size2 = 40;
    const ptr2 = mmu.new(pid2, size2);


}

export { actualPageId, main };




