let OnCount = 0;
let OffCount = 0;

const ButtonCountOn = document.getElementById("ButtonCountON");
const ButtonCountOff = document.getElementById("ButtonCountOff");


const buttonstatus = document.getElementById("Buttonstatus");
const buttonOn = document.getElementById("buttonOn");
const buttonOff = document.getElementById("buttonOff");

const communicationStatus = document.getElementById("communicationStatus");

buttonOn.addEventListener(
    "click",
    () => {
        OnCount++;
        ButtonCountOn.textContent = "ON press count: " + OnCount;
        buttonstatus.textContent = "Button Status: ON";
        console.log("LED_ON");
        communicationStatus.textContent = "Communication: Sending LED_ON...";
        setTimeout(
            () => {
                communicationStatus.textContent = "Communication: Command sent";
            },
            3000
        );
    }
);
buttonOff.addEventListener(
    "click",
    () => {
        OffCount++;
        ButtonCountOff.textContent = "OFF press count: " + OffCount;
        buttonstatus.textContent = "Button Status: OFF";
        console.log("LED_OFF");
        communicationStatus.textContent = "Communication: Sending LED_OFF...";
        setTimeout(
            () => {
                communicationStatus.textContent = "Communication: Command sent";
            },
            3000
        );
    }
);
