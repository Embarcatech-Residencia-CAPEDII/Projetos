

export async function buscarTemperaturas() {
  const token = localStorage.getItem("token");
  const response = await fetch("http://localhost:3000/api/temperatures",
    {headers: {
       "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`   
    }
  });

  if (!response){
    throw new Error(`Erro na requisição: ${response.status}`);
  }
  const dados = await response.json();
  return dados;
}


export async function buscarTemperaturaSala(salaId)  {
    const token = localStorage.getItem("token");
    const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures`,
    {headers: {
       "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`   
    }
  });

  if (!response){
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  return dados;
}


export async function buscarTempIntervalo(salaId, start, end){
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures/?start=${start}&end=${end}`,
    {headers: {
       "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`   
    }
  });

  if (!response){
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  return dados;
}