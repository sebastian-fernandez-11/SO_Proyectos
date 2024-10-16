import AlgorithmStrategy from "./AlgorithmStrategy";
import Page from "../Page";

class Optimal implements AlgorithmStrategy {
    type = 'Optimal';

    selectPage(realMemory: Page[], usesArray: number[], symbolTable: Map<number, number[]>): number {
        for (let page of realMemory) {
            if(page.isInRealMemory && !usesArray.find(ptr => symbolTable.get(ptr)!.includes(page.id))){
                return page.realAddress;                
            }
        }

        for(let i = usesArray.length - 1; i >= 0; i--){
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