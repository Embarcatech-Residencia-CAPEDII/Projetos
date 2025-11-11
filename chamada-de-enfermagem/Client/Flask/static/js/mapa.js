const vermelho = "#a1423a"
const verde = "#87c454"
const azul = "#4384c4"

function renderMapa(dados, containerId) {
    const div = document.getElementById(containerId);
    div.className = 'celula-enfermaria'
    div.id = dados._id;
    div.innerHTML = '';

    dados.forEach(dado => {
        div.appendChild(Object.assign(document.createElement("p"), {
          textContent: `Dispositivo: ${dado.device}`
        }));
    
        div.appendChild(Object.assign(document.createElement("p"), {
          textContent: `Sala: ${dado.room_number}`
        }));

        div.appendChild(Object.assign(document.createElement("p"), {
          textContent: `Estado: ${dado.status}`
        }));

        data = new Date(dado.updateAt)

        div.appendChild(Object.assign(document.createElement("p"), {
          textContent: `Última atualização: ${data.toLocaleString("pt-BR")}`
        }));

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