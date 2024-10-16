import { Routes, Route } from 'react-router-dom';
import ConfView from './components/ConfView';
import  SimulatorView  from './components/SimulatorView';

function Router() {
    return (
        <Routes>
            <Route path="/" element={<ConfView />} />
            <Route path="/simulation" element={<SimulatorView />} />
        </Routes>
    );
}

export default Router;