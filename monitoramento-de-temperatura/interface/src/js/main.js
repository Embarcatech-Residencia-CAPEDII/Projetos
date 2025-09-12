
import { buscarTemperaturas } from './api.js';
import { createGauge } from './gauge.js';


//carrega dados de temperatura de todas as salas cadastradas no sistema.
async function carregarTemperaturas() {
  const token = localStorage.getItem("token");

  if (!token) {
    alert("Sessão expirou, Faça login novamente");
    window.location.href = "login.html";
    return;
  }
  try {
    const dados = await buscarTemperaturas();

    if (!dados.length) {
      console.error("Nenhum dado encontrado.");
      return;
    }

    // Agrupa por sala e pega a última leitura
    const ultimasPorSala = {};
    dados.forEach(dado => {
      const sala = dado.room; // certifique-se que o campo no JSON se chama "room"
      if (!ultimasPorSala[sala._id] || new Date(dado.timestamp) > new Date(ultimasPorSala[sala._id].timestamp)) {
        ultimasPorSala[sala._id] = dado;
      }
    });

    // Transforma em array
    const salas = Object.values(ultimasPorSala);

    sortRoom(salas);

    // Renderiza grid
    const grid = document.getElementById("salasGrid");
    grid.innerHTML = "";

    salas.forEach(sala => {
      const card = document.createElement("div");
      card.className = "temp-sala";
      card.onclick = () => abrirSala(sala.room._id);

      // ids únicos para cada gauge
      const tempGaugeId = `gauge-temp-${sala.room._id}`;
      const humGaugeId = `gauge-hum-${sala.room._id}`;

      card.innerHTML = `
    <h3>${sala.room.name.toUpperCase()}</h3>
    <div id="${tempGaugeId}" style="width:200px; height:200px;"></div>
    
    <p><em>Última atualização: ${new Date(sala.timestamp).toLocaleString("pt-BR")}</em></p>
  `;

      grid.appendChild(card);

      // cria o gauge de temperatura
      createGauge(tempGaugeId, humGaugeId, sala);
    });

  } catch (error) {
    console.error("Erro ao carregar salas:", error);

    if (error.message.includes("401") || error.message.includes("403")) {
      alert("Acesso não autorizado. Faça login novamente.");
      localStorage.removeItem("token");
      window.location.href = "login.html";
    }
  }
}


carregarTemperaturas();
setInterval(carregarTemperaturas, 60000); // atualiza a cada 1min


//redirecionamento para pagina de uma sala apartir do id
function abrirSala(salaId) {
  window.location.href = `room.html?sala=${salaId}`;
}

// Ordena pelo nome da sala (ordem crescente)
function sortRoom(salas) {
  salas.sort((a, b) => {
    // extrai apenas os números do nome (ex: sala-01 -> 1)
    const numA = parseInt(a.room.name.replace(/\D/g, ''), 10);
    const numB = parseInt(b.room.name.replace(/\D/g, ''), 10);

    return numA - numB;
  });
}


//eventListener de logout
document.getElementById("logoutBtn").addEventListener("click", async function () {
  localStorage.removeItem("token");
  window.location.href = "login.html";
});
