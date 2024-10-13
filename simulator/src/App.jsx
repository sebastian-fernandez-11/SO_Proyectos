import './App.css'
import { main, readInstructions } from './Simulator'
import { useState } from 'react'

function App() {
  const [fileContent, setFileContent] = useState('');

  const handleFileChange = (event) => {
    const selectedFile = event.target.files[0];

    const reader = new FileReader();
    reader.onload = (e) => {
      setFileContent(e.target.result); 
    };
    reader.readAsText(selectedFile);
  };

  return (
    <div>
      <button onClick={() => main()} >Probar</button>
      <div>
        <input type="file" onChange={handleFileChange} />
        <button onClick={() => readInstructions(fileContent)} >Procesar archivo</button>
      </div>
      <div>
        <h3>Contenido del archivo:</h3>
        <pre>{fileContent}</pre>
      </div>
    </div>
  )
}

export default App