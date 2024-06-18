
const delay = 100; //ms

function makeRequest(terminal) {
    console.log("clicou")
    var url = `http://192.168.15.4/${terminal}`;
    var xhr = new XMLHttpRequest();
    xhr.onload = function() {
        if (xhr.status >= 200 && xhr.status < 300) {
            console.log('Resposta:', xhr.responseText);
            } else {
                console.error('Erro ao fazer a solicitação. Código de status:', xhr.status);
                }
                };
                xhr.open('GET', url);
                xhr.send();
                }

function startRequest(action) {
    intervalId = setInterval(() => {
        makeRequest(action);
    }, delay); // Envia uma requisição a cada 1 segundo. Ajuste conforme necessário.
}

function stopRequest() {
    clearInterval(intervalId);
}

let intervalId;

document.getElementById('up').addEventListener('mousedown', () => startRequest('iu'));
document.getElementById('up').addEventListener('mouseup', stopRequest);
document.getElementById('up').addEventListener('mouseleave', stopRequest);

document.getElementById('down').addEventListener('mousedown', () => startRequest('id'));
document.getElementById('down').addEventListener('mouseup', stopRequest);
document.getElementById('down').addEventListener('mouseleave', stopRequest);

document.getElementById('left').addEventListener('mousedown', () => startRequest('il'));
document.getElementById('left').addEventListener('mouseup', stopRequest);
document.getElementById('left').addEventListener('mouseleave', stopRequest);

document.getElementById('right').addEventListener('mousedown', () => startRequest('ir'));
document.getElementById('right').addEventListener('mouseup', stopRequest);
document.getElementById('right').addEventListener('mouseleave', stopRequest);



document.getElementById('s').addEventListener('click', () => makeRequest('s'));
document.getElementById('m').addEventListener('click', () => makeRequest('m'));
document.getElementById('a').addEventListener('click', () => makeRequest('a'));
document.getElementById('i').addEventListener('click', () => makeRequest('i'));
document.getElementById('r').addEventListener('click', () => makeRequest('r'));
document.getElementById('l').addEventListener('click', () => makeRequest('l'));
document.getElementById('p').addEventListener('click', () => makeRequest('p'));


function LdrRequest() {
    //console.log("clicou LDR")
    var url = `http://192.168.15.4/ldr`;
    var xhr = new XMLHttpRequest();
    xhr.onload = function() {
        if (xhr.status >= 200 && xhr.status < 300) {
            var ldrValue = xhr.responseText;
            console.log(ldrValue)
            //console.log('Valor do LDR:', ldrValue);
            //console.log('Resposta:', xhr.responseText);
            displayLDRValue(ldrValue);
            } else {
                console.error('Erro ao fazer a solicitação. Código de status:', xhr.status);
                }
                };
                xhr.open('GET', url);
                xhr.send();
};



function updateGradient(divname) {
    
    var value = document.getElementById(divname).textContent;
    console.log(value)
    var gradientColor = interpolateColor(value, [255, 255, 0], [255, 0, 0]);
    document.getElementById(divname).style.background = gradientColor;
}

function interpolateColor(factor, color1, color2) {
    var result = color1.slice();
    for (var i = 0; i < 3; i++) {
        result[i] = Math.round(result[i] + factor * (color2[i] - color1[i]));
    }
    return "rgb(" + result.join(",") + ")";
}

function displayLDRValue(ldrValue) {
        let padrao = /(?!:)\d+(?=:)/g;
        let numeros = ldrValue.match(padrao);
        var tl = document.getElementById('tl');
        var tr = document.getElementById('tr');
        var bl = document.getElementById('bl');
        var br = document.getElementById('br');
        if (tl) {
            tl.textContent = ((numeros[0])/4096).toFixed(2)
            tr.textContent = ((numeros[1])/4096).toFixed(2)
            bl.textContent = ((numeros[2])/4096).toFixed(2)
            br.textContent = ((numeros[3])/4096).toFixed(2)
            updateGradient('tl')
            updateGradient('tr')
            updateGradient('bl')
            updateGradient('br')
        } else {
            console.error('Elemento HTML não encontrado para exibir o valor do LDR');
        }
    }

var intervalIdLDR;

function toggleSendingRequests() {
    console.log('clique')
    let button = document.getElementById('botao');
    console.log(button)
    if (intervalIdLDR) {
        clearInterval(intervalIdLDR);
        intervalIdLDR = null;
        button.textContent = 'Iniciar Função';
    } else {
        intervalIdLDR = setInterval(LdrRequest, 1000); // 10000 milissegundos = 10 segundos
        button.textContent = 'Parar Envio de Requisições';
    }
}
