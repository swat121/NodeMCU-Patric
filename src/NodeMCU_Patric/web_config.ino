//Raw content of index.html in plain text
static const char *index_html = R"--espform--(
<!DOCTYPE html>
<html lang="en">

<head>
	<title> Simple Textbox </title>
	<style>
body {
		padding: 20px;
    position: fixed; 
    left: 39%;
	}
	
	label {
		font-size: 17px;
		font-family: sans-serif;
	}
	
	input {
		display: block;
		width: 300px;
		height: 40px;
		padding: 4px 10px;
		margin: 10 0 10 0;
		border: 1px solid #03A9F4;
		background: #cce6ff;
		color: #1c87c9;
		font-size: 17px;
	}

  #submit {
    cursor:pointer;
    display: block;
		width: 100px;
		height: 40px;
		padding: 4px 10px;
		margin: 10 0 10 0;
		border: 1px solid #03A9F4;
		background: #39ad50;
		color: #ffffff;
		font-size: 17px;
  }
}
	</style>
</head>

<body>
	<form method="POST" action="/submit">
		<label for="ssid">SSID to device</label>
		<input type="text" id="ssid" name="ssid"/>
    <label for="pass">PASS to device</label>
		<input type="text" id="pass" name="pass"/>
    <input type="submit" id="submit" value="Save"/>
  </form>
</body>

</html>
)--espform--";


//-----------------------------------------------------------------------------------------------------

void handleFormSubmit() {
  if (server.hasArg("ssid") && server.hasArg("pass")) {
    String arg1 = server.arg("ssid");
    String arg2 = server.arg("pass");
    // Do something with the message, e.g. print it to Serial
    Serial.println(arg1);
    Serial.println(arg2);

    server.send(200, "text/plain", "OK");
    writeToEEPROM(arg1, arg2);
    delay(3000);
    Serial.println("Reset..");
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}
//-----------------------------------------------------------------------------------------------------

void handleMainHtmlPage() {
  server.send(200, "text/html", index_html);
}
