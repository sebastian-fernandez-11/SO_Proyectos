import  AlgorithmStrategy  from './AlgorithmStrategy'
import Page from '../Page'

class SecondChance implements AlgorithmStrategy {
    type = 'Second Chance';
    
    selectPage(realMemory: Page[]): number {
        let min = Infinity;
        let minIndex = -1;
        realMemory.forEach((page, index) => {
            if(page.chanceBit === false && page.isInRealMemory && page.timestampMRU < min){
                min = page.timestampMRU;
                minIndex = index;
            }
            else if(page.chanceBit === true && page.isInRealMemory){
                page.chanceBit = false;
            }
        })

        return realMemory[minIndex].realAddress;
    }
}

export default SecondChance;