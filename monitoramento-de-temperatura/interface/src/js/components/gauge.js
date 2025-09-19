
export function createGauge(tempId, umidId, dado) {

    new JustGage({

        id: tempId, // the id of the html element
        value: dado.temperature,
        min: 0,
        max: 100,
        decimals: 0,
        symbol: '°c',
        gaugeWidthScale: 1,
        title: "Lone Ranger",
        // label: "Temperatura",
        pointer: "true",
        pointerOptions: {
            toplength: 0,
            bottomlength: 15,
            bottomwidth: 3,
            stroke: 'none',
            stroke_width: 0,
            stroke_linecap: 'square',
            color: 'dark'
        },
        levelColors: ["#069625ff", "#d9ff00ff", "#ff0000ff"],
        gaugeColor: "rgba(219, 216, 216, 1)",
        valueFontColor: 'white'
    });

    // // update the value randomly
    // setInterval(() => {
    //   gauge.refresh(Math.random() * 100);
    // }, 5000)

    new JustGage({

        id: umidId, // the id of the html element
        value: dado.humidity,
        min: 0,
        max: 100,
        decimals: 0,
        gaugeWidthScale: 1,
        symbol: '%',
        // label: "Umidade",
        pointer: "true",
        pointerOptions: {
            toplength: 0,
            bottomlength: 15,
            bottomwidth: 3,
            stroke: 'none',
            stroke_width: 0,
            stroke_linecap: 'square',
            color: 'dark'
        },
        levelColors: ["#9bd4faff", "#008cffff", "#0400ffff"],
        gaugeColor: "rgba(219, 216, 216, 1)",
        valueFontColor: 'white'
    });

}
