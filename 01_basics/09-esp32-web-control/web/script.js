let OnCount = 0;
let OffCount = 0;

const ButtonCountOn = document.getElementById("ButtonCountON");
const ButtonCountOff = document.getElementById("ButtonCountOff");


const buttonstatus = document.getElementById("Buttonstatus");
const buttonOn = document.getElementById("buttonOn");
const buttonOff = document.getElementById("buttonOff");

buttonOn.addEventListener(
    "click",
    () => {
        OnCount++;
        ButtonCountOn.textContent = "ON press count: " + OnCount;
        buttonstatus.textContent = "Button Status: ON";
    }
);
buttonOff.addEventListener(
    "click",
    () => {
        OffCount++;
        ButtonCountOff.textContent = "OFF press count: " + OffCount;
        buttonstatus.textContent = "Button Status: OFF";
    }
);
