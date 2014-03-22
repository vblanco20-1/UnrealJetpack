This is a simple jetpack system made for the tutorial here: https://forums.unrealengine.com/showthread.php?48-Victor-s-ShooterGame-Jetpack-TUTORIAL
Includes 2 classes, the character, wich takes care of the walljumping and initing the jetpack, and the movement component, wich takes care of the upwards force that gets added when the jetpack is used.
Works multiplayer just fine, and this code is updated to newest 4.0.1 engine version.
If you have any doubts, improvements, or something, feel free to talk to me.

How to use:

You need to add the code file to shootergame Source folder. The .cpp files should go to the Private folder, and the .h files to the Classes folder.
To play with it, you just need to change the parent class of the PlayerPawn blueprint to JetpackCharacter. And then call the functions StartJetpack and StopJetpack in some keybind you want. Same with the Walljump function, call it using a keyibind or in the StartJump event.