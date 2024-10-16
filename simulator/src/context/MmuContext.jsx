import { createContext, useState, useContext, useEffect } from 'react';
import { mmu as initialMmu } from '../Simulator';

const MmuContext = createContext();

export const MmuProvider = ({ children }) => {
    const [mmu, setMmu] = useState(initialMmu);

    useEffect(() => {
        setMmu(initialMmu);
    }, [initialMmu]);

    return (
        <MmuContext.Provider value={{ mmu, setMmu }}>
            {children}
        </MmuContext.Provider>
    );
};

export const useMmu = () => useContext(MmuContext);