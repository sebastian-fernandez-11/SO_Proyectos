import MMU from "./MMU";

class Process{
    pid: number;
    size: number;
    symbolTable: Map<number, number[]>;

    constructor(pid: number){
        this.pid = pid;
        this.size = 0;
        this.symbolTable = new Map();
    }

    new(pid:number, size: number): number{
        this.pid = pid;
        this.size = size;
        const mmu = new MMU();
        const ptr = mmu.new(pid, size);
        this.symbolTable.set(ptr, [pid, size]);
        
        return ptr;
    }

    use(ptr: number){
        const mmu = new MMU();
        mmu.use(ptr);
    }

    delete(ptr: number){
        const mmu = new MMU();
        this.symbolTable.delete(ptr);
        mmu.delete(ptr);
    }

    kill(pid: number){
        const mmu = new MMU();
        this.symbolTable.forEach((_, key) => {           
            this.symbolTable.delete(key);
            mmu.delete(key);            
        });
    }
}