document.getElementsByClassName('up arr')[0].addEventListener('click', () => makeRequest('up'));
document.getElementsByClassName('down arr')[0].addEventListener('click', () => makeRequest('down'));
document.getElementsByClassName('left arr')[0].addEventListener('click', () => makeRequest('left'));
document.getElementsByClassName('right arr')[0].addEventListener('click', () => makeRequest('right'));

function makeRequest(terminal) {
    var url = `http://192.168.15.3/${terminal}`;
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

