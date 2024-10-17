import '../styles/SimulatorView.css';

function StatsInfo({ pagesLoaded, unloadedPages, trashing, time, fragmentation }) {
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
                        <td>{unloadedPages}</td>
                        <td>{trashing}s</td>
                        <td>{Math.round((100 / time) * trashing) + '%'}</td>
                        <td>{fragmentation + 'KB'}</td>
                    </tr>
                </tbody>
            </table>
        </div>
    );
}

export default StatsInfo;