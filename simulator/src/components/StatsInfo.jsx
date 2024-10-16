import '../styles/SimulatorView.css';

function StatsInfo({ pagesLoaded, pagesUnloead, trashing, fragmentation }) {
    return (
        <div className="stats-info-table">
            <table>
                <thead>
                    <th>Páginas cargadas</th>
                    <th>Páginas no cargadas</th>
                    <th colSpan="2">Trashing</th>
                    <th>Fragmentación</th>
                </thead>
                <tbody>
                    <tr>
                        <td>{pagesLoaded}</td>
                        <td>{pagesUnloead}</td>
                        <td>{trashing}</td>
                        <td>{trashing}</td>
                        <td>{fragmentation + 'KB'}</td>
                    </tr>
                </tbody>
            </table>
        </div>
    );
}

export default StatsInfo;