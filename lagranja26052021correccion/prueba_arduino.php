<?php

$arrayDatos = array();
// Es mejor tener los datos de conexión por separado, así es más fácil cambiarlos
echo "----------- Se han recibido por LORA las siguientes parejas CLAVE => VALOR ";
foreach ($_POST as $clave => $valor) {
	echo ($clave . " => " . $valor . " ----------- \n\n");
}

// // Cojo unos valores según la fecha y la hora, para que los datos que introducimos en la BBDD no sean siempre iguales
// $LoRaData = "defecto " . Date(" d/m/y - h:m:s");	// Valor por defetco de LoRaData, por si no viene definido
// $numero = Date("hms");							// Valor para el número, que es la hora minuto y segundo seguidos


//Primero compruebo si se ha pasado por POST el valor para LoRaData
if (isset($_POST['idEmisor'])) {
	// Si se ha definido lo cojo.
	$idEmisor = $_POST['idEmisor'];
}
echo "Valor final de idEmisor: " . $idEmisor . " \n";


// //Primero compruebo si se ha pasado por POST el valor para waterSensor

if (isset($_POST['datoSensor1'])) {
	// Si se ha definido lo cojo.
	$datoSensor1 = $_POST['datoSensor1'];
	
}

if (isset($_POST['tipoMedidaSensor1'])) {
	// Si se ha definido lo cojo.
	$tipoMedidaSensor1 = $_POST['tipoMedidaSensor1'];
	$arrayDatos += [$datoSensor1 => $tipoMedidaSensor1];
}

if (isset($_POST['datoSensor2'])) {
	// Si se ha definido lo cojo.
	$datoSensor2 = $_POST['datoSensor2'];
}

if (isset($_POST['tipoMedidaSensor2'])) {
	// Si se ha definido lo cojo.
	$tipoMedidaSensor2 = $_POST['tipoMedidaSensor2'];
	$arrayDatos += [$datoSensor2 => $tipoMedidaSensor2];

}if (isset($_POST['datoSensor3'])) {
	// Si se ha definido lo cojo.
	$datoSensor3 = $_POST['datoSensor3'];
}

if (isset($_POST['tipoMedidaSensor3'])) {
	// Si se ha definido lo cojo.
	$tipoMedidaSensor3 = $_POST['tipoMedidaSensor3'];
	$arrayDatos += [$datoSensor3 => $tipoMedidaSensor3];

}if (isset($_POST['datoSensor4'])) {
	// Si se ha definido lo cojo.
	$datoSensor4 = $_POST['datoSensor4'];
}

if (isset($_POST['tipoMedidaSensor4'])) {
	// Si se ha definido lo cojo.
	$tipoMedidaSensor4 = $_POST['tipoMedidaSensor4'];
	$arrayDatos += [$datoSensor4 => $tipoMedidaSensor4];
}
echo "Valor final de waterSensor: " . $waterSensor." y es de tipo ";
echo gettype($waterSensor), "\n";

echo "Valor final de tipoMedidaWater: " . $tipoMedidaWater." y es de tipo ";
echo gettype($tipoMedidaWater), "\n";

try{
	$url = 'http://lagranja40.agllabs.es/api/dispositivos/setMedida';
	
	
	$ch = curl_init($url);
	
	foreach ($arrayDatos as $key => $value) {
	$data = array(
	   //Sustituir los datos por los datos de $POST
		'id' => ".$idEmisor.",
		'medida' => ".$key.",
		'tipoMedida' => ".$value."
	);
	$payload = json_encode($data);
	
	
	curl_setopt($ch, CURLOPT_POSTFIELDS, $payload);
	
	
	curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type:application/json'));
	
	
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	
	
	$result = curl_exec($ch);
}
	curl_close($ch);
	
	}catch(Exception $e){
		echo $e;
	}



