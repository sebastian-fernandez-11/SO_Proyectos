import { createRoot } from 'react-dom/client'
import { BrowserRouter } from 'react-router-dom'
import Router from './Router.jsx'
import './index.css'

const container = document.getElementById('root');
const root = createRoot(container);


root.render(
  <BrowserRouter>
    <Router />
  </BrowserRouter>
);
