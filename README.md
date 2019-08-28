UWP Speech
==========

An example console app which uses UWP speech synthesis API.

### Prerequisites

- Windows 10 v1903 or later
- VisualStudio 2019 community edition
- git bash for Windows

## Build

Launch the developer command prompt and then hit the following commands:

```console
git clone https://github.com/moutend/uwpspeech
cd uwpspeech
mkdir build
cd build
cmake ..
msbuild all_build.vcxproj
```

Hint: The developer command prompt comes with the VisualStudio installation. Hit the Windows key and then type "dev" or "developer" to launch.

## Test

```console
cd Debug
uwpspeech.exe
```

Then `output.wav` will be created.

## LICENSE

See LICENSE.

## Author

[Yoshiyuki Koyanagi <moutend@gmail.com>](https://github.com/moutend)
