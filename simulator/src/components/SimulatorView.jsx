import { useState, useEffect } from 'react'
import { algorithm, instructions, newProcess, usePtr, deletePtr, killProcess } from '../Simulator'
import { useNavigate } from "react-router-dom";

import '../styles/SimulatorView.css';

import RamViewer from './RamViewer';
import MmuViewer from './MmuViewer';
import ProcessInfo from './ProcessInfo';
import RamInfo from './RamInfo';
import StatsInfo from './StatsInfo';
import MMU from '../classes/MMU';

function SimulatorView() {
    const navigate = useNavigate();
    const [mmuState, setMmuState] = useState(new MMU());
    const [optimalMmmuState, setOptimalMmuState] = useState(new MMU());
    const [isPaused, setIsPaused] = useState(false);

    const handleStart = () => {
        processInstructions();
    }

    const handlePause = () => {
        setIsPaused(!isPaused);
    }

    async function sleep(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }

    const processInstructions = async () => {
        const lines = instructions.split('\n');
        for (const line of lines) {
            console.log("PROCESANDO LINEA: ", line);
            const match = line.match(/(\w+)\((\d+)(?:,\s*(\d+))?\)/);
            if (match) {
                const operation = match[1];
                const id = parseInt(match[2], 10);
                const size = match[3] ? parseInt(match[3], 10) : undefined;

                let mmu, optimalMMU;
                switch (operation) {
                    case 'new':
                        [mmu, optimalMMU] = await newProcess(id, size);
                        break;
                    case 'use':
                        [mmu, optimalMMU] = await usePtr(id);
                        break;
                    case 'delete':
                        [mmu, optimalMMU] = await deletePtr(id);
                        break;
                    case 'kill':
                        [mmu, optimalMMU] = await killProcess(id);
                        break;
                    default:
                        console.error(`Operación desconocida: ${operation}`);
                }

                setMmuState({ ...mmu });
                setOptimalMmuState({ ...optimalMMU });

                await sleep(500);

                while (isPaused) {
                    await sleep(100); // Esperar 100 ms antes de verificar nuevamente
                }
            }
        }
    }

    const handleGoBack = () => {
        navigate('/');
    }

    return (
        <div>
            <div className="ram-container">
                <RamViewer mmu={optimalMmmuState} algorithm="Optimo" />
                <RamViewer mmu={mmuState} algorithm={algorithm.type} />
                <div className="tables-container">
                    <div className="table-section">
                        <MmuViewer mmu={optimalMmmuState} algorithm="Optimo" />
                        <ProcessInfo processes={0} time={optimalMmmuState.clock} />
                        <RamInfo ram={0} vram={0} />
                        <StatsInfo pagesLoaded={0} pagesNotLoaded={0} trashing={optimalMmmuState.trashing} fragmentation={0} />
                    </div>
                    <div className="table-section">
                        <MmuViewer mmu={mmuState} algorithm={algorithm.type} />
                        <ProcessInfo processes={0} time={mmuState.clock} />
                        <RamInfo ram={0} vram={0} />
                        <StatsInfo pagesLoaded={0} pagesNotLoaded={0} trashing={mmuState.trashing} fragmentation={0} />
                    </div>
                </div>
            </div>
            <div>
                <button onClick={handleGoBack}>Regresar</button>
                <button onClick={handlePause}>{isPaused === true ? 'Reanudar' : 'Pausar'}</button>
                <button onClick={handleStart}>Iniciar</button>
            </div>
        </div>
    );
}

export default SimulatorView;