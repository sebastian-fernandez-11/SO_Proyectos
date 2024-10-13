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
        this.timestampMRU= Date.now();
        this.chanceBit = false;
    }
}

export default Page;