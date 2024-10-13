class Page {
    id: number;
    realAddress: number;
    isInRealMemory: boolean;
    timestamp: number;
    
    constructor(id: number, isInRealMemory: boolean, realAddress: number){
        this.id = id;
        this.isInRealMemory = isInRealMemory;
        this.realAddress = realAddress;
        this.timestamp = Date.now();
    }
}

export default Page;