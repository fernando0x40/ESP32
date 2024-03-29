#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "esp32";
const char* ssid = "ESP32-FER0X40";
const char* password = "13791379";

const int ledPin = 4;
bool ledState = false;

WebServer server(80);

//Check if header is present and correct
bool is_authentified();

//login page, also called for disconnect
void handleLogin();

//root page can be accessed only if authentification is ok
void handleRoot();

//upload page
void handleUpload();

//test LED page
void handleTestLED();

//serial page
void handleSerial();

//update led value
void handleLedState();

//no need authentification
void handleNotFound();

/*
 * setup function
 */
void setup(void) {
  Serial.begin(115200);

  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,HIGH);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  for (int timeOut = 0 ; WiFi.status() != WL_CONNECTED ; timeOut++) {
    if (timeOut == 10) {
      Serial.println("\n--- Connection Timeout ---\n  Restarting ESP32...");
      ESP.restart();
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/upload",handleUpload);
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.on("/ledstate", handleTestLED);
  server.on("/readLed", handleLedState);
  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
}

void loop(void) {
  server.handleClient();
  delay(1);
  if (server.hasArg("LED")) {
    if (server.arg("LED") == "ON") {
      digitalWrite(ledPin, LOW);
    }
    else if (server.arg("LED") == "OFF") {
      digitalWrite(ledPin, HIGH);
    }
  }
}

/* Style */
String style = {
"<style>"
"  #file-input,"
"  input{"
"    width:100%;"
"    height:44px;"
"    border-radius:4px;"
"    margin:10px auto;"
"    font-size:15px"
"  }input{"
"    background:#f1f1f1;"
"    border:0;"
"    padding:0 15px"
"  }body{"
"    background:#3498db;"
"    font-family:sans-serif;"
"    font-size:14px;"
"    color:#777"
"  }#file-input{"
"    padding:0;"
"    border:1px solid #ddd;"
"    line-height:44px;"
"    text-align:left;"
"    display:block;"
"    cursor:pointer"
"  }#bar,#prgbar{"
"    background-color:#f1f1f1;"
"    border-radius:10px"
"  }#bar{"
"    background-color:#3498db;"
"    width:0%;"
"    height:10px"
"  }form{"
"    background:#fff;"
"    max-width:258px;"
"    margin:75px auto;"
"    padding:30px;"
"    border-radius:5px;"
"    text-align:center"
"  }.btn{"
"    background:#3498db;"
"    color:#fff;"
"    cursor:pointer"
"  }.ledbtn{"
"    background:#ec1f1f;"
"    color:#fff;"
"    cursor:pointer"
"  }"
"</style>" }
;

bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

void handleLogin() {
  Serial.println("Enter handleLogin");
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == "FER0X40" &&  server.arg("PASSWORD") == "13791379") {
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
    msg = "Wrong username/password! try again.";
    Serial.println("Log in Failed");
  }
  String content = "<form name=loginForm>"
  "<h1>ESP32 Login</h1>"
  "<input name=USERNAME placeholder='User ID'> "
  "<input name=PASSWORD placeholder=Password type=Password> "
  "<input type=submit onclick=check(this.form) class=btn value=Login></form>" + style;
  server.send(200, "text/html", content);
}

