const buttonstatus = document.getElementById("Buttonstatus");
const buttonOn = document.getElementById("buttonOn");
const buttonOff = document.getElementById("buttonOff");

buttonOn.addEventListener(
    "click",
    () => {
            buttonstatus.textContent = "Button Status: ON";
    }
);
buttonOff.addEventListener(
    "click",
    () => {
            buttonstatus.textContent = "Button Status: OFF";
    }
);
