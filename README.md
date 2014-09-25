# Zune Now Playing
This is an alternate Git repo for Zune Now Playing at http://znp.codeplex.com/

## Project Description
Zune Now Playing retrieves the information of the current song being played in Zune Software
and stores it in the Windows Registry. It runs as a background process and the song metadata
it stores in the registry can be retrieved by any application to use as required.

## Quick-Start Guide:
To find the metadata, just read the registry values at ```HKEY_CURRENT_USER\Software\ZuneNowPlaying```.
It contains the following values:
* ```Playing``` - Its value is either 1 or 0 based on whether Zune Software is playing a song or not.
  (Please note that Zune - Now Playing doesn't identify PAUSE event in Zune Software)
* ```Artist``` - If ```Playing``` = 1, it contains the name of artist whose song is being played,
  otherwise null string .
* ```Album``` - If ```Playing``` = 1, it contains the name of album from which the song is being played,
  otherwise null string.
* ```Title``` - If ```Playing``` = 1, it contains the title of the song being played, otherwise null string.

You can include the executable with your project and run it as required. Only one instance of the
executable will run at any particular moment.

## Projects Using It
* **Zune Lyrics** - http://zunelyrics.codeplex.com/
* **Ztatus** - Now Playing App for Zune Software -  http://inameri.ca/ztatus/
* **PowerToys for Zune** - http://ptfz.codeplex.com/
