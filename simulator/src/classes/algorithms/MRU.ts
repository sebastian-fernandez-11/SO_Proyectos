import  AlgorithmStrategy  from './AlgorithmStrategy'
import Page from '../Page'

class MRU implements AlgorithmStrategy {
    selectPage(realMemory: Page[]): number {
        let max = -Infinity;
        let maxIndex = -1;
        realMemory.forEach((page, index) => {
            if(page.timestampMRU > max && page.isInRealMemory){
                max = page.timestampMRU;
                maxIndex = index;
            }
        })

        return realMemory[maxIndex].realAddress;
    }
}

export default MRU;