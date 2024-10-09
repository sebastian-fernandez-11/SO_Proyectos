import Page from "./classes/Page";
import MMU from "./classes/MMU";
import { act } from "react";

let symbolTable = new Map<number, number[]>();
const mmu = new MMU();
let actualPageId = 0

function main(){
    const pid = 1;
    const size = 4096;
    const ptr = mmu.new(pid, size);
    symbolTable.set(ptr, [pid, size]);
    console.log(symbolTable);
    console.log(mmu.realMemory);
    console.log(mmu.pageTable);
}

export { actualPageId, main };




