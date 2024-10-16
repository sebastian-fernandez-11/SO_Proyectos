import  AlgorithmStrategy  from './AlgorithmStrategy';
import Page from '../Page';

class Random implements AlgorithmStrategy {
    type = 'Random';
    
    selectPage(realMemory: Page[]): number {
        let index =  Math.floor(Math.random() * realMemory.length);
        while(!realMemory[index].isInRealMemory){
            index =  Math.floor(Math.random() * realMemory.length);
        }
        return realMemory[index].realAddress;
    }
}

export default Random;