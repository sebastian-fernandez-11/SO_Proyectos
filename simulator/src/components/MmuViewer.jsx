import '../styles/SimulatorView.css';

function MmuViewer({ mmu, algorithm, colors, symbolTable }) {
    const totalMemory = [...mmu.realMemory, ...mmu.virtualMemory];

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
        <>
            <h2>{'MMU - ' + algorithm}</h2>
            <div className="table">
                <table >
                    <thead>
                        <tr>
                            <th>Page</th>
                            <th>Real Address</th>
                            <th>IsInRealMemory</th>
                        </tr>
                    </thead>
                    <tbody>
                        {totalMemory.map((page, index) => (
                            <tr key={index} style={{ backgroundColor: getColor(page.id) }}>
                                <td>{page.id === -1 ? ' ': page.id}</td>
                                <td>{page.realAddress === -1 ? ' ': page.realAddress}</td>
                                <td>{page.id === -1 ? ' ':                                
                                page.isInRealMemory ? 'Sí' : 'No'}</td>
                            </tr>
                        ))}
                    </tbody>
                </table>
            </div>
        </>
    );
}

export default MmuViewer;