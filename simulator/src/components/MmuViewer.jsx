import '../styles/SimulatorView.css';

function MmuViewer({ mmu, algorithm }) {
    return (
        <div>
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
                        {mmu.realMemory.map((page, index) => (
                            <tr key={index}>
                                <td>{page.id === -1 ? ' ': page.id}</td>
                                <td>{page.realAddress === -1 ? ' ': page.realAddress}</td>
                                <td>{page.id === -1 ? ' ':                                
                                page.isInRealMemory ? 'Sí' : 'No'}</td>
                            </tr>
                        ))}
                    </tbody>
                </table>
            </div>
        </div>
    );
}

export default MmuViewer;