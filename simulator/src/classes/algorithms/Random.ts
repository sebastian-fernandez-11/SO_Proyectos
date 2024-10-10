import { AlgorithmStrategy } from './AlgorithmStrategy';
import Page from '../Page';

class Random implements AlgorithmStrategy {
    selectPage(realMemory: Page[]): number {
        const index =  Math.floor(Math.random() * realMemory.length);
        return realMemory[index].id
    }
}

export default Random;