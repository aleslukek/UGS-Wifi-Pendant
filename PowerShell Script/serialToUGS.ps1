# -- PARAMETERS --
$PORT='COMX'
$BAUDRATE=115200
$Parity=[System.IO.Ports.Parity]::None # System.IO.Ports.Parity
$dataBits=8
$StopBits=[System.IO.Ports.StopBits]::one # System.IO.Ports.StopBits
# END PARAMETERS ---------------------------------


# -- GLOBAL VARIABLES
$NewCommand=0
$CommandStage=0
$PostCommand=0
$PostHeader=""
$PostBody=""
$CommandURL=""
$SerialStarted=0
# --END VARIABLES


$port= new-Object System.IO.Ports.SerialPort $PORT,$BAUDRATE,$Parity,$dataBits,$StopBits

#run in endless loop
while(1){
      #list avaiable COM devices
      $COMportList = [System.IO.Ports.SerialPort]::getportnames()

      #if UGS Pendant is connected to the computer read it's serial
      if($COMportList -Match $PORT){ #UGS Pendant available
    	$port.Open()

      # Debug
    	Write-Output 'PORT OPENED'

    	# Gets the data from the com port for the specified interval
    	while ($port.IsOpen) {
        	  $message=$port.ReadLine()
          	if(($CommandStage -eq 1) -and (($message -Match '1') -or ($message -Match '0'))){
              	  $CommandStage=2
              	  $PostCommand=$message
              	  $PostCommand = $PostCommand -as [int]
          	}elseif($CommandStage -eq 2){
              	  $CommandStage=3
              	  $PostBody=$message
          	}elseif($CommandStage -eq 3){
              	  $CommandStage=4
              	  $CommandURL=$message
          	}elseif($message -Match 'END'){
              	  $NewCommand=0
              	  $CommandStage=0
          	}elseif($message -Match 'Setup finished'){
              	  $NewCommand=0
              	  $CommandStage=0
          	}
          	#copy string until ' character
          	Write-Output $message
        	  if($message -Match 'BEGIN'){
              	  $NewCommand=1
              	  $CommandStage=1
        	  }

          	if($CommandStage -eq 4){
              	  if($PostCommand -eq 0){ #GET
              		    Invoke-RestMethod -Uri $CommandURL
              	  }elseif($PostCommand -eq 1){ #POST
                  		$header = @{
                  		    "Content-Type"="application/json"
                  		}
                  		$body = @{
                  		    "commands"=$PostBody
                  		} | ConvertTo-Json
                  		Invoke-RestMethod -Uri $CommandURL -Method 'Post' -Body $Body -Headers $header
              	  }
          	}
    	}
    	$port.Close()

    	#Debug
    	Write-Output 'PORT CLOSED'
    	#--

      }
      #if UGS Pendant is not connected (or port is closed) to computer wait for 30 seconds and re run this while loop
      Start-Sleep -s 30
}
