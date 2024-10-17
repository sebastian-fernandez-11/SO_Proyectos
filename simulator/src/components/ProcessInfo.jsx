import '../styles/SimulatorView.css';

function ProcessInfo({ processes, time }) {
    return (
        <div className="process-info-table">
            <table>
                <thead>
                    <tr>
                        <th>Procesos</th>
                        <th>Tiempo de simulación</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>{processes}</td>
                        <td>{time}s</td>
                    </tr>
                </tbody>
            </table>
        </div>
    );
}

export default ProcessInfo;