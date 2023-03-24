//===========================================================================
void formElementEventCallback(ESPFormClass::HTMLElementItem element) {
  Serial.println();
  Serial.println("***********************************");
  Serial.println("id: " + element.id);
  Serial.println("value: " + element.value);
  Serial.println("type: " + element.type);
  Serial.println("event: " + ESPForm.getElementEventString(element.event));
  Serial.println("***********************************");
  Serial.println();
  if (element.id.equals("text1")) {
    ssid = element.value;
  }
  if (element.id.equals("text3")) {
    pass = element.value;
  }
  if (element.id.equals("click")) {
    writeToEEPROM(ssid, pass);
    ESPForm.setElementContent("argee", "Yes");
    Serial.println("RESTART....");
    ESP.restart();
  }
}
//===========================================================================
void serverTimeoutCallback() {

  //If server timeout (no client connected within specific time)
  Serial.println("***********************************");
  Serial.println("Server Timeout");
  Serial.println("***********************************");
  Serial.println();
}
//===========================================================================
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

  #click {
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
  
  #agree {
    display: block;
		width: 70px;
		height: 40px;
		padding: 4px 10px;
		margin: 10 0 10 0;
		border: 1px solid #03A9F4;
		background: #cce6ff;
		color: #e80000;
		font-size: 17px;
  }
}
	</style>
</head>

<body>
	<form>
		<label for="text1">SSID to device</label>
		<input type="text" id="text1" name="text1" value="Change me..."/>
    <label for="text3">PASS to device</label>
		<input type="text" id="text3" name="text3" value="Change me..."/>
		<label for="text2">Your WiFi ssid</label>
		<input type="text" id="ssid" name="text2" /> 
    <label for="text4">Your WiFi password</label>
		<input type="text" id="pass" name="text4" />
    <input type="button" id="click" name="buttonSubmit" value="Save"/>
    <label for="text5">Argee?</label>
		<input type="text" id="argee" name="argee" readonly/>
  </form>
</body>

</html>
)--espform--";