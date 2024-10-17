import '../styles/SimulatorView.css';

function RamViewer({ mmu, algorithm, colors, symbolTable }) {

    const getColor = (id) => {
        for (let [ptr, value] of symbolTable) {
            if (value.includes(id)) {
                if (colors.has(ptr)) {
                    return colors.get(ptr);
                }
            }
        }
        return '#FFFFFF'; // Color por defecto si no se encuentra
    }

    return (
        <div className='ram-container'>
            <h2>{'RAM - ' + algorithm}</h2>
            <div className="ram-section">
                {mmu.realMemory.map((page, index) => (
                    <div
                        key={index}
                        className="ram-cell"
                        style={{ backgroundColor: getColor(page.id) }}
                    >
                        {/* {page.id === -1 ? ' ' : page.id} */}
                    </div>
                ))}
            </div>
        </div>
    );
}

export default RamViewer;