import  AlgorithmStrategy  from './AlgorithmStrategy'
import Page from '../Page'

class FIFO implements AlgorithmStrategy {
    type = 'FIFO';
    
    selectPage(realMemory: Page[]): number {
        let min = Infinity;
        let minIndex = -1;
        realMemory.forEach((page, index) => {
            if(page.timestampFIFO < min && page.isInRealMemory){
                min = page.timestampFIFO;
                minIndex = index;
            }
        })

        return realMemory[minIndex].realAddress;
    }
}

export default FIFO;

