
// Include libraries
#include <Servo.h>
#include <WiFiS3.h>

//Create servo objects
Servo motor;
Servo turn;


const char ssid[] = "ssid";
const char pass[] = "psw";

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // Initialize servo objects
  motor.attach(9);
  turn.attach(10);

  // Check firmware version
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // Connected now so print wifi status
  printWifiStatus();
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    // read the first line of HTTP request header
  String HTTP_req = "";
    while (client.connected()) {
      if (client.available()) {
        Serial.println("New HTTP Request");
        HTTP_req = client.readStringUntil('\n');  // read the first line of HTTP request
        Serial.print("<< ");
        Serial.println(HTTP_req);  // print HTTP request to Serial Monitor
        break;
      }
    }

    // read the remaining lines of HTTP request header
    while (client.connected()) {
      if (client.available()) {
        String HTTP_header = client.readStringUntil('\n');  // read the header line of HTTP request

        if (HTTP_header.equals("\r"))  // the end of HTTP request
          break;

        Serial.print("<< ");
        Serial.println(HTTP_header);  // print HTTP request to Serial Monitor
      }
    }

    if (HTTP_req.indexOf("GET") == 0) {  // check if request method is GET
    //Check what was clicked drive the car in that way by turning the servos.
      if (HTTP_req.indexOf("rc/stop") > -1) {  // check the path
      turnServo(motor, 90);
      turnServo(turn, 100);
      Serial.println("Turned Motor off");

      } else if (HTTP_req.indexOf("rc/forwardonly") > -1) {  // check the path
      turnServo(motor, 70);
      turnServo(turn, 100);
      Serial.println("Forward");

      } else if (HTTP_req.indexOf("rc/forwardleft") > -1) {  // check the path
      turnServo(motor, 75);
      turnServo(turn, 50);
      Serial.println("Turned Left");

      }else if (HTTP_req.indexOf("rc/forwardright") > -1) {  // check the path
      turnServo(motor, 75);
      turnServo(turn, 150);
      Serial.println("Right");

      }else if (HTTP_req.indexOf("rc/back") > -1) {  // check the path
      turnServo(motor, 180);
      turnServo(turn, 100);
      Serial.println("Stopped");
      }else {
        Serial.println("No command");
      }
    }

    // send the HTTP response
    // send the HTTP response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();                     // the separator between HTTP header and body
    // send the HTTP response body
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<link rel=\"icon\" href=\"data:,\">");
    client.println("</head>");
    client.println("<a href=\"/rc/forwardonly\">Forward</a>");
    client.println("<br><br>");
    client.println("<a href=\"/rc/back\">back</a>");
    client.println("<br><br>");
    client.println("<a href=\"/rc/stop\">stop</a>");
    client.println("<br><br>");
    client.println("<a href=\"/rc/forwardleft\">Forward left</a>");
    client.println("<br><br>");
    client.println("<a href=\"/rc/forwardright\">Forward Right</a>");
    client.println("</html>");
    client.flush();

    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
  }
}

void printWifiStatus() {
  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

//Function to turn the servo that passes the servo object and an integer for the angle.
void turnServo(Servo &servo, int angle) {
  //Make sure the angle isnt over 180 degrees
  angle = constrain(angle, 0, 180);
  // turn the servo
  servo.write(angle);
  //delay between each turn
  delay(250);
}
