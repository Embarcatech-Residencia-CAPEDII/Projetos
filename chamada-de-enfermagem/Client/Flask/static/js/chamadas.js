const contagemChamada = document.getElementById("contagemChamada")
const contagemDispositivos = document.getElementById("contagemDispositivos")

function renderChamadas(dados, containerId) {
  const container = document.getElementById(containerId);

  dados.forEach(item => {
    const div = document.createElement("div");
    div.className = "chamada-exibir borda-conteiner";

    div.appendChild(Object.assign(document.createElement("p"), {
      textContent: `Dispositivo: ${item.dispositivo_id}`
    }));

    div.appendChild(Object.assign(document.createElement("p"), {
      textContent: `Local: ${item.local}`
    }));

    data = new Date(item.data)
    div.appendChild(Object.assign(document.createElement("p"), {
      textContent: `Data: ${data.toLocaleString("pt-BR")}`
    }));

    container.appendChild(div);
  });
}

async function fetchChamadas(containerId) {
    try {
        const response = await fetch("/api/chamadas"); 
        if (!response.ok) {
            throw new Error("Erro ao buscar chamadas");
        }

        const dados = await response.json(); 
        renderChamadas(dados, containerId);

    }
     catch (error) {
        console.error(error);
    }   

    try {
        const response = await fetch("/api/chamadas/dia/contagem"); 
        const dados = await response.json()

        contagemChamada.innerHTML = dados.Quantidade;
    } catch (error) {
      console.error(error);
    }

    try {
        const response = await fetch("/api/devices/quantidade"); 
        const dados = await response.json()

        contagemDispositivos.innerHTML = dados.Quantidade;
    } catch (error) {
      console.error(error);
    }
}

fetchChamadas("chamadas-lista");