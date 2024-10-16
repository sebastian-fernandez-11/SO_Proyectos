import "../App.css"
import { start, readInstructions } from '../Simulator'
import { useState } from 'react'
import { useNavigate } from "react-router-dom";

function ConfView() {
  const [fileContent, setFileContent] = useState('');
  const [seed, setSeed] = useState('');
  const [algorithm, setAlgorithm] = useState('fifo');
  const [processes, setProcesses] = useState(10);
  const [instructions, setInstructions] = useState(500);
  const navigate = useNavigate();

  const handleFileChange = (event) => {
    const selectedFile = event.target.files[0];

    const reader = new FileReader();
    reader.onload = (e) => {
      setFileContent(e.target.result);
    };
    reader.readAsText(selectedFile);
  };

  const handleSeedChange = (event) => {
    setSeed(event.target.value);
  }

  const handleAlgorithmChange = (event) => {
    setAlgorithm(event.target.value);
  }

  const handleProcessesChange = (event) => {
    const value = event.target.value;
    setProcesses(parseInt(value));
  }

  const handleInstructionsChange = (event) => {
    const value = event.target.value;
    setInstructions(parseInt(value));
  }

  const handleGenerateInstructions = () => {
    if (seed === '') {
      alert('Debe ingresar una semilla');
      return;
    }

    start(seed, processes, instructions, algorithm);
  }

  const handleStart = () => {
    if (fileContent === '') {
      alert('Primero debe cargar un archivo de instrucciones');
      return;
    }
    if(algorithm === ''){
      alert('Debe seleccionar un algoritmo');
      return;
    }
    
    navigate('/simulation');
    readInstructions(fileContent, algorithm);
  }

  return (
    <div className="container">
      <h1>Simulador de algoritmos de Paginación</h1>
      <h2>Configuración de la simulación</h2>

      <div className="input-group">
        <h3>Indique la semilla:</h3>
        <input type="number" id="semilla" value={seed} onChange={handleSeedChange} />
      </div>

      <div className="input-group">
        <h3>Seleccione el algoritmo a simular:</h3>
        <select id="algorithm" value={algorithm} onChange={handleAlgorithmChange}>
          <option value="fifo">FIFO</option>
          <option value="sc">Second Chance</option>
          <option value="mru">MRU</option>
          <option value="random">Random</option>
        </select>
      </div>

      <div className="input-group">
        <h3>Seleccione un archivo de instrucciones:</h3>
        <input type="file" onChange={handleFileChange} />
      </div>

      <div className="input-group">
        <h3>Indique la cantidad de procesos:</h3>
        <select id="processes" value={processes} onChange={handleProcessesChange}>
          <option value="10">10</option>
          <option value="50">50</option>
          <option value="100">100</option>
        </select>
      </div>

      <div className="input-group">
        <h3>Indique la cantidad de operaciones:</h3>
        <select id="instructions" value={instructions} onChange={handleInstructionsChange}>
          <option value="500">500</option>
          <option value="1000">1000</option>
          <option value="5000">5000</option>
        </select>
      </div>

      <div className="button-group">
        <button className="button button-primary" onClick={handleGenerateInstructions}>Generar archivo</button>
        <button className="button button-secondary" onClick={handleStart}>Empezar la simulación</button>
      </div>

      <div className="file-content">
        <h3>Contenido del archivo:</h3>
        <pre>{fileContent}</pre>
      </div>
    </div>
  );
}

export default ConfView;