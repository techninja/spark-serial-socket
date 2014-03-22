TCPClient client;

// Globals!
String cmd = "";
int led = 7;
long speed = 9600;
bool serialSetup = false;

// Convert IP string into byte array
void ipArrayFromString(byte ipArray[], String ipString) {
  int dot1 = ipString.indexOf('.');
  ipArray[0] = ipString.substring(0, dot1).toInt();
  int dot2 = ipString.indexOf('.', dot1 + 1);
  ipArray[1] = ipString.substring(dot1 + 1, dot2).toInt();
  dot1 = ipString.indexOf('.', dot2 + 1);
  ipArray[2] = ipString.substring(dot2 + 1, dot1).toInt();
  ipArray[3] = ipString.substring(dot1 + 1).toInt();
}

// Spark API function handler to connect this core to a local server
// 'connect' format: [IPv4]:[PORT]@[BAUD] -- EG 192.168.0.42:9025@9600
int connectToMyServer(String connect) {
  String ip = connect.substring(0, connect.indexOf(':'));
  byte serverAddress[4];
  ipArrayFromString(serverAddress, ip);

  int port = connect.substring(connect.indexOf(':') + 1, connect.indexOf('@')).toInt();
  speed = (long) connect.substring(connect.indexOf('@') + 1).toInt();


  if (client.connect(serverAddress, port)) {
    return 1; // successfully connected
  } else {
    return -1; // failed to connect
  }
}


void setup() {
  Spark.function("connect", connectToMyServer);
  pinMode(led, OUTPUT);
 }

// Stub function for setting up serial because speed is set at connect time
// and therefore cannot be part of setup()
void setupSerial() {
  serialSetup = true;
  client.print("Serial begin at ");
  client.println(speed);
  Serial1.begin(speed);
}

void loop() {
  if (client.connected()) {
    if (client.available()) {
      char pin = client.read() - '0' + D0;
      char level = client.read();
      if ('h' == level) {
        digitalWrite(pin, HIGH);
      } else {
        digitalWrite(pin, LOW);
      }
    }
  }
}
