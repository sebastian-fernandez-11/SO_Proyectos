import Page from './Page';
import AlgorithmStrategy from './algorithms/AlgorithmStrategy';

class MMU {
    realMemory: Page[];
    virtualMemory: Page[];
    processTable: Map<number, number[]>;
    symbolTable: Map<number, number[]>;
    selectStrategy: AlgorithmStrategy;
    actualPageId: number;
    actualPtr: number;

    constructor(selectStrategy: AlgorithmStrategy) {
        this.realMemory = new Array(100).fill(null).map(() => new Page(-1, false, -1));
        this.virtualMemory = [];
        this.processTable = new Map();
        this.symbolTable = new Map();
        this.selectStrategy = selectStrategy;
        this.actualPageId = 1;
        this.actualPtr = 0;
    }

    // Función que realiza el hit para las páginas indicadas 
    setRealMemory(pageNeeded: number) {
        for (let i = 0; i < pageNeeded; i++) {
            const address = this.getSpaceIndex();
            if (address === -1) {
                console.error('No hay espacio en memoria');
                return;
            }

            const page = new Page(this.actualPageId++, true, address);
            this.realMemory[address] = page;

            if (this.symbolTable.has(this.actualPtr)) {
                const value = this.symbolTable.get(this.actualPtr);
                if (value) {
                    value.push(page.id);
                    this.symbolTable.set(this.actualPtr, value);
                }
                else {
                    console.error('Error al agregar la pagina al symbol table');
                }
            }
            else {
                this.symbolTable.set(this.actualPtr, [page.id]);
            }

        }
    }

    // Función que realiza el swap de una página de la memoria real a la memoria virtual
    swapRealToVirtual(realAddress: number) {
        const pageToReplace = this.realMemory[realAddress];

        if (pageToReplace) {
            pageToReplace.isInRealMemory = false;
            pageToReplace.realAddress = -1;
            this.virtualMemory.push(pageToReplace);
        } else {
            console.error('Page to replace not found');
        }

        this.realMemory = this.realMemory.map(page => page.id === pageToReplace.id ? new Page(-1, false, -1) : page);

        console.log('Página swuapeada de real a virtual: ', pageToReplace);
    }

    // Función que realiza el swap de una página de la memoria virtual a la memoria real
    swapVirtualToReal(page: Page, ptr: number) {
        if (this.countFreeSpace() === 0) {
            let idPageToReplace = this.selectStrategy.selectPage(this.realMemory);

            this.swapRealToVirtual(idPageToReplace);
        }
        const address = this.getSpaceIndex();
        if (address === -1) {
            console.error('No hay espacio en memoria');
            return;
        }

        page.isInRealMemory = true;
        page.realAddress = address;
        page.timestampFIFO = Date.now();

        //page.timestampMRU = Date.now();

        this.realMemory[address] = page;
        this.virtualMemory = this.virtualMemory.filter(p => p.id !== page.id);

        console.log('Página swuapeada de virtual a real: ', page);
    }

    // Función que busca una página en ambas memorias y la retorna
    getPage(id: number): Page | undefined {
        let page = this.realMemory.find(p => p.id === id);
        if (page) {
            return page;
        }
        page = this.virtualMemory.find(p => p.id === id);
        return page;
    }

    // Función que busca un espacio en memoria y lo retorna
    getSpaceIndex(): number {
        const index = this.realMemory.findIndex(page => page.isInRealMemory === false);
        return index;
    }

    // Función que cuenta los espacios libres en memoria
    countFreeSpace(): number {
        return this.realMemory.filter(page => page.isInRealMemory === false).length;
    }

    // Función que revisa que las páginas estén en memoria real 
    checkPagesInMemory(pagesId: number[]): boolean {
        for (const id of pagesId) {
            const page = this.realMemory.find(p => p.id === id);
            if (!page?.isInRealMemory) {
                return false;
            }
        }
        return true;
    }

    removeChance() {
        this.realMemory.forEach(page => {
            if (page.chanceBit) {
                page.chanceBit = false;
            }
        });
    }

    new(pid: number, size: number): number {
        this.actualPtr++;
        const pageNeeded = Math.ceil(size / 4096);

        console.log('Page needed: ', pageNeeded);
        if (this.getSpaceIndex() === -1 || this.countFreeSpace() < pageNeeded) {
            console.log('No hay suficiente espacio en memoria');

            while (this.countFreeSpace() < pageNeeded) {
                this.swapRealToVirtual(this.selectStrategy.selectPage(this.realMemory));
            }
        }

        this.setRealMemory(pageNeeded);

        // Agrega el proceso a la tabla de procesos con su ptr
        if (this.processTable.has(pid)) {
            const value = this.processTable.get(pid);
            if (value) {
                value.push(this.actualPtr);
                this.processTable.set(pid, value);
            }
            else {
                console.error('Error al agregar el ptr al process table');
            }
        }
        else {
            this.processTable.set(pid, [this.actualPtr]);
        }

        return this.actualPtr;
    }

    use(ptr: number) {
        if (this.symbolTable.has(ptr)) {
            const value = this.symbolTable.get(ptr);
            if (value) {
                value.forEach(pageId => {
                    const page = this.getPage(pageId);
                    if (page && page.isInRealMemory) {
                        page.chanceBit = true;
                    } else if(page) {
                        page.timestampMRU = Date.now();
                    } else {
                        console.log('Entre antes del remove chance');
                        this.removeChance();
                        console.log('Hice remove chance');
                    }
                })
                while (!this.checkPagesInMemory(value)) {
                    console.log('Páginas no están en memoria real');
                    this.removeChance();
                    value.forEach(pageId => {
                        const page = this.getPage(pageId); 
                        if (page && !page.isInRealMemory) {     
                            this.swapVirtualToReal(page, ptr);
                        } 
                    })
                }
            }
        }
        else {
            console.error('No se encontró el ptr en el symbol table');
        }
    }

    delete(ptr: number) {
        if (this.symbolTable.has(ptr)) {
            console.log('Deleting ptr: ', ptr);
            const value = this.symbolTable.get(ptr);
            if (value) {
                value.forEach(pageId => {
                    const page = this.getPage(pageId);
                    if (page) {
                        if (page.isInRealMemory) {
                            this.realMemory = this.realMemory.map(p => p.id === page.id ? new Page(-1, false, -1) : p);
                        }
                        else {
                            this.virtualMemory = this.virtualMemory.filter(p => p.id !== page.id);
                        }
                    }
                })
                this.symbolTable.delete(ptr);
            }
        }
        else {
            console.error('No se encontró el ptr en el symbol table');
        }
    }

    kill(pid: number) {
        if (this.processTable.has(pid)) {
            const value = this.processTable.get(pid);
            if (value) {
                value.forEach(ptr => {
                    if(this.symbolTable.has(ptr)) {
                        this.delete(ptr);
                    }
                })
            }
            this.processTable.delete(pid);
        }
        else {
            console.error('No se encontró el pid en el process table');
        }
    }

}

export default MMU;