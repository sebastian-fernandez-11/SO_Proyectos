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
                        {mmu.realMemory.map(page => (
                            <tr key={page.id}>
                                <td>{page.id}</td>
                                <td>{page.realAddress}</td>
                                <td>{page.isInRealMemory ? 'Yes' : 'No'}</td>
                            </tr>
                        ))}
                    </tbody>
                </table>
            </div>
        </div>
    );
}

export default MmuViewer;