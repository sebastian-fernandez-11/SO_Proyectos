import AlgorithmStrategy from "./AlgorithmStrategy";
import Page from "../Page";

class Optimal implements AlgorithmStrategy {
    type = 'Optimal';

    selectPage(realMemory: Page[], usesArray: number[], symbolTable: Map<number, number[]>): number {
        let pointers: number[] = [];
        // Búsqueda de punteros que estén en memoria real
        for (let page of realMemory) {
            if (page.isInRealMemory) {
                for (let [ptr, value] of symbolTable) {
                    if (value.includes(page.id) && !pointers.includes(ptr)) {
                        pointers.push(ptr);
                        break;
                    }
                }
            }
        }

        // Si en memoria real hay punteros que no se usan, se selecciona la página de ese puntero
        for(let ptr of pointers) {
            if (!usesArray.includes(ptr)) {
                for (let page of realMemory) {
                    if (page.isInRealMemory && symbolTable.get(ptr)?.includes(page.id)) {
                        return page.realAddress;                      
                    }
                }
            }
        }

        // Si todos los punteros de la memoria se usan, se selecciona el que se usará más adelante
        for(let i = usesArray.length - 1; i >= 0; i--) {
            for (let page of realMemory) {
                if (page.isInRealMemory && symbolTable.get(usesArray[i])?.includes(page.id)) {
                    return page.realAddress;
                }
            }
        }

        console.error('No se encontró página a reemplazar');
        return -1;
    }
}

export default Optimal;