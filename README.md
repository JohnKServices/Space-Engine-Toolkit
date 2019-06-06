# Space-Engine-Tookit
Just a little "toolkit" I made! Took a while. You can get Space Engine at http://spaceengine.org/ !

To use, make Visual Studio C++ DLL project, add this code, compile, open the Visual Studio command prompt go to the Space Engine directory (Should be C:\SpaceEngine\system\) then do `link /edit /dynamicbase:NO SpaceEngine.exe`, then use any injector you would like to inject into Space Engine. Windows only. (Shouldn't matter though since Space Engine is only on windows anyways)  
Make sure you are using version 0980e! 0980 will not work.  

I prefer to use `injectAllThings` but you (should be able to) use any you want!

Commands are:  
`consolePrint: first second third` <- Prints text to ingame console.  
`setVar: testFloat/testInt xx.xxx` <- Sets the in-game `testX` variables to value.  
`getVar: testFloat/testInt xx.xxx` <- Gets the in-game `testX` value.  
`clear` <- Clears console.  
`command: (ingame command) (argument1) (argument2)` <- Executes ingame command.  
  
That should be all! Cheers.  
