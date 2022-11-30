# apkmgr
apkmgr (abbreviation of APK Manager) plans to be a fully featured cli package manager for Android apps, based on the F-Droid repositories.

Current requirements are:
* [termux](https://f-droid.org/en/packages/com.termux/) (for running the program)
* any android app that allows you to open APKs and install them (I recommend [App Manager](https://fdroid.gitlab.io/fdroid-website/en/packages/io.github.muntashirakon.AppManager/))

Current build requirements are:
* [libcurl](https://curl.se/libcurl)
* [cjson](https://github.com/DaveGamble/cJSON)

To do:
* Add a function for searching
* Put all apkmgr functions into a header file
* Use snprintf() instead of sprintf() as it is safer
* Fix arbitrary buffer size
