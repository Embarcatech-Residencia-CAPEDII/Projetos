
export const tempChart = new Chart(document.getElementById('tempChart'), {
    type: 'line',
    data: {
        labels: []
        ,
        datasets: [{
            label: 'Temperatura',

            data: [],
            borderWidth: 6,
            borderColor: 'rgba(255, 251, 0, 0.85)',
            // backgroundColor: 'transparent',
        },

        ]
    },
    options: {
        plugins: {
            title: {
                display: true,
                text: 'Histórico de Temperatura',
                font: {
                    size: 20
                }
            },
            legend: {
                display: true,
                position: 'top'
            }
        },
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

//Type, Data e Options
export const umidChart = new Chart(document.getElementById('umidChart'), {
    type: 'line',
    data: {
        labels: []
        ,
        datasets: [{
            label: 'Umidade',

            data: [],
            borderWidth: 6,
        }

        ]
    },
    options: {
        plugins: {
            title: {
                display: true,
                text: 'Histórico de Umidade',
                font: {
                    size: 20
                }
            },
            legend: {
                display: true,
                position: 'top'
            }
        },
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});
