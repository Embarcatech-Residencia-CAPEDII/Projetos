
const params = new URLSearchParams(window.location.search);
const salaAtual = params.get("sala");

import { buscarTemperaturaSala, buscarTempIntervalo } from './api.js';
import { createGauge } from './gauge.js';
import { tempChart, umidChart } from './charts.js';


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

    const grid = document.getElementById("salaGrid");
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




function calcularIntervalo() {
  const agora = new Date();
  const start = new Date(agora);
  start.setHours(agora.getHours() - 24); // Subtrai 24 horas para obter o "start" de 24 horas atrás

  // Formatar as datas no formato ISO 8601 para passar na URL
  const startISO = start.toISOString();
  const endISO = agora.toISOString(); // A data de "agora" será o "end"

  return { start: startISO, end: endISO };
}


// Função que busca e exibe a temperatura das últimas 24 horas
async function tempIntervalRoom() {

  const token = localStorage.getItem("token");

  if (!token) {
    alert("Sessão expirou, Faça login novamente");
    window.location.href = "login.html";
    return;
  }
  try {
    const { start, end } = calcularIntervalo(); // Calcula o intervalo de 24 horas

    const dados = await buscarTempIntervalo(salaAtual, start, end);
    console.log(dados);

    if (!dados.length) {
      console.error("Nenhum dado encontrado.");
      return;
    }

    // Renderiza grid
    const grid = document.getElementById("list");
    grid.innerHTML = "";

    dados.forEach(sala => {
      const list = document.createElement("div");
      list.className = "temp-sala";

      list.innerHTML = `
    <h3>${sala.room.name.toUpperCase()}</h3>
    <h3>Temperatura: ${sala.temperature}</h3>
    <h3>Umidade: ${sala.humidity}</h3>    
    <p><em>Última atualização: ${new Date(sala.timestamp).toLocaleString("pt-BR")}</em></p>
    <p>-----------------------------------------------------</p>
  `;

      grid.appendChild(list);
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

// Chama a função intervalo
tempIntervalRoom();
setInterval(tempIntervalRoom, 60000); // Atualiza a cada 1 minuto



//Atualiza dados do grafico
async function atualizarGraficos() {
  try {
    const { start, end } = calcularIntervalo();
    const dados = await buscarTempIntervalo(salaAtual, start, end);

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

// Inicializa e atualiza a cada minuto
atualizarGraficos();
setInterval(atualizarGraficos, 60000);

