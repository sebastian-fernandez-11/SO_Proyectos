import Page from './Page';
import  AlgorithmStrategy  from './algorithms/AlgorithmStrategy';

class MMU{
    realMemory: Page[];
    virtualMemory: Page[];
    pageTable: Map<number, Page>;
    processTable: Map<number, number[]>;
    symbolTable : Map<number, number[]>;
    selectStrategy: AlgorithmStrategy;
    actualPageId: number;
    actualPtr: number;

    constructor(selectStrategy: AlgorithmStrategy){
        this.realMemory = [];
        this.virtualMemory = [];
        this.pageTable = new Map();
        this.processTable = new Map();
        this.symbolTable = new Map();
        this.selectStrategy = selectStrategy;
        this.actualPageId = 1;
        this.actualPtr = 0;
    }

    // Función que realiza el hit para las páginas indicadas 
    setRealMemory(pageNeeded: number){
        for(let i = 0; i < pageNeeded; i++){
            const page = new Page(this.actualPageId++, true, this.realMemory.length); // el address es el indice de la pagina en la memoria real
            this.realMemory.push(page);
            this.pageTable.set(this.actualPageId, page);
           if(this.symbolTable.has(this.actualPtr)){
               const value = this.symbolTable.get(this.actualPtr);
               if(value){
                   value.push(page.id);
                   this.symbolTable.set(this.actualPtr, value);
               }
               else{
                    console.error('Error al agregar la pagina al symbol table');
               }
            }
            else{
                this.symbolTable.set(this.actualPtr, [page.id]);
            }

        }
    }

    // Función que realiza el swap de una página de la memoria real a la memoria virtual
    swapRealToVirtual(idPageToReplace: number): number{
        const pageToReplace = this.realMemory.find(page => page.id === idPageToReplace);
        this.realMemory = this.realMemory.filter(page => page.id !== idPageToReplace);

        if (pageToReplace) {
            pageToReplace.isInRealMemory = false;
            pageToReplace.realAddress = null;
            this.pageTable.set(pageToReplace.id, pageToReplace);
            this.virtualMemory.push(pageToReplace);
        } else {
            console.error('Page to replace not found');
        }

        console.log('Página swuapeada de real a virtual: ', pageToReplace);
        return idPageToReplace;
    }

    // Función que realiza el swap de una página de la memoria virtual a la memoria real
    // pero sin hacer swap de una página que se ocupe en el momento
    swapVirtualToReal(page: Page,  ptr: number){
        page.isInRealMemory = true;
        page.realAddress = this.realMemory.length;

        if(this.realMemory.length >= 100){
            let idPageToReplace = this.selectStrategy.selectPage(this.realMemory);
            let value = this.symbolTable.get(ptr);
            while(value?.find(id => id === idPageToReplace)){
                idPageToReplace = this.selectStrategy.selectPage(this.realMemory);
            }

            this.swapRealToVirtual(idPageToReplace);
        }

        this.realMemory.push(page);
        this.virtualMemory = this.virtualMemory.filter(p => p.id !== page.id);
        
        console.log('Página swuapeada de virtual a real: ', page);
    }

    // Función que busca una página en ambas memorias y la retorna
    getPage(id: number): Page | undefined{
        let page = this.realMemory.find(p => p.id === id);
        if(page){
            return page;
        }
        page = this.virtualMemory.find(p => p.id === id);
        return page;
    }

    new(pid: number, size: number): number{
        this.actualPtr++;
        const pageNeeded = Math.ceil(size / 4096);

        console.log('Page needed: ', pageNeeded);

        if(this.realMemory.length > 100 || this.realMemory.length + pageNeeded > 100){
            console.log('No hay suficiente espacio en memoria'); 

            while(this.realMemory.length + pageNeeded > 100){
                this.swapRealToVirtual(this.selectStrategy.selectPage(this.realMemory));
            }
        }
        
        this.setRealMemory(pageNeeded);

        // Agrega el proceso a la tabla de procesos con su ptr
        if(this.processTable.has(pid)){
            const value = this.processTable.get(pid);
            if(value){
                value.push(this.actualPtr);
                this.processTable.set(pid, value);
            }
            else{
                console.error('Error al agregar el ptr al process table');
            }
        }
        else{
            this.processTable.set(pid, [this.actualPtr]);
        }

        return this.actualPtr;
    }

    use(ptr: number){
        if(this.symbolTable.has(ptr)){
            const value = this.symbolTable.get(ptr);
            if(value){
                value.forEach(pageId => {
                    // const page = this.pageTable.get(pageId);
                    const page = this.getPage(pageId);
                    if(page && !page.isInRealMemory){
                        this.swapVirtualToReal(page, ptr);
                    }
                })                
            }
        }
        else{
            console.error('No se encontró el ptr en el symbol table');
        }
    }

    delete(ptr: number){
        if(this.symbolTable.has(ptr)){
            console.log('Deleting ptr: ', ptr);
            const value = this.symbolTable.get(ptr);
            if(value){
                value.forEach(pageId => {
                    // const page = this.pageTable.get(pageId);
                    const page = this.getPage(pageId);
                    if(page){
                        if(page.isInRealMemory){
                            this.realMemory = this.realMemory.filter(p => p.id !== page.id);
                        }
                        else{
                            this.virtualMemory = this.virtualMemory.filter(p => p.id !== page.id);
                        }
                        // this.pageTable.delete(page.id);
                    }
                })                
            }
        }
        else{
            console.error('No se encontró el ptr en el symbol table');
        }
    }

    kill(pid: number){
        if(this.processTable.has(pid)){
            const value = this.processTable.get(pid);
            if(value){
                value.forEach(ptr => {
                    this.delete(ptr);
                })
            }
            this.processTable.delete(pid);
        }
        else{
            console.error('No se encontró el pid en el process table');
        }
    }

}

export default MMU;