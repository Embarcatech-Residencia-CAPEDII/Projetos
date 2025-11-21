const vermelho = "#a1423a"
const verde = "#87c454"
const azul = "#4384c4"

function renderMapa(dados, containerId) {
    const container = document.getElementById(containerId);
    container.innerHTML = '';
    
    dados.forEach(dado => {
      div = document.createElement("div")
      div.className = 'celula-enfermaria'
      div.id = dado._id;

      div.appendChild(Object.assign(document.createElement("p"), {
        textContent: `Dispositivo: ${dado.device}`
      }));
  
      div.appendChild(Object.assign(document.createElement("p"), {
        textContent: `Sala: ${dado.room_number}`
      }));

      div.appendChild(Object.assign(document.createElement("p"), {
        textContent: `Estado: ${dado.status}`
      }));

      dataRAW =  dado.updateAt.replace("ISODate('", "").replace("')", "");
      data = new Date(dataRAW);

      div.appendChild(Object.assign(document.createElement("p"), {
        textContent: `Última atualização: ${data.toLocaleString("pt-BR")}`
      }));

      container.appendChild(div);

    });
}

async function fetchMapa(containerId) {
    try {
        const response = await fetch("/api/mapa/status"); 
        if (!response.ok) {
            throw new Error("Erro ao buscar mapa");
        }

        const dados = await response.json(); 
        renderMapa(dados, containerId);

    }
     catch (error) {
        console.error(error);
    }   
}

fetchMapa("mapa");