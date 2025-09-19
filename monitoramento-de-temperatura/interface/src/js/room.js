
const params = new URLSearchParams(window.location.search);
const salaAtual = params.get("sala");

import { buscarTemperaturaSala, roomTempInterval } from './api.js';
import { createGauge } from './components/gauge.js';
import { tempChart, umidChart } from './components/charts.js';


//carrega e atualiza a temperatura da sala a partir do id
async function roomUpdate() {
  const token = localStorage.getItem("token");

  if (!token) {
    alert("Sessão expirou, Faça login novamente");
    window.location.href = "login.html";
    return;
  }
  try {
    const dados = await buscarTemperaturaSala(salaAtual);

    if (!dados.length) {
      console.error(`Nenhum dado encontrado para ${salaAtual}`);
      return;
    }

    const ultimaLeitura = dados[dados.length - 1];

    const grid = document.getElementById("roomGrid");
    grid.innerHTML = "";

    const tempGaugeId = `gauge-temp-${salaAtual}`;
    const humGaugeId = `gauge-hum-${salaAtual}`;

    grid.innerHTML = `
              <div class="temp">
                <div id="${tempGaugeId}" style="width:320px; height:240px;"></div>
                <h4>Temperatura</h4>
              </div>
              <div class="umid">
                <div id="${humGaugeId}" style="width:320px; height:240px;"></div>
                <h4>Umidade</h4>
              </div>
          </div>    
        `;
    createGauge(tempGaugeId, humGaugeId, ultimaLeitura);

  } catch (error) {
    console.error("Erro ao carregar sala:", error);

    if (error.message.includes("401") || error.message.includes("403")) {
      alert("Acesso não autorizado. Faça login novamente.");
      localStorage.removeItem("token");
      window.location.href = "login.html";
    }
  }
}

roomUpdate();
setInterval(roomUpdate, 60000); // atualiza de 1 em 1 minuto



//Atualiza dados do grafico
async function atualizarGraficos() {
  try {
    const { start, end } = calcularIntervalo();
    const dados = await roomTempInterval(salaAtual, start, end);

    if (!dados || dados.length === 0) {
      console.warn("Nenhum dado retornado para o gráfico.");
      return;
    }

    // Ordenar por timestamp
    dados.sort((a, b) => new Date(a.timestamp) - new Date(b.timestamp));

    console.log(dados);

    const labels = [];
    const temperaturas = [];
    const umidades = [];

    dados.forEach(item => {
      const hora = new Date(item.timestamp).getHours();
      const horaFormatada = `${hora.toString().padStart(2, "0")}h`;

      labels.push(horaFormatada);
      temperaturas.push(item.temperature);
      umidades.push(item.humidity);
    });

    // Atualiza gráfico de temperatura
    tempChart.data.labels = labels;
    tempChart.data.datasets[0].data = temperaturas;
    tempChart.update();

    // Atualiza gráfico de umidade
    umidChart.data.labels = labels;
    umidChart.data.datasets[0].data = umidades;
    umidChart.update();

  } catch (err) {
    console.error("Erro ao atualizar gráficos:", err);
  }
}

function calcularIntervalo() {
  const agora = new Date();
  const start = new Date(agora);
  start.setHours(agora.getHours() - 24); // Subtrai 24 horas para obter o "start" de 24 horas atrás

  // Formatar as datas no formato ISO 8601 para passar na URL
  const startISO = start.toISOString();
  const endISO = agora.toISOString(); // A data de "agora" será o "end"

  return { start: startISO, end: endISO };
}


// Inicializa e atualiza a cada minuto
atualizarGraficos();
setInterval(atualizarGraficos, 60000);


//eventListener de logout
document.getElementById("logoutBtn").addEventListener("click", async function () {
  localStorage.removeItem("token");
  window.location.href = "login.html";
});

