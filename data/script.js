
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onload);

counter = 0;

function initElements() {
    console.log('Setting up..');
    document.getElementById('success_sound').src = 'warfare_spear.mp3';
    document.getElementById('success_sound').muted = false;
}

function onload(event) {
    initElements();
    initWebSocket();
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}


function onMessage(event) {
    counter++;
    play();
    var message = event.data;
    console.log(message);

    var clearSensorValue = message.split(": ")[1].replace("\"", "");
    document.getElementById('shootedList').innerHTML += "<p>" + counter + ". " + clearSensorValue + "</p>";
}

function play() {
    document.getElementById('success_sound').play();
}

function volumeChange() {
    if(document.getElementById('soundButton').src.includes("volume-mute-solid.svg")) {
        document.getElementById('soundButton').src = "volume-up-solid.svg"
    } else {
        document.getElementById('soundButton').src = "volume-mute-solid.svg"
    }
}
