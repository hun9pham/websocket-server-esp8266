    /* NUT BAM TEST BIEU DO 
count_temp =0;
function cnt_temp(){
	document.getElementById("cnt_temp").innerHTML = count_temp;
	count_temp ++;
	count_temp = count_temp %4;
	return count_temp;
}
count_humidity = 0;
function cnt_humidity() {
    document.getElementById("cnt_humidity").innerHTML = count_humidity;
    count_humidity++;
    count_humidity = count_humidity % 5;
    return count_humidity;
}
function TEMP(){
	setInterval(cnt_temp, 1000);
}
function HUMIDITY() {
    setInterval(cnt_humidity, 1000);
}
*/

window.onload = function () {
    //CHART TEMP
var dps = [];
var chart = new CanvasJS.Chart("chartContainer_temp", {
exportEnabled: true,
title :{
	text: "Temperature"
},
data: [{
    type: "spline",
    color: "red",
	markerSize: 0,
	dataPoints: dps
}]
});

var xVal = 0;
//var yVal = 0;
var dataLength = 10; // number of dataPoints visible at any point

var updateChart = function () {
// count is number of times loop runs to generate random dataPoints.
    var getTemp = document.getElementById("val_temp").innerHTML;
for (var j = 0; j < parseInt(getTemp) ; j++) {
	//yVal = 100;
	dps.push({
		x: xVal,
		y: parseInt(getTemp)
	});
	xVal++;
}
if (dps.length > dataLength) {
	dps.shift();
}
chart.render();
};
updateChart(dataLength);
setInterval(function () { updateChart() }, 1000);

/*----------------------------------------------------------------------------*/
    //CHART HUMIDITY
var dps_humidity = [];
var chart_humidity = new CanvasJS.Chart("chartContainer_humidity", {
exportEnabled: true,
title :{
	text: "Humidity"
},
data: [{
    type: "line",
    color: "darkblue",
	markerSize: 0,
	dataPoints: dps_humidity
}]
});

var xVal_humidity = 0;
var dataLength_humidity = 10; // number of dataPoints visible at any point

var updateChart_humidity = function () {
// count is number of times loop runs to generate random dataPoints.
var getHumidity = document.getElementById("val_humidity").innerHTML;
for (var j = 0; j < parseInt(getHumidity) ; j++) {
	yVal_humidity = 100;
	dps_humidity.push({
		x: xVal_humidity,
		y: parseInt(getHumidity)
	});
	xVal_humidity++;
}
if (dps_humidity.length > dataLength_humidity) {
	dps_humidity.shift();
}
chart_humidity.render();
};

updateChart(dataLength_humidity);
setInterval(function(){ updateChart_humidity() }, 1000);

}