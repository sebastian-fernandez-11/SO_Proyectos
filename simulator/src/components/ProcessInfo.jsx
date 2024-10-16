import '../styles/SimulatorView.css';

function ProcessInfo({ processes, time }) {
    return (
        <div className="process-info-table">
            <table>
                <thead>
                    <th>Procesos</th>
                    <th>Tiempo de simulación</th>
                </thead>
                <tbody>
                    <tr>
                        <td>{processes}</td>
                        <td>{time}</td>
                    </tr>
                </tbody>
            </table>
        </div>
    );
}

export default ProcessInfo;