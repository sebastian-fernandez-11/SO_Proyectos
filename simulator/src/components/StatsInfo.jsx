import '../styles/SimulatorView.css';

function StatsInfo({ pagesLoaded, unloadedPages, trashing, time, fragmentation }) {
    return (
        <div className="stats-info-table">
            <table>
                <thead>
                    <tr>
                        <th>Páginas cargadas</th>
                        <th>Páginas no cargadas</th>
                        <th colSpan="2" style={{ backgroundColor: trashing > time/2 ? 'rgba(255, 0, 0, 0.5)' : '#d5d5d5' }}
                        >Trashing</th>
                        <th>Fragmentación</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>{pagesLoaded}</td>
                        <td>{unloadedPages}</td>
                        <td style={{ backgroundColor: trashing > time/2 ? 'rgba(255, 0, 0, 0.5)' : '#ffffff' }}
                        >{trashing}s</td>
                        <td style={{ backgroundColor: trashing > time/2 ? 'rgba(255, 0, 0, 0.5)' : '#ffffff' }}
                        >{trashing === 0 ? '0%': Math.round((100 / time) * trashing) + '%'}</td>
                        <td>{fragmentation + 'KB'}</td>
                    </tr>
                </tbody>
            </table>
        </div>
    );
}

export default StatsInfo;