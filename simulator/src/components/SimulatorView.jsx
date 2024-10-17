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
    const [colors, setColors] = useState(new Map());

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

                await sleep(300);

                while (isPaused) {
                    await sleep(100); // Esperar 100 ms antes de verificar nuevamente
                }
            }
        }
    }

    const generateUniqueColor = () => {
        let color;
        do {
            // Generar un color aleatorio
            color = '#' + Math.floor(Math.random() * 16777215).toString(16).padStart(6, '0');

            // Convertir el color a su representación RGB
            let r = parseInt(color.slice(1, 3), 16);
            let g = parseInt(color.slice(3, 5), 16);
            let b = parseInt(color.slice(5, 7), 16);

            // Reducir la intensidad de cada componente RGB
            r = Math.floor(r * 0.9);
            g = Math.floor(g * 0.9);
            b = Math.floor(b * 0.9);

            // Convertir a formato RGBA con opacidad reducida
            color = `rgba(${r}, ${g}, ${b}, 0.5)`; // 0.5 es la opacidad (50%)
        } while (Array.from(colors.values()).includes(color));
        return color;
    };

    const assignColors = () => {
        const newColors = new Map(colors);
        for (let [ptr, value] of mmuState.symbolTable) {
            if (!newColors.has(ptr)) {
                const color = generateUniqueColor();
                newColors.set(ptr, color);
            }
        }
        setColors(newColors);
    };

    useEffect(() => {
        assignColors();
    }, [mmuState, optimalMmmuState]);

    const handleGoBack = () => {
        navigate('/');
    }

    return (
        <div className='simulator-view'>
            <div>
                <RamViewer mmu={optimalMmmuState} algorithm="Óptimo" colors={colors} symbolTable={optimalMmmuState.symbolTable} />
                <RamViewer mmu={mmuState} algorithm={algorithm.type} colors={colors} symbolTable={mmuState.symbolTable} />
                <div className="tables-container">
                    <div className="table-section">
                        <MmuViewer mmu={optimalMmmuState} algorithm="Óptimo" colors={colors} symbolTable={optimalMmmuState.symbolTable} />
                        <ProcessInfo processes={optimalMmmuState.activeProcess} time={optimalMmmuState.clock} />
                        <RamInfo ram={optimalMmmuState.actualRealMemoryUse} vram={optimalMmmuState.actualVirtualMemoryUse} />
                        <StatsInfo pagesLoaded={optimalMmmuState.loadedPages} unloadedPages={optimalMmmuState.unloadedPages} trashing={optimalMmmuState.trashing} time={optimalMmmuState.clock} fragmentation={optimalMmmuState.fragmentation} />
                    </div>
                    <div className="table-section">
                        <MmuViewer mmu={mmuState} algorithm={algorithm.type} colors={colors} symbolTable={mmuState.symbolTable}/>
                        <ProcessInfo processes={mmuState.activeProcess} time={mmuState.clock} />
                        <RamInfo ram={mmuState.actualRealMemoryUse} vram={mmuState.actualRealMemoryUse} />
                        <StatsInfo pagesLoaded={mmuState.loadedPages} unloadedPages={mmuState.unloadedPages} trashing={mmuState.trashing} time={mmuState.clock} fragmentation={mmuState.fragmentation} />
                    </div>
                </div>
            </div>
            <div className='button-section'>
                <button onClick={handleGoBack}>Regresar</button>
                <button onClick={handlePause}>{isPaused === true ? 'Reanudar' : 'Pausar'}</button>
                <button onClick={handleStart}>Iniciar</button>
            </div>
        </div>
    );
}

export default SimulatorView;