void handleRoot() {
  Serial.println("Enter handleRoot");
  String header;
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("UPDLOAD")) {
    Serial.println("Entering update page");
    server.sendHeader("Location", "/upload");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  if (server.hasArg("LED")) {
    Serial.println("Entering LED test page");
    server.sendHeader("Location", "/ledstate");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  String content = {
  "<form action='index.html' method='post' id='home_form'>"
  "  <input type='button' id='LogoutButton' class='btn' value='Logout'>"
  "  <input type='button' id='UploadButton' class='btn' value='Upload'>"
  "  <input type='button' id='LedTestButton' class='btn' value='Led Test'>"
  "  <input type='button' id='SerialButton' class='btn' value='Serial'>"
  "  <script type='text/javascript'>"
  "    document.getElementById('LogoutButton').onclick = function () {"
  "        location.href = '/login?DISCONNECT=YES';"
  "    };"
  "    document.getElementById('UploadButton').onclick = function () {"
  "        location.href = '/upload';"
  "    };"
  "    document.getElementById('LedTestButton').onclick = function () {"
  "        location.href = '/ledstate';"
  "    };"
  "  </script>"
  "</form>" + style
  };
  server.send(200, "text/html", content);
}

void handleUpload() {
  Serial.println("Enter handleUpload");
  String header;
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  String content = {
    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
    "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
    "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
    "<label id='file-input' for='file'>   Choose file...</label>"
    "<input type='submit' class=btn value='Update'>"
    "<br><br>"
    "<div id='prg'></div>"
    "<br><div id='prgbar'><div id='bar'></div></div><br>"
    "<input type='button' id='HomeButton' class=btn value='Home'>"
    "<input type='button' id='LogoutButton' class=btn value='Logout'>"
    "<script type='text/javascript'>"
    "    document.getElementById('LogoutButton').onclick = function () {"
    "        location.href = '/login?DISCONNECT=YES';"
    "    };"
    "    document.getElementById('HomeButton').onclick = function () {"
    "        location.href = '/';"
    "    };"
    "</script></form>"
    "<script>"
    "function sub(obj){"
    "var fileName = obj.value.split('\\\\');"
    "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
    "};"
    "$('form').submit(function(e){"
    "e.preventDefault();"
    "var form = $('#upload_form')[0];"
    "var data = new FormData(form);"
    "$.ajax({"
    "url: '/update',"
    "type: 'POST',"
    "data: data,"
    "contentType: false,"
    "processData:false,"
    "xhr: function() {"
    "var xhr = new window.XMLHttpRequest();"
    "xhr.upload.addEventListener('progress', function(evt) {"
    "if (evt.lengthComputable) {"
    "var per = evt.loaded / evt.total;"
    "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
    "$('#bar').css('width',Math.round(per*100) + '%');"
    "}"
    "}, false);"
    "return xhr;"
    "},"
    "success:function(d, s) {"
    "console.log('success!')"
    "},"
    "error: function (a, b, c) {"
    "}"
    "});"
    "});"
    "</script>" + style
  };
  server.send(200, "text/html", content);
}

void handleTestLED() {
  Serial.println("Enter handleUpload");
  String header;
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  String content = {
    "<form class='' action='index.html' method='post'>"
    "  <input type='button' id='LedState' class=ledbtn value='LED OFF'>"
    "  <input type='button' id='HomeButton' class=btn value='Home'>"
    "  <input type='button' id='LogoutButton' class=btn value='Logout'>"
    "  <script type='text/javascript'>"
    "    var gbutton = '#20e018';"
    "    var rbutton = '#ec1f1f';"
    "    var currentState = 'OFF';"
    "    document.getElementById('LogoutButton').onclick = function () {"
    "        location.href = '/login?DISCONNECT=YES';"
    "    };"
    "    document.getElementById('HomeButton').onclick = function () {"
    "        location.href = '/';"
    "    };"
    "    document.getElementById('LedState').onclick = function () {"
    "      var xhttp = new XMLHttpRequest();"
    "      xhttp.onreadystatechange = function() {"
    "        if (this.readyState == 4 && this.status == 200) {"
    "          currentState = this.responseText;"
    "        }"
    "      };"
    "      xhttp.open('GET', 'readLed', true);"
    "      xhttp.send();"
    "      if (currentState == 'ON') {"
    "        document.getElementById('LedState').style.background = gbutton;"
    "        document.getElementById('LedState').value = 'LED ON';"
    "      }else if(currentState == 'OFF') {"
    "        document.getElementById('LedState').style.background = rbutton;"
    "        document.getElementById('LedState').value = 'LED OFF';"
    "      }"
    "    };"
    "  </script>"
    "</form>" + style
  };
  server.send(200, "text/html", content);
}

void handleLedState() {
  if (ledState == false) {
    digitalWrite(ledPin,LOW);
    ledState = true;
    server.send(200, "text/plane", "OFF");
  }else {
    digitalWrite(ledPin,HIGH);
    ledState = false;
    server.send(200, "text/plane", "ON");
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
