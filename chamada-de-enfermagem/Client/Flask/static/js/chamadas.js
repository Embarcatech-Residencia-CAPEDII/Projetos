function renderChamadas(dados, containerId) {
  const container = document.getElementById(containerId);

  dados.forEach(item => {
    const div = document.createElement("div");
    div.className = "chamada-exibir borda-conteiner";

    div.appendChild(Object.assign(document.createElement("p"), {
      textContent: `Enfermaria: ${item.enfermaria}`
    }));

    div.appendChild(Object.assign(document.createElement("p"), {
      textContent: `Data: ${item.data}`
    }));

    div.appendChild(Object.assign(document.createElement("p"), {
      textContent: `Hora: ${item.hora}`
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
    } catch (error) {
        console.error(error);
    }   
}

fetchChamadas("chamadas-lista");