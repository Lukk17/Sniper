# Sniper app.

Target practice.

--------------------------------------------
### Updating app icons

```
flutter pub get
```
```
flutter pub run flutter_launcher_icons
```

--------------------------------------------
### Compiling Flutter Web:

After compilation DELETE from: `build\web\assets`  
file: `NOTICES` - it's only aggregated licenses of dependencies, safe to delete - size: almost 1mb

Change renderer (release parameter need to be last to work):
```
flutter build web --web-renderer html --release
```
By default it is set to `auto`,  
which chooses the HTML renderer when the app is running in a mobile browser,  
and CanvasKit renderer when the app is running in a desktop browser.
Other option are: `html` (smaller download size) and `canvaskit` (adds about 2MB in download size).

Normal release:
```
flutter build web --release
```