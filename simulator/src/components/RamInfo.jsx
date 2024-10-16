import '../styles/SimulatorView.css';

function RamInfo({ ram, vram }) {
    return (
        <div className="ram-info-table">
            <table>
                <thead>
                    <th>RAM KB</th>
                    <th>RAM %</th>
                    <th>V-RAM KB</th>
                    <th>V-RAM %</th>
                </thead>
                <tbody>
                    <tr>
                        <td>{ram}</td>
                        <td>{(100 / 400) * ram}</td>
                        <td>{vram}</td>
                        <td>{(vram / 400) * 100}</td>
                    </tr>
                </tbody>
            </table>
        </div>
    );
}

export default RamInfo;
