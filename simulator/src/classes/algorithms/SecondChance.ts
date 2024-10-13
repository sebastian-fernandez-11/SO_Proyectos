import  AlgorithmStrategy  from './AlgorithmStrategy'
import Page from '../Page'

class SecondChance implements AlgorithmStrategy {
    selectPage(realMemory: Page[]): number {
        let min = Infinity;
        let minIndex = -1;
        realMemory.forEach((page, index) => {
            if(page.chanceBit === false && page.isInRealMemory && page.timestampFIFO < min){
                min = page.timestampFIFO;
                minIndex = index;
            }
        })

        return realMemory[minIndex].realAddress;
    }
}

export default SecondChance;