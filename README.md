# SDK-CPP

The symbol core SDK which implements the low level functionality of a client.

## Package Organization

The sdk code is organized as follows:

| Folder name | Description |
| -------------|--------------|
| /external | External dependencies that are built with the sdk. |
| /src/symbolSDK/core | The core SDK functionality with no external dependencies |
| /src/symbolSDK/extended | The extended SDK functionality with dependencies on core |
| /src/symbolSDK/plugins | Modules that introduce new and different ways to alter the chain's state via transactions. |
| /tests | Collection of unit tests that test the sdk functionality. |
| /delete | Temporary, will be deleted soon. Contains code from catapult-client.|


## Compiling the SDK

The instructions for compiling are the exact same as the [catapult-client](https://github.com/symbol/catapult-client/blob/dev/docs/BUILD-conan.md). Below is a quick summary:

- conan remote add nemtech https://catapult.jfrog.io/artifactory/api/conan/ngl-conan
- git clone https://github.com/symbol/sdk-cpp.git
- cd sdk-cpp
- mkdir _build && cd _build
- conan install .. --build missing
- cmake -DUSE_CONAN=ON -DCMAKE_BUILD_TYPE=Release -G Ninja ..
- ninja -j4

## Running unit tests

From the '_build' dir:

- export LD_LIBRARY_PATH=$PWD/deps
- bin/tests.catapult.parsers
- bin/tests.catapult.extensions
- bin/tests.catapult.builders

etc.

## License

Copyright (c) 2018 Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp Licensed under the [GNU Lesser General Public License v3](LICENSE.txt)

[developer documentation]: https://docs.symbolplatform.com/
[forum]: https://forum.nem.io/c/announcement
[issues]: https://github.com/symbol/sdk-java/issues
[discord]: https://discord.gg/xymcity
[catapult-client]: https://github.com/symbol/sdk-cpp
[symbol-rest]: https://github.com/symbol/catapult-rest
[nem]: https://nem.io
[symbol]: https://symbolplatform.com/

