import Page from "../Page";

interface AlgorithmStrategy {
    selectPage(realMemory: Page[]): number;
}

export default AlgorithmStrategy;