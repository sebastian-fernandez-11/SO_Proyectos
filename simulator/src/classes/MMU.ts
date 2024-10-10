import Page from './Page';
import { AlgorithmStrategy } from './algorithms/AlgorithmStrategy';

class MMU{
    realMemory: Page[];
    virtualMemory: Page[];
    pageTable: Map<number, Page[]>;
    selectStrategy: AlgorithmStrategy;
    actualPageId: number;

    constructor(selectStrategy: AlgorithmStrategy){
        this.realMemory = [];
        this.virtualMemory = [];
        this.pageTable = new Map();
        this.selectStrategy = selectStrategy;
        this.actualPageId = 0;
    }

    new(pid: number, size: number): number{
        const pageNeeded = Math.ceil(size / 4096);

        console.log('Page needed: ', pageNeeded);

        if(this.realMemory.length > 100 || this.realMemory.length + pageNeeded > 100){
            console.log('No hay espacio en memoria'); // Aca se deberia utilizar un algortimo de paginacion
            const idPageToReplace = this.selectStrategy.selectPage(this.realMemory);
            const pageToReplace = this.realMemory.find(page => page.id === idPageToReplace);
            this.realMemory = this.realMemory.filter(page => page.id !== idPageToReplace);
            
            if (pageToReplace) {
                pageToReplace.isInRealMemory = false;
                this.pageTable.set(pageToReplace.id, [pageToReplace]);
                this.virtualMemory.push(pageToReplace);
            } else {
                console.error('Page to replace not found');
            }

            




           

            console.log('Id de la pagina a reeemplazar: ', pageToReplace);
            // Reemplazar la pagina
         

            
        }

        for(let i = 0; i < pageNeeded; i++){
            const page = new Page(this.actualPageId++, true, this.realMemory.length); // el address es el indice de la pagina en la memoria real
            this.realMemory.push(page);
            this.pageTable.set(this.actualPageId, [page]);
        }

        return this.actualPageId;
    }

    use(ptr: number){}

    delete(ptr: number){}

    kill(pid: number){}

}

export default MMU;