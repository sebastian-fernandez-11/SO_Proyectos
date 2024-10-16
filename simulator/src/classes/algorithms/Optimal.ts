import AlgorithmStrategy from "./AlgorithmStrategy";
import Page from "../Page";

class Optimal implements AlgorithmStrategy {
    type = 'Optimal';

    selectPage(realMemory: Page[], usesArray: number[], symbolTable: Map<number, number[]>): number {
        for(let ptr of symbolTable.keys()){
            if(!usesArray.includes(ptr)){
                for(let page of realMemory){
                    if(page.isInRealMemory && symbolTable.get(ptr)?.includes(page.id)){
                        return page.realAddress;
                    }
                }
            }
        }
        
        // for (let page of realMemory) {
        //     if(page.isInRealMemory && !usesArray.find(ptr => symbolTable.get(ptr)!.includes(page.id))){
        //         return page.realAddress;                
        //     }
        // }

        console.log('------SEGUNDO FOR------');

        for(let i = usesArray.length - 1; i >= 0; i--){
            if(!symbolTable.has(usesArray[i])){
                continue;
            }
            for(let page of realMemory){
                if(page.isInRealMemory && symbolTable.get(usesArray[i])!.includes(page.id)){
                    return page.realAddress;
                }
            }
        }

        console.error('No se encontró página a reemplazar');
        return -1;
    }
}

export default Optimal;