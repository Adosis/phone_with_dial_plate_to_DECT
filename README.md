# phone_with_dial_plate_to_DECT
This is an aruino code to connect an old telephone with dial plate to an DECT phone. 

The code is for the arduino ESP32 nodeMCU. It is part of the project 
‘Lyon Retro Telephone With Dial Plate Convert to DECT Telephone Using Arduino and an Old DECT Phone’ you can find on instructables.com
The function of this code is:
-	controlling the DECT phone via serial port
-	detecting incoming calls
-	controlling the electromechanical bell of the Lyon phone
-	detecting picking and hanging up the handset of the Lyon phone
-	convert the information of the dialling plate into signals for the DECT phone
-	sending sequence of keystrokes for automated connection to a DECT basis or fritzBox

Therefore, different functions were included.

### 1.	loop () ###

a.	 Checking if there was an input vial serial port
b.	Detecting an incoming call. Therefore the signal of the LED from the DECT phone signals an incoming call by blinking was detected with a phototransistor. To be sure that LED is really off (no incoming call) and that there is not the short time between two ON phases, we repeat the checking in a loop.
c.	Checking if there was a process of picking up / hanging up the phone. We have to detect the process of picking up / hanging up, because we only have to send a short signal to the DECT button matrix.
d.	Checking if there was a dialling process after picking up the handset. The dialling plate has four cables: two for the dialling process and two for the pulses while dialling. If a dialling process is detected, the number of pulses will be counted. After the dialling process is finished the number of counted pulses is the current dialled number.
e.	Checking if there was a dialling process with hanged up handset. When dialling while the handset is hanged up it is a signal to connect the phone to a basis or a fritzBox automatically. 9 starts a sequence of keystroke to connect the DECT to a basis and 0 starts a sequence of keystroke to connect the DECT to a fritzbox.


### 2.	matrixCalc () ###

The button matrix of the DECT phone was connected to the arduino via optocoupler. You can see the schematic of the matrix on instructables.com. When a ‘button should be pressed’ a signal was send to one bit of Port A and one bit of Port B by the command matrixWrite (int a, int b). You can see most buttons of the DECT phone and the numbers of the matrix.


### 3.	matrixWrite (int a, int b) ###

The outputs of the pins for port A and port B were set to HIGH for nearly 125ms. Then the outputs were set to LOW. This imitates a keystroke. The outpins pins were declared in teh variable pinMatrix[]. 


### 4.	linkFritz () and linkBasis () ###

Here are a sequences of keystrokes to connect the DECT to a basis or to a fritzBox.


### 5.	ring () ###

The electromechanical bell needs pulsed voltage. This function sends pulses to the output of the bell. By changing the numbers in the ‘for’ command (125) and in the ‘delay’ command (1500) you can change the sound al little bit. You can also configure your own sound, of course.






 
