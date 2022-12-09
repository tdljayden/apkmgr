# apkmgr
apkmgr (abbreviation of APK Manager) plans to be a fully featured cli package manager for Android apps, based on the F-Droid repositories.

apkmgr consists of two parts: a cli program written in C and an android app written in Java. The cli program is accessed through the termux terminal emulator in order to download APKs and open them in the the android app to install them.

NOTE: apkmgr is currently alpha-quality software and is unreliable, doesn't work, with a lot of features missing.

Current requirements are:
* [termux](https://f-droid.org/en/packages/com.termux/) (for running the program)
* any android app that allows you to open APKs and install them (I recommend [App Manager](https://fdroid.gitlab.io/fdroid-website/en/packages/io.github.muntashirakon.AppManager/))

Current build requirements are:
* [libcurl](https://curl.se/libcurl)
* [cjson](https://github.com/DaveGamble/cJSON)

To do:
* Add a function for searching
* Add help screen along with help function
* Remove requirement of third-party program in order to open APKs (explained in description). (WIP) 
