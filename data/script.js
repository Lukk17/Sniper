
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onload);

counter = 0;

function initElements() {
    console.log('Setting up..');
    document.getElementById('success_sound').src = 'warfare_spear.mp3';
    document.getElementById('success_sound').muted = false;

    document.body.addEventListener('scroll', documentInteraction);
    document.body.addEventListener('keydown', documentInteraction);
    document.body.addEventListener('click', documentInteraction);
    document.body.addEventListener('touchstart', documentInteraction);
}

// function need to be variable for event remover to work
// it need to be identical as when adding listener
var documentInteraction = function () {
    console.log('First user interaction. Removing listeners and unmuting..');

    document.body.removeEventListener('scroll', documentInteraction);
    document.body.removeEventListener('keydown', documentInteraction);
    document.body.removeEventListener('click', documentInteraction);
    document.body.removeEventListener('touchstart', documentInteraction);

    volumeChange();
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
    var message = event.data;
    console.log(message);

    var messageSplited = message.split(": ");

    if (messageSplited[0].includes("Welcome")) {
        var clearWelcome = messageSplited[1]

        var welcomeMsg = JSON.parse(clearWelcome);

        var stats = "";
        var keys = Object.keys(welcomeMsg);
        keys.forEach(key => {
            stats += '<p class="stats">' + key + ': ' + welcomeMsg[key] + '</p>';
        });

        document.getElementById('stats').innerHTML = stats;

    } else if(messageSplited[0].includes("Sensor value")){
        play();
        var clearSensorValue = messageSplited[1].replace("\"", "");
    
        if (document.getElementById('shootedList').innerHTML.includes("Start !")) {
            document.getElementById('shootedList').innerHTML = '<p class="state">' + counter + '. ' + clearSensorValue + '</p>';
    
        } else {
            document.getElementById('shootedList').innerHTML += '<p class="state">' + counter + '. ' + clearSensorValue + '</p>';
        }
    }

    counter++;
}

function play() {
    document.getElementById('success_sound').play();
}

function volumeChange() {

    if (document.getElementById('soundButton').src.includes("volume-mute-solid.svg")) {
        console.log('Unmuting..');
        document.getElementById('soundButton').src = "volume-up-solid.svg"
        document.getElementById('soundLabel').innerHTML = "Mute"
        document.getElementById('success_sound').muted = false;

    } else {
        console.log('Muting..');
        document.getElementById('soundButton').src = "volume-mute-solid.svg"
        document.getElementById('soundLabel').innerHTML = "Unmute"
        document.getElementById('success_sound').muted = true;
    }
}

function updateInterval(event) {
    event.preventDefault();
    var operation = "updateInterval"
    var miliseconds = document.getElementById('interval').value;
    console.log("Changing WebSocket interval to: " + miliseconds + "ms.");
    websocket.send(operation + " " + miliseconds.toString());
    updateStats();
}

function updateStats() {
    var operation = "updateStats"
    console.log("Updating controller stats..");
    websocket.send(operation);
}

function clearShoots() {
    console.log('Clearing shoots table..');
    document.getElementById('shootedList').innerHTML = '<p class="state" >Start !</p>';
    counter = 1;
}

function changeLedPin(event){
    event.preventDefault();
    var operation = "changeLedPin"
    var pin = document.getElementById('ledPin').value;
    console.log("Changing LED pin to: " + pin + ".");
    websocket.send(operation + " " + pin.toString());
    updateStats();
     
}