let globalMRU = 0;

class Page {
    id: number;
    realAddress: number;
    isInRealMemory: boolean;
    timestampFIFO: number;
    timestampMRU: number;
    chanceBit: boolean;
    
    constructor(id: number, isInRealMemory: boolean, realAddress: number){
        this.id = id;
        this.isInRealMemory = isInRealMemory;
        this.realAddress = realAddress;
        this.timestampFIFO= Date.now();
        this.timestampMRU= globalMRU;
        this.chanceBit = false;
    }

    increseMRU(){
        this.timestampMRU = globalMRU++;
    }
}

export default Page;