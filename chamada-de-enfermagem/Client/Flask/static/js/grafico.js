// documentacao:
// https://www.chartjs.org/docs/latest/developers/api.html

labels = {
  hora: ["00h","01h","02h","03h","04h","05h",
                  "06h","07h","08h","09h","10h","11h",
                  "12h","13h","14h","15h","16h","17h",
                  "18h","19h","20h","21h","22h","23h"],
        
  mes: ["Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"]
}

axesY = {
  mes: [{ticks: {min: 0, max:50}}],
  hora: [{ticks: {min: 0, max:10}}]
}
grafico = new Chart("grafico-chamadas", {
  type: "line",
  data: {
    labels: [],
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: " rgba(255, 0, 0, 1.0)",
      borderColor: "rgba(0, 0, 0, 0.66)",
    }]
  },
  options: {
    legend: {display: false},
  }
}); 

async function atualizarGrafico(tipo = "mes") {
  try {
    fetch(`/api/chamadas/dia/mes`).then(res => res.json()).then(dados => {

      dias = dados.map(item => item.day)
      quant = dados.map(item => item.count)

      grafico.data.datasets[0].data = quant;
      grafico.data.labels = dias;
      grafico.options.scales.yAxes = axesY[tipo];
      grafico.update();
    });
  } catch (err) {
    console.error("Erro ao carregar dados do gráfico:", err);
  }
}

document.getElementById("tipo-grafico").addEventListener("change", (e) => {
  atualizarGrafico(e.target.value);
})

atualizarGrafico();