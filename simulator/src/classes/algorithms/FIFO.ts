import  AlgorithmStrategy  from './AlgorithmStrategy'
import Page from '../Page'

class FIFO implements AlgorithmStrategy {
    selectPage(realMemory: Page[]): number {
        let min = Infinity;
        let minIndex = -1;
        realMemory.forEach((page, index) => {
            if(page.timestamp < min && page.isInRealMemory){
                min = page.timestamp;
                minIndex = index;
            }
        })

        return realMemory[minIndex].realAddress;
    }
}

export default FIFO;