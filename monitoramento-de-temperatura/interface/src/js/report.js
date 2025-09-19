
import { TempIntervalo } from "./api.js";

document.getElementById('emitirRelatorio').addEventListener('click', async () => {
  const token = localStorage.getItem("token");

  if (!token) {
    alert("Sessão expirou, Faça login novamente");
    window.location.href = "login.html";
    return;
  }

  // Converte os valores dos inputs em ISO
  const  start = new Date(document.getElementById('start').value).toISOString();
  const  end = new Date(document.getElementById('end').value).toISOString();

  try {
    const dados = await TempIntervalo(start, end);
    console.log(dados);

    if (!dados.length) {
      console.error(`Nenhum dado encontrado para o período`);
      window.alert("Sem dados para o período");
      return;
    }

    // Monta CSV
    const csvHeader = 'Data;Temperatura;Umidade\n';
    const csvRows = dados.map(salas => {
      const data = new Date(salas.timestamp).toLocaleString("pt-BR");
      const temp = salas.temperature || '';
      const umid = salas.humidity || '';
      return `${data};${temp};${umid}`;
    });

    const csvContent = csvHeader + csvRows.join('\n');
    downloadCSV(csvContent, `relatorio_${start}_${Date.now()}.csv`);

  } catch (error) {
    console.error("Erro ao carregar sala:", error);

    if (error.message.includes("401") || error.message.includes("403")) {
      alert("Acesso não autorizado. Faça login novamente.");
      localStorage.removeItem("token");
      window.location.href = "login.html";
    }
  }
});


//funcao para baixar arquivo csv
function downloadCSV(content, filename) {
  const blob = new Blob([content], { type: 'text/csv;charset=utf-8;' });
  const link = document.createElement("a");
  if (link.download !== undefined) {
    const url = URL.createObjectURL(blob);
    link.setAttribute("href", url);
    link.setAttribute("download", filename);
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
  }
}

//eventListener de logout
document.getElementById("logoutBtn").addEventListener("click", async function () {
  localStorage.removeItem("token");
  window.location.href = "login.html";
});