import { useState } from 'react'
import { mmu } from '../Simulator'
import Page from '../classes/Page';
import MMU from '../classes/MMU';
import '../styles/SimulatorView.css';
import RamViewer from './RamViewer';
import MmuViewer from './MmuViewer';
import ProcessInfo from './ProcessInfo';
import RamInfo from './RamInfo';
import StatsInfo from './StatsInfo';

function SimulatorView() {
    return (
        <div>
            <div className="ram-container">
                <RamViewer mmu={mmu} algorithm="Optimo" />
                <RamViewer mmu={mmu} algorithm="Algoritmo" />
                <div className="tables-container">
                    <div className="table-section">
                        <MmuViewer mmu={mmu} algorithm="Optimo"/>
                        <ProcessInfo processes={0} time={0} />
                        <RamInfo ram={0} vram={0} />
                        <StatsInfo pagesLoaded={0} pagesNotLoaded={0} trashing={0} fragmentation={0} />  
                    </div>
                    <div className="table-section">
                        <MmuViewer mmu={mmu} algorithm="Algoritmo"/>
                        <ProcessInfo processes={0} time={0} />
                        <RamInfo ram={0} vram={0} />
                        <StatsInfo pagesLoaded={0} pagesNotLoaded={0} trashing={0} fragmentation={0} />                    
                    </div>
                </div>  
            </div>
            <div>
                <button>Regresar</button>
                <button>Pausar</button>
                <button>Iniciar</button>
            </div>
        </div>
    );
}

export default SimulatorView;