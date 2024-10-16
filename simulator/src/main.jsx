import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import { BrowserRouter } from 'react-router-dom'
import App from './App.jsx'
import ConfView from './components/ConfView.jsx'
import SimulatorView from './components/SimulatorView.jsx'
import Router from './Router.jsx'
import './index.css'

const container = document.getElementById('root');
const root = createRoot(container);


root.render(
  <BrowserRouter>
    <Router />
  </BrowserRouter>
);
