var myWebSocket;

function connectToWS() {
    var endpoint = document.getElementById("endpoint").value;
    if (myWebSocket !== undefined) {
        myWebSocket.close();
    }
    myWebSocket = new WebSocket(endpoint);


    myWebSocket.onmessage = function (event) {
        var leng;
        if (event.data.size === undefined) {
            leng = event.data.length;
        } else {
            leng = event.data.size;
        }
        log("onmessage. size: " + leng + ", content: " + event.data);
        var NoiDung = event.data;
        var mang = NoiDung.split(':');
        switch (mang[0]) {
            case "temp":
                document.getElementById("val_temp").innerHTML = mang[1];
                break;
            case "humidity":
                document.getElementById("val_humidity").innerHTML = mang[1];
                break;
        }
    };

    myWebSocket.onopen = function (evt) {
        log("onopen.");
    };

    myWebSocket.onclose = function (evt) {
        log("onclose.");
    };

    myWebSocket.onerror = function (evt) {
        log("Error!");
    };
}

function LED_TOGGLE(val) {
    var doc = document.getElementById("myMessage").value;
    if (doc == "off") {
        document.getElementById("myMessage").value = "on";
        myWebSocket.send(doc);
    }
    else if (doc == "on") {
        document.getElementById("myMessage").value = "off";
        myWebSocket.send(doc);
    }

}

function closeConn() {
    myWebSocket.close();
}

function log(msg) {
    var output = document.getElementById("output");
    output.value = output.value + msg + "\n";
    output.scrollTop = output.scrollHeight;
}

function clearLog(msg) {
    var output = document.getElementById("output");
    output.value = "";
}