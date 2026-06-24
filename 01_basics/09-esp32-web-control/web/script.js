const message = document.getElementById("message");

const ws = new WebSocket("ws://192.168.1.104:81");

ws.onopen = () => {
    console.log("Connected to ESP32");
}
ws.onmessage = (event) => {
    console.log(event.data);
}

message.addEventListener(
    "click",
    () => {
        ws.send("hellow");
    }
);

