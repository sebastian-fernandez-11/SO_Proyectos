import '../styles/SimulatorView.css';

function RamViewer({ mmu, algorithm }) {
    return(
        <div>
            <h2>{'RAM - ' + algorithm}</h2>
            <div className="ram-section">
                {mmu.realMemory.map(page => (
                <div key={page.id} className="ram-cell">{page.id}</div>
                ))}
            </div>
        </div>
    );
}

export default RamViewer;