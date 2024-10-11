import { AlgorithmStrategy } from './AlgorithmStrategy'
import Page from '../Page'

class FIFO implements AlgorithmStrategy {
    selectPage(realMemory: Page[]): number {
        return realMemory[0].id
    }
}

export default FIFO;