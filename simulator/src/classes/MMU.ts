import Page from './Page';
import { actualPageId } from '../Simulator';

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
        const pageNeeded = Math.ceil(size / 4096);

        if(this.realMemory.length >= 100){
            console.log('No hay espacio en memoria'); // Aca se deberia utilizar un algortimo de paginacion
            return -1;
        }

        for(let i = 0; i < pageNeeded; i++){
            const page = new Page(actualPageId, true, this.realMemory.length); // el address es el indice de la pagina en la memoria real
            this.realMemory.push(page);
            this.pageTable.set(actualPageId, [page]);
        }

        return actualPageId;
    }

    use(ptr: number){}

    delete(ptr: number){}

    kill(pid: number){}

}

export default MMU;