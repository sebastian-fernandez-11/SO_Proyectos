import { useState, useEffect } from 'react'
import { algorithm, instructions } from '../Simulator'
import { mmu, optimalMMU, getUsesOptimal } from '../Simulator'
import MMU from '../classes/MMU';
import { useNavigate } from "react-router-dom";
import '../styles/SimulatorView.css';
import RamViewer from './RamViewer';
import MmuViewer from './MmuViewer';
import ProcessInfo from './ProcessInfo';
import RamInfo from './RamInfo';
import StatsInfo from './StatsInfo';

function SimulatorView() {
    const navigate = useNavigate();
    let [mmuState, setMmuState] = useState(mmu);


    async function readInstructions() {
        optimalMMU.setUsesArray(getUsesOptimal(instructions));
        console.log('Optimal uses:', optimalMMU.usesArray);
    
        const lines = instructions.split('\n');
    
        lines.forEach(async line => {
            const match = line.match(/(\w+)\((\d+)(?:,\s*(\d+))?\)/);
            if (match) {
                const operation = match[1];
                const id = parseInt(match[2], 10);
                const size = match[3] ? parseInt(match[3], 10) : undefined;
    
                switch (operation) {
                    case 'new':
                        console.log('New process with pid:', id, 'and size:', size);
                        console.log('Types:', typeof id, typeof size);
                        //await Promise.all([mmu.new(id, size!), optimalMMU.new(id, size!)]);
                         mmu.new(id, size);
                         setMmuState({ ...mmu });
                        // optimalMMU.new(id, size!);
    
                        console.log('Memory:', mmu.realMemory);
                        console.log('Memory Optimal:', optimalMMU.realMemory);
                        break;
                    case 'use':
                        console.log('Using ptr:', id);
                        console.log('Types:', typeof id);
                        //await Promise.all([mmu.use(id), optimalMMU.use(id)]);
                         mmu.use(id);
                         setMmuState({ ...mmu });
                        // optimalMMU.use(id);
                        console.log('Memory:', mmu.realMemory);
                        console.log('Memory Optimal:', optimalMMU.realMemory);
                        break;
                    case 'delete':
                        console.log('Deleting ptr:', id);
                        console.log('Types:', typeof id);
                        // await Promise.all([mmu.delete(id), optimalMMU.delete(id)]);
                        mmu.delete(id);
                        setMmuState({ ...mmu });
                        // optimalMMU.delete(id);
                        break;
                    case 'kill':
                        console.log('Killing pid:', id);
                        console.log('Types:', typeof id);
                        // await Promise.all([mmu.kill(id), optimalMMU.kill(id)]);
                        mmu.kill(id);
                        setMmuState({ ...mmu });
                        // optimalMMU.kill(id);
                        break;
                    default:
                        console.error(`Operación desconocida: ${operation}`);
                }
            }
        });
    }

   

    const handleStart = () => {
        //setMmuState(mmu);
        console.log('Soy el mmu state', mmuState);
        readInstructions();
    }

    const handleGoBack = () => {
        navigate('/');
    }

    return (
        <div>
            <div className="ram-container">
                <RamViewer mmu={mmuState} algorithm="Optimo" />
                <RamViewer mmu={mmuState} algorithm={algorithm.type} />
               {/*<div className="tables-container">
                    <div className="table-section">
                        <MmuViewer mmu={mmuState} algorithm="Optimo"/>
                        <ProcessInfo processes={0} time={0} />
                        <RamInfo ram={0} vram={0} />
                        <StatsInfo pagesLoaded={0} pagesNotLoaded={0} trashing={0} fragmentation={0} />  
                    </div>
                    <div className="table-section">
                        <MmuViewer mmu={mmuState} algorithm={algorithm.type}/>
                        <ProcessInfo processes={0} time={0} />
                        <RamInfo ram={0} vram={0} />
                        <StatsInfo pagesLoaded={0} pagesNotLoaded={0} trashing={0} fragmentation={0} />                    
                    </div>
                </div> */} 
            </div>
            <div>
                <button onClick={handleGoBack}>Regresar</button>
                <button>Pausar</button>
                <button onClick={handleStart}>Iniciar</button>
            </div>
        </div>
    );
}

export default SimulatorView;