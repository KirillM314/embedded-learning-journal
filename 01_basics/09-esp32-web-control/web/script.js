let OnCount = 0;
let OffCount = 0;

const ButtonCountOn = document.getElementById("ButtonCountON");
const ButtonCountOff = document.getElementById("ButtonCountOff");


const buttonstatus = document.getElementById("Buttonstatus");
const buttonOn = document.getElementById("buttonOn");
const buttonOff = document.getElementById("buttonOff");

const communicationStatus = document.getElementById("communicationStatus");

const ledIndicator = document.getElementById("ledIndicator")
const ledStatus = document.getElementById("ledStatus")

buttonOn.addEventListener(
    "click",
    () => {
        OnCount++;
        buttonstatus.textContent = "Button Status: ON";
        ButtonCountOn.textContent = "ON press count: " + OnCount;
        communicationStatus.textContent = "Communication: Sending ledOn...";
        fetch("http://192.168.1.104/ledOn")
            .then(response => response.text())
            .then(data => {
                console.log("route data /ledOn: ", data);
                //buttonstatus.textContent = "Button Status: ON";
                communicationStatus.textContent = "Communication: successful sending";
                //ledIndicator.style.backgroundColor = "green";
                //ledStatus.textContent = "ON";
            });
        fetch("http://192.168.1.104/status")
            .then(response => response.json())
            .then(data => {
                ledIndicator.style.backgroundColor = data.color;
                ledStatus.textContent = data.led;
            })
        
    }
);
buttonOff.addEventListener(
    "click",
    () => {
        OffCount++;
        buttonstatus.textContent = "Button Status: OFF"
        ButtonCountOff.textContent = "OFF press count: " + OffCount;
        communicationStatus.textContent = "Communication: Sending ledOff...";
        fetch("http://192.168.1.104/ledOff")
            .then(response => response.text())
            .then(data => {
                console.log("route data /ledOff: ", data);
                //buttonstatus.textContent = "Button Status: OFF";
                communicationStatus.textContent = "Communication: successful sending";
                //ledIndicator.style.backgroundColor = "gray";
                //ledStatus.textContent = "OFF";
            });
        fetch("http://192.168.1.104/status")
            .then(response => response.json())
            .then(data => {
                ledIndicator.style.backgroundColor = data.color;
                ledStatus.textContent = data.led;
            })
        
    }
);

