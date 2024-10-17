import Page from "../Page";

interface AlgorithmStrategy {
    selectPage(realMemory: Page[], usesArray: number[], symbolTable: Map<number, number[]>, actualUsePtr: number): number;
    type: string;
}

export default AlgorithmStrategy;