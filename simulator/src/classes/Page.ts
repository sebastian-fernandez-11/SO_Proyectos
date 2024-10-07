class Page {
    id: number;
    realAddress: number | null;
    isInRealMemory: boolean;
    
    constructor(id: number, isInRealMemory: boolean, realAddress: number | null){
        this.id = id;
        this.isInRealMemory = isInRealMemory;
        this.realAddress = realAddress;
    }
}

export default Page;