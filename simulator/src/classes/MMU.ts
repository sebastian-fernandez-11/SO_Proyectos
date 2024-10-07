import Page from './Page';

class MMU{
    realMemory: Page[];
    virtualMemory: Page[];
    pageTable: Map<number, Page[]>;

    constructor(){
        this.realMemory = [];
        this.virtualMemory = [];
        this.pageTable = new Map();
    }

    new(pid: number, size: number): number{
        

        
    }

    use(ptr: number){}

    delete(ptr: number){}

    kill(pid: number){}

}

export default MMU;