#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>


WebServer server(80);

Servo servo1;
Servo servo2;

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Sail Boat Control</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<style>
body {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  height: 100vh;
  margin: 0;
  font-family: Arial, sans-serif;
}
.slider {
  width: 80%;
  margin: 20px;
}
</style>
</head>
<body>
<h1>Sail Boat Servos Control</h1>
<div>
  <label for="servo1">Servo 1:</label>
  <input type="range" id="servo1" min="0" max="180" value="90" class="slider">
</div>
<div>
  <label for="servo2">Servo 2:</label>
  <input type="range" id="servo2" min="0" max="180" value="90" class="slider">
</div>
<script>
const servo1Slider = document.getElementById('servo1');
const servo2Slider = document.getElementById('servo2');
let returnInterval;

function setServo1(value) {
  fetch(`/servo1?value=${value}`);
}

function setServo2(value) {
  fetch(`/servo2?value=${value}`);
}

servo1Slider.addEventListener('input', function() {
  clearInterval(returnInterval);
  setServo1(this.value);
});

servo1Slider.addEventListener('change', function() {
  returnInterval = setInterval(function() {
    let currentValue = parseInt(servo1Slider.value);
    if (currentValue > 90) {
      servo1Slider.value = currentValue - 1;
      setServo1(currentValue - 1);
    } else if (currentValue < 90) {
      servo1Slider.value = currentValue + 1;
      setServo1(currentValue + 1);
    } else {
      clearInterval(returnInterval);
    }
  }, 100);
});

servo2Slider.addEventListener('input', function() {
  setServo2(this.value);
});
</script>
</body>
</html>
)rawliteral";

void setup() {
  WiFi.softAP("Sail Boat");
  // Access the webpage at http://192.168.4.1

  servo1.attach(14);
  servo2.attach(13);
  servo1.write(90);
  servo2.write(90);


  server.on("/", HTTP_GET, [](){
  server.send(200, "text/html", htmlPage);
});

server.on("/servo1", HTTP_GET, [](){
  if (server.hasArg("value")) {
    String valueStr = server.arg("value");
    int value = valueStr.toInt();
    servo1.write(value);
    server.send(200, "text/plain", "OK");
  }
});

server.on("/servo2", HTTP_GET, [](){
  if (server.hasArg("value")) {
    String valueStr = server.arg("value");
    int value = valueStr.toInt();
    servo2.write(value);
    server.send(200, "text/plain", "OK");
  }
});


server.begin();

}

void loop() {
  server.handleClient();
}